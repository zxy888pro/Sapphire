#pragma once

#include <Sapphire.h>
#include "Shader.h"
#include <glm.hpp>
#include "RenderState.h"


namespace Sapphire
{
	enum MeshType
	{
		MT_BaseMesh,
		MT_BaseLighMesh,
		MT_BaseLightMapMesh,
		MT_EmissionMesh,
		MT_StandardMaterialMesh,
		MT_SkyBoxMesh,
		MT_EnviromentMapMesh,
		MT_SimpleUBOMesh,
		MT_UI,
		MT_MaxCount
	};

	class BaseLight;

	class BaseMesh :public RefCounter
	{
	public:

		BaseMesh();
		virtual ~BaseMesh();
		virtual void LoadBaseShader(const char* vs, const char* ps, const char* gs = NULL);
		virtual void Init();
		virtual void Render();
		virtual void Update(std::vector<SharedPtr<BaseLight>>& lightVec);

		glm::vec3 getPos() const { return m_pos; }
		void setPos(glm::vec3 val) { m_pos = val; }
		glm::vec3 getRot() const { return m_rot; }
		void setRot(glm::vec3 val) { m_rot = val; }
		glm::vec3 getScale() const { return m_scale; }
		void setScale(glm::vec3 val) { m_scale = val; }
		MeshType getType(){ return m_type; }



	protected:

		virtual void Release();
		virtual void BackupRenderState();
		virtual void RestoreRenderState();

		GLuint m_mVbo;
		GLuint m_mVao;
		GLuint m_mEbo;
		glm::vec3  m_pos;
		glm::vec3  m_rot;
		glm::vec3  m_scale;
		RenderState  m_state;
		Shader* m_pShader;
		float*  m_vertices;
		MeshType m_type;

	private:




	};

}