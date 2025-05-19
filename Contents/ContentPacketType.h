#pragma once

enum class EContentsPacketType : __int8
{
	OBJECT_UPDATE,
	WORLD_OBJECT_LIST,
	CHAT,
};

enum class EObjectUpdateType : __int8
{
	MOVE,
	DESTROY,
	CHAT
};
