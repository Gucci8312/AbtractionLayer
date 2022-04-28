#pragma once
#include "Snct_Utility.h"
#include "Render/DirectX/Snct_DXRender.h"
#include "Render/DirectX/Snct_DXObject.h"

class ISnctRender;

class ISnctScene {

public:
	void SetRender(SnctDXRender* pRender) { m_pRender = pRender; };

	void Initialize()
	{
		SceneObjects();
		for (const auto& object : m_pObjects) object.second->Create(m_pRender->GetDevice());
	}
	void Update() 
	{
		//for (const auto& object : m_pObjects) object.second->Update();
	}
	void Draw(ISnctDXContext* pContext)
	{
		for (const auto& object : m_pObjects) object.second->Draw(pContext);
	}
	void Finalize()
	{
		//for (const auto& object : m_pObjects) object.second->Finalize();
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
	SnctDXRender*	m_pRender	= nullptr;
	std::unordered_map<HashKey, std::unique_ptr<SnctDXObject>> m_pObjects;
};

template<class T>
inline void ISnctScene::AddObject(HashKey key)
{
	m_pObjects.emplace(key, std::make_unique<T>());
	m_pObjects.at(key)->SetRender(m_pRender);
}

inline void ISnctScene::ReleaseObject(HashKey  key)
{
	m_pObjects.erase(key);
}