#define DIALOG_ACTOR_CMENU (350)
#define DIALOG_ACTOR_CNAME (351)
#define DIALOG_ACTOR_CSKIN (352)

const Float: vert_offset = 1.0;

enum eActorData
{
	Actor,
	Text3D: AttachName
}
new ActorData[MAX_CWORLDS][MAX_CWORLD_PREM_ACTORS][eActorData];

public OnGameModeInit()
{
	for(new i; i != MAX_CWORLDS; i++)
	{
		for(new j; j != MAX_CWORLD_PREM_ACTORS; j++)
		{
		    if(IsValidDynamicActor(ActorData[i][j][Actor]))
			{
			    DestroyDynamic3DTextLabel(ActorData[i][j][AttachName]);
				DestroyDynamicActor(ActorData[i][j][Actor]);
			}
			else
			{
				ActorData[i][j][Actor] = -1;
			}
		}
	}
	print("Actors System loaded!");

    #if defined sandactor_OnGameModeInit
    	return sandactor_OnGameModeInit();
    #else
        return 1;
    #endif
}
#if defined _ALS_OnGameModeInit
	#undef OnGameModeInit
#else
	#define _ALS_OnGameModeInit
#endif
#define OnGameModeInit sandactor_OnGameModeInit
#if defined sandactor_OnGameModeInit
forward sandactor_OnGameModeInit();
#endif

public OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
	switch(dialogid)
	{
	    case DIALOG_ACTOR_CMENU:
	    {
			if(response)
			{
			    if(listitem == 0)
			    {
			        ShowPlayerDialog(playerid, DIALOG_ACTOR_CNAME, DIALOG_STYLE_INPUT, "{FFFFFF}Íàèìåíîâàíèå àêòåðà", "{FFFFFF}Ââåäèòå èìÿ àêòåðà:", "Âûáðàòü", "Îòìåíà");
			    }
			    if(listitem == 1)
			    {
			        ShowPlayerDialog(playerid, DIALOG_ACTOR_CSKIN, DIALOG_STYLE_INPUT, "{FFFFFF}Ñêèí àêòåðà", "{FFFFFF}Ââåäèòå ñêèí(0-311) àêòåðà:", "Âûáðàòü", "Îòìåíà");
			    }
			    if(listitem == 2)
			    {
			        new wd = GetPlayerVirtualWorld(playerid);
			        if(IsDynamicActorInvulnerable(ActorData[wd - 10][SPData[playerid][SelActor]][Actor]))
			        {
                        SetDynamicActorInvulnerable(ActorData[wd - 10][SPData[playerid][SelActor]][Actor], 0);
			        }
			        else SetDynamicActorInvulnerable(ActorData[wd - 10][SPData[playerid][SelActor]][Actor], 1);
			    }
			    if(listitem == 3)
			    {
					if(IsWorldAdmin(playerid))
					{
						new acto = ActorData[GetPlayerVirtualWorld(playerid) - 10][SPData[playerid][SelActor]][Actor], Float: X, Float: Y, Float: Z, Float: A;
						GetPlayerPos(playerid, X, Y, Z);
						GetPlayerFacingAngle(playerid, A);
					    SetDynamicActorPos(acto, X, Y, Z);
					    SetDynamicActorFacingAngle(acto, A);
					    Streamer_SetIntData(STREAMER_TYPE_ACTOR, acto, E_STREAMER_INTERIOR_ID, GetPlayerInterior(playerid));
					    new wd = GetPlayerVirtualWorld(playerid);
					    if(IsValidDynamic3DTextLabel(ActorData[wd - 10][SPData[playerid][SelActor]][AttachName]))
					    {
					        Streamer_SetFloatData(STREAMER_TYPE_3D_TEXT_LABEL, ActorData[wd - 10][SPData[playerid][SelActor]][AttachName], E_STREAMER_X, X);
							Streamer_SetFloatData(STREAMER_TYPE_3D_TEXT_LABEL, ActorData[wd - 10][SPData[playerid][SelActor]][AttachName], E_STREAMER_Y, Y);
							Streamer_SetFloatData(STREAMER_TYPE_3D_TEXT_LABEL, ActorData[wd - 10][SPData[playerid][SelActor]][AttachName], E_STREAMER_Z, Z + vert_offset);
					    }
					}
			    }
			    if(listitem == 4)
			    {
			        if(IsWorldAdmin(playerid))
			        {
			        	new acto = ActorData[GetPlayerVirtualWorld(playerid) - 10][SPData[playerid][SelActor]][Actor];
			        	if(IsValidDynamic3DTextLabel(ActorData[GetPlayerVirtualWorld(playerid) - 10][SPData[playerid][SelActor]][AttachName])) DestroyDynamic3DTextLabel(ActorData[GetPlayerVirtualWorld(playerid) - 10][SPData[playerid][SelActor]][AttachName]);
			        	if(IsValidDynamicActor(acto)) DestroyDynamicActor(acto);
			        }
			    }
			}
	    }
		case DIALOG_ACTOR_CNAME:
		{
		    if(response)
			{
			    if(IsWorldAdmin(playerid))
			    {
			        if(strlen(inputtext) > 26) return SCM(playerid, -1, "Íåâåðíàÿ äëèíà íàèìåíîâàíèÿ àêòåðà!");
			        new wd = GetPlayerVirtualWorld(playerid);
			        if(IsValidDynamic3DTextLabel(ActorData[wd - 10][SPData[playerid][SelActor]][AttachName])) DestroyDynamic3DTextLabel(ActorData[wd - 10][SPData[playerid][SelActor]][AttachName]);
			        new buf[32], Float: X, Float: Y, Float: Z;
			        format(buf, sizeof buf, "%s (%d)", inputtext, SPData[playerid][SelActor]);
			        Streamer_GetFloatData(STREAMER_TYPE_ACTOR, ActorData[wd - 10][SPData[playerid][SelActor]][Actor], E_STREAMER_X, X);
					Streamer_GetFloatData(STREAMER_TYPE_ACTOR, ActorData[wd - 10][SPData[playerid][SelActor]][Actor], E_STREAMER_Y, Y);
					Streamer_GetFloatData(STREAMER_TYPE_ACTOR, ActorData[wd - 10][SPData[playerid][SelActor]][Actor], E_STREAMER_Z, Z);
			        ActorData[wd - 10][SPData[playerid][SelActor]][AttachName] = CreateDynamic3DTextLabel(buf, -1, X, Y, Z + vert_offset, 20.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, GetPlayerVirtualWorld(playerid), GetPlayerInterior(playerid));
				}
			}
		}
		case DIALOG_ACTOR_CSKIN:
		{
		    if(response)
			{
			    if(IsWorldAdmin(playerid))
			    {
			        if(!(strval(inputtext) >= 0 && strval(inputtext) <= 311)) return SCM(playerid, -1, "Íåâåðíûé ñêèí!");
					Streamer_SetIntData(STREAMER_TYPE_ACTOR, ActorData[GetPlayerVirtualWorld(playerid) - 10][SPData[playerid][SelActor]][Actor], E_STREAMER_MODEL_ID, strval(inputtext));
				}
			}
		}
	}
	#if defined sandactor_OnDialogResponse
    	return sandactor_OnDialogResponse(playerid, dialogid, response, listitem, inputtext);
    #else
        return 1;
    #endif
}
#if defined _ALS_OnDialogResponse
	#undef OnDialogResponse
#else
	#define _ALS_OnDialogResponse
#endif
#define OnDialogResponse sandactor_OnDialogResponse
#if defined sandactor_OnDialogResponse
forward sandactor_OnDialogResponse(playerid, dialogid, response, listitem, inputtext[]);
#endif

ClearWorldActors(cworld)
{
    for(new i; i != MAX_CWORLD_PREM_ACTORS; i++)
	{
	    if(IsValidDynamic3DTextLabel(ActorData[cworld][i][AttachName])) DestroyDynamic3DTextLabel(ActorData[cworld][i][AttachName]);
	}
	foreach(new i: Actor)
	{
	    if((cworld + 10) == GetDynamicActorVirtualWorld(i)) DestroyDynamicActor(i);
	}
	return 1;
}

GetFreeActor(cworld)
{
	for(new i; i != (IsPlayerPremium(FindPlayerByGID(WorldData[cworld][OwnerID]))) ? (MAX_CWORLD_PREM_ACTORS) : (MAX_CWORLD_ACTORS); i++)
	{
	    if(!IsValidDynamicActor(ActorData[cworld][i][Actor])) return i;
	}
	return -1;
}

