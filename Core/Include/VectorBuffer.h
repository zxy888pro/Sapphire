#pragma once


#include "Sapphire.h"
#include "Serializer.h"
#include "Deserializer.h"


namespace Sapphire
{

	/// 動態調整大小的緩衝器，並且能作為一個流讀取和寫入
	class SAPPHIRE_CLASS VectorBuffer : public Serializer, public Deserializer
	{
	public:

		VectorBuffer();

		VectorBuffer(const std::vector<byte>& data);

		VectorBuffer(const void* data, unsigned size);

		VectorBuffer(Deserializer& source, unsigned size);

		/// 從緩衝區讀取字節數據，返回實際讀取的字節數
		virtual unsigned Read(void* dest, unsigned size);
		// 設置緩衝區的當前開始位置
		virtual unsigned Seek(unsigned position);
		/// 向緩衝區寫入字節數據，返回實際寫入的字節數
		virtual unsigned Write(const void* data, unsigned size);
		/// 從另外一個緩衝區設置數據
		void SetData(const std::vector<byte>& data);
		/// 從另外一段內存設置數據
		void SetData(const void* data, unsigned size);
		/// 從一個反序列化對象設置數據
		void SetData(Deserializer& source, unsigned size);

		void Clear();

		void Resize(unsigned size);

		const unsigned char* GetData() const { return m_uSize ? &buffer_[0] : 0; }

		unsigned char* GetModifiableData() { return m_uSize ? &buffer_[0] : 0; }

		const std::vector<byte>& GetBuffer() const { return buffer_; }

	private:

		std::vector<byte>   buffer_;

	};

}