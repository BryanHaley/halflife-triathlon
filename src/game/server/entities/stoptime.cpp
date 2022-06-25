#include "cbase.h"
#include "pm_defs.h"
#include "pm_shared.h"
#include "UserMessages.h"

class CStopTime : public CBaseEntity
{
	void Use(CBaseEntity* pActivator, CBaseEntity* pCaller, USE_TYPE useType, float value) override;
};

void CStopTime::Use(CBaseEntity* pActivator, CBaseEntity* pCaller, USE_TYPE useType, float value)
{
	MESSAGE_BEGIN(MSG_ALL, gmsgStopTime, nullptr);
	MESSAGE_END();
}

LINK_ENTITY_TO_CLASS(info_stop_time, CStopTime);