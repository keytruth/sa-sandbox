/*
	Codes:
		1 - Teleport
		2 - Speed Hack
		3 - Slapper (invalid name)
		4 - Health Hack
		5 - Armour Hack
		6 - Money Hack
		7 - High Ping
		8 - Weapon&Ammo Hack
		9 - Invisible Hack
		10 - Parkour Mod
		11 - CJ-Run
		12 - Slapper
		13 - Silent Aimbot
		14 - Pro Aimbot
		15 - Tele-Kill (Custom)
*/

#if defined _included_sandac
    #endinput
#endif
#define _included_sandac

const MAX_DIST = 7;

enum ePlayerSync
{
	Float: acHealth,
	Float: acArmour,

	acMoney,
	acWeapon[47],

	Float: acX,
	Float: acY,
	Float: acZ,
	
	bool: acSpectate
}
static PlayerSync[MAX_PLAYERS][ePlayerSync];
static SecSync;

forward OnPlayerCheat(playerid, const code);

public OnGameModeInit()
{
	SecSync = SetTimer("sandac_OnSecSync", 1000, true);

    #if defined sandac_OnGameModeInit
        return sandac_OnGameModeInit();
    #else
        return 1;
    #endif
}

#if defined _ALS_OnGameModeInit
    #undef OnGameModeInit
#else
    #define _ALS_OnGameModeInit
#endif
#define OnGameModeInit sandac_OnGameModeInit
#if defined sandac_OnGameModeInit
forward sandac_OnGameModeInit();
#endif

public OnGameModeExit()
{
	KillTimer(SecSync);

    #if defined sandac_OnGameModeExit
        return sandac_OnGameModeExit();
    #else
        return 1;
    #endif
}

#if defined _ALS_OnGameModeExit
    #undef OnGameModeExit
#else
    #define _ALS_OnGameModeExit
#endif
#define OnGameModeExit sandac_OnGameModeExit
#if defined sandac_OnGameModeExit
forward sandac_OnGameModeExit();
#endif

public OnPlayerDeath(playerid, killerid, reason)
{
	for(new i; i != 47; i++)
	{
	    PlayerSync[playerid][acWeapon][i] = 0;
	}

    #if defined sandac_OnPlayerDeath
        return sandac_OnPlayerDeath(playerid, killerid, reason);
    #else
        return 1;
    #endif
}

#if defined _ALS_OnPlayerDeath
    #undef OnPlayerDeath
#else
    #define _ALS_OnPlayerDeath
#endif
#define OnPlayerDeath sandac_OnPlayerDeath
#if defined sandac_OnPlayerDeath
forward sandac_OnPlayerDeath(playerid, killerid, reason);
#endif

public OnPlayerStateChange(playerid, newstate, oldstate)
{
    if(newstate == PLAYER_STATE_SPECTATING && !PlayerSync[playerid][acSpectate]) OnPlayerCheat(playerid, 9);
	if(newstate == PLAYER_STATE_DRIVER || newstate == PLAYER_STATE_PASSENGER)
    {
         new model = GetVehicleModel(GetPlayerVehicleID(playerid));
         switch(model)
         {
             case 592, 577, 511, 512, 520, 593, 553, 476, 519, 460, 513, 548, 425, 417, 487, 488, 497, 563, 447, 469: PlayerSync[playerid][acWeapon][46] = 1;
             case 457: PlayerSync[playerid][acWeapon][2] = 1;
             case 596, 597, 598, 599: PlayerSync[playerid][acWeapon][25] = 1;
         }
    }
    
    #if defined sandac_OnPlayerStateChange
        return sandac_OnPlayerStateChange(playerid, newstate, oldstate);
    #else
        return 1;
    #endif
}

#if defined _ALS_OnPlayerStateChange
    #undef OnPlayerStateChange
#else
    #define _ALS_OnPlayerStateChange
