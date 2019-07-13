#pragma once

#include "Sapphire.h"
#include "Serializer.h"
#include "Deserializer.h"


namespace Sapphire
{

	class SAPPHIRE_CLASS MemoryBuffer : public Serializer, public Deserializer
	{
	public:

		 
		MemoryBuffer(void* data, unsigned size);

		MemoryBuffer(const void* data, unsigned size);

		MemoryBuffer(std::vector<byte>& data);

	
		virtual unsigned Read(void* dest, unsigned size);

		virtual unsigned Seek(unsigned position);

		virtual unsigned Write(const void* data, unsigned size);


		unsigned char* GetData() { return buffer_; }


		bool IsReadOnly() { return readOnly_; }

	private:
	 
		unsigned char* buffer_;
	 
		bool readOnly_;



	};



}