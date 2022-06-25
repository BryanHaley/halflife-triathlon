#pragma once
#include <cstdint>
#include "hud.h"

class CHudTimer : public CHudBase
{
	float time;
	bool paused;

	cvar_t* hud_timer;
	cvar_t* hud_timer_height;

public:
	virtual bool Init();
	virtual bool VidInit();
	virtual bool Draw(float time);

	void ResetTime();
	void StopTime();
};

void HudTimerReset();
void HudTimerStop();