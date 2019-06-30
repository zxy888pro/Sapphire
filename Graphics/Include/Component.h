#pragma once

#include "Sapphire.h"


namespace Sapphire
{
	class Node;
	//组件类型
	enum  ComponentType
	{
		ComponentType_Default,
		ComponentType_Transform,   //变换组件
		ComponentType_Logic,	  //逻辑组件
		ComponentType_Drawable,	  //可绘制组件
		ComponentType_Camera,
		ComponentType_MaxCount
	};

	class Scene;

	//组件，场景节点的组件
	class SAPPHIRE_CLASS Component : public BaseObject
	{
		SAPPHIRE_OBJECT(Component, BaseObject)

		friend class Node;
		friend class Scene;

	public:
		Component(Core* pCore);
		virtual ~Component();
		void			Remove();  //从节点上移除该组件,如果没有别的引用，会被析构
		UINT			GetUID() const { return m_uID; }
		Node*			GetNode() const;

		bool			IsActive() const { return m_bActive; }
		bool			SetActive(bool val);
		void			SetNode(Node* node);
		void            SetScene(Scene* scene);
		Scene*			GetScene() const { return m_scene; }
		ComponentType   GetComponentType() { return m_eCompType; }

		         
	protected:

		virtual  void  OnMarkedDirty(Node* node){};   //被标记为脏
		virtual  void  OnNodeSet(Node* node) {};     //绑定节点
		virtual  void  OnSceneSet(Scene* scene){};	 //场景被设置
		virtual  void  OnActiveChanged(){};   //该组件被激活
		

		Node*					m_pNode;      //组件所属的节点
		ComponentType           m_eCompType;  //组件类型
		UINT					m_uID;		  //唯一ID
		bool					m_bActive;   //是否激活
		Scene*					m_scene;
		 
	};

}