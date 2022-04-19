#pragma once
#include "Snct_Utility.h"
#include "Snct_Object.h"

class ISnctRender;

class ISnctScene {

public:
	ISnctScene(ISnctRender* pRender) : m_pRender(pRender) {}

	void Initialize()
	{
		SceneObjects();
		for (const auto& object : m_pObjects) object.second->Initialize();
	}
	void Update() 
	{
		for (const auto& object : m_pObjects) object.second->Update();
	}
	void Draw() 
	{
		for (const auto& object : m_pObjects) object.second->Draw();
	}
	void Finalize()
	{
		for (const auto& object : m_pObjects) object.second->Finalize();
	}

protected:
	/// <summary>
	/// set start scene objects
	/// </summary>
	virtual void SceneObjects() = 0;

	/// <summary>
	/// add scene object instance
	/// </summary>
	/// <typeparam name="T"> object type </typeparam>
	/// <param name="key"> regist hash key </param>
	template<class T>
	void AddObject(HashKey key);
	
	/// <summary>
	/// release object instance
	/// </summary>
	/// <param name="key"> regist hash key </param>
	void ReleaseObject(HashKey key);

private:
	ISnctRender* m_pRender = nullptr;
	std::unordered_map<HashKey, std::unique_ptr<ISnctObject>> m_pObjects;
};

template<class T>
inline void ISnctScene::AddObject(HashKey key)
{
	m_pObjects.emplace(key, std::make_unique<T>());
}

inline void ISnctScene::ReleaseObject(HashKey  key)
{
	m_pObjects.erase(key);
}