#pragma once
#include <cstdint>
#include "hud.h"

class CHudTimer : public CHudBase
{
	float time;
	bool paused;

	cvar_t* hud_timer;
	cvar_t* hud_timer_height;
	cvar_t* hud_timer_stay_yellow;

public:
	virtual bool Init();
	virtual bool VidInit();
	virtual bool Draw(float time);

	bool MsgFunc_StopTime(const char* pszName, int iSize, void* pbuf);

	void ResetTime();
	void StopTime();
};

void HudTimerReset();
void HudTimerStop();