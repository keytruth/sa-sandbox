enum eLabelData
{
	Text3D: LabelText
}
new LabelData[MAX_CWORLDS][MAX_CWORLD_PREM_LABELS][eLabelData];

public OnGameModeInit()
{
	for(new i; i != MAX_CWORLDS; i++)
	{
		for(new j; j != MAX_CWORLD_PREM_LABELS; j++)
		{
		    if(IsValidDynamic3DTextLabel(LabelData[i][j][LabelText])) DestroyDynamic3DTextLabel(LabelData[i][j][LabelText]);
		}
	}
	print("Labels System loaded!");

    #if defined sandlabels_OnGameModeInit
    	return sandlabels_OnGameModeInit();
    #else
        return 1;
    #endif
}
#if defined _ALS_OnGameModeInit
	#undef OnGameModeInit
#else
	#define _ALS_OnGameModeInit
#endif
#define OnGameModeInit sandlabels_OnGameModeInit
#if defined sandlabels_OnGameModeInit
forward sandlabels_OnGameModeInit();
#endif

ClearWorldLabels(cworld)
{
	for(new i; i != MAX_CWORLD_PREM_LABELS; i++)
	{
	    if(IsValidDynamic3DTextLabel(LabelData[cworld][i][LabelText]))
	    {
	        if((cworld + 10) == Streamer_GetIntData(STREAMER_TYPE_3D_TEXT_LABEL, LabelData[cworld][i][LabelText], E_STREAMER_WORLD_ID)) DestroyDynamic3DTextLabel(LabelData[cworld][i][LabelText]);
	    }
	}
	return 1;
}

GetFreeWorldLabel(cworld)
{
	for(new i; i != (IsPlayerPremium(FindPlayerByGID(WorldData[cworld][OwnerID]))) ? (MAX_CWORLD_PREM_LABELS) : (MAX_CWORLD_LABELS); i++)
	{
	    if(!IsValidDynamic3DTextLabel(LabelData[cworld][i][LabelText])) return i;
	}
	return -1;
}

GetWorldCountLabels(cworld)
{
	new count;
	for(new i; i != 1024; i++)
	{
		if((cworld + 10) == Streamer_GetIntData(STREAMER_TYPE_3D_TEXT_LABEL, i, E_STREAMER_WORLD_ID)) count++;
	}
	return count;
}

CMD:label(playerid, params[])
{
	if(IsWorldAdmin(playerid))
	{
	    if(GetFreeWorldLabel(GetPlayerVirtualWorld(playerid) - 10) == -1) return SCM(playerid, -1, "Превышен лимит лейблов!");
		if(sscanf(params, "s[128]", params[0])) return SCM(playerid, -1, "/label [Текст]");
		new buf[135], wd = GetPlayerVirtualWorld(playerid), freeid = GetFreeWorldLabel(wd - 10), Float: X, Float: Y, Float: Z;
		format(buf, sizeof buf, "%s ( %d )", params[0], freeid);
		GetPlayerPos(playerid, X, Y, Z);
	    LabelData[wd - 10][freeid][LabelText] = CreateDynamic3DTextLabel(buf, -1, X, Y, Z, 25.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, wd, GetPlayerInterior(playerid));
	}
	return 1;
}

CMD:dellabel(playerid, params[])
{
	if(IsWorldAdmin(playerid))
	{
		if(sscanf(params, "d", params[0])) return SCM(playerid, -1, "/dellabel [ID]");
		new wd = GetPlayerVirtualWorld(playerid);
		if(IsValidDynamic3DTextLabel(LabelData[wd - 10][params[0]][LabelText]))
		{
  			if(wd == Streamer_GetIntData(STREAMER_TYPE_3D_TEXT_LABEL, LabelData[wd - 10][params[0]][LabelText], E_STREAMER_WORLD_ID)) DestroyDynamic3DTextLabel(LabelData[wd - 10][params[0]][LabelText]);
		}
	}
	return 1;
}
