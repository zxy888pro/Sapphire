#include "SceneEventDef.h"
#include "Component.h"
#include "Scene.h"
#include "Node.h"
#include "Variant.h"


namespace Sapphire
{

	Component::Component(Core* pCore) :BaseObject(pCore)
	{
		m_uID = pCore->GenUID();
		m_eCompType = ComponentType_Default;
		m_pNode = NULL;
	}

	Component::~Component()
	{

	}

	void Component::Remove()
	{
		if (m_pNode)
		{
			m_pNode->RemoveComponent(m_eCompType);
		}
	}

	Sapphire::Node* Component::GetNode() const
	{
		return m_pNode;
	}

	bool Component::SetActive(bool val)
	{
		if (val != m_bActive)
		{
			OnActiveChanged();
			m_bActive = val;

			 
			if (m_scene)
			{
				 
				VariantVector eventData;
				eventData.push_back(Variant(m_scene));
				eventData.push_back(Variant(m_pNode));
				eventData.push_back(Variant(this));

				m_scene->FireEvent(ET_SCENE_EVENT, EVENT_SCENE_COMPONENT_ACTIVECHANGED, &eventData); //发送一个场景组件激活态改变事件
			}
		}
		

		return m_bActive;
	}

	void Component::SetNode(Node* node)
	{
		m_pNode = node;
		OnNodeSet(node);
		
	}

	void Component::SetScene(Scene* scene)
	{
		m_scene = scene;
		OnSceneSet(scene);
	}

}