#pragma once
#include <cmath>
#include <cstring>

#include "hud.h"
#include "cl_util.h"
#include "parsemsg.h"
//#include "hudgl.h"

#include "hud_speedometer.h"

bool CHudSpeedometer::Init()
{
	m_iFlags = HUD_ACTIVE;

	hud_speedometer = CVAR_CREATE("hud_speedometer", "1", FCVAR_ARCHIVE);
	hud_speedometer_below_cross = CVAR_CREATE("hud_speedometer_below_cross", "0", FCVAR_ARCHIVE);
	hud_speedometer_height = CVAR_CREATE("hud_speedometer_height", "0", FCVAR_ARCHIVE);
	hud_speedometer_stay_yellow = CVAR_CREATE("hud_speedometer_stay_yellow", "0", FCVAR_ARCHIVE);

	gHUD.AddHudElem(this);
	return 0;
}

bool CHudSpeedometer::VidInit()
{
	return 1;
}

bool CHudSpeedometer::Draw(float time)
{
	if (hud_speedometer->value == 0.0f)
		return false;

	int r, g, b;
	if (hud_speedometer_stay_yellow->value == 0.0f)
		UnpackRGB(r, g, b, gHUD.m_HudColor);
	else
		UnpackRGB(r, g, b, RGB_YELLOWISH);

	int y;
	if (hud_speedometer_below_cross->value != 0.0f)
		y = ScreenHeight / 2 + gHUD.m_iFontHeight / 2;
	else if (hud_speedometer_height->value != 0.0f)
		y = hud_speedometer_height->value;
	else
		y = ScreenHeight - gHUD.m_iFontHeight - gHUD.m_iFontHeight / 2;

	gHUD.DrawHudNumberCentered(ScreenWidth / 2, y, speed, r, g, b);

	return true;
}

void CHudSpeedometer::UpdateSpeed(const float velocity[2])
{
	speed = std::round(std::hypot(velocity[0], velocity[1]));
}
