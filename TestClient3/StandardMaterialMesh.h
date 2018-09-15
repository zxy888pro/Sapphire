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

	protected:





		virtual void Release() override;

	private:
		virtual void BackupRenderState() override;


		virtual void RestoreRenderState() override;

	};
}