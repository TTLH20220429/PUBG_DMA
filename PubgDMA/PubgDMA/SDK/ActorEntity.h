#pragma once
#include "EngineStructs.h"
enum class EPlayerRole: uint8_t
{
	EPlayerRole__VE_None = 0,
	EPlayerRole__VE_Slasher = 1,
	EPlayerRole__VE_Camper = 2,
	EPlayerRole__VE_Observer = 3,
	EPlayerRole__Max = 4,
	EPlayerRole__EPlayerRole_MAX = 5
};

struct Index
{
	int Head,
		neck,
		pelvis,
		Lshoulder,
		Lelbow,
		Lhand,
		Rshoulder,
		Relbow,
		Rhand;
	int	Lbuttock,
		Lknee,
		Lfoot,
		Rbuttock,
		Rknee,
		Rfoot;
};

class FMatrix
{
public:
	float _11, _12, _13, _14;
	float _21, _22, _23, _24;
	float _31, _32, _33, _34;
	float _41, _42, _43, _44;
	float m[4][4];
	FMatrix MatrixMultiplication(const FMatrix& other);
};
struct FQuat
{
	float X;
	float Y;
	float Z;
	float W;
};
struct FTransform
{
	FQuat Rotation;
	FQuat Translation;
	FQuat Scale3D;

	FMatrix ToMatrixWithScale();
};


class ActorEntity
{
public:

	FTransform Head, neck, pelvis, Lshoulder, Lelbow, Lhand, Rshoulder, Relbow, Rhand,
		Lbuttock, Lknee, Lfoot, Rbuttock, Rknee, Rfoot;
	UEVector Head3D, neck3D, pelvis3D, Lshoulder3D, Lelbow3D, Lhand3D, Rshoulder3D, Relbow3D, Rhand3D,
		Lbuttock3D, Lknee3D, Lfoot3D, Rbuttock3D, Rknee3D, Rfoot3D;


	uint64_t Class = 0;
	int PlayerRole;
	bool isDie = true;
	int TempId;
	Index index{};
	uint64_t PlayerState, RootComponent, MeshLastTeamNum, Mesh, LastTeamNum, BoneArray, AcknowledgedPawn, RelativeLocation;
	FTransform ToWorld;
	bool isCheck = FALSE;
	std::wstring Name = LIT(L"Entity");
	UEVector UEPosition;
	Vector3 Position;
	ActorEntity(uint64_t address, VMMDLL_SCATTER_HANDLE handle);
	void SetUp1(VMMDLL_SCATTER_HANDLE handle);
	void SetUp2(VMMDLL_SCATTER_HANDLE handle);
	void SetUp3();
	uint64_t GetClass();
	int GetPlayerRole();
	std::wstring GetName();
	Vector3 GetPosition();
	void UpdatePosition(VMMDLL_SCATTER_HANDLE handle);
	UEVector GetBoneMatrix(FTransform bone);
	void UpdateBone(VMMDLL_SCATTER_HANDLE handle);
};