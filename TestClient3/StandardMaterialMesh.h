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
	protected:

		virtual void Release() override;

		float m_shininess;

	private:

		virtual void BackupRenderState() override;
		virtual void RestoreRenderState() override;

	};
}