const Float: pass_range = 2.5;

enum ePassData
{
	InputPass,
	OutputPass
}
new PassData[MAX_CWORLDS][MAX_CW_PREM_PASSES][ePassData];

public OnGameModeInit()
{
	for(new i; i != MAX_CWORLDS; i++)
	{
		for(new j; j != MAX_CW_PREM_PASSES; j++)
		{
		    if(IsValidDynamicPickup(PassData[i][j][InputPass])) DestroyDynamicPickup(PassData[i][j][InputPass]);
		    else PassData[i][j][InputPass] = -1;
		    if(IsValidDynamicPickup(PassData[i][j][OutputPass])) DestroyDynamicPickup(PassData[i][j][OutputPass]);
		    else PassData[i][j][OutputPass] = -1;
		}
	}
	print("Passes System loaded!");

    #if defined sandpasses_OnGameModeInit
    	return sandpasses_OnGameModeInit();
    #else
        return 1;
    #endif
}
#if defined _ALS_OnGameModeInit
	#undef OnGameModeInit
#else
	#define _ALS_OnGameModeInit
#endif
#define OnGameModeInit sandpasses_OnGameModeInit
#if defined sandpasses_OnGameModeInit
forward sandpasses_OnGameModeInit();
#endif

public OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
	if(newkeys & KEY_SECONDARY_ATTACK)
	{
		if(GetPlayerVirtualWorld(playerid) >= 10)
		{
		    new Float: X, Float: Y, Float: Z, Float: exitX, Float: exitY, Float: exitZ,
		        wd = GetPlayerVirtualWorld(playerid);
		        
		    for(new i; i != MAX_CW_PREM_PASSES; i++)
		    {
		        if(IsValidDynamicPickup(PassData[wd - 10][i][InputPass]) && IsValidDynamicPickup(PassData[wd - 10][i][OutputPass]))
		        {
		        	Streamer_GetFloatData(STREAMER_TYPE_PICKUP, PassData[wd - 10][i][InputPass], E_STREAMER_X, X);
		        	Streamer_GetFloatData(STREAMER_TYPE_PICKUP, PassData[wd - 10][i][InputPass], E_STREAMER_Y, Y);
		        	Streamer_GetFloatData(STREAMER_TYPE_PICKUP, PassData[wd - 10][i][InputPass], E_STREAMER_Z, Z);
		        	
		        	Streamer_GetFloatData(STREAMER_TYPE_PICKUP, PassData[wd - 10][i][OutputPass], E_STREAMER_X, exitX);
		        	Streamer_GetFloatData(STREAMER_TYPE_PICKUP, PassData[wd - 10][i][OutputPass], E_STREAMER_Y, exitY);
		        	Streamer_GetFloatData(STREAMER_TYPE_PICKUP, PassData[wd - 10][i][OutputPass], E_STREAMER_Z, exitZ);
		        }
		        else continue;
		        
		        if(IsPlayerInRangeOfPoint(playerid, pass_range, X, Y, Z))
				{
					SetPlayerPos(playerid, exitX, exitY, exitZ);
					break;
				}
				else
				{
					if(IsPlayerInRangeOfPoint(playerid, pass_range, exitX, exitY, exitZ))
					{
						SetPlayerPos(playerid, X, Y, Z);
						break;
					}
				}
		    }
		}
	}
	#if defined spass_OnPlayerKeyStateChange
    	return spass_OnPlayerKeyStateChange(playerid, newkeys, oldkeys);
    #else
        return 1;
    #endif
}
#if defined _ALS_OnPlayerKeyStateChange
	#undef OnPlayerKeyStateChange
#else
	#define _ALS_OnPlayerKeyStateChange
#endif
#define OnPlayerKeyStateChange spass_OnPlayerKeyStateChange
#if defined spass_OnPlayerKeyStateChange
forward spass_OnPlayerKeyStateChange(playerid, newkeys, oldkeys);
#endif

