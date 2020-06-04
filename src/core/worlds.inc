#define MAX_CWORLDS (10)
#define DIALOG_WORLD (1)
#define DIALOG_LIST (2)
#define DIALOG_WOPTIONS (3)
#define DIALOG_WO_NAME (4)
#define DIALOG_WO_DESC (5)
#define DIALOG_WO_TIME (6)
#define DIALOG_WO_WEATHER (7)
#define DIALOG_WO_NONE (8)
#define IsWorldOwnerOnline(%0) (WorldData[%0][LastTimeOwner] > (gettime() - (5 * 60)))
#define IsWorldAdmin(%0) (PlayerData[%0][pID] == WorldData[GetPlayerVirtualWorld(%0) - 10][OwnerID])
#define IsWorldModer(%0) (SPData[%0][Moder] == (GetPlayerVirtualWorld(%0) - 10))

enum WorldInfo
{
	Name[16],
	Description[128],

	bool: Public,
	bool: Private,
	
	bool: AC,
	
	Time,
	Weather,
	
	OwnerID,
	LastTimeOwner,
	
	Float: spawnX, Float: spawnY, Float: spawnZ, Float: spawnA
}
new WorldData[MAX_CWORLDS][WorldInfo];
new clear_worlddata[WorldInfo];
new WorldTimerSec,
	LocalWorlds[MAX_CWORLDS];

public OnGameModeInit()
{
	for(new i; i != MAX_CWORLDS; i++)
	{
	    WorldData[i] = clear_worlddata;
	    WorldData[i][OwnerID] = -1;
	    
		LocalWorlds[i] = -1;
	}
	WorldTimerSec = SetTimer("OnWorldTimer", 1000, true);
	print("Worlds System loaded!");
	
    #if defined sandworlds_OnGameModeInit
    	return sandworlds_OnGameModeInit();
    #else
        return 1;
    #endif
}
#if defined _ALS_OnGameModeInit
	#undef OnGameModeInit
#else
	#define _ALS_OnGameModeInit
#endif
#define OnGameModeInit sandworlds_OnGameModeInit
#if defined sandworlds_OnGameModeInit
forward sandworlds_OnGameModeInit();
#endif

public OnGameModeExit()
{
	KillTimer(WorldTimerSec);
	
    #if defined sandworlds_OnGameModeExit
    	return sandworlds_OnGameModeExit();
    #else
        return 1;
    #endif
}
#if defined _ALS_OnGameModeExit
	#undef OnGameModeExit
#else
	#define _ALS_OnGameModeExit
#endif
#define OnGameModeExit sandworlds_OnGameModeExit
#if defined sandworlds_OnGameModeExit
forward sandworlds_OnGameModeExit();
#endif

public OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
	switch(dialogid)
	{
	    case DIALOG_WORLD:
	    {
	        if(response)
	        {
				new buf[256] = "{FFFFFF}Наименование\t{FFFFFF}Кол-во игроков", lw = -1;
				if(listitem == 0)
				{
				    for(new i; i != MAX_CWORLDS; i++)
				    {
						if(WorldData[i][Public] && (GetWorldOnline(i + 10) > 0))
						{
						    lw++;
						    LocalWorlds[lw] = i;
							format(buf, sizeof buf, "%s\n%s\t%d", buf, WorldData[i][Name], GetWorldOnline(i + 10));
							ShowPlayerDialog(playerid, DIALOG_LIST, DIALOG_STYLE_TABLIST_HEADERS, "{FFFFFF}Миры", buf, "Выбрать", "Отмена");
						}
					}
				}
				if(listitem == 1)
				{
					if(GetWorldByOwnerID(PlayerData[playerid][pID]) != -1) SCM(playerid, -1, "У Вас уже существует мир, вернитесь в него.");
					else
					{
						if(GetActiveWorlds() < 10)
						{
						    new cworld = GetFreeWorld();
						    WorldData[cworld][OwnerID] = PlayerData[playerid][pID];
							ConnectPlayerToWorld(playerid, cworld);
						}
						else SCM(playerid, -1, "Слишком много существует активных миров!");
					}
				}
				if(listitem == 2)
				{
				    new wdl = GetWorldByOwnerID(PlayerData[playerid][pID]);
					WorldData[wdl][LastTimeOwner] = gettime();
					ConnectPlayerToWorld(playerid, wdl);
				}
				if(listitem == 3) SetPlayerVirtualWorld(playerid, 0), HidePagenatorForPlayer(playerid);
	        }
	    }
	    case DIALOG_LIST:
	    {
	        if(response)
	        {
				if(!IsPlayerConnected(FindPlayerByGID(WorldData[LocalWorlds[listitem]][OwnerID]))) return SCM(playerid, -1, "Данный мир заморожен до возвращения его владельца!");
				if(GetPlayerVirtualWorld(FindPlayerByGID(WorldData[LocalWorlds[listitem]][OwnerID])) != (LocalWorlds[listitem] + 10)) return SCM(playerid, -1, "Данный мир заморожен до возвращения его владельца!");
				if(WorldData[LocalWorlds[listitem]][Private])
				{
				    new buf[100];
				    format(buf, sizeof buf, "Данный мир является приватным: отпишите его владельцу %s(%d)", GetName(FindPlayerByGID(WorldData[LocalWorlds[listitem]][OwnerID])), FindPlayerByGID(WorldData[LocalWorlds[listitem]][OwnerID]));
					SCM(playerid, -1, buf);
					return 1;
				}
				ConnectPlayerToWorld(playerid, LocalWorlds[listitem]);
			}
	    }
	    case DIALOG_WOPTIONS:
	    {
	        if(response)
	        {
	            new wd = (GetPlayerVirtualWorld(playerid) - 10);
				if(WorldData[wd][OwnerID] == PlayerData[playerid][pID])
				{
					if(listitem == 0) ShowPlayerDialog(playerid, DIALOG_WO_NAME, DIALOG_STYLE_INPUT, "{FFFFFF}Наименование мира", "Введите наименование мира:", "Выбрать", "Отмена");
					if(listitem == 1) ShowPlayerDialog(playerid, DIALOG_WO_DESC, DIALOG_STYLE_INPUT, "{FFFFFF}Описание мира", "Введите описание мира:", "Выбрать", "Отмена");
					if(listitem == 2) WorldData[wd][Public] = !WorldData[wd][Public], ShowWorldOptions(playerid);
					if(listitem == 3) WorldData[wd][Private] = !WorldData[wd][Private], ShowWorldOptions(playerid);
					if(listitem == 4) WorldData[wd][AC] = !WorldData[wd][AC], ShowWorldOptions(playerid);
					if(listitem == 5) ShowPlayerDialog(playerid, DIALOG_WO_TIME, DIALOG_STYLE_INPUT, "{FFFFFF}Время мира", "Введите время(0-23) мира:", "Выбрать", "Отмена");
					if(listitem == 6) ShowPlayerDialog(playerid, DIALOG_WO_WEATHER, DIALOG_STYLE_INPUT, "{FFFFFF}Погода мира", "Введите погода(0-255) мира:", "Выбрать", "Отмена");
					if(listitem == 7)
					{
					    new buf[512];
						foreach(new i: Player)
					    {
					        format(buf, sizeof buf, "%s%s%s(%d)%s\n", buf, (IsWorldAdmin(playerid) || IsWorldModer(playerid)) ? ("{FF2525}") : (""), GetName(i), i, (IsWorldAdmin(playerid) || IsWorldModer(playerid)) ? ("{FFFFFF}") : (""));
					    }
						ShowPlayerDialog(playerid, DIALOG_WO_NONE, DIALOG_STYLE_LIST, "{FFFFFF}Онлайн мира", buf, "Закрыть", "");
					}
					if(listitem == 8)
					{
						new buf[256];
						format(buf, sizeof buf, "{FFFFFF}Объект\t{FFFFFF}Кол-во");
						format(buf, sizeof buf, "%s\nИгроки\t[ %d ]", buf, GetWorldOnline(GetPlayerVirtualWorld(playerid)));
						format(buf, sizeof buf, "%s\nТранспорт\t[ %d / %d ]", buf, GetWorldVehCount(GetPlayerVirtualWorld(playerid)), (IsPlayerPremium(GetCWorldOwnerPlayer(GetPlayerVirtualWorld(playerid) - 10))) ? (MAX_WORLD_PREM_VEH) : (MAX_WORLD_VEH));
						format(buf, sizeof buf, "%s\nПроходы\t[ %d / %d ]", buf, GetCountPasses(GetPlayerVirtualWorld(playerid) - 10), (IsPlayerPremium(GetCWorldOwnerPlayer(GetPlayerVirtualWorld(playerid) - 10))) ? (MAX_CW_PREM_PASSES) : (MAX_CW_PREM_PASSES));
						format(buf, sizeof buf, "%s\nОбъекты\t[ %d / %d ]", buf, GetWorldCountObj(GetPlayerVirtualWorld(playerid) - 10), (IsPlayerPremium(GetCWorldOwnerPlayer(GetPlayerVirtualWorld(playerid) - 10))) ? (MAX_CWORLD_PREM_OBJ) : (MAX_CWORLD_OBJ));
						format(buf, sizeof buf, "%s\nЛейблы\t[ %d / %d ]", buf, GetWorldCountLabels(GetPlayerVirtualWorld(playerid) - 10), (IsPlayerPremium(GetCWorldOwnerPlayer(GetPlayerVirtualWorld(playerid) - 10))) ? (MAX_CWORLD_PREM_LABELS) : (MAX_CWORLD_LABELS));
						format(buf, sizeof buf, "%s\nАктеры\t[ %d / %d ]", buf, GetWorldCountActors(GetPlayerVirtualWorld(playerid)), (IsPlayerPremium(GetCWorldOwnerPlayer(GetPlayerVirtualWorld(playerid) - 10))) ? (MAX_CWORLD_PREM_ACTORS) : (MAX_CWORLD_ACTORS));
						format(buf, sizeof buf, "%s\nСценарные области\t[ %d / %d ]", buf, GetCountScenariosWorld(GetPlayerVirtualWorld(playerid) - 10), (IsPlayerPremium(GetCWorldOwnerPlayer(GetPlayerVirtualWorld(playerid) - 10))) ? (MAX_CW_PREM_SCENARIOS) : (MAX_CW_SCENARIOS));
						ShowPlayerDialog(playerid, DIALOG_WO_NONE, DIALOG_STYLE_TABLIST_HEADERS, "{FFFFFF}Статистика мира", buf, "Закрыть", "");
					}
					if(listitem == 9)
					{
					    if(IsWorldAdmin(playerid))
					    {
						    GetPlayerPos(playerid, WorldData[GetPlayerVirtualWorld(playerid) - 10][spawnX], WorldData[GetPlayerVirtualWorld(playerid) - 10][spawnY], WorldData[GetPlayerVirtualWorld(playerid) - 10][spawnZ]);
						    GetPlayerFacingAngle(playerid, WorldData[GetPlayerVirtualWorld(playerid) - 10][spawnA]);
					    }
					}
				}
	        }
	    }
	    case DIALOG_WO_NAME:
	    {
	        format(WorldData[(GetPlayerVirtualWorld(playerid) - 10)][Name], 16, inputtext);
	        ShowWorldOptions(playerid);
	    }
	    case DIALOG_WO_DESC:
	    {
	        format(WorldData[(GetPlayerVirtualWorld(playerid) - 10)][Description], 128, inputtext);
	        ShowWorldOptions(playerid);
	    }
	    case DIALOG_WO_TIME:
	    {
	        if(strval(inputtext) > -1 && strval(inputtext) < 24) WorldData[(GetPlayerVirtualWorld(playerid) - 10)][Time] = strval(inputtext);
	        ShowWorldOptions(playerid);
	        new wdl = GetPlayerVirtualWorld(playerid);
	        foreach(new i: Player)
	        {
	            if(wdl == GetPlayerVirtualWorld(i)) SetPlayerTime(i, WorldData[wdl - 10][Time], 0);
	        }
	    }
		case DIALOG_WO_WEATHER:
		{
		    if(strval(inputtext) > -1 && strval(inputtext) < 256) WorldData[(GetPlayerVirtualWorld(playerid) - 10)][Weather] = strval(inputtext);
		    ShowWorldOptions(playerid);
		    new wdl = GetPlayerVirtualWorld(playerid);
	        foreach(new i: Player)
	        {
	            if(wdl == GetPlayerVirtualWorld(i)) SetPlayerWeather(i, WorldData[wdl - 10][Weather]);
	        }
		}
	}
	#if defined sandworlds_OnDialogResponse
    	return sandworlds_OnDialogResponse(playerid, dialogid, response, listitem, inputtext);
    #else
        return 1;
    #endif
}
#if defined _ALS_OnDialogResponse
	#undef OnDialogResponse
#else
	#define _ALS_OnDialogResponse
#endif
#define OnDialogResponse sandworlds_OnDialogResponse
#if defined sandworlds_OnDialogResponse
forward sandworlds_OnDialogResponse(playerid, dialogid, response, listitem, inputtext[]);
#endif

FindPlayerByGID(gid)
{
	foreach(new i: Player)
	{
	    if(PlayerData[i][pID] == gid) return i;
	}
	return -1;
}

ConnectPlayerToWorld(playerid, cworld)
{
	SPData[playerid][Moder] = -1;
    HidePagenatorForPlayer(playerid);
    ResetPlayerMoney(playerid);
    ResetPlayerWeapons(playerid);
	SetPlayerVirtualWorld(playerid, cworld + 10);
	SetPlayerTime(playerid, WorldData[cworld][Time], 0);
	SetPlayerWeather(playerid, WorldData[cworld][Weather]);
	SetSpawnInfo(playerid, NO_TEAM, 0, WorldData[cworld][spawnX], WorldData[cworld][spawnY], WorldData[cworld][spawnZ], WorldData[cworld][spawnA], 0, 0, 0, 0, 0, 0);
	return 1;
}

GetWorldOnline(worldid)
{
	new count;
	foreach(new i: Player)
	{
	    if(worldid == GetPlayerVirtualWorld(i)) count++;
	}
	return count;
}

GetWorldVehCount(worldid)
{
	new count;
	foreach(new i: Vehicle)
	{
	    if(worldid == GetVehicleVirtualWorld(i)) count++;
	}
	return count;
}

GetFreeWorld()
{
	for(new i; i != MAX_CWORLDS; i++)
	{
	    if(WorldData[i][OwnerID] == -1) return i;
	}
	return -1;
}

GetWorldByOwnerID(ownerid)
{
	for(new i; i != MAX_CWORLDS; i++)
	{
	    if(ownerid == WorldData[i][OwnerID]) return i;
	}
	return -1;
}

GetActiveWorlds()
{
	new count;
	for(new i; i != 10; i++)
	{
	    if(WorldData[i][OwnerID] != -1) count++;
	}
	return count;
}

ShowWorldOptions(playerid)
{
    new wd = (GetPlayerVirtualWorld(playerid) - 10);
	if(WorldData[wd][OwnerID] == PlayerData[playerid][pID])
	{
	    new buf[512];
	    format(buf, sizeof buf, "{FFFFFF}Опция\t{FFFFFF}Параметр");
	    format(buf, sizeof buf, "%s\nНазвание мира\t[ %s ]", buf, WorldData[wd][Name]);
	    format(buf, sizeof buf, "%s\nОписание мира\t[ %s ]", buf, WorldData[wd][Description]);
	    format(buf, sizeof buf, "%s\nПубличность\t[ %s ]", buf, WorldData[wd][Public] ? ("+") : ("-"));
	    format(buf, sizeof buf, "%s\nПриватность\t[ %s ]", buf, WorldData[wd][Private] ? ("+") : ("-"));
	    format(buf, sizeof buf, "%s\nАнти-Чит\t[ %s ]", buf, WorldData[wd][AC] ? ("+") : ("-"));
	    format(buf, sizeof buf, "%s\nВремя\t[ %d ]", buf, WorldData[wd][Time]);
	    format(buf, sizeof buf, "%s\nПогода\t[ %d ]", buf, WorldData[wd][Weather]);
		format(buf, sizeof buf, "%s\nОнлайн\t[ %d ]", buf, GetWorldOnline(wd + 10));
		format(buf, sizeof buf, "%s\nСтатистика\t[ Отображаемо ]", buf);
		format(buf, sizeof buf, "%s\nПозиция спавна\t[ Установить ]", buf);
	    ShowPlayerDialog(playerid, DIALOG_WOPTIONS, DIALOG_STYLE_TABLIST_HEADERS, "{FFFFFF}Управление миром", buf, "Выбрать", "Отмена");
	}
	return 1;
}

CMD:world(playerid)
{
	ShowPlayerDialog(playerid, DIALOG_WORLD, DIALOG_STYLE_LIST, "{FFFFFF}Взаимодействие с мирами", "Список миров\nСоздать мир\nВернуться в мир\nПокинуть данный мир", "Выбрать", "Отмена");
	return 1;
}

CMD:ew(playerid)
{
	if(IsWorldAdmin(playerid))
	{
		ShowWorldOptions(playerid);
	}
	return 1;
}

CMD:invite(playerid, params[])
{
	if(sscanf(params, "d", params[0])) return SCM(playerid, -1, "/invite [ID]");
	if(!IsPlayerConnected(params[0])) return 1;
	if(playerid == params[0]) return 1;
	if(IsWorldAdmin(playerid) || IsWorldModer(playerid))
	{
	    new buf[128];
	    format(buf, sizeof buf, "%s %s(%d) приглашает Вас в мир %s! (/accept - принять)", IsWorldModer(playerid) ? ("Модератор") : ("Администратор"), GetName(playerid), playerid, WorldData[GetPlayerVirtualWorld(playerid) - 10][Name]);
		SCM(params[0], -1, buf);
		format(buf, sizeof buf, "Вы пригласили в мир игрока %s(%d).", GetName(params[0]), params[0]);
		SCM(playerid, -1, buf);
		SPData[params[0]][InviteOffer] = (GetPlayerVirtualWorld(playerid) - 10);
		SPData[params[0]][InviteTime] = gettime();
	}
	return 1;
}

CMD:accept(playerid)
{
	if(SPData[playerid][InviteOffer] != -1 && (SPData[playerid][InviteTime] > (gettime() - 15)))
	{
		ConnectPlayerToWorld(playerid, SPData[playerid][InviteOffer]);
		SPData[playerid][InviteOffer] = -1;
		SPData[playerid][InviteTime] = 0;
	}
	else SCM(playerid, -1, "У Вас нет действующего приглашения в мир!");
	return 1;
}

CMD:wkick(playerid, params[])
{
	if(sscanf(params, "d", params[0])) return SCM(playerid, -1, "/wkick [ID]");
	if(IsWorldModer(playerid) || IsWorldAdmin(playerid))
	{
	    if(!IsPlayerConnected(params[0])) return 1;
	    if(GetPlayerVirtualWorld(playerid) != GetPlayerVirtualWorld(params[0])) return 1;
		new buf[128];
		format(buf, sizeof buf, "Вы кикнули игрока %s(%d) из мира!", GetName(params[0]), params[0]);
		SCM(playerid, -1, buf);
		SetPlayerVirtualWorld(params[0], 0);
		SCM(params[0], -1, "Вас кикнул модератор из мира!"); // !!!!
		HidePagenatorForPlayer(params[0]);
	}
	return 1;
}

CMD:setmoder(playerid, params[])
{
	if(IsWorldAdmin(playerid))
	{
		if(sscanf(params, "d", params[0])) return SCM(playerid, -1, "/setmoder [ID]");
		if(!IsPlayerConnected(params[0])) return 1;
		if(playerid == params[0]) return 1;
		new wd = GetPlayerVirtualWorld(playerid);
		if(wd == GetPlayerVirtualWorld(params[0]))
		{
		    new buf[128];
		    if(SPData[params[0]][Moder] != (wd - 10))
		    {
		        SPData[params[0]][Moder] = (wd - 10);
				format(buf, sizeof buf, "Вы выдали права модератора игроку %s(%d)!", GetName(params[0]), params[0]);
				SCM(playerid, -1, buf);
				SCM(params[0], -1, "Вам выдали права модератора мира!");
			}
			else if(SPData[params[0]][Moder] == (wd - 10))
		    {
		        SPData[params[0]][Moder] = -1;
				format(buf, sizeof buf, "Вы забрали права модератора у игрока %s(%d)!", GetName(params[0]), params[0]);
				SCM(playerid, -1, buf);
				SCM(params[0], -1, "Ваши права модератора мира были аннулированы!");
			}
		}
	}
	return 1;
}
