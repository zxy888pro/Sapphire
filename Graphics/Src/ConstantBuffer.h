#pragma once
#include "Graphics.h"
#include "GPUObject.h"


namespace Sapphire
{
	class GLGraphicDriver;
	//变量数据缓冲区
	class ConstantBuffer :  public BaseObject, public GPUObject
	{
		SAPPHIRE_OBJECT(ConstantBuffer, BaseObject)
	public:

		ConstantBuffer(Core* pCore);

		virtual ~ConstantBuffer();


		virtual void Release() override;


		virtual void OnDeviceReset() override;

		bool SetSize(uint size); //设置大小，如果缓冲区对象不存在则创建它

		uint GetSize() const;

		void SetParameter(uint offset, uint size, const void* pdata);

		void SetVector3ArrayParameter(unsigned offset, unsigned rows, const void* data);

		void Apply();

		bool IsDirty();

	protected:

	private:

		//影子数据
		SharedArrayPtr<byte> m_shadowData;
		///缓冲区大小
		unsigned m_uSize;
		//
		bool m_bDirty;
		GLGraphicDriver*  m_pDriver;
	};

}