GetFreePass(cworld)
{
	for(new i; i != (IsPlayerPremium(FindPlayerByGID(WorldData[cworld][OwnerID]))) ? (MAX_CW_PREM_PASSES) : (MAX_CW_PASSES); i++)
	{
	    if(IsValidDynamicPickup(PassData[cworld][i][InputPass]))
	    {
	        if(!IsValidDynamicPickup(PassData[cworld][i][OutputPass])) return i;
	    }
	    else return i;
	}
	return -1;
}

GetFreeSubPass(cworld, pass)
{
	if(!IsValidDynamicPickup(PassData[cworld][pass][InputPass])) return 0;
	if(!IsValidDynamicPickup(PassData[cworld][pass][OutputPass])) return 1;
	return -1;
}

GetCountPasses(cworld)
{
	new count;
	for(new i; i != MAX_CW_PREM_PASSES; i++)
	{
	    if(IsValidDynamicPickup(PassData[cworld][i][InputPass]) && IsValidDynamicPickup(PassData[cworld][i][OutputPass])) count++;
	}
	return count;
}

ClearWorldPasses(cworld)
{
	for(new i; i != MAX_CW_PREM_PASSES; i++)
	{
	    if(IsValidDynamicPickup(PassData[cworld][i][InputPass])) DestroyDynamicPickup(PassData[cworld][i][InputPass]), PassData[cworld][i][InputPass] = -1;
		if(IsValidDynamicPickup(PassData[cworld][i][OutputPass])) DestroyDynamicPickup(PassData[cworld][i][OutputPass]), PassData[cworld][i][OutputPass] = -1;
	}
	return 1;
}

CMD:pass(playerid)
{
	if(IsWorldAdmin(playerid))
	{
	    new wd = GetPlayerVirtualWorld(playerid);
		new passid = GetFreePass(wd - 10);
		new subpassid = GetFreeSubPass(wd - 10, passid);
		
		if(passid == -1) return SCM(playerid, -1, "Превышен лимит проходов!");
	    else
	    {
			if(subpassid != -1)
			{
	        	new Float: X, Float: Y, Float: Z;
	        	GetPlayerPos(playerid, X, Y, Z);
	        	new buf[128];
        		if(subpassid == 0)
				{
					PassData[wd - 10][passid][InputPass] = CreateDynamicPickup(1318, 1, X, Y, Z, GetPlayerVirtualWorld(playerid));
					format(buf, sizeof buf, "Вы установили вход проходу №%d. Установите выход в другом месте - /pass.", passid);
					SCM(playerid, -1, buf);
				}
				else if(subpassid == 1)
				{
					PassData[wd - 10][passid][OutputPass] = CreateDynamicPickup(1318, 1, X, Y, Z, GetPlayerVirtualWorld(playerid));
					format(buf, sizeof buf, "Вы установили выход проходу №%d. (Удалить проход - /delpass)", passid);
					SCM(playerid, -1, buf);
				}
			}
			else return SCM(playerid, -1, "Превышен лимит проходов!");
		}
	}
	return 1;
}

CMD:delpass(playerid, params[])
{
	if(sscanf(params, "d", params[0])) return SCM(playerid, -1, "/delpass [Pass ID]");
	if(IsWorldAdmin(playerid))
	{
	    new wd = GetPlayerVirtualWorld(playerid);
	    if(IsValidDynamicPickup(PassData[wd - 10][params[0]][InputPass]) && IsValidDynamicPickup(PassData[wd - 10][params[0]][OutputPass]))
	    {
	        new buf[128];
	        format(buf, sizeof buf, "Вы успешно удалили проход №%d!", params[0]);
	        SCM(playerid, -1, buf);
			if(IsValidDynamicPickup(PassData[wd - 10][params[0]][InputPass])) DestroyDynamicPickup(PassData[wd - 10][params[0]][InputPass]), PassData[wd - 10][params[0]][InputPass] = -1;
			if(IsValidDynamicPickup(PassData[wd - 10][params[0]][OutputPass])) DestroyDynamicPickup(PassData[wd - 10][params[0]][OutputPass]), PassData[wd - 10][params[0]][OutputPass] = -1;
		}
		else SCM(playerid, -1, "Данного прохода не существует или у него не сделан выход!");
	}
	return 1;
}
