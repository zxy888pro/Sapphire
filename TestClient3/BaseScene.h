#pragma once
#include <Sapphire.h>
#include <Str.h>
#include "BaseLight.h"
#include "BaseMesh.h"

namespace Sapphire
{

	class Scene
	{
	public:

		Scene();
		virtual ~Scene();

		virtual void Upate();
		virtual void Render();

		int GetLightCount();
		bool AddLight(SharedPtr<BaseLight> pLight);
		bool AddMesh(std::string name, SharedPtr<BaseMesh> pMesh);
		bool RemoveLight(int index);
		bool RemoveLight(SharedPtr<BaseLight> pLight);
		bool RemoveMesh(std::string name);
		

	protected:

		std::map<std::string, SharedPtr<BaseMesh>>  m_meshMap;
		std::vector < SharedPtr<BaseLight> >  m_lights;
	};
}