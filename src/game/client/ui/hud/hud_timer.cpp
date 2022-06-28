#include <cmath>
#include <cstring>
#include <sstream>

#include "hud.h"
#include "cl_util.h"
#include "parsemsg.h"

#include "hud_timer.h"

DECLARE_MESSAGE(m_Timer, StopTime);

bool CHudTimer::Init()
{
	HOOK_MESSAGE(StopTime);

	m_iFlags = HUD_ACTIVE;

	hud_timer = CVAR_CREATE("hud_timer", "1", FCVAR_ARCHIVE);
	hud_timer_height = CVAR_CREATE("hud_timer_height", "0", FCVAR_ARCHIVE);
	hud_timer_precision = CVAR_CREATE("hud_timer_precision", "2", FCVAR_ARCHIVE);
	hud_timer_stay_yellow = CVAR_CREATE("hud_timer_stay_yellow", "0", FCVAR_ARCHIVE);
	hud_timer_draw_as_string = CVAR_CREATE("hud_timer_draw_as_string", "0", FCVAR_ARCHIVE);

	m_fTime = 0;
	m_bPaused = false;

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
	if (!m_bPaused) m_fTime += gpGlobals->frametime;

	if (hud_timer->value == 0.0f)
		return false;

	RGB24 color = RGB_YELLOWISH;
	if (hud_timer_stay_yellow->value == 0.0f)
		color = gHUD.m_HudColor;

	int y;
	if (hud_timer_height->value != 0.0f)
		y = hud_timer_height->value;
	else
		y = (ScreenHeight/2) - (gHUD.m_iFontHeight/2);

	// Convert time (seconds) into hours, minutes, seconds with std::chrono
	std::chrono::hh_mm_ss chrono_time{std::chrono::seconds((int) m_fTime)};
	int hours = chrono_time.hours().count();
	int minutes = chrono_time.minutes().count();
	int seconds = chrono_time.seconds().count();

	// Convert time to int and subtract from time for milliseconds
	int time_i = (int) m_fTime;
	int milliseconds = (m_fTime - time_i) * 1000;

	// Convert hours, minutes, seconds, milliseconds into individual digits
	// Will technically break if we go over 99 hours but this is a speedrun so it's fine
	int hours_tens = hours / 10;
	int hours_ones = hours % 10;
	int minutes_tens = minutes / 10;
	int minutes_ones = minutes % 10;
	int seconds_tens = seconds / 10;
	int seconds_ones = seconds % 10;
	int milli_huns = milliseconds / 100;
	int milli_tens = (milliseconds / 10) % 10;
	int milli_ones = milliseconds % 10;

	// Draw using DrawHudNumber (much larger than DrawHudString)
	if (hud_timer_draw_as_string->value == 0.0f)
	{
		int width = gHUD.GetHudNumberWidth(1, 1, DHN_2DIGITS);
		int x = gHUD.m_iFontHeight/2;

		int r = color.Red;
		int g = color.Green;
		int b = color.Blue;

		// Draw hours
		if (hours > 0)
		{
			// in theory maybe we shouldn't even bother drawing hours_tens since it's
			// extremely unlikely a speedrun will go for 10 hours, but it looks nicer
			gHUD.DrawHudNumber(x,       y, hours_tens, r, g, b);
			gHUD.DrawHudNumber(x+width, y, hours_ones, r, g, b);
			gHUD.DrawHudStringCentered(x+width*2+width/2, y, ":", r, g, b);
		}

		else x -= (x+width*2); // shift minutes etc left when not drawing hours
		
		// Draw minutes
		gHUD.DrawHudNumber(x+width*3, y, minutes_tens, r, g, b);
		gHUD.DrawHudNumber(x+width*4, y, minutes_ones, r, g, b);
		gHUD.DrawHudStringCentered(x+width*5+width/2, y, ":", r, g, b);
		
		// Draw seconds
		gHUD.DrawHudNumber(x+width*6, y, seconds_tens, r, g, b);
		gHUD.DrawHudNumber(x+width*7, y, seconds_ones, r, g, b);

		// Draw milliseconds
		if (hud_timer_precision->value >= 1.0f)
		{
			gHUD.DrawHudStringCentered(x+width*8+width/2, y+gHUD.m_iFontHeight/4, ".", r, g, b);
			gHUD.DrawHudNumber(x+width*9, y, milli_huns, r, g, b);
		}
		if (hud_timer_precision->value >= 2.0f)
			gHUD.DrawHudNumber(x+width*10, y, milli_tens, r, g, b);
		if (hud_timer_precision->value >= 3.0f)
			gHUD.DrawHudNumber(x+width*11, y, milli_ones, r, g, b);
	}

	// Draw time as a string. Quite tiny at HD resolutions.
	else
	{
		std::stringstream ss;
		
		// Add hours to string
		if (hours > 0)	
			ss << hours_tens << hours_ones << ":";

		// Add minutes and seconds to string
		ss << minutes_tens << minutes_ones << ":";
		ss << seconds_tens << seconds_ones;
		
		// Add milliseconds to string
		if (hud_timer_precision->value >= 1.0f)
		{
			ss << "." << milli_huns;
		}
		if (hud_timer_precision->value >= 2.0f)
			ss << milli_tens;
		if (hud_timer_precision->value >= 3.0f)
			ss << milli_ones;

		gHUD.DrawHudString(10, y, ScreenWidth / 2, ss.str().c_str(), color);
	}

	return true;
}

bool CHudTimer::MsgFunc_StopTime(const char* pszName, int iSize, void* pbuf)
{
	m_bPaused = true;
	return true;
}

void CHudTimer::ResetTime()
{
	m_fTime = 0;
	m_bPaused = false;
}

void CHudTimer::StopTime()
{
	m_bPaused = true;
}

void HudTimerReset()
{
	gHUD.m_Timer.ResetTime();
}

void HudTimerStop()
{
	gHUD.m_Timer.StopTime();
}