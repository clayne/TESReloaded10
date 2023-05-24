#pragma once

class CommandManager;		extern CommandManager*		TheCommandManager;
class SettingManager;		extern SettingManager*		TheSettingManager;
class RenderManagerBase;	extern RenderManagerBase*	TheRenderManager;
class ShadowManager;		extern ShadowManager*		TheShadowManager;

class Main;					extern Main*					Global;
class TES;					extern TES*						Tes;
class PlayerCharacter;		extern PlayerCharacter*			Player;
class SceneGraph;			extern SceneGraph*				WorldSceneGraph;
class MainDataHandler;		extern MainDataHandler*			DataHandler;
class MenuInterfaceManager; extern MenuInterfaceManager*	InterfaceManager;
class QueuedModelLoader;	extern QueuedModelLoader*		ModelLoader;
class ShadowSceneNode;		extern ShadowSceneNode*			SceneNode;

#include "SettingManager.h"
#include "CommandManager.h"
