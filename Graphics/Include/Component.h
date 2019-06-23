#pragma once

#include "Sapphire.h"


namespace Sapphire
{
	class Node;
	//�������
	enum  ComponentType
	{
		ComponentType_Default,
		ComponentType_Transform,   //�任���
		ComponentType_Logic,	  //�߼����
		ComponentType_MaxCount
	};

	//����������ڵ�����
	class SAPPHIRE_CLASS Component : public BaseObject
	{
		SAPPHIRE_OBJECT(Component, BaseObject)

		friend class Node;
		friend class Scene;
	public:
		Component(Core* pCore);
		virtual ~Component();
		void			Remove();  //�ӽڵ����Ƴ������,���û�б�����ã��ᱻ����
		UINT			GetUID() const { return m_uID; }
		WeakPtr<Node>   GetNode() const;

		bool			IsActive() const { return m_bActive; }
		bool			SetActive(bool val);
		void			SetNode(WeakPtr<Node> node);

		         
	protected:

		virtual  void  OnMarkedDirty(Node* node){};
		virtual  void  OnNodeSet(WeakPtr<Node> node) {};
		

		WeakPtr<Node>         m_pNode;      //��������Ľڵ�
		ComponentType           m_eCompType;  //�������
		UINT					m_uID;		  //ΨһID
		bool					m_bActive;   //�Ƿ񼤻�
		 
	};

}