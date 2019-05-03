#pragma once
#include "Graphics.h"
#include "GPUObject.h"


namespace Sapphire
{
	class GLGraphicDriver;
	//�������ݻ�����
	class ConstantBuffer :  public BaseObject, public GPUObject
	{
		SAPPHIRE_OBJECT(ConstantBuffer, BaseObject)
	public:

		ConstantBuffer(Core* pCore);

		virtual ~ConstantBuffer();


		virtual void Release() override;


		virtual void OnDeviceReset() override;

		bool SetSize(uint size); //���ô�С��������������󲻴����򴴽���

		uint GetSize() const;

		void SetParameter(uint offset, uint size, const void* pdata);

		void SetVector3ArrayParameter(unsigned offset, unsigned rows, const void* data);

		void Apply();

		bool IsDirty();

	protected:

	private:

		//Ӱ������
		SharedArrayPtr<byte> m_shadowData;
		///��������С
		unsigned m_uSize;
		//
		bool m_bDirty;
		GLGraphicDriver*  m_pDriver;
	};

}
