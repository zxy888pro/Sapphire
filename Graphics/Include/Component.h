#pragma once

#include "Sapphire.h"


namespace Sapphire
{
	class Node;
	class Scene;
	//组件类型
	enum  ComponentType
	{
		ComponentType_Default,
		ComponentType_Transform,   //变换组件
		ComponentType_MaxCount
	};

	//组件，场景节点的组件
	class SAPPHIRE_CLASS Component : public BaseObject
	{
		SAPPHIRE_OBJECT(Component, BaseObject)
	public:
		Component(Core* pCore);
		virtual ~Component();
		void	Remove();  //从节点上移除该组件,如果没有别的引用，会被析构
		UINT	GetUID() const { return m_uID; }
		SharedPtr<Node>        GetNode() const;

		bool    IsActive() const { return m_bActive; }
		bool    SetActive(bool val);
		


	protected:

		SharedPtr<Node>         m_pNode;      //组件所属的节点
		ComponentType           m_eCompType;  //组件类型
		UINT					m_uID;		  //唯一ID
		bool					m_bActive;   //是否激活
		 
	};

}