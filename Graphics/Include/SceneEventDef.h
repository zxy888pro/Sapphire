#pragma once


enum SceneEventDef
{
	EVENT_SCENE_NODEADDED,    //节点添加
	EVENT_SCENE_NODE_RENAME,  //节点重命名
	EVENT_SCENE_NODE_REMOVE,
	EVENT_SCENE_COMPONENT_REMOVE,  //组件移除
	EVENT_SCENE_COMPONENT_ADDED,   
	EVENT_SCENE_COMPONENT_ACTIVECHANGED,  //组件激活状态改变
	EVENT_SCENE_MAXCOUNT
};