#include "Pch.h"
#include "ActorEntity.h"
#include "Camera.h"
#include "Globals.h"
FMatrix FTransform::ToMatrixWithScale()
{
	FMatrix m;

	m._41 = Translation.X;
	m._42 = Translation.Y;
	m._43 = Translation.Z;

	float x2 = Rotation.X + Rotation.X;
	float y2 = Rotation.Y + Rotation.Y;
	float z2 = Rotation.Z + Rotation.Z;

	float xx2 = Rotation.X * x2;
	float yy2 = Rotation.Y * y2;
	float zz2 = Rotation.Z * z2;
	m._11 = (1.0f - (yy2 + zz2)) * Scale3D.X;
	m._22 = (1.0f - (xx2 + zz2)) * Scale3D.Y;
	m._33 = (1.0f - (xx2 + yy2)) * Scale3D.Z;


	float yz2 = Rotation.Y * z2;
	float wx2 = Rotation.W * x2;
	m._32 = (yz2 - wx2) * Scale3D.Z;
	m._23 = (yz2 + wx2) * Scale3D.Y;


	float xy2 = Rotation.X * y2;
	float wz2 = Rotation.W * z2;
	m._21 = (xy2 - wz2) * Scale3D.Y;
	m._12 = (xy2 + wz2) * Scale3D.X;


	float xz2 = Rotation.X * z2;
	float wy2 = Rotation.W * y2;
	m._31 = (xz2 + wy2) * Scale3D.Z;
	m._13 = (xz2 - wy2) * Scale3D.X;

	m._14 = 0.0f;
	m._24 = 0.0f;
	m._34 = 0.0f;
	m._44 = 1.0f;

	return m;
}


FMatrix FMatrix::MatrixMultiplication(const FMatrix& other)
{
	FMatrix NewMatrix;

	NewMatrix._11 = this->_11 * other._11 + this->_12 * other._21 + this->_13 * other._31 + this->_14 * other._41;
	NewMatrix._12 = this->_11 * other._12 + this->_12 * other._22 + this->_13 * other._32 + this->_14 * other._42;
	NewMatrix._13 = this->_11 * other._13 + this->_12 * other._23 + this->_13 * other._33 + this->_14 * other._43;
	NewMatrix._14 = this->_11 * other._14 + this->_12 * other._24 + this->_13 * other._34 + this->_14 * other._44;
	NewMatrix._21 = this->_21 * other._11 + this->_22 * other._21 + this->_23 * other._31 + this->_24 * other._41;
	NewMatrix._22 = this->_21 * other._12 + this->_22 * other._22 + this->_23 * other._32 + this->_24 * other._42;
	NewMatrix._23 = this->_21 * other._13 + this->_22 * other._23 + this->_23 * other._33 + this->_24 * other._43;
	NewMatrix._24 = this->_21 * other._14 + this->_22 * other._24 + this->_23 * other._34 + this->_24 * other._44;
	NewMatrix._31 = this->_31 * other._11 + this->_32 * other._21 + this->_33 * other._31 + this->_34 * other._41;
	NewMatrix._32 = this->_31 * other._12 + this->_32 * other._22 + this->_33 * other._32 + this->_34 * other._42;
	NewMatrix._33 = this->_31 * other._13 + this->_32 * other._23 + this->_33 * other._33 + this->_34 * other._43;
	NewMatrix._34 = this->_31 * other._14 + this->_32 * other._24 + this->_33 * other._34 + this->_34 * other._44;
	NewMatrix._41 = this->_41 * other._11 + this->_42 * other._21 + this->_43 * other._31 + this->_44 * other._41;
	NewMatrix._42 = this->_41 * other._12 + this->_42 * other._22 + this->_43 * other._32 + this->_44 * other._42;
	NewMatrix._43 = this->_41 * other._13 + this->_42 * other._23 + this->_43 * other._33 + this->_44 * other._43;
	NewMatrix._44 = this->_41 * other._14 + this->_42 * other._24 + this->_43 * other._34 + this->_44 * other._44;

	return NewMatrix;
}
void GetIndex(Index& index)
{
	index.Head = 35;//66ÃÏ¡È∏«
	index.neck = 5;
	index.pelvis = 1;
	index.Lshoulder = 88;
	index.Lelbow = 89;
	index.Lhand = 90;
	index.Rshoulder = 115;
	index.Relbow = 116;
	index.Rhand = 117;
	index.Lbuttock = 177;
	index.Lknee = 173;
	index.Lfoot = 174;
	index.Rbuttock = 183;
	index.Rknee = 179;
	index.Rfoot = 180;

}
ActorEntity::ActorEntity(uint64_t address,VMMDLL_SCATTER_HANDLE handle)
{
	Class = address;
	if(!address)
		return;
	TargetProcess.AddScatterReadRequest(handle, Class + SDK.PlayerState, reinterpret_cast<void*>(&PlayerState), sizeof(uint64_t));
	TargetProcess.AddScatterReadRequest(handle, Class + SDK.AcknowledgedPawn, reinterpret_cast<void*>(&AcknowledgedPawn), sizeof(uint64_t));
	TargetProcess.AddScatterReadRequest(handle, Class + SDK.RootComponent, reinterpret_cast<void*>(&RootComponent),sizeof(uint64_t));
	TargetProcess.AddScatterReadRequest(handle, Class + SDK.Mesh, reinterpret_cast<void*>(&Mesh), sizeof(uint64_t));
	TargetProcess.AddScatterReadRequest(handle, Class + SDK.LastTeamNum, reinterpret_cast<void*>(&LastTeamNum), sizeof(uint64_t));
	GetIndex(index);
}
void ActorEntity::SetUp1(VMMDLL_SCATTER_HANDLE handle)
{
	if (!Class)
		return;

	if (!RootComponent)
		return;
	if (Mesh < 65535) {
		return;
	}
	if (PlayerState) // players aren't pawns
	{
		TargetProcess.AddScatterReadRequest(handle, Mesh + SDK.StaticMesh, reinterpret_cast<void*>(&BoneArray), sizeof(BoneArray));
		TargetProcess.AddScatterReadRequest(handle, Mesh + SDK.ComponentToWorld, reinterpret_cast<void*>(&ToWorld), sizeof(FTransform));
	}
}

UEVector ActorEntity::GetBoneMatrix(FTransform bone)
{
	FMatrix Matrix;
	Matrix = bone.ToMatrixWithScale().MatrixMultiplication(ToWorld.ToMatrixWithScale());
	return UEVector(Matrix._41, Matrix._42, Matrix._43);
}

void ActorEntity::SetUp2(VMMDLL_SCATTER_HANDLE handle)
{

	if (!Class)
		isCheck = false; return;
	if (!RootComponent)
		isCheck = false;
		return;
	if (Mesh < 65535) {
		isCheck = false;
		return;
	}

	if (isCheck && PlayerState) // players aren't pawns
	{
		TargetProcess.AddScatterReadRequest(handle, BoneArray + index.Head * 0x30, reinterpret_cast<void*>(&Head), sizeof(FTransform));
		TargetProcess.AddScatterReadRequest(handle, BoneArray + index.neck * 0x30, reinterpret_cast<void*>(&neck), sizeof(FTransform));
		TargetProcess.AddScatterReadRequest(handle, BoneArray + index.pelvis * 0x30, reinterpret_cast<void*>(&pelvis), sizeof(FTransform));
		TargetProcess.AddScatterReadRequest(handle, BoneArray + index.Lshoulder * 0x30, reinterpret_cast<void*>(&Lshoulder), sizeof(FTransform));
		TargetProcess.AddScatterReadRequest(handle, BoneArray + index.Lelbow * 0x30, reinterpret_cast<void*>(&Lelbow), sizeof(FTransform));
		TargetProcess.AddScatterReadRequest(handle, BoneArray + index.Lhand * 0x30, reinterpret_cast<void*>(&Lhand), sizeof(FTransform));
		TargetProcess.AddScatterReadRequest(handle, BoneArray + index.Rshoulder * 0x30, reinterpret_cast<void*>(&Rshoulder), sizeof(FTransform));
		TargetProcess.AddScatterReadRequest(handle, BoneArray + index.Relbow * 0x30, reinterpret_cast<void*>(&Relbow), sizeof(FTransform));
		TargetProcess.AddScatterReadRequest(handle, BoneArray + index.Rhand * 0x30, reinterpret_cast<void*>(&Rhand), sizeof(FTransform));
		TargetProcess.AddScatterReadRequest(handle, BoneArray + index.Lbuttock * 0x30, reinterpret_cast<void*>(&Lbuttock), sizeof(FTransform));
		TargetProcess.AddScatterReadRequest(handle, BoneArray + index.Lknee * 0x30, reinterpret_cast<void*>(&Lknee), sizeof(FTransform));
		TargetProcess.AddScatterReadRequest(handle, BoneArray + index.Lfoot * 0x30, reinterpret_cast<void*>(&Lfoot), sizeof(FTransform));
		TargetProcess.AddScatterReadRequest(handle, BoneArray + index.Rbuttock * 0x30, reinterpret_cast<void*>(&Rbuttock), sizeof(FTransform));
		TargetProcess.AddScatterReadRequest(handle, BoneArray + index.Rknee * 0x30, reinterpret_cast<void*>(&Rknee), sizeof(FTransform));
		TargetProcess.AddScatterReadRequest(handle, BoneArray + index.Rfoot * 0x30, reinterpret_cast<void*>(&Rfoot), sizeof(FTransform));

	}
}
void ActorEntity::SetUp3()
{
	Head3D = GetBoneMatrix(Head);
	neck3D = GetBoneMatrix(neck);
	pelvis3D = GetBoneMatrix(pelvis);
	Lshoulder3D = GetBoneMatrix(Lshoulder);
	Lelbow3D = GetBoneMatrix(Lelbow);
	Lhand3D = GetBoneMatrix(Lhand);
	Rshoulder3D = GetBoneMatrix(Rshoulder);
	Relbow3D = GetBoneMatrix(Relbow);
	Rhand3D = GetBoneMatrix(Rhand);
	Lbuttock3D = GetBoneMatrix(Lbuttock);
	Lknee3D = GetBoneMatrix(Lknee);
	Lfoot3D = GetBoneMatrix(Lfoot);
	Rbuttock3D = GetBoneMatrix(Rbuttock);
	Rknee3D = GetBoneMatrix(Rknee);
	Rfoot3D = GetBoneMatrix(Rfoot);

}
int ActorEntity::GetPlayerRole()
{
	return PlayerRole;
}

uint64_t ActorEntity::GetClass()
{
	return Class;
}

std::wstring ActorEntity::GetName()
{
	return Name;
}

Vector3 ActorEntity::GetPosition()
{
	Position = Vector3(UEPosition.X, UEPosition.Y, UEPosition.Z);
	return Position;
}

// TODO
void ActorEntity::UpdatePosition(VMMDLL_SCATTER_HANDLE handle)
{
	if (!Class)
		return;
	if (!RootComponent)
		return;
	if (!PlayerState)
		return;
	if (!AcknowledgedPawn || !isCheck) // players aren't pawns
		return;
	TargetProcess.AddScatterReadRequest(handle, RootComponent + RelativeLocation, reinterpret_cast<void*>(&UEPosition), sizeof(UEVector));
}
void ActorEntity::UpdateBone(VMMDLL_SCATTER_HANDLE handle)
{
	if (!Class)
		return;
	if (!RootComponent)
		return;
	if (!PlayerState)
		return;
	if (!isCheck) // players aren't pawns
		return;
	if (Mesh < 65535) {
		return;
	}
	TargetProcess.AddScatterReadRequest(handle, Mesh + SDK.ComponentToWorld, reinterpret_cast<void*>(&ToWorld), sizeof(FTransform));
	TargetProcess.AddScatterReadRequest(handle, BoneArray + index.Head * 0x30, reinterpret_cast<void*>(&Head), sizeof(FTransform));
	TargetProcess.AddScatterReadRequest(handle, BoneArray + index.neck * 0x30, reinterpret_cast<void*>(&neck), sizeof(FTransform));
	TargetProcess.AddScatterReadRequest(handle, BoneArray + index.pelvis * 0x30, reinterpret_cast<void*>(&pelvis), sizeof(FTransform));
	TargetProcess.AddScatterReadRequest(handle, BoneArray + index.Lshoulder * 0x30, reinterpret_cast<void*>(&Lshoulder), sizeof(FTransform));
	TargetProcess.AddScatterReadRequest(handle, BoneArray + index.Lelbow * 0x30, reinterpret_cast<void*>(&Lelbow), sizeof(FTransform));
	TargetProcess.AddScatterReadRequest(handle, BoneArray + index.Lhand * 0x30, reinterpret_cast<void*>(&Lhand), sizeof(FTransform));
	TargetProcess.AddScatterReadRequest(handle, BoneArray + index.Rshoulder * 0x30, reinterpret_cast<void*>(&Rshoulder), sizeof(FTransform));
	TargetProcess.AddScatterReadRequest(handle, BoneArray + index.Relbow * 0x30, reinterpret_cast<void*>(&Relbow), sizeof(FTransform));
	TargetProcess.AddScatterReadRequest(handle, BoneArray + index.Rhand * 0x30, reinterpret_cast<void*>(&Rhand), sizeof(FTransform));
	TargetProcess.AddScatterReadRequest(handle, BoneArray + index.Lbuttock * 0x30, reinterpret_cast<void*>(&Lbuttock), sizeof(FTransform));
	TargetProcess.AddScatterReadRequest(handle, BoneArray + index.Lknee * 0x30, reinterpret_cast<void*>(&Lknee), sizeof(FTransform));
	TargetProcess.AddScatterReadRequest(handle, BoneArray + index.Lfoot * 0x30, reinterpret_cast<void*>(&Lfoot), sizeof(FTransform));
	TargetProcess.AddScatterReadRequest(handle, BoneArray + index.Rbuttock * 0x30, reinterpret_cast<void*>(&Rbuttock), sizeof(FTransform));
	TargetProcess.AddScatterReadRequest(handle, BoneArray + index.Rknee * 0x30, reinterpret_cast<void*>(&Rknee), sizeof(FTransform));
	TargetProcess.AddScatterReadRequest(handle, BoneArray + index.Rfoot * 0x30, reinterpret_cast<void*>(&Rfoot), sizeof(FTransform));
}