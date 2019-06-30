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
		ComponentType_Drawable,	  //�ɻ������
		ComponentType_Camera,
		ComponentType_MaxCount
	};

	class Scene;

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
		Node*			GetNode() const;

		bool			IsActive() const { return m_bActive; }
		bool			SetActive(bool val);
		void			SetNode(Node* node);
		void            SetScene(Scene* scene);
		Scene*			GetScene() const { return m_scene; }
		ComponentType   GetComponentType() { return m_eCompType; }

		         
	protected:

		virtual  void  OnMarkedDirty(Node* node){};   //�����Ϊ��
		virtual  void  OnNodeSet(Node* node) {};     //�󶨽ڵ�
		virtual  void  OnSceneSet(Scene* scene){};	 //����������
		virtual  void  OnActiveChanged(){};   //�����������
		

		Node*					m_pNode;      //��������Ľڵ�
		ComponentType           m_eCompType;  //�������
		UINT					m_uID;		  //ΨһID
		bool					m_bActive;   //�Ƿ񼤻�
		Scene*					m_scene;
		 
	};

}