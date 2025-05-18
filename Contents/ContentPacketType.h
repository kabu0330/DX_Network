#pragma once

enum class EContentsPacketType : __int8
{
	OBJECT_UPDATE = -2,
	WORLD_OBJECT_LIST = -1,
	CHAT = 1,
};

enum class EObjectUpdateType : __int8
{
	MOVE,
	DESTROY,
	CHAT
};
