#pragma once
#include "ActorEntity.h"
#include "EngineStructs.h"
// For these offsets just 7 dumper the game and open Engine_Classes
class Engine
{

private:
	uint64_t OwningActor;
	int MaxPacket;
	int localTempId;
	CameraCacheEntry CameraEntry; // ScriptStruct Engine.CameraCacheEntry
	MinimalViewInfo CameraViewInfo; // ScriptStruct Engine.MinimalViewInfo
	std::vector<std::shared_ptr<ActorEntity>> Actors;

	uint64_t UWorld, CurrentLevel, GameInstance, LocalPlayers, PlayerController, AcknowledgedPawn, PlayerCameraManager, GNames;

public:
	Engine();
	inline bool InitDecrypt(uint64_t offset);
	void Cache();
	void UpdatePlayers();
	std::vector<std::shared_ptr<ActorEntity>> GetActors();
	CameraCacheEntry GetCameraCache();
	void RefreshViewMatrix(VMMDLL_SCATTER_HANDLE handle);
	uint64_t GetActorSize();
	uintptr_t xe_decrypt(const uintptr_t encrypted);
	DWORD DecryptCIndex(DWORD Encrypted);
	void GetGNames();
	std::string GetNames(DWORD ID);
};