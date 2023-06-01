/*
* Passes light information to RTX Remix
*/
void FixedFunctionLighting() {
	static int lastEnabled = 0;

	IDirect3DDevice9* Device = TheRenderManager->device;
	NiTList<ShadowSceneLight>::Entry* Entry = SceneNode->lights.start;
	NiPoint3* CameraPosition = (NiPoint3*)0x011F8E9C;
	//NiPoint3* CameraPosition = (NiPoint3*)0x011FA2A0;
	float Strength = SettingManager::LightIntensity;

	// Fill in sun light
	if (SettingManager::SunLight) {
		TimeGlobals* GameTimeGlobals = TimeGlobals::Get();
		float GameHour = GameTimeGlobals->GameHour->data;
		NiDirectionalLight* sunLight = Tes->directionalLight;
		Sky* WorldSky = Tes->sky;
		NiNode* SunRoot = WorldSky->sun->RootNode;
		float SunriseStart = WorldSky->GetSunriseBegin();
		float SunsetEnd = WorldSky->GetSunsetEnd();
		bool night = GameHour > SunsetEnd || GameHour < SunriseStart;
		float Intensity = night ? Strength * SettingManager::MoonIntensity : Strength;

		D3DLIGHT9 DirLight;
		ZeroMemory(&DirLight, sizeof(DirLight));
		DirLight.Type = D3DLIGHT_DIRECTIONAL;
		DirLight.Diffuse = D3DXCOLOR(sunLight->Diff.r * sunLight->Dimmer * Intensity, sunLight->Diff.g * sunLight->Dimmer * Intensity, sunLight->Diff.b * sunLight->Dimmer * Intensity, 1);
		if (!SettingManager::VisualSun || night) {
			// Night time, use lighting direction
			DirLight.Direction = D3DXVECTOR3(Tes->directionalLight->direction.x, Tes->directionalLight->direction.y, Tes->directionalLight->direction.z);
		}
		else {
			// Day time, flip sun position to direction
			DirLight.Direction = D3DXVECTOR3(-SunRoot->m_localTransform.pos.x, -SunRoot->m_localTransform.pos.y, -SunRoot->m_localTransform.pos.z);
		}
		Device->SetLight(0, &DirLight);
	}
	Device->LightEnable(0, SettingManager::SunLight);

	// Fill in various point lights
	int i = 1; // Sun is at 0
	while (Entry) {
		NiPointLight* Light = Entry->data->sourceLight;

		D3DLIGHT9 DX9Light;
		ZeroMemory(&DX9Light, sizeof(DX9Light));
		DX9Light.Type = D3DLIGHT_POINT;
		// TODO: Diffuse seems to be 0 to 1, Specular is not 0-1, not 0-255, maybe a radius?, Ambient is usually black
		DX9Light.Diffuse = D3DXCOLOR(Light->Diff.r * Light->Dimmer * Strength, Light->Diff.g * Light->Dimmer * Strength, Light->Diff.b * Light->Dimmer * Strength, 1.0f);
		//DX9Light.Specular = D3DXCOLOR(Light->Spec.r, Light->Spec.g, Light->Spec.b, 1.0f);
		DX9Light.Ambient = D3DXCOLOR(Light->Amb.r, Light->Amb.g, Light->Amb.b, 1.0f);
		DX9Light.Position = D3DXVECTOR3(Light->m_worldTransform.pos.x - CameraPosition->x, Light->m_worldTransform.pos.y - CameraPosition->y, Light->m_worldTransform.pos.z - CameraPosition->z);
		if (SettingManager::LightRangeMode == 0) {
			// Actual range value according to Wall_SoGB
			DX9Light.Range = Light->Spec.r;
		}
		else if (SettingManager::LightRangeMode == 1) {
			// Calculate range such that attenuation = 1/255
			if (Light->Atten2 > 0) {
				DX9Light.Range = (-Light->Atten1 + std::sqrt(Light->Atten1 * Light->Atten1 + 1020.0f * Light->Atten2)) / (2.0f * Light->Atten2);
			}
			else if (Light->Atten1 > 0) {
				DX9Light.Range = 255.0f / Light->Atten1;
			}
		}
		else {
			// Let Remix figure it out
			DX9Light.Range = INFINITY;
		}

		DX9Light.Attenuation0 = Light->Atten0;
		DX9Light.Attenuation1 = Light->Atten1;
		DX9Light.Attenuation2 = Light->Atten2;
		Device->SetLight(i, &DX9Light);
		Device->LightEnable(i, TRUE);
		i++;

		Entry = Entry->next;
	}
	for (int j = i; j < lastEnabled; j++) {
		Device->LightEnable(j, FALSE);
	}
	lastEnabled = i;
}

__declspec(naked) void RenderShadowMapHook() {

	__asm {
		pushad
		call	FixedFunctionLighting
		popad
		jmp		Jumpers::Shadows::RenderShadowMapReturn
	}
}

void AttachLightHooks() {
	SafeWriteJump(Jumpers::Shadows::RenderShadowMapHook, (UInt32)RenderShadowMapHook);
}