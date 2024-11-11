#include "Pch.h"
#include "Engine.h"
#include "ActorEntity.h"
#include "Globals.h"
#include <codecvt>
#include <locale>
Engine::Engine()
{

	if (InitDecrypt(SDK.Decrypt)) {

		uintptr_t base_data = TargetProcess.GetBaseAddress(ProcessName);
		printf("base_data = 0x%x\n", base_data);
		UWorld = xe_decrypt(TargetProcess.Read<uintptr_t>(TargetProcess.GetBaseAddress(ProcessName) + SDK.UWorld));
		printf("GWorld: 0x%x\n", UWorld);
		CurrentLevel = xe_decrypt(TargetProcess.Read<uint64_t>(UWorld + SDK.CurrentLevel));
		printf("PersistentLevel: 0x%x\n", CurrentLevel);
		GameInstance = xe_decrypt(TargetProcess.Read<uint64_t>(UWorld + SDK.GameInstance));
		printf("OwningGameInstance: 0x%x\n", GameInstance);
		LocalPlayers = xe_decrypt(TargetProcess.Read<uint64_t>(TargetProcess.Read<uint64_t>(GameInstance + SDK.LocalPlayers)));
		printf("LocalPlayers: 0x%x\n", LocalPlayers);
		PlayerController = xe_decrypt(TargetProcess.Read<uint64_t>(LocalPlayers + SDK.PlayerController));
		printf("PlayerController: 0x%x\n", PlayerController);
		AcknowledgedPawn = xe_decrypt(TargetProcess.Read<uint64_t>(PlayerController + SDK.AcknowledgedPawn));
		printf("AcknowledgedPawn: 0x%x\n", AcknowledgedPawn);
		PlayerCameraManager = TargetProcess.Read<uint64_t>(PlayerController + SDK.PlayerCameraManager);
		printf("CameraManager: 0x%x\n", PlayerCameraManager);
		CameraEntry.POV.FOV = TargetProcess.Read<float>(PlayerCameraManager + SDK.CameraFov);
		CameraEntry.POV.Location = TargetProcess.Read<UEVector>(PlayerCameraManager + SDK.CameraPos);
		CameraEntry.POV.Rotation = TargetProcess.Read<UERotator>(PlayerCameraManager + SDK.CameraRot);
		printf("CameraCacheEntry: 0x%x\n", CameraEntry);
		GetGNames();
	}
}


typedef int64_t(__fastcall* DecryptFunctoin)(uintptr_t key, uintptr_t argv);
inline DecryptFunctoin fnDecryptFunctoin = NULL;
inline uint64_t Tmpadd;
inline bool Engine::InitDecrypt(uint64_t offset)
{
	uintptr_t DecryptPtr = TargetProcess.Read<uintptr_t>(TargetProcess.GetBaseAddress(ProcessName) + offset);
	while (!DecryptPtr)
	{
		DecryptPtr = TargetProcess.Read<uintptr_t>(TargetProcess.GetBaseAddress(ProcessName) + offset);
		Sleep(1000);
	}
	printf("DecryptPtr = %p\n",DecryptPtr);
	int32_t Tmp1Add = TargetProcess.Read<int32_t>(DecryptPtr + 3);
	Tmpadd = Tmp1Add + DecryptPtr + 7;
	unsigned char ShellcodeBuff[1024] = { NULL };
	ShellcodeBuff[0] = 0x90;
	ShellcodeBuff[1] = 0x90;
	VMMDLL_MemReadEx(TargetProcess.vHandle, TargetProcess.ReturnPid(), DecryptPtr, &ShellcodeBuff[2], sizeof(ShellcodeBuff) - 2, NULL, VMMDLL_FLAG_NOCACHE | VMMDLL_FLAG_NOCACHEPUT | VMMDLL_FLAG_ZEROPAD_ON_FAIL | VMMDLL_FLAG_NOPAGING_IO | VMMDLL_FLAG_NOPAGING);
	ShellcodeBuff[2] = 0x48;
	ShellcodeBuff[3] = 0x8B;
	ShellcodeBuff[4] = 0xC1;
	ShellcodeBuff[5] = 0x90;
	ShellcodeBuff[6] = 0x90;
	ShellcodeBuff[7] = 0x90;
	ShellcodeBuff[8] = 0x90;
	fnDecryptFunctoin = (DecryptFunctoin)VirtualAlloc(NULL, sizeof(ShellcodeBuff) + 4, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	RtlCopyMemory((LPVOID)fnDecryptFunctoin, (LPVOID)ShellcodeBuff, sizeof(ShellcodeBuff));
	return 1;
}
inline uintptr_t Engine::xe_decrypt(const uintptr_t encrypted)
{
	uintptr_t data = fnDecryptFunctoin(Tmpadd, encrypted);
	return data;
}
// DecryptCIndex
auto _SHL_(auto value, auto x) {
	return value << x;
}
auto _XOR_(auto a, auto b) {
	return a ^ b;
}

auto _ROR_(auto value, auto steps) {
	steps %= 32;
	return (value << steps) | (value >> (32 - steps));
}

auto _ROL_(auto value, auto steps) {
	steps %= 32;
	return (value >> steps) | (value << (32 - steps));
}
DWORD Engine::DecryptCIndex(DWORD Encrypted)
{
	int _TableOne = SDK.NameIndexOne;
	int _TableTwo = SDK.NameIndexTwo;
	uint64_t _DecryptOne = SDK.NameIndexXor1;
	uint64_t _DecryptTwo = SDK.NameIndexXor2;
	if (SDK.NameIsROR == 3) return _XOR_(_XOR_(_SHL_(Encrypted, _TableTwo), _DecryptTwo), Encrypted);
	Encrypted = SDK.NameIsROR == 1 ? _ROR_(_XOR_(Encrypted, _DecryptOne), _TableOne) : _ROL_(_XOR_(Encrypted, _DecryptOne), _TableOne);
	return _XOR_(_XOR_(_SHL_(Encrypted, _TableTwo), _DecryptTwo), Encrypted);
}
std::wstring string_to_wstring(const std::string& str) {
	std::wstring ws(str.begin(), str.end());
	return ws;
}
bool contains(const std::string& haystack, const std::string& needle) {
	return haystack.find(needle) != std::string::npos;
}
void Engine::Cache()
{

	OwningActor = xe_decrypt(TargetProcess.Read<uint64_t>(CurrentLevel + SDK.Actors));
	AcknowledgedPawn = xe_decrypt(TargetProcess.Read<uint64_t>(PlayerController + SDK.AcknowledgedPawn));
	Local.Teamid = TargetProcess.Read<uint64_t>(AcknowledgedPawn + SDK.LastTeamNum);
	Local.SpectatedCount = TargetProcess.Read<uint64_t>(AcknowledgedPawn + SDK.SpectatedCount);
	MaxPacket = TargetProcess.Read<int>(OwningActor + 8);
	printf("Actor Array: %p\n", OwningActor);
	printf("Actor Array Size: %d\n", MaxPacket);
	if (MaxPacket == 0) {
		return;
	}

	OwningActor = TargetProcess.Read<uint64_t>(OwningActor);
	std::vector<uint64_t> entitylist;
	entitylist.resize(MaxPacket);
	std::unique_ptr<uint64_t[]> object_raw_ptr = std::make_unique<uint64_t[]>(MaxPacket);
	TargetProcess.Read(OwningActor, object_raw_ptr.get(), MaxPacket * sizeof(uint64_t));
	for (size_t i = 0; i < MaxPacket; i++)
	{
		entitylist[i] = object_raw_ptr[i];
	}
	std::list<std::shared_ptr<ActorEntity>> actors;
	auto handle = TargetProcess.CreateScatterHandle();
	for (uint64_t address : entitylist)
	{
		uintptr_t actor = address;
		if (actor == AcknowledgedPawn)
			continue;
		if (!actor)
			continue;
		std::shared_ptr<ActorEntity> entity = std::make_shared<ActorEntity>(actor, handle);
		actors.push_back(entity);
		
	}
	TargetProcess.ExecuteReadScatter(handle);
	TargetProcess.CloseScatterHandle(handle);


	handle = TargetProcess.CreateScatterHandle();
	for (std::shared_ptr<ActorEntity> entity : actors)
	{
		if (entity->RootComponent) {
			entity->isCheck = true;
		}
		entity->SetUp1(handle);
	}
	TargetProcess.ExecuteReadScatter(handle);
	TargetProcess.CloseScatterHandle(handle);

	handle = TargetProcess.CreateScatterHandle();
	std::vector<std::shared_ptr<ActorEntity>> playerlist;
	for (std::shared_ptr<ActorEntity> entity : actors)
	{
		entity->SetUp2(handle);

	}
	TargetProcess.ExecuteReadScatter(handle);
	TargetProcess.CloseScatterHandle(handle);
	for (std::shared_ptr<ActorEntity> entity : actors)
	{
		entity->SetUp3();
		if (entity->Head3D.X == 0 && entity->Head3D.Y == 0 && entity->Head3D.Z)
			continue;
		playerlist.push_back(entity);
	}
	Actors = playerlist;
}
void Engine::GetGNames()
{
	GNames = xe_decrypt(TargetProcess.Read<uint64_t>(TargetProcess.GetBaseAddress(ProcessName)+ SDK.GNames));
	GNames = xe_decrypt(TargetProcess.Read<uint64_t>(GNames + SDK.GNames_offset));
}
typedef struct StringA
{
	char buffer[1024];
};
std::string Engine::GetNames(DWORD ID)
{
	std::string emp = "Unknown";
	if (ID <= 0) return emp;
	uint32_t IdDiv = ID / SDK.ElementsPerChunk;
	uint32_t Idtemp = ID % SDK.ElementsPerChunk;
	uint64_t Serial = TargetProcess.Read<uint64_t>(GNames + IdDiv * 0x8);
	if (!Serial || Serial < 0x100000)
		return emp;
	uint64_t pName = TargetProcess.Read<uint64_t>(Serial + 0x8 * Idtemp);
	if (!pName || pName < 0x100000)
		return emp;

	StringA names = TargetProcess.Read<StringA>(pName + 0x10);
	char te[256];
	memset(&te, 0, 256);
	if (memcmp(names.buffer, te, 256) == 0)
		return emp;

	std::string str(names.buffer);
	return str;
}
void Engine::UpdatePlayers()
{
	auto handle = TargetProcess.CreateScatterHandle();
	for (std::shared_ptr<ActorEntity> entity : Actors)
	{

		entity->UpdateBone(handle);
	}
	TargetProcess.ExecuteReadScatter(handle);
	TargetProcess.CloseScatterHandle(handle);
	for (std::shared_ptr<ActorEntity> entity : Actors)
	{

		entity->SetUp3();
	}
}


void Engine::RefreshViewMatrix(VMMDLL_SCATTER_HANDLE handle)
{
	TargetProcess.AddScatterReadRequest(handle, PlayerCameraManager + SDK.CameraFov, reinterpret_cast<void*>(&CameraEntry.POV.FOV), sizeof(float));
	TargetProcess.AddScatterReadRequest(handle, PlayerCameraManager + SDK.CameraPos, reinterpret_cast<void*>(&CameraEntry.POV.Location), sizeof(UEVector));
	TargetProcess.AddScatterReadRequest(handle, PlayerCameraManager + SDK.CameraRot,reinterpret_cast<void*>(&CameraEntry.POV.Rotation),sizeof(UERotator));
}

CameraCacheEntry Engine::GetCameraCache()
{
	return CameraEntry;
}

std::vector<std::shared_ptr<ActorEntity>> Engine::GetActors()
{
	return Actors;
}

uint64_t Engine::GetActorSize()
{
	return MaxPacket;
}
