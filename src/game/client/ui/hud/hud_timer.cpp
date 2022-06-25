#include <cmath>
#include <cstring>

#include "hud.h"
#include "cl_util.h"
#include "parsemsg.h"

#include "hud_timer.h"

bool CHudTimer::Init()
{
	m_iFlags = HUD_ACTIVE;

	hud_timer = CVAR_CREATE("hud_timer", "1", FCVAR_ARCHIVE);
	hud_timer_height = CVAR_CREATE("hud_timer_height", "0", FCVAR_ARCHIVE);

	time = 0;
	paused = false;

	gEngfuncs.pfnAddCommand("hud_timer_reset", HudTimerReset);
	gEngfuncs.pfnAddCommand("hud_timer_stop", HudTimerStop);

	gHUD.AddHudElem(this);
	return true;
}

bool CHudTimer::VidInit()
{
	return true;
}

bool CHudTimer::Draw(float flTime)
{
	if (!paused) time += gpGlobals->frametime;

	if (hud_timer->value == 0.0f)
		return false;

	int r, g, b;
	UnpackRGB(r, g, b, gHUD.m_HudColor);

	int y;
	if (hud_timer_height->value != 0.0f)
		y = hud_timer_height->value;
	else
		y = (ScreenHeight/2) - (gHUD.m_iFontHeight/2);

	std::chrono::hh_mm_ss chrono_time{std::chrono::seconds((int) time)};

	int time_i = (int) time;

	int hours = chrono_time.hours().count();
	int minutes = chrono_time.minutes().count();
	int seconds = chrono_time.seconds().count();
	int milliseconds = (time - time_i) * 1000;

	int hours_x = gHUD.m_iFontHeight/2;
	int colon_1_x = hours_x + gHUD.GetHudNumberWidth(hours, 2, DHN_2DIGITS);
	int minutes_x = colon_1_x + gHUD.GetHudNumberWidth(0, 1, DHN_DRAWZERO);
	int colon_2_x = minutes_x + gHUD.GetHudNumberWidth(minutes_x, 2, DHN_2DIGITS);
	int seconds_x = colon_2_x + gHUD.GetHudNumberWidth(0, 1, DHN_DRAWZERO);
	int colon_3_x = seconds_x + gHUD.GetHudNumberWidth(seconds_x, 2, DHN_2DIGITS);
	int millis_x = colon_3_x + gHUD.GetHudNumberWidth(0, 1, DHN_DRAWZERO);

	gHUD.DrawHudNumber(hours_x, y, hours, r, g, b);
	gHUD.DrawHudStringCentered(colon_1_x, y, ":", r, g, b);
	gHUD.DrawHudNumber(minutes_x, y, minutes, r, g, b);
	gHUD.DrawHudStringCentered(colon_2_x, y, ":", r, g, b);
	gHUD.DrawHudNumber(seconds_x, y, seconds, r, g, b);
	gHUD.DrawHudStringCentered(colon_3_x, y, ".", r, g, b);
	gHUD.DrawHudNumber(millis_x, y, milliseconds, r, g, b);

	return true;
}

void CHudTimer::ResetTime()
{
	time = 0;
	paused = false;
}

void CHudTimer::StopTime()
{
	paused = true;
}

void HudTimerReset()
{
	gHUD.m_Timer.ResetTime();
}

void HudTimerStop()
{
	gHUD.m_Timer.StopTime();
}