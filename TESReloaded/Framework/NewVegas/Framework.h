#pragma once

#pragma warning (disable: 4244) //disable warning for possible loss of data in implicit cast between int, float and double

#define DIRECTINPUT_VERSION 0x0800

#include <windows.h>
#include <d3dx9math.h>
#include "../Common/Base/Logger.h"
#include "../Common/Base/Types.h"
#include "../Common/Base/SafeWrite.h"
#include "../Common/Base/PluginVersion.h"
#include "Plugin.h"
#include "GameNi.h"
#include "GameHavok.h"
#include "Game.h"
#include "Defines.h"
#include "Base.h"
#include "Managers.h"
#include "../Core/Hooks/GameCommon.h"
#include "../Core/Hooks/NewVegas/Culling.h"
#include "../Core/Hooks/NewVegas/Game.h"
#include "../Core/Hooks/NewVegas/Lights.h"
