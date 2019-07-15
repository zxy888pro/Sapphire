#pragma once
#include "Sapphire.h"
#include "TimeSystem.h"
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

		//��������
		static VariantMap ParseParameters(const std::vector<std::string>& arguments);

		static bool HasParameter(const VariantMap& parameters, const String& parameter);

		static const Variant
			& GetParameter(const VariantMap& parameters, const String& parameter, const Variant& defaultValue = Variant::EMPTY);

		//����һ֡
		void RunFrame();

		//����֡�����¼�֪ͨ����ϵͳ����
		void Update();
		//֡�����Ժ���Ⱦ
		void Render();

		void Exit();

		bool IsExiting()  const { return m_bExiting; }

		bool IsInitialized() const { return m_bInitialized; }

		//Ӧ��֡�����ƣ� ������һ֡��ʱ�䲽��,�����Ҫ����һ��ʱ�������
		void ApplyFrameLimit();	 
		 
		unsigned GetMinFps() const { return m_uMinFps; }

		void SetMinFps(unsigned val) { m_uMinFps = val; }


		unsigned GetMaxFps() const { return m_uMaxFps; }

		void SetMaxFps(unsigned val) { m_uMaxFps = val; }

		unsigned GetMaxInactiveFps() const { return m_uMaxInactiveFps; }

		void SetMaxInactiveFps(unsigned val) { m_uMaxInactiveFps = val; }

		bool IsPauseMinimized() const { return m_bPauseMinimized; }

		void SetPauseMinimized(bool val) { m_bPauseMinimized = val; }

		//// �����һ֡��ʱ�䲽���� ��ApplyFrameLimit������
		float GetNextTimeStep() const { return m_uTimeStep; }
		///  ������һ֡��ʱ�䲽��
		void SetNextTimeStep(float seconds);
			
	private:

		void DoExit();


		/// ֡����timer
		HiresTimer m_frameTimer;
		/// Ϊ��ƽ������֮ǰ��ʱ�䲽��
		std::vector<float> m_lastTimeSteps;
		/// ��һ֡��ʱ�䲽��
		float m_uTimeStep;
		/// �ö���֡��ƽ��ֵ��ƽ��ʱ�䲽��
		unsigned m_uTimeStepSmoothing;
		/// ÿ����С֡��
		unsigned m_uMinFps;
		/// ÿ����С֡��
		unsigned m_uMaxFps;
		/// ��appû��ȡ�����뽹��ʱ��ÿ�����֡��
		unsigned m_uMaxInactiveFps;
		/// �Ƿ���С���ı�־
		bool m_bPauseMinimized;
		//	��ʼ����־
		bool m_bInitialized;
		//  �˳���־
		bool m_bExiting;


	};

}