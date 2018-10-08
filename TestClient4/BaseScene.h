#pragma once
#include <Sapphire.h>
#include <Str.h>
#include "BaseLight.h"
#include "BaseMesh.h"

namespace Sapphire
{

	class BaseRenderTarget;
	class BaseRenderSurface;

	class Scene
	{
	public:

		Scene();
		virtual ~Scene();
		virtual void Initialize();
		virtual void Upate();
		virtual void Render();
		virtual void DrawPostEffect();

		int GetLightCount();
		bool AddLight(SharedPtr<BaseLight> pLight);
		bool AddMesh(std::string name, SharedPtr<BaseMesh> pMesh);
		bool RemoveLight(int index);
		bool RemoveLight(SharedPtr<BaseLight> pLight);
		bool RemoveMesh(std::string name);
		

		bool getEnablePost() const { return m_bEnablePost; }
		void setEnablePost(bool val) { m_bEnablePost = val; }

	protected:

		std::map<std::string, SharedPtr<BaseMesh>>  m_meshMap;
		std::vector < SharedPtr<BaseLight> >  m_lights;
		bool m_bEnablePost;

		//∫Û∆⁄‰÷»æRTT
		BaseRenderTarget*  m_PostRtt;
		BaseRenderSurface* m_PostRs;
	};
}