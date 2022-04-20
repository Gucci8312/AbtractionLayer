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
	//------------------------------------------------------------------------------
	/// set start scene objects
	//------------------------------------------------------------------------------
	virtual void SceneObjects() = 0;

	//------------------------------------------------------------------------------
	/// Add scene object instance
	/// \param[in]      Hash key
	//------------------------------------------------------------------------------
	template<class T>
	void AddObject(HashKey key);
	
	//------------------------------------------------------------------------------
	/// Release object instance
	/// \param[in]      Hash key
	//------------------------------------------------------------------------------
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