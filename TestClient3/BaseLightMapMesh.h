#pragma once

#include "BaseLightMesh.h"

namespace Sapphire
{
	class BaseLightMapMesh : public BaseLightMesh
	{
	public:
		BaseLightMapMesh();

		virtual ~BaseLightMapMesh();

		virtual void Init() override;

		virtual void Render() override;

		virtual void SetDiffuseMap(const char* fileName);

		virtual void SetSepcularMap(const char* fileName);

		glm::vec3 getDffiuseColor() const { return m_dffiuseColor; }
		void setDffiuseColor(glm::vec3 val) { m_dffiuseColor = val; }
		glm::vec3 getSepcularColor() const { return m_sepcularColor; }
		void setSepcularColor(glm::vec3 val) { m_sepcularColor = val; }
		glm::vec3 getAmbientColor() const { return m_ambientColor; }
		void setAmbientColor(glm::vec3 val) { m_ambientColor = val; }
		float getShinniess() const { return m_shinniess; }
		void setShinniess(float val) { m_shinniess = val; }

	protected:


		RHANDLE m_diffuseMap;
		RHANDLE m_sepcularMap;
		glm::vec3   m_dffiuseColor;
		glm::vec3   m_sepcularColor;
		glm::vec3   m_ambientColor;
		float     m_shinniess;

		float*  m_vertices;
		GLuint   m_diffMapObj;
		GLuint   m_sepcMapObj;

		virtual void Release() override;

	private:

		

		virtual void BackupRenderState() override;
		virtual void RestoreRenderState() override;

	};

}

