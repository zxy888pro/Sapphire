#pragma once
#include "Sapphire.h"
#include "Component.h"

namespace Sapphire
{
	enum NodeType
	{
		NodeType_Node,
		NodeType_SceneNode, //场景节点
		NodeType_UIElement,  //UI元素节点
		NodeType_Scene,    //整个场景根节点
		NodeType_Canvas,   //UI根节点
		NodeType_MaxCount
	};


	class SAPPHIRE_CLASS Node : public BaseObject
	{
		SAPPHIRE_OBJECT(Node, BaseObject)
	public:

		Node(Core* pCore);
		virtual ~Node();

		virtual void		OnCreate(){};		
		//打开时执行
		virtual void		OnEnable(){};
		//关闭时执行
		virtual void		OnDisable(){};
		//销毁时执行
		virtual void		OnDestory(){};

		SharedPtr<Node>     GetChild(int index = 0) const;   
		SharedPtr<Node>     GetChild(const char* name) const;
		SharedPtr<Node>     GetChild(const std::string& name) const;
		const std::vector<SharedPtr<Node>>& GetChildren() const { return m_children; }
		bool				AddChild(SharedPtr<Node> child);
		bool				RemoveChild(const char* childName);
		bool				RemoveChild(int index);
		void				RemoveAllChildren();
		void				Remove(); //从父节点删除，如果没有引用会导致对象被删除
		void				SetParent(SharedPtr<Node> parent);
		WeakPtr<Node>		GetParent() const;



		UINT				GetInstanceID() const { return m_UID; };
		bool				IsActive() const { return m_bActive; }
		void				SetActive(bool val) { m_bActive = val; }
		void				SetDeepActive(bool enable);  //影响到所有的孩子节点
		std::string			GetNodeName() const { return m_nodeName; }
		void				SetNodeName(std::string val);
		const StringHash&	 GetNameHash() const { return m_nodeNameHash; }

		//获取组件
		SharedPtr<Component>			 GetComponent(ComponentType type) const;
		bool							 HasComponent(ComponentType type) const;
		int								 GetComponentsNum() const;
		void                             AddComponent(SharedPtr<Component> component);
		void                             RemoveComponent(ComponentType type);
		void						     RemoveComponent(SharedPtr<ComponentType> component);
		


	protected:

		WeakPtr<Node>						m_parent;
		std::vector<SharedPtr<Node>>	    m_children;  // （避免循环引用）
		std::map<ushort, SharedPtr<Component>>   m_componentMap;    //每种类型的组件只能有一个

		std::string							m_nodeName;  //节点名字
		StringHash							m_nodeNameHash;
		//是否激活
		bool								m_bActive;
		NodeType							m_eNodeType;
		UINT								m_UID;   //id用对象地址来表示ID

	};

}