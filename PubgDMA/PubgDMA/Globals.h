#pragma once
#include "Engine.h"
extern std::shared_ptr<Engine> EngineInstance;
extern std::string ProcessName;
struct _Local
{
	int Teamid;
	int SpectatedCount;

}inline Local;

struct OFFSET {
	uint64_t GNames_offset = 0x0;
	// 2024-11-16
	uint64_t UWorld = 0xFF9BDE8;
	uint64_t Decrypt = 0xE612A28;
	uint64_t GNames = 0x101FAE98;
	uint32_t ElementsPerChunk = 0x40E0;

	uint32_t Offset = 0x18;
	uint64_t NameIndexXor1 = 0xAF29A017;
	uint32_t NameIndexOne = 0xF;
	uint32_t NameIndexTwo = 0x10;
	uint64_t NameIndexXor2 = 0x4ABD1BAB;
	uint32_t NameIsROR = 0x1;

	uint32_t CurrentLevel = 0x30;
	uint32_t GameInstance = 0x160;
	uint32_t LocalPlayers = 0x30;
	uint32_t Actors = 0x198;
	uint32_t ObjectID = 0x8;
	uint32_t PlayerController = 0x30;
	uint32_t AcknowledgedPawn = 0x4A8;
	uint32_t PlayerCameraManager = 0x4D0;
	uint32_t RootComponent = 0xF0;
	uint32_t PlayerState = 0x428;
	uint32_t PlayerStatistics = 0x4B4;
	uint32_t Mesh = 0x5A8;
	uint32_t AnimScriptInstance = 0xDC8;
	uint32_t StaticMesh = 0xAB0;

	uint32_t Health = 0x940;
	uint32_t HeaFlag = 0x38;
	uint32_t Health1 = 0x970;
	uint32_t Health2 = 0x988;
	uint32_t Health3 = 0x954;
	uint32_t Health4 = 0x940;
	uint32_t Health5 = 0x955;
	uint32_t Health6 = 0x950;
	uint64_t HealthXorKey0 = 0xCEC7A59F;
	uint64_t HealthXorKey1 = 0x9B63B23E;
	uint64_t HealthXorKey2 = 0xCAF75ABD;
	uint64_t HealthXorKey3 = 0x3E38486F;
	uint64_t HealthXorKey4 = 0xE8911D0A;
	uint64_t HealthXorKey5 = 0x23DDAD1C;
	uint64_t HealthXorKey6 = 0x9456FC8;
	uint64_t HealthXorKey7 = 0xBD39B621;
	uint64_t HealthXorKey8 = 0xBAD7A58;
	uint64_t HealthXorKey9 = 0xA8EF3E87;
	uint64_t HealthXorKey10 = 0xE2752BB6;
	uint64_t HealthXorKey11 = 0x9F8ADBF4;
	uint64_t HealthXorKey12 = 0xBDE8FBD5;
	uint64_t HealthXorKey13 = 0x3E936F07;
	uint64_t HealthXorKey14 = 0x6F099E38;
	uint64_t HealthXorKey15 = 0xE2D72AE4;

	uint32_t GroggyHealth = 0x10F0;
	uint32_t LastTeamNum = 0x1558;
	uint32_t CharacterName = 0x1218;
	uint32_t SpectatedCount = 0x1498;
	uint32_t Eyes = 0x73C;

	uint32_t WorldToMap = 0x9B4;
	uint32_t ComponentToWorld = 0x220;
	uint32_t ComponentLocation = 0x230;
	uint32_t ComponentVelocity = 0x358;
	uint32_t CameraFov = 0x9F8;
	uint32_t CameraRot = 0x470;
	uint32_t CameraPos = 0x464;

	uint32_t ItemID = 0x268;
	uint32_t ItemTable = 0xA8;
	uint32_t DroppedItemGroup = 0x260;
	uint32_t DroppedItemGroup_UItem = 0x840;

	uint32_t WeaponProcessor = 0x968;
	uint32_t EquippedWeapons = 0x1F8;
	uint32_t CurrentWeaponIndex = 0x301;
	uint32_t WeaponTrajectoryData = 0x10C8;
	uint32_t TrajectoryConfig = 0x100;
	uint32_t ControlRotation_CP = 0xC90;
	uint32_t RecoilADSRotation_CP = 0x90C;
	uint32_t LeanLeftAlpha_CP = 0xCE4;
	uint32_t LeanRightAlpha_CP = 0xCE8;

	// uint32_t TrainingMapGrid = 0x0;
	uint32_t AimOffsets = 0x1A98;
	uint32_t ReplicatedMovement = 0x60;
	uint32_t VehicleRiderComponent = 0x2030;
	uint32_t LastVehiclePawn = 0x260;
}inline SDK;