GetWorldCountActors(worldid)
{
	new count;
	foreach(new i: Actor)
	{
		if(worldid == GetDynamicActorVirtualWorld(i)) count++;
	}
	return count;
}

GetDynamicActorModel(actorid)
{
	if(IsValidDynamicActor(actorid)) return Streamer_GetIntData(STREAMER_TYPE_ACTOR, actorid, E_STREAMER_MODEL_ID);
	return -1;
}

ShowActorMenu(playerid, cactorid)
{
	SPData[playerid][SelActor] = cactorid;
	new name[21], buf[256] = "{FFFFFF}Îïöèÿ\tÏàðàìåòð", cworld = (GetPlayerVirtualWorld(playerid) - 10), acn[32];
	GetDynamic3DTextLabelText(ActorData[cworld][cactorid][AttachName], acn);
	format(name, sizeof name, "{FFFFFF}Àêòåð ¹%d", cactorid);
	format(buf, sizeof buf, "{FFFFFF}Îïöèÿ\tÏàðàìåòð");
	format(buf, sizeof buf, "%s\nÍàèìåíîâàíèå\t[ %s ]", buf, acn);
	format(buf, sizeof buf, "%s\nÑêèí àêòåðà\t[ %d ]", buf, GetDynamicActorModel(ActorData[cworld][cactorid][Actor]));
	format(buf, sizeof buf, "%s\nÍåóÿçâèìîñòü\t[ %s ]", buf, (IsDynamicActorInvulnerable(ActorData[cworld][cactorid][Actor])) ? ("+") : ("-"));
	format(buf, sizeof buf, "%s\nÎáíîâèòü ïîçèöèþ\t[ Îïöèîíàëüíî ]", buf);
	format(buf, sizeof buf, "%s\nÓäàëèòü àêòåðà\t[ Îïöèîíàëüíî ]", buf);
	ShowPlayerDialog(playerid, DIALOG_ACTOR_CMENU, DIALOG_STYLE_TABLIST_HEADERS, name, buf, "Âûáðàòü", "Îòìåíà");
	return 1;
}

CMD:actor(playerid, params[])
{
	if(IsWorldAdmin(playerid))
	{
		if(sscanf(params, "d", params[0]))
		{
		    if(GetFreeActor(GetPlayerVirtualWorld(playerid) - 10) != -1)
		    {
		    	new Float: X, Float: Y, Float: Z, Float: A;
		    	GetPlayerPos(playerid, X, Y, Z);
		    	GetPlayerFacingAngle(playerid, A);
		    	SetTimerEx("OnCreatingActor", 1000, false, "dffff", playerid, X, Y, Z, A);
		    }
		    else SCM(playerid, -1, "Ïðåâûøåí ëèìèò àêòåðîâ â äàííîì ìèðå!");
		}
		else
		{
			if(IsValidDynamicActor(ActorData[GetPlayerVirtualWorld(playerid) - 10][params[0]][Actor]))
			{
			    if(GetPlayerVirtualWorld(playerid) == GetDynamicActorVirtualWorld(ActorData[GetPlayerVirtualWorld(playerid) - 10][params[0]][Actor]))
			    {
			        SPData[playerid][SelActor] = params[0];
			        ShowActorMenu(playerid, params[0]);
			    }
			}
			else SCM(playerid, -1, "Äàííîãî àêòåðà íå ñóùåñòâóåò!");
		}
	}
	return 1;
}

forward OnCreatingActor(playerid, Float: X, Float: Y, Float: Z, Float: A);
public OnCreatingActor(playerid, Float: X, Float: Y, Float: Z, Float: A)
{
	if(IsWorldAdmin(playerid) && (GetFreeActor(GetPlayerVirtualWorld(playerid) - 10) != -1))
	{
		new wd = GetPlayerVirtualWorld(playerid), frect = GetFreeActor(wd - 10);
	    ActorData[wd - 10][frect][Actor] = CreateDynamicActor(0, X, Y, Z, A, 1, 100.0, wd, GetPlayerInterior(playerid));
		new buf[128];
		format(buf, sizeof buf, "Àêòåð ¹%d óñïåøíî ñîçäàí!", frect);
		SCM(playerid, -1, buf);
	}
	return 1;
}
