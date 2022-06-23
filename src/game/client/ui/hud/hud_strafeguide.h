#pragma once
#include <cstdint>
#include <complex>
#include "hud.h"

class CHudStrafeGuide : public CHudBase
{
	double angles[6] = {0.};

	std::complex<double> lastSimvel = 0.;

	cvar_t* hud_strafeguide;
	cvar_t* hud_strafeguide_zoom;
	cvar_t* hud_strafeguide_height;
	cvar_t* hud_strafeguide_size;

public:
	virtual bool Init();
	virtual bool VidInit();
	virtual bool Draw(float time);

	void Update(struct ref_params_s* ppmove);
};