#endif
#define OnPlayerStateChange sandac_OnPlayerStateChange
#if defined sandac_OnPlayerStateChange
forward sandac_OnPlayerStateChange(playerid, newstate, oldstate);
#endif

public OnPlayerWeaponShot(playerid, weaponid, hittype, hitid, Float:fX, Float:fY, Float:fZ)
{
	if(PlayerSync[playerid][acWeapon][weaponid] > 0) PlayerSync[playerid][acWeapon][weaponid]--;
	if(hittype == BULLET_HIT_TYPE_PLAYER)
	{
	    new Float:HitPosX, Float:HitPosY, Float:HitPosZ, Float:Temp;
    	GetPlayerLastShotVectors(playerid, Temp, Temp, Temp, HitPosX, HitPosY, HitPosZ);
    	if(!IsPlayerInRangeOfPoint(playerid, 3.0, HitPosX, HitPosY, HitPosZ) && !IsPlayerFacingCoords(playerid, HitPosX, HitPosY, MAX_DIST)) OnPlayerCheat(playerid, 13);
    	
	    if(IsPlayerConnected(hitid))
	    {
	        GetPlayerLastShotVectors(playerid, Temp, Temp, Temp, HitPosX, HitPosY, HitPosZ);
	       	new Float:dist = GetPlayerDistanceFromPoint(hitid, HitPosX, HitPosY, HitPosZ);
			if(dist >= 1.0) OnPlayerCheat(playerid, 14);
	    
		    new Float: htX, Float: htY, Float: htZ;
		    GetPlayerPos(hitid, htX, htY, htZ);
		    if(GetPlayerDistanceFromPoint(playerid, htX, htY, htZ) > 1000.0) OnPlayerCheat(playerid, 15);
	    }
	}

    #if defined sandac_OnPlayerWeaponShot
        return sandac_OnPlayerWeaponShot(playerid, weaponid, hittype, hitid, fX, fY, fZ);
    #else
        return 1;
    #endif
}

#if defined _ALS_OnPlayerWeaponShot
    #undef OnPlayerWeaponShot
#else
    #define _ALS_OnPlayerWeaponShot
#endif
#define OnPlayerWeaponShot sandac_OnPlayerWeaponShot
#if defined sandac_OnPlayerWeaponShot
forward sandac_OnPlayerWeaponShot(playerid, weaponid, hittype, hitid, Float:fX, Float:fY, Float:fZ);
#endif

stock Float: GetPlayerSpeed(playerid)
{
	new Float: velX, Float: velY, Float: velZ;
	if(IsPlayerInAnyVehicle(playerid)) GetVehicleVelocity(GetPlayerVehicleID(playerid), velX, velY, velZ);
	else GetPlayerVelocity(playerid, velX, velY, velZ);
	return (VectorSize(velX, velY, velZ) * 100.0);
}

stock GetNearbyPlayers(playerid)
{
	new count, intid, worldid, Float: posX, Float: posY, Float: posZ;
	intid = GetPlayerInterior(playerid);
	worldid = GetPlayerVirtualWorld(playerid);
	GetPlayerPos(playerid, posX, posY, posZ);
	#if defined foreach
 	foreach(new i: Player) {
	#else
 	for(new i; i != MAX_PLAYERS; i++) {
 		if(!IsPlayerConnected(i)) continue;
	#endif
	    if(playerid == i) continue;
		if(intid == GetPlayerInterior(i) && worldid == GetPlayerVirtualWorld(i) && IsPlayerInRangeOfPoint(i, posX, posY, posZ, 50.0)) count++;
	}
	return count;
}

forward sandac_OnSecSync();
public sandac_OnSecSync()
{
	#if defined foreach
 	foreach(new i: Player) {
	#else
 	for(new i; i != MAX_PLAYERS; i++) {
 		if(!IsPlayerConnected(i)) continue;
	#endif

		if((GetPlayerDistanceFromPoint(i, PlayerSync[i][acX], PlayerSync[i][acY], PlayerSync[i][acZ]) <= 85.0) || (PlayerSync[i][acSpectate]) || (GetPlayerSurfingObjectID(i) == INVALID_OBJECT_ID)) GetPlayerPos(i, PlayerSync[i][acX], PlayerSync[i][acY], PlayerSync[i][acZ]);
		else OnPlayerCheat(i, 1);

		new Float: tsyncvelX, Float: tsyncvelY, Float: tsyncvelZ;
		GetPlayerVelocity(i, tsyncvelX, tsyncvelY, tsyncvelZ);
		if(GetPlayerSpeed(i) > 250.0 && GetPlayerState(i) == 1 || GetPlayerState(i) == 2 && GetPlayerSpeed(i) > 250.0) OnPlayerCheat(i, 2);
		if(GetNearbyPlayers(i) >= 2 && ((tsyncvelX == -0.0, tsyncvelY == -20.0) || (tsyncvelX == 20.0, tsyncvelY == 0.0) || (tsyncvelX == 20.0, tsyncvelY == -0.0))) OnPlayerCheat(i, 3);
		
		new Float: tsyncHealth;
		GetPlayerHealth(i, tsyncHealth);
		if(tsyncHealth <= PlayerSync[i][acHealth]) GetPlayerHealth(i, PlayerSync[i][acHealth]);
		else OnPlayerCheat(i, 4);
		
		new Float: tsyncArmour;
		GetPlayerArmour(i, tsyncArmour);
		if(tsyncArmour <= PlayerSync[i][acArmour]) GetPlayerArmour(i, PlayerSync[i][acArmour]);
		else OnPlayerCheat(i, 5);
		
		if(GetPlayerMoney(i) > PlayerSync[i][acMoney]) OnPlayerCheat(i, 6);
		
		if(GetPlayerPing(i) >= 150) OnPlayerCheat(i, 7);
		
		if(GetPlayerAmmo(i) > PlayerSync[i][acWeapon][GetPlayerWeapon(i)]) OnPlayerCheat(i, 8);

		new Float: parkX, Float: parkY, Float: parkZ;
		GetPlayerPos(i, parkX, parkY, parkZ);
		if(GetPlayerAnimationIndex(i) == -1 && parkZ > 0) OnPlayerCheat(i, 10);
		
		new animlib[30], animname[30];
		if(!IsPlayerInAnyVehicle(i))
		{
	        GetAnimationName(GetPlayerAnimationIndex(i), animlib, sizeof animlib, animname, sizeof animname);
	        if(strcmp(animlib, "PED", true) == 0 && strcmp(animname, "RUN_PLAYER", true) == 0 && GetPlayerSkin(i) != 0) OnPlayerCheat(i, 11);
        }
        
        if(GetPlayerState(i) == 1)
		{
		    new Float: ghX, Float: ghY, Float: ghZ;
			GetPlayerVelocity(i, ghX, ghY, ghZ);
		    if((ghX == -0.0 && ghY == -20.0) || (ghX == -20.0 && ghY == 0.0) || (ghX == 0.0 && ghY == 20.0)) OnPlayerCheat(i, 12);
		}
	}
	return 1;
}

stock sandac_SetPlayerPos(playerid, Float: X, Float: Y, Float: Z)
{
	PlayerSync[playerid][acX] = X;
	PlayerSync[playerid][acY] = Y;
	PlayerSync[playerid][acZ] = Z;

    return SetPlayerPos(playerid, X, Y, Z);
}
#if defined _ALS_SetPlayerPos
        #undef SetPlayerPos
#else
        #define _ALS_SetPlayerPos
#endif
#define SetPlayerPos sandac_SetPlayerPos

stock sandac_SetPlayerHealth(playerid, Float: Health)
{
	PlayerSync[playerid][acHealth] = Health;
    return SetPlayerHealth(playerid, Health);
}
#if defined _ALS_SetPlayerHealth
        #undef SetPlayerHealth
#else
        #define _ALS_SetPlayerHealth
#endif
#define SetPlayerHealth sandac_SetPlayerHealth

stock sandac_GetPlayerHealth(playerid, &Float: Health)
{
	Health = PlayerSync[playerid][acHealth];
    return 1;
}
#if defined _ALS_GetPlayerHealth
        #undef GetPlayerHealth
#else
        #define _ALS_GetPlayerHealth
#endif
#define GetPlayerHealth sandac_GetPlayerHealth

stock sandac_SetPlayerArmour(playerid, Float: Armour)
{
	PlayerSync[playerid][acArmour] = Armour;
    return SetPlayerArmour(playerid, Armour);
}
#if defined _ALS_SetPlayerArmour
        #undef SetPlayerArmour
#else
        #define _ALS_SetPlayerArmour
#endif
#define SetPlayerArmour sandac_SetPlayerArmour

stock sandac_GetPlayerArmour(playerid, &Float: Armour)
{
	Armour = PlayerSync[playerid][acArmour];
    return 1;
}
#if defined _ALS_GetPlayerArmour
        #undef GetPlayerArmour
#else
        #define _ALS_GetPlayerArmour
#endif
#define GetPlayerArmour sandac_GetPlayerArmour

stock sandac_GivePlayerMoney(playerid, money)
{
	PlayerSync[playerid][acMoney] += money;
    return GivePlayerMoney(playerid, money);
}
#if defined _ALS_GivePlayerMoney
        #undef GivePlayerMoney
#else
        #define _ALS_GivePlayerMoney
#endif
#define GivePlayerMoney sandac_GivePlayerMoney

stock sandac_ResetPlayerMoney(playerid)
{
	PlayerSync[playerid][acMoney] = 0;
    return ResetPlayerMoney(playerid);
}
#if defined _ALS_ResetPlayerMoney
        #undef ResetPlayerMoney
#else
        #define _ALS_ResetPlayerMoney
#endif
#define ResetPlayerMoney sandac_ResetPlayerMoney

stock sandac_GetPlayerMoney(playerid)
{
    return PlayerSync[playerid][acMoney];
}
#if defined _ALS_GetPlayerMoney
        #undef GetPlayerMoney
#else
        #define _ALS_GetPlayerMoney
#endif
#define GetPlayerMoney sandac_GetPlayerMoney

stock sandac_SetSpawnInfo(playerid, team, skin, Float:x, Float:y, Float:z, Float:Angle, weapon1, weapon1_ammo, weapon2, weapon2_ammo, weapon3, weapon3_ammo)
{
	PlayerSync[playerid][acX] = x;
	PlayerSync[playerid][acY] = y;
	PlayerSync[playerid][acZ] = z;
	PlayerSync[playerid][acHealth] = 100.0;
	PlayerSync[playerid][acArmour] = 0.0;

	PlayerSync[playerid][acWeapon][weapon1] = weapon1_ammo;
	PlayerSync[playerid][acWeapon][weapon2] = weapon2_ammo;
	PlayerSync[playerid][acWeapon][weapon3] = weapon3_ammo;

	return SetSpawnInfo(playerid, team, skin, x, y, z, Angle, weapon1, weapon1_ammo, weapon2, weapon2_ammo, weapon3, weapon3_ammo);
}
#if defined _ALS_SetSpawnInfo
        #undef SetSpawnInfo
#else
        #define _ALS_SetSpawnInfo
#endif
#define SetSpawnInfo sandac_SetSpawnInfo

stock sandac_ResetPlayerWeapons(playerid)
{
	for(new i; i != 47; i++)
	{
		PlayerSync[playerid][acWeapon] = 0;
	}
	return ResetPlayerWeapons(playerid);
}
#if defined _ALS_ResetPlayerWeapons
        #undef ResetPlayerWeapons
#else
        #define _ALS_ResetPlayerWeapons
#endif
#define ResetPlayerWeapons sandac_ResetPlayerWeapons

stock sandac_GivePlayerWeapon(playerid, weaponid, ammo)
{
	PlayerSync[playerid][acWeapon][weaponid] += ammo;
	return GivePlayerWeapon(playerid, weaponid, ammo);
}
#if defined _ALS_GivePlayerWeapon
        #undef GivePlayerWeapon
#else
        #define _ALS_GivePlayerWeapon
#endif
#define GivePlayerWeapon sandac_GivePlayerWeapon

stock sandac_GetPlayerAmmo(playerid)
{
	return PlayerSync[playerid][acWeapon][GetPlayerWeapon(playerid)];
}
#if defined _ALS_GetPlayerAmmo
        #undef GetPlayerAmmo
#else
        #define _ALS_GetPlayerAmmo
#endif
#define GetPlayerAmmo sandac_GetPlayerAmmo

stock sandac_SetPlayerAmmo(playerid, weaponid, ammo)
{
    PlayerSync[playerid][acWeapon][weaponid] = ammo;
	return SetPlayerAmmo(playerid, weaponid, ammo);
}
#if defined _ALS_SetPlayerAmmo
        #undef SetPlayerAmmo
#else
        #define _ALS_SetPlayerAmmo
#endif
#define SetPlayerAmmo sandac_SetPlayerAmmo

stock sandac_TogglePlayerSpectating(playerid, toggle)
{
	PlayerSync[playerid][acSpectate] = true;
	return TogglePlayerSpectating(playerid, toggle);
}
#if defined _ALS_TogglePlayerSpectating
        #undef TogglePlayerSpectating
#else
        #define _ALS_TogglePlayerSpectating
#endif
#define TogglePlayerSpectating sandac_TogglePlayerSpectating

stock sandac_GetPlayerPos(playerid, &Float: X, &Float: Y, &Float: Z)
{
	X = PlayerSync[playerid][acX];
	Y = PlayerSync[playerid][acY];
	Z = PlayerSync[playerid][acZ];

    return 1;
}
#if defined _ALS_GetPlayerPos
        #undef GetPlayerPos
#else
        #define _ALS_GetPlayerPos
#endif
#define GetPlayerPos sandac_GetPlayerPos


public OnPlayerDisconnect(playerid, reason)
{
	PlayerSync[playerid][acSpectate] = false;
	ResetPlayerMoney(playerid);
	SetPlayerHealth(playerid, 100.0);
	SetPlayerArmour(playerid, 0.0);

    #if defined sandac_OnPlayerDisconnect
        return sandac_OnPlayerDisconnect(playerid, reason);
    #else
        return 1;
    #endif
}

#if defined _ALS_OnPlayerDisconnect
    #undef OnPlayerDisconnect
#else
    #define _ALS_OnPlayerDisconnect
#endif
#define OnPlayerDisconnect sandac_OnPlayerDisconnect
#if defined sandac_OnPlayerDisconnect
forward sandac_OnPlayerDisconnect(playerid, reason);
#endif

Float:GetAngle(Float:x, Float:y, Float:ix, Float:iy)
{
	new Float:absoluteangle, Float:tmpangle, Float:misc = 5.0;
	tmpangle = 180.0 - atan2(x-ix,y-iy);
	tmpangle += misc;
	misc *= -1;
	absoluteangle = tmpangle + misc;
	return absoluteangle;
}

stock IsPlayerFacingCoords(playerid, Float:destx, Float:desty, offset = 0)
{
	new Float:px, Float:py, Float:pz, Float:a, Float:a2;
	GetPlayerPos(playerid, px, py, pz);
	GetPlayerFacingAngle(playerid, a);
	a2 = GetAngle(px, py, destx, desty);
	if((a2 + offset) > 360) a2 = 360 - offset;
	if((a2 - offset) < 0) a2 = 0 + offset;
	if(a < (a2 + offset) && a > (a2 - offset)) return 1;
	return 0;
}
