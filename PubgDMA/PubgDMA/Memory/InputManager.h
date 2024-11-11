#pragma once
#include "pch.h"
#include "Registry.h"

namespace Keyboard
{
	bool InitKeyboard();

	void UpdateKeys();
	bool IsKeyDown(uint64_t virtual_key_code);
};
