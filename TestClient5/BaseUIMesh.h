#pragma once


#include <Sapphire.h>
#include "Shader.h"
#include <glm.hpp>
#include "RenderState.h"
#include "BaseMesh.h"


namespace Sapphire
{

	class BaseUIMesh : public BaseMesh
	{
	public:
		BaseUIMesh();


		virtual ~BaseUIMesh();


		virtual void Init() override;


		virtual void Render() override;

	protected:

		uint m_vboObj;


	};

}
