#include "BaseEnviromentMapMesh.h"
#include "Sapphire.h"
#include "GraphicDriver.h"
#include "ITextureMgr.h"
#include "Camera.h"

namespace Sapphire
{

	void BaseEnviromentMapMesh::LoadBaseShader(const char* vs, const char* ps)
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	void BaseEnviromentMapMesh::Init()
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	void BaseEnviromentMapMesh::Render()
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	void BaseEnviromentMapMesh::Update(std::vector<SharedPtr<BaseLight>>& lightVec)
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	void BaseEnviromentMapMesh::SetCubeMap(ITexture* pTexture)
	{

	}

	void BaseEnviromentMapMesh::Release()
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

}