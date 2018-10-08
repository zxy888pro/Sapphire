#pragma once


#include "BaseLightMapMesh.h"


namespace Sapphire
{
	class StandardMaterialMesh : public BaseLightMapMesh
	{
	public:

		StandardMaterialMesh();

		virtual ~StandardMaterialMesh();



		virtual void Init() override;
		virtual void Render() override;


		virtual void Update(std::vector<SharedPtr<BaseLight>>& lightVec) override;

		float getShininess() const { return m_shininess; }
		void setShininess(float val) { m_shininess = val; }
		bool getShowOutline() const { return m_showOutline; }
		void setShowOutline(bool val) { m_showOutline = val; }
		float getOutlineSize() const { return m_outlineSize; }
		void setOutlineSize(float val) { m_outlineSize = val; }
	protected:

		virtual void Release() override;
		virtual bool LoadOutlineShader(const char*  vs, const char* ps);

		float m_shininess;
		bool  m_showOutline;
		float m_outlineSize;
		Shader* m_pOutlineShader;


	private:

		virtual void BackupRenderState() override;
		virtual void RestoreRenderState() override;

	};
}