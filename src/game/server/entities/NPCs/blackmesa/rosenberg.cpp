/***
*
*	Copyright (c) 1996-2001, Valve LLC. All rights reserved.
*
*	This product contains software technology licensed from Id
*	Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc.
*	All Rights Reserved.
*
*   This source code contains proprietary and confidential information of
*   Valve LLC and its suppliers.  Access to this code is restricted to
*   persons who have executed a written SDK license with Valve.  Any access,
*   use or distribution of this code by or to any unlicensed person is illegal.
*
****/
//=========================================================
// human scientist (passive lab worker)
//=========================================================

#include "cbase.h"
#include "talkmonster.h"
#include "defaultai.h"
#include "scripted.h"
#include "scientist.h"

//=======================================================
// Scientist
//=======================================================

class CRosenberg : public CScientist
{
public:
	void Precache() override;

	void StartTask(Task_t* pTask) override;
	bool TakeDamage(entvars_t* pevInflictor, entvars_t* pevAttacker, float flDamage, int bitsDamageType) override;
	void DeclineFollowing() override;
	MONSTERSTATE GetIdealState() override;

	void Scream();

	void PainSound() override;

	void TalkInit() override;
};

LINK_ENTITY_TO_CLASS(monster_rosenberg, CRosenberg);

void CRosenberg::DeclineFollowing()
{
	Talk(10);
	m_hTalkTarget = m_hEnemy;
	PlaySentence("RO_POK", 2, VOL_NORM, ATTN_NORM);
}

void CRosenberg::Scream()
{
	if (FOkToSpeak())
	{
		Talk(10);
		m_hTalkTarget = m_hEnemy;
		PlaySentence("RO_SCREAM", RANDOM_FLOAT(3, 6), VOL_NORM, ATTN_NORM);
	}
}

void CRosenberg::StartTask(Task_t* pTask)
{
	switch (pTask->iTask)
	{
	case TASK_SAY_HEAL:
		//		if ( FOkToSpeak() )
		Talk(2);
		m_hTalkTarget = m_hTargetEnt;
		PlaySentence("RO_HEAL", 2, VOL_NORM, ATTN_IDLE);

		TaskComplete();
		break;

	case TASK_SAY_FEAR:
		if (FOkToSpeak())
		{
			Talk(2);
			m_hTalkTarget = m_hEnemy;
			if (m_hEnemy->IsPlayer())
				PlaySentence("RO_PLFEAR", 5, VOL_NORM, ATTN_NORM);
			else
				PlaySentence("RO_FEAR", 5, VOL_NORM, ATTN_NORM);
		}
		TaskComplete();
		break;

	default:
		CScientist::StartTask(pTask);
		break;
	}
}

//=========================================================
// Precache - precaches all resources this monster needs
//=========================================================
void CRosenberg::Precache()
{
	PrecacheModel(STRING(pev->model));
	PRECACHE_SOUND("rosenberg/ro_pain1.wav");
	PRECACHE_SOUND("rosenberg/ro_pain2.wav");
	PRECACHE_SOUND("rosenberg/ro_pain3.wav");
	PRECACHE_SOUND("rosenberg/ro_pain4.wav");
	PRECACHE_SOUND("rosenberg/ro_pain5.wav");

	// every new scientist must call this, otherwise
	// when a level is loaded, nobody will talk (time is reset to 0)
	TalkInit();

	CTalkMonster::Precache();
}

// Init talk data
void CRosenberg::TalkInit()
{
	CTalkMonster::TalkInit();

	// scientists speach group names (group names are in sentences.txt)

	m_szGrp[TLK_ANSWER] = "RO_ANSWER";
	m_szGrp[TLK_QUESTION] = "RO_QUESTION";
	m_szGrp[TLK_IDLE] = "RO_IDLE";
	m_szGrp[TLK_STARE] = "RO_STARE";
	m_szGrp[TLK_USE] = "RO_OK";
	m_szGrp[TLK_UNUSE] = "RO_WAIT";
	m_szGrp[TLK_STOP] = "RO_STOP";
	m_szGrp[TLK_NOSHOOT] = "RO_SCARED";
	m_szGrp[TLK_HELLO] = "RO_HELLO";

	m_szGrp[TLK_PLHURT1] = "!RO_CUREA";
	m_szGrp[TLK_PLHURT2] = "!RO_CUREB";
	m_szGrp[TLK_PLHURT3] = "!RO_CUREC";

	m_szGrp[TLK_PHELLO] = "RO_PHELLO";
	m_szGrp[TLK_PIDLE] = "RO_PIDLE";
	m_szGrp[TLK_PQUESTION] = "RO_PQUEST";
	m_szGrp[TLK_SMELL] = "RO_SMELL";

	m_szGrp[TLK_WOUND] = "RO_WOUND";
	m_szGrp[TLK_MORTAL] = "RO_MORTAL";

	m_voicePitch = 100;
}

bool CRosenberg::TakeDamage(entvars_t* pevInflictor, entvars_t* pevAttacker, float flDamage, int bitsDamageType)
{
	//Disable scientist damage handling so Rosenberg keeps following the player
	return CTalkMonster::TakeDamage(pevInflictor, pevAttacker, flDamage, bitsDamageType);
}

//=========================================================
// PainSound
//=========================================================
void CRosenberg::PainSound()
{
	if (gpGlobals->time < m_painTime)
		return;

	m_painTime = gpGlobals->time + RANDOM_FLOAT(0.5, 0.75);

	switch (RANDOM_LONG(0, 4))
	{
	case 0:
		EMIT_SOUND_DYN(ENT(pev), CHAN_VOICE, "rosenberg/ro_pain1.wav", 1, ATTN_NORM, 0, 100);
		break;
	case 1:
		EMIT_SOUND_DYN(ENT(pev), CHAN_VOICE, "rosenberg/ro_pain2.wav", 1, ATTN_NORM, 0, 100);
		break;
	case 2:
		EMIT_SOUND_DYN(ENT(pev), CHAN_VOICE, "rosenberg/ro_pain3.wav", 1, ATTN_NORM, 0, 100);
		break;
	case 3:
		EMIT_SOUND_DYN(ENT(pev), CHAN_VOICE, "rosenberg/ro_pain4.wav", 1, ATTN_NORM, 0, 100);
		break;
	case 4:
		EMIT_SOUND_DYN(ENT(pev), CHAN_VOICE, "rosenberg/ro_pain5.wav", 1, ATTN_NORM, 0, 100);
		break;
	}
}

MONSTERSTATE CRosenberg::GetIdealState()
{
	switch (m_MonsterState)
	{
	case MONSTERSTATE_ALERT:
	case MONSTERSTATE_IDLE:
		if (HasConditions(bits_COND_NEW_ENEMY))
		{
			if (IsFollowing())
			{
				int relationship = IRelationship(m_hEnemy);
				if (relationship != R_FR || relationship != R_HT && !HasConditions(bits_COND_LIGHT_DAMAGE | bits_COND_HEAVY_DAMAGE))
				{
					// Don't go to combat if you're following the player
					m_IdealMonsterState = MONSTERSTATE_ALERT;
					return m_IdealMonsterState;
				}
				//StopFollowing(true);
			}
		}
		else if (HasConditions(bits_COND_LIGHT_DAMAGE | bits_COND_HEAVY_DAMAGE))
		{
			// Stop following if you take damage
			/*if (IsFollowing())
				StopFollowing(true);*/
		}
		break;

	case MONSTERSTATE_COMBAT:
	{
		CBaseEntity* pEnemy = m_hEnemy;
		if (pEnemy != nullptr)
		{
			if (DisregardEnemy(pEnemy)) // After 15 seconds of being hidden, return to alert
			{
				// Strip enemy when going to alert
				m_IdealMonsterState = MONSTERSTATE_ALERT;
				m_hEnemy = nullptr;
				return m_IdealMonsterState;
			}
			// Follow if only scared a little
			if (m_hTargetEnt != nullptr)
			{
				m_IdealMonsterState = MONSTERSTATE_ALERT;
				return m_IdealMonsterState;
			}

			if (HasConditions(bits_COND_SEE_ENEMY))
			{
				m_fearTime = gpGlobals->time;
				m_IdealMonsterState = MONSTERSTATE_COMBAT;
				return m_IdealMonsterState;
			}
		}
	}
	break;
	}

	return CTalkMonster::GetIdealState();
}