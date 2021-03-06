/*
===========================================================================
Copyright (C) 2000 - 2013, Raven Software, Inc.
Copyright (C) 2001 - 2013, Activision, Inc.
Copyright (C) 2013 - 2015, OpenJK contributors

This file is part of the OpenJK source code.

OpenJK is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License version 2 as
published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, see <http://www.gnu.org/licenses/>.
===========================================================================
*/

#include "g_local.h"
#include "b_local.h"
#include "wp_saber.h"
#include "w_local.h"


//---------------
//	Bryar Pistol
//---------------

//---------------------------------------------------------
void WP_FireBryarPistolMissile(gentity_t* ent, vec3_t start, vec3_t dir, qboolean alt_fire)
//---------------------------------------------------------
{
	int velocity = BRYAR_PISTOL_VEL;
	int	damage = !alt_fire ? weaponData[WP_BRYAR_PISTOL].damage : weaponData[WP_BRYAR_PISTOL].altDamage;

	//VectorCopy(muzzle, start);
	WP_TraceSetStart(ent, start, vec3_origin, vec3_origin);//make sure our start point isn't on the other side of a wall

	WP_CalcSpread(ent, dir, alt_fire);

	WP_MissileTargetHint(ent, start, dir);

	gentity_t* missile = CreateMissile(start, dir, BRYAR_PISTOL_VEL, 10000, ent, alt_fire);

	missile->classname = "bryar_proj";

	if (ent->s.weapon == WP_BLASTER_PISTOL
		|| ent->s.weapon == WP_JAWA)
	{//*SIGH*... I hate our weapon system...
		missile->s.weapon = ent->s.weapon;
	}
	else
	{
		missile->s.weapon = WP_BRYAR_PISTOL;
	}

	/*
	if (alt_fire)
	{
		int count = (level.time - ent->client->ps.weaponChargeTime) / BRYAR_CHARGE_UNIT;

		if (count < 1)
		{
			count = 1;
		}
		else if (count > 5)
		{
			count = 5;
		}

		damage *= count;
		missile->count = count; // this will get used in the projectile rendering code to make a beefier effect
	}
	*/

	missile->damage = damage;
	missile->dflags = DAMAGE_DEATH_KNOCKBACK;

	if (alt_fire)
	{
		missile->methodOfDeath = MOD_BRYAR_ALT;
	}
	else
	{
		missile->methodOfDeath = MOD_BRYAR;
	}

	missile->clipmask = MASK_SHOT | CONTENTS_LIGHTSABER;

	// we don't want it to bounce forever
	missile->bounceCount = 8;
}

//----------------------------------------------------------
void WP_FireBryarPistol( gentity_t *ent, qboolean alt_fire )
//----------------------------------------------------------
{
	vec3_t		eyeposVec;
	vec3_t		dir;

	if (!alt_fire) // main-fire from hip
	{
		if (!ent->s.number && isUsingStaticCrosshair) // proper crosshair aim for player
		{
			//WP_FireBlasterMissile(ent, muzzle, forwardVec, alt_fire);
			VectorSubtract(crosshairAimPos, muzzle, dir);
			VectorNormalize(dir);
			WP_FireBryarPistolMissile(ent, muzzle, dir, alt_fire);
		}
		else
		{
			WP_FireBryarPistolMissile(ent, muzzle, forwardVec, alt_fire);
		}
		//WP_FireBryarPistolMissile(ent, muzzle, forwardVec, alt_fire);
	}
	else // alt-fire from scope/eye
	{
		VectorCopy(ent->client->renderInfo.eyePoint, eyeposVec);
		AngleVectors(ent->client->renderInfo.eyeAngles, forwardVec, NULL, NULL);
		WP_FireBryarPistolMissile(ent, eyeposVec, forwardVec, alt_fire);
		//WP_FireBryarPistolMissile(ent, muzzle, forwardVec, alt_fire);
	}

	if ( ent->weaponModel[1] > 0 )
	{//dual pistols, toggle the muzzle point back and forth between the two pistols each time he fires
		ent->count = (ent->count)?0:1;
	}
}