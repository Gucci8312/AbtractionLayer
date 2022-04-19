#pragma once
#include <unordered_map>
#include <string>

// Classes that manage Resources
class SnctResouceLibary
{
public:
	// Method	

	//------------------------------------------------------------------------------
	/// Create resource
	/// \param[in]      Hash key
	/// \param[in]      Resource data
	/// \return			Nothing
	//------------------------------------------------------------------------------
	template  <class T> inline void CreateResource(std::string Hash, T* Resource)
	{
		m_resource[Hash] = Resource;
	}


	//------------------------------------------------------------------------------
	/// Release resource
	/// \param[in]      Hash key
	/// \return			Nothing
	//------------------------------------------------------------------------------
	inline void ReleaseResource(std::string Hash)
	{
		m_resource.erase(Hash);
	}


	//------------------------------------------------------------------------------
	/// Get resource
	/// \param[in]      Hash key
	/// \return			Value
	//------------------------------------------------------------------------------
	template <class T>inline T* GetResource(std::string Hash)
	{
		return reinterpret_cast<T*>(m_resource[Hash]);
	}

private:
	// Member
	// Key.String  Value.Template(Resource)
	std::unordered_map<std::string, void*> m_resource;
};
