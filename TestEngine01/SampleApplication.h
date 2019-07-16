#pragma once
#include "Sapphire.h"
#include "Application.h"

namespace Sapphire
{
	class SampleApplication : public Application
	{
		SAPPHIRE_OBJECT(SampleApplication, Application)
	public:
		SampleApplication(Sapphire::Core* pCore);
		virtual ~SampleApplication();

		virtual void Setup() override;

		virtual void Start() override;

		virtual void Stop() override;

	};
}

