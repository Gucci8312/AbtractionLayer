#include <iostream>
#include <chrono>
#include <atomic>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <string>
#include <sstream>
#include <random>
#include <queue>

class table
{
    std::mutex mutex_;
    std::condition_variable condition_;

    std::queue<std::string> buffer_;
    const int count_;

public:
    explicit table(int count)
        : buffer_{}, count_{ count }
    {
    }

    table(table const&) = delete;
    table& operator=(table const&) = delete;

    void push(std::string const& cake, std::string const& thread_name)
    {
        std::unique_lock<std::mutex> lock{ mutex_ };
        while (static_cast<int>(buffer_.size()) >= count_) {
            condition_.wait(lock);
        }
        buffer_.push(cake);
        condition_.notify_all();
        std::cout << thread_name << " puts " << cake << std::endl;
    }

    std::string pop(std::string const& thread_name)
    {
        std::unique_lock<std::mutex> lock{ mutex_ };
        while (buffer_.empty()) {
            condition_.wait(lock);
        }
        std::string cake = std::move(buffer_.front());
        buffer_.pop();
        condition_.notify_all();
        std::cout << thread_name << " takes " << cake << std::endl;
        return cake;
    }
};

class cake_maker
{
    table& table_;
    std::string name_;
    std::uniform_int_distribution<> rand_;
    std::mt19937 engine_;
    static std::atomic<int> id_;

public:
    cake_maker(table& table, std::string name, std::mt19937::result_type seed)
        : table_{ table }, name_{ std::move(name) }, rand_{ 0, 1000 }, engine_{ seed }
    {
    }

    void operator()()
    {
        while (true) {
            std::this_thread::sleep_for(std::chrono::milliseconds{ rand_(engine_) });
            std::ostringstream oss;
            oss << "[cake no." << id_ << "] by " << name_;
            ++id_;
            table_.push(oss.str(), name_);
        }
    }
};

std::atomic<int> cake_maker::id_ = 0;

class cake_eater
{
    table& table_;
    std::string name_;
    std::uniform_int_distribution<> rand_;
    std::mt19937 engine_;

public:
    cake_eater(table& table, std::string name, std::mt19937::result_type seed)
        : table_{ table }, name_{ std::move(name) }, rand_{ 0, 1000 }, engine_{ seed }
    {
    }

    void operator()()
    {
        while (true) {
            std::string const& cake = table_.pop(name_);
            std::this_thread::sleep_for(std::chrono::milliseconds{ rand_(engine_) });
        }
    }
};

int main()
{
    table table{ 10 };
    std::thread maker1{ cake_maker{table, "Maker-1", 31415} };
    std::thread maker2{ cake_maker{table, "Maker-2", 92653} };
    std::thread maker3{ cake_maker{table, "Maker-3", 58979} };
    std::thread eater1{ cake_eater{table, "Eater-1", 32384} };
    std::thread eater2{ cake_eater{table, "Eater-2", 62643} };
    std::thread eater3{ cake_eater{table, "Eater-3", 38327} };
    maker1.join();
    maker2.join();
    maker3.join();
    eater1.join();
    eater2.join();
    eater3.join();
}