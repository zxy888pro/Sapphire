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

	class Scene;

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
		//销毁之前执行
		virtual void		OnDestory(){};  

		SharedPtr<Node>     GetChild(int index = 0) const;   
		SharedPtr<Node>     GetChild(const char* name) const;
		SharedPtr<Node>     GetChild(const std::string& name) const;
		const std::vector<SharedPtr<Node>>& GetChildren() const { return m_children; }
		void                GetAllChildren(std::vector<SharedPtr<Node>>& dest, bool recursive = false);
		bool				AddChild(SharedPtr<Node> child, uint index = 0);
		bool                RemoveChild(Node* node); //从孩子节点中找到node并移除
		void				RemoveAllChildren();  //移除所有的孩子节点
		void				RemoveChildren(bool recursive);
		uint				GetNumChildren(bool recursive = false); //是否递归获取所有孩子数目
		void				Remove(); //从父节点删除，如果没有引用会导致对象被删除
		void				SetParent(Node* parent);
		WeakPtr<Node>		GetParent() const;

		//设置场景根节点, 由Scene调用
		void SetScene(Scene* scene);
		//重置场景,ID或所有者  由Scene调用
		void ResetScene();

		Scene* GetScene() const { return m_scene; }

		UINT64				GetInstanceID() const { return m_UID; };
		bool				IsActive() const { return m_bActive; }
		void				SetActive(bool val) { m_bActive = val; }
		void				SetDeepActive(bool enable);  //影响到所有的孩子节点
		std::string			GetNodeName() const { return m_nodeName; }
		void				SetNodeName(std::string val);
		const StringHash&	 GetNameHash() const { return m_nodeNameHash; }


		//标记节点和子节点为脏，需要重写计算世界空间变换。 通知所有监听组件
		void MarkDirty();
		//是否为脏
		bool IsDirty() const { return m_bDirty; }
		void ClearDirtyFlag() { m_bDirty = false; }

		//获取组件
		SharedPtr<Component>			 GetComponent(ComponentType type) const;
		bool							 HasComponent(ComponentType type) const;
		int								 GetNumComponents() const;
		void                             AddComponent(SharedPtr<Component> component);
		void                             RemoveComponent(ComponentType type);
		void							 RemoveAllComponent();   //移除所有组件
		

		void							 AddListener(SharedPtr<Component> component);   //增加一个监听的组件
		void							 RemoveListener(SharedPtr<Component> component);   //增加一个监听的组件
		virtual void                     Destroy();  //销毁节点，清理内容
		
		template <class T>  T*		 GetComponent() const;

		typedef std::map<ushort, SharedPtr<Component>>  NODE_COMPONENT_MAP;
		typedef std::map<ushort, SharedPtr<Component>>::iterator  NODE_COMPONENT_MAP_ITERATOR;
		typedef std::map<ushort, SharedPtr<Component>>::const_iterator  NODE_COMPONENT_MAP_CITERATOR;

		typedef std::vector<SharedPtr<Node>>						NODE_CHILDREN_LIST;
		typedef std::vector<SharedPtr<Node>>::iterator				NODE_CHILDREN_LIST_ITERATOR;
		typedef std::vector<SharedPtr<Node>>::const_iterator		NODE_CHILDREN_LIST_CITERATOR;

	protected:

		WeakPtr<Node>						m_parent;
		std::vector<SharedPtr<Node>>	    m_children;  // （避免循环引用）
		std::vector<WeakPtr<Component>>   m_listeners;    //节点监听者
		std::map<ushort, SharedPtr<Component>>   m_componentMap;    //每种类型的组件只能有一个

		std::string							m_nodeName;  //节点名字
		StringHash							m_nodeNameHash;
		//是否激活
		bool								m_bActive;
		NodeType							m_eNodeType;
		UINT64								m_UID;   //id用对象地址来表示ID
		Scene*								m_scene;   //场景根节点
	    mutable bool						m_bDirty;   //脏标志
	};


	template <class T>
	 T* Sapphire::Node::GetComponent() const
	{
		for (std::map<ushort, SharedPtr<Component>>::const_iterator it = m_componentMap.begin(); it != m_componentMap.end(); it++)
		{
			
			T* component = dynamic_cast<T*>(it->second.Get());
			if (component)
				return component;
		}
		return NULL;
	}

}