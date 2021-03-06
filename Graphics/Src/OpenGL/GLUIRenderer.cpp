#include "GLUIRenderer.h"
#include "VertexBuffer.h"
#include "UI/UIElement.h"
#include "Math/Matrix4x4.h"
#include "Math/Matrix3x4.h"
#include "IDisplayContext.h"
#include "IGraphicDriver.h"


namespace Sapphire
{

	GLUIRenderer::GLUIRenderer(Core* pcore, GLGraphicDriver* pDriver) : IRenderer(pcore), m_pGraphicDriver(pDriver)
	{
		m_rootElement = new UIElment(pcore); //暂时创建一个测试UIElement
		Initialize();
	}

	GLUIRenderer::~GLUIRenderer()
	{

	}

	void GLUIRenderer::Initialize()
	{
		m_assert(m_pGraphicDriver);
		m_rootElement->SetName("root");
		m_rootElement->SetSize(IntVector2(400, 150));
		m_vertexBuffer = SharedPtr<VertexBuffer>(new VertexBuffer(m_pCore, m_pGraphicDriver));
		
		SubscribeEvent(ET_CORE_EVENT, EVENT_CORE_BEGINFRAME,SAPPHIRE_HANDLER(GLUIRenderer, OnBeginFrame));
		SubscribeEvent(ET_CORE_EVENT, EVENT_CORE_POSTUPDATE, SAPPHIRE_HANDLER(GLUIRenderer, OnPostUpdate));
		SubscribeEvent(ET_CORE_EVENT, EVENT_CORE_RENDERUPDATE, SAPPHIRE_HANDLER(GLUIRenderer, OnRenderUpdate));
		m_bIsInitilaized = true;
	}

	void GLUIRenderer::Clear()
	{

	}

	void GLUIRenderer::SetVertexData(VertexBuffer* dest, const std::vector<float>& vertexData)
	{
		if (vertexData.empty())
			return;

		//计算出顶点数
		uint numVerices = vertexData.size() / UI_VERTEX_SIZE;
		if (dest->GetVertexCount() < numVerices || dest->GetVertexCount() > numVerices * 2)
			dest->SetSize(numVerices, MASK_POSITION | MASK_COLOR | MASK_TEXCOORD1, true);  //设置顶点缓存区大小
		dest->SetData(&vertexData[0]);

	}

	void GLUIRenderer::Update(float timeStep)
	{

	}

	void GLUIRenderer::RenderUpdate()
	{
		m_bIsRenderered = false;

		m_batches.clear();
		m_vertexData.clear();
		//获取要渲染的非模态UI元素
		const IntVector2& rootSize = m_rootElement->GetSize();
		IntRect currentScissor = IntRect(0, 0, rootSize.x_, rootSize.y_);
		
		m_rootElement->GetBatches(m_batches, m_vertexData, currentScissor);

		m_batcheSize = m_batches.size();

	}

	void GLUIRenderer::Render(bool resetRenderTargets)
	{

		if (resetRenderTargets && m_bIsRenderered)
			return;

		SetVertexData(m_vertexBuffer, m_vertexData);
		Render(resetRenderTargets, m_vertexBuffer, m_batches, 0, m_batcheSize);
		m_bIsRenderered = true;
	}

	void GLUIRenderer::Render(bool resetRenderTargets, VertexBuffer* buffer, const std::vector<UIBatch>& batches, unsigned batchStart, unsigned batchEnd)
	{
		m_assert(m_pGraphicDriver && m_pGraphicDriver->IsInitialized());

		if (batches.empty())
			return;

		//设置变换矩阵, 把顶点坐标变换到0~2区间
		Vector2  invScreenSize = Vector2(1.0 / ((float)m_pGraphicDriver->GetDisplayContext()->GetWindowSize().x_), 1.0 / ((float)m_pGraphicDriver->GetDisplayContext()->GetWindowSize().y_));
		Vector2 scale(2.0f * invScreenSize.x_, -2.0f * invScreenSize.y_);
		Vector2 offset(-1.0f, 1.0f);

		Matrix4x4 projection(Matrix4x4::IDENTITY);
		projection.m00_ = scale.x_;
		projection.m03_ = offset.x_;
		projection.m11_ = scale.y_;
		projection.m13_ = offset.y_;
		projection.m22_ = 1.0f;
		projection.m23_ = 0.0f;
		projection.m33_ = 1.0f;

		
		//IShaderVariation*   noTextureVs = m_pGraphicDriver->

		//清理掉shader参数源
		m_pGraphicDriver->ClearParameterSources();
		m_pGraphicDriver->SetCullMode(CULL_CCW);
		m_pGraphicDriver->SetFillMode(FillMode::FILL_SOLID);
		m_pGraphicDriver->SetColorWrite(true); //开启颜色缓存区写入
		m_pGraphicDriver->SetDepthWrite(false); //UI不写入深度
		m_pGraphicDriver->SetStencilTest(false);  //UI不进行模板测试
		//是否重置所有renderTarget;
		if (resetRenderTargets)
			m_pGraphicDriver->ResetRenderTargets();
		m_pGraphicDriver->SetVertexBuffer(buffer);


		//纯顶点色vertex shader
		IShaderVariation* noTextureVS = m_pGraphicDriver->GetShader(VS, "Basic", "VERTEXCOLOR");
		IShaderVariation* noTexturePS = m_pGraphicDriver->GetShader(PS, "Basic", "VERTEXCOLOR");

		uint alphaFormat = GLGraphicDriver::GetHWAlphaFormat();

		for (int i = batchStart; i < batchEnd; i++)
		{
			const UIBatch& batch = batches[i];
			if (batch.GetVertexStart() == batch.GetVertexEnd())
			{
				continue;
			}

			IShaderVariation* vs = NULL;
			IShaderVariation* ps = NULL;

			if (batch.GetTexture() == NULL)
			{
				//无纹理，顶点色
				vs = noTextureVS;
				ps = noTexturePS;
			}
			else
			{
				//带纹理
			}
			m_pGraphicDriver->SetShaders(vs, ps);
			if (m_pGraphicDriver->NeedParameterUpdate(SP_OBJECT, this))
				m_pGraphicDriver->SetShaderParameter(VSP_MODEL, Matrix3x4::IDENTITY); //对象位置变化
			if (m_pGraphicDriver->NeedParameterUpdate(SP_CAMERA, this))
				m_pGraphicDriver->SetShaderParameter(VSP_VIEWPROJ, projection);  //相机投影
			if (m_pGraphicDriver->NeedParameterUpdate(SP_MATERIAL, this))
				m_pGraphicDriver->SetShaderParameter(PSP_MATDIFFCOLOR, Color(1.0f, 1.0f, 1.0f, 1.0f));

			m_pGraphicDriver->SetBlendMode(batch.GetBlendMode());
			m_pGraphicDriver->SetScissorTest(true, batch.GetScissor());
			m_pGraphicDriver->BindTexture(batch.GetTexture(),TextureUnit::TU_DIFFUSE);
			m_pGraphicDriver->Draw(TRIANGLE_LIST, batch.GetVertexStart() / UI_VERTEX_SIZE,
				(batch.GetVertexEnd() - batch.GetVertexStart()) / UI_VERTEX_SIZE);
			
		}


	}

	void GLUIRenderer::OnBeginFrame(ushort eEventType, ushort eEvent, void* eventData)
	{
		SAPPHIRE_LOG("GLUIRenderer::OnBeginFrame");
	}

	void GLUIRenderer::OnRenderUpdate(ushort eEventType, ushort eEvent, void* eventData)
	{
		SAPPHIRE_LOG("GLUIRenderer::OnRenderUpdate");
		RenderUpdate();
	}

	void GLUIRenderer::OnPostUpdate(ushort eEventType, ushort eEvent, void* eventData)
	{
		SAPPHIRE_LOG("GLUIRenderer::OnPostUpdate");
	}

	//void GLUIRenderer::Invoke(ushort eEventType, ushort eEvent, EventContext* src, void* eventData /*= NULL*/)
	//{
	//	switch (eEventType)
	//	{
	//	case ET_CORE_EVENT:
	//	{
	//		switch (eEvent)
	//		{
	//		case EVENT_CORE_BEGINFRAME:
	//		{

	//		}
	//			break;
	//		case EVENT_CORE_RENDERUPDATE:
	//		{
	//			RenderUpdate();
	//		}
	//		break;
	//		default:
	//			break;
	//		}
	//	}
	//		break;
	//	case ET_GRAPHIC_EVENT:
	//		break;
	//	default:
	//		break;
	//	}
	//}

	void GLUIRenderer::GetBatches(UIElment* element, IntRect curScissor)
	{
		
	}

}