#pragma once
#include <SapphireDef.h>
#include "BaseMesh.h"
#include "BaseRenderTarget.h"

namespace Sapphire
{

	class BaseRenderSurface : public BaseMesh
	{
	public:

		BaseRenderSurface();

		virtual ~BaseRenderSurface();



		virtual void Init() override;


		virtual void Render() override;
		virtual void SetTexture(GLuint texture);
		virtual GLuint GetTexture();

		virtual void Update(std::vector<SharedPtr<BaseLight>>& lightVec) override;

	protected:
		
		GLuint  m_texObj;
		virtual void Release() override;

	};

}