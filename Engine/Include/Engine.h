#pragma once
#include "Sapphire.h"
#include "Variant.h"

namespace Sapphire
{

	class SAPPHIRE_CLASS Engine : public BaseObject
	{
		SAPPHIRE_OBJECT(Engine, BaseObject)

	public:

		Engine(Core* pCore);
		virtual ~Engine();

		bool Initialize(const VariantMap& parameters);

		//½âÎö²ÎÊý
		static VariantMap ParseParameters(const std::vector<std::string>& arguments);


		void RunFrame();

		void Update();

		void Render();

		void Exit();


	};

}