#pragma once
#include "Room.h"

// Ό³Έν :
class URoomManager
{
public:
	// constrcuter destructer
	URoomManager();
	~URoomManager();

	// delete Function
	URoomManager(const URoomManager& _Other) = delete;
	URoomManager(URoomManager&& _Other) noexcept = delete;
	URoomManager& operator=(const URoomManager& _Other) = delete;
	URoomManager& operator=(URoomManager&& _Other) noexcept = delete;

	void CreateRoom(class AGameMode* _GameMode);

	//std::vector<ARoom*>& GetRoomsRef()
	//{
	//	std::vector<ARoom*> NewRooms;
	//	for (int i = 0; i < Rooms.size(); i++)
	//	{
	//		NewRooms.push_back(Rooms[i].get());
	//	}
	//	return NewRooms;
	//}

	std::vector<std::shared_ptr<ARoom>> GetRoomsRef()
	{
		return Rooms;
	}

	ARoom* GetPrevRoom()
	{
		return PrevRoom;
	}
	void SetPrevRoom(ARoom* _Room)
	{
		PrevRoom = _Room;
	}

protected:

private:
	inline static class AGameMode* GameMode = nullptr;
	std::shared_ptr<ARoom> CreateRoom(std::string_view _RoomName, std::string_view _BackgroundName, std::string_view _PixelCollisionName, FVector _Size, float _ScaleRatio = 1.0f);
	void SetInitCurRoom(ARoom* _InitRoom);
	void LoadPixelCollisionTexture(ARoom* _Room, UEngineWinImage* _BmpTexture, std::string_view _FileName, FVector _Size, float _ScaleRatio = 1.0f);

	class ARoom* PrevRoom = nullptr;
	std::vector<std::shared_ptr<ARoom>> Rooms;
};

