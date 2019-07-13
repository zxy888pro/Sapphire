#pragma once


#include "Sapphire.h"
#include "Serializer.h"
#include "Deserializer.h"


namespace Sapphire
{

	/// �ӑB�{����С�ľ��n�����K��������һ�����xȡ�͌���
	class SAPPHIRE_CLASS VectorBuffer : public Serializer, public Deserializer
	{
	public:

		VectorBuffer();

		VectorBuffer(const std::vector<byte>& data);

		VectorBuffer(const void* data, unsigned size);

		VectorBuffer(Deserializer& source, unsigned size);

		/// �ľ��n�^�xȡ�ֹ����������،��H�xȡ���ֹ���
		virtual unsigned Read(void* dest, unsigned size);
		// �O�þ��n�^�Į�ǰ�_ʼλ��
		virtual unsigned Seek(unsigned position);
		/// ���n�^�����ֹ����������،��H������ֹ���
		virtual unsigned Write(const void* data, unsigned size);
		/// ������һ�����n�^�O�Ô���
		void SetData(const std::vector<byte>& data);
		/// ������һ�΃ȴ��O�Ô���
		void SetData(const void* data, unsigned size);
		/// ��һ�������л������O�Ô���
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