#pragma once


enum EventType
{
	ET_SUBSYTEM_EVENT,  //��ϵͳ�¼�
	ET_OBJECT_EVENT,    //�����¼�
	ET_OTHER_EVENT,	//ȫ�ֹ㲥
	ET_MAXCOUNT
};


enum EventDef
{
	//��ϵͳ�¼�
	EVENT_SUBSYS_WORKCOMLPETED,
	EVENT_SUBSYS_RESOURCEREQUEST_EVENT,
	//�����¼�
	EVENT_OBJECT_CUSTOM_EVENT,
	//ȫ��
	
	EVENT_MAXCOUNT
};
