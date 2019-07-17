#pragma once


enum EventType
{
	ET_CORE_EVENT,  //核心系统事件
	ET_SCENE_EVENT,    //场景事件
	ET_GRAPHIC_EVENT,  //图形渲染事件
	ET_ENGINE_EVENT,  //引擎事件
	ET_OTHER_EVENT,	//全局广播
	ET_MAXCOUNT
};


enum CoreEventDef
{
	//子系统事件
	EVENT_CORE_WORKCOMLPETED,
	EVENT_CORE_RESOURCEREQUEST_EVENT,
	EVENT_CORE_RESOURCELOADEND_EVENT,
	EVENT_CORE_BEGINFRAME,   //帧的开始
	EVENT_CORE_ENDFRAME,	//帧的结束
	EVENT_CORE_UPDATE,		//逻辑更新
	EVENT_CORE_POSTUPDATE,	//后逻辑更新
	EVENT_CORE_RENDERUPDATE,  //渲染更新
	EVENT_CORE_POSTRENDERUPDATE, //后渲染更新
	//对象事件
	EVENT_OBJECT_CUSTOM_EVENT,
	
	EVENT_CORE_MAXCOUNT
};
