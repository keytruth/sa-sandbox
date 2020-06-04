// Author: keytruth

#include <a_samp>

////////////////////////////////_____ADDONS_____////////////////////////////////

#include "addons/jit"
#include "addons/crashdetect"
#include "addons/a_mysql"
#include "addons/sscanf2"
#include "addons/Pawn.RakNet"
#include "addons/streamer"
#include "addons/rakcheat"
#include "addons/whirlpool"
#include "addons/mxini"
#include "addons/Pawn.CMD"
#include "addons/foreach"
#include "addons/dini2"
//#include "addons/nex-ac"

///////////////////////////////_____DEFINES_____////////////////////////////////

#undef MAX_PLAYERS
#define MAX_PLAYERS			(256)

#define MYSQL_HOST        	"localhost"
#define MYSQL_USER        	"root"
#define MYSQL_PASS        	""
#define MYSQL_DATABASE    	"sandbox"

#define SCM SendClientMessage
#define SCMTA SendClientMessageToAll

#if !defined isnull
    #define isnull(%1) ((!(%1[0])) || (((%1[0]) == '\1') && (!(%1[1]))))
#endif

////////////////////////////////_____GVARS_____/////////////////////////////////

enum ePlayerData
{
	pID,

	pName[MAX_PLAYER_NAME + 1],
	pPassword[129],

	pAdmin,
	pPremium,

	pBanTime,
	pMuteTime,
	
	Cache: pPlayer_Cache,
	
	bool: pFinded,
	bool: pLogged
}
new PlayerData[MAX_PLAYERS][ePlayerData];
new clear_playerdata[ePlayerData];

enum eSPData
{
	Moder,
	InviteOffer,
	InviteTime,
	bool: NametagsDisable,
	SelActor,
	
	SelSce,
	SelSceLine
}
new SPData[MAX_PLAYERS][eSPData];
new clear_spdata[eSPData];

new MySQL: MySQL,
	Corrupt_Check[MAX_PLAYERS];
	
new PlayerText: RegTextDraw[MAX_PLAYERS][5],
	TimedPassword[MAX_PLAYERS][129];


/////////////////////////////////_____CORE_____/////////////////////////////////

#define MAX_WORLD_VEH (32)
#define MAX_WORLD_PREM_VEH (64)
#define MAX_CWORLD_OBJ (512)
#define MAX_CWORLD_PREM_OBJ (4096)
#define MAX_CW_PASSES (16)
#define MAX_CW_PREM_PASSES (32)
#define MAX_CWORLD_LABELS (32)
#define MAX_CWORLD_PREM_LABELS (64)
#define MAX_CWORLD_ACTORS (32)
#define MAX_CWORLD_PREM_ACTORS (64)
#define MAX_CW_SCENARIOS (64)
#define MAX_CW_PREM_SCENARIOS (150)

#include "core/anticheat"
#include "core/chats"
#include "core/worlds"
#include "core/passes"
#include "core/vehicles"
#include "core/pagenator"
#include "core/perplayer"
#include "core/labels"
#include "core/worldutil"
#include "core/actors"
#include "core/anims"
#include "core/scenarioareas"

////////////////////////////////////////////////////////////////////////////////

main()
{
    new a[][] =     {"Unarmed (Fist)","Brass K"};
	#pragma unused a
}

public OnGameModeInit()
{
	SetGameModeText("Sandbox");
	DisableInteriorEnterExits();
	EnableStuntBonusForAll(false);
	ShowNameTags(true);
	ShowPlayerMarkers(0);
	
	new MySQLOpt: option_id = mysql_init_options();
	mysql_set_option(option_id, AUTO_RECONNECT, true);
	MySQL = mysql_connect(MYSQL_HOST, MYSQL_USER, MYSQL_PASS, MYSQL_DATABASE, option_id);
	if(MySQL == MYSQL_INVALID_HANDLE || mysql_errno(MySQL) != 0)
	{
		print("Íå óäàëîñü ïîäêëþ÷èòüñÿ ê MySQL.");

		SendRconCommand("exit");
		return 1;
	}
	print("Óñïåøíî óñòàíîâëåíî ñîåäèíåíèå ñ MySQL.");
	mysql_tquery(MySQL, "CREATE TABLE IF NOT EXISTS `Accounts` (`ID` int(11) NOT NULL AUTO_INCREMENT,`Name` varchar(24) NOT NULL,`Password` varchar(129) NOT NULL, `Admin` int(11), `Premium` int(11), `BanTime` int(11),`MuteTime` int(11), PRIMARY KEY (`ID`), UNIQUE KEY `Name` (`Name`))");

	foreach(new i: Vehicle)
	{
	    VehLock[i] = -1;
	}
	return 1;
}

public OnGameModeExit()
{
    foreach(new i: Player)
    {
		if(IsPlayerConnected(i)) // ?
		{
			OnPlayerDisconnect(i, 1);
		}
	}
	
	KillTimer(EverySecondTimer);
	return 1;
}

public OnPlayerRequestClass(playerid, classid)
{
	SpawnPlayer(playerid);
	return 1;
}

public OnPlayerConnect(playerid)
{
	SetPlayerColor(playerid, -1);
	PlayerData[playerid] = clear_playerdata;
	SPData[playerid] = clear_spdata;
	SPData[playerid][Moder] = -1;
	SPData[playerid][InviteOffer] = -1;
	SPData[playerid][SelActor] = -1;
	SPData[playerid][SelSce] = -1;
	SPData[playerid][SelSceLine] = -1;

	format(TimedPassword[playerid], 129, "");
	
	RegTextDraw[playerid][0] = CreatePlayerTextDraw(playerid, 318.000000, 174.000000, "_");
	PlayerTextDrawAlignment(playerid, RegTextDraw[playerid][0], 2);
	PlayerTextDrawBackgroundColor(playerid, RegTextDraw[playerid][0], 255);
	PlayerTextDrawFont(playerid, RegTextDraw[playerid][0], 1);
	PlayerTextDrawLetterSize(playerid, RegTextDraw[playerid][0], 0.500000, 8.199995);
	PlayerTextDrawColor(playerid, RegTextDraw[playerid][0], -1);
	PlayerTextDrawSetOutline(playerid, RegTextDraw[playerid][0], 0);
	PlayerTextDrawSetProportional(playerid, RegTextDraw[playerid][0], 1);
	PlayerTextDrawSetShadow(playerid, RegTextDraw[playerid][0], 0);
	PlayerTextDrawUseBox(playerid, RegTextDraw[playerid][0], 1);
	PlayerTextDrawBoxColor(playerid, RegTextDraw[playerid][0], 877231871);
	PlayerTextDrawTextSize(playerid, RegTextDraw[playerid][0], 300.000000, 99.000000);
	PlayerTextDrawSetSelectable(playerid, RegTextDraw[playerid][0], 0);

	RegTextDraw[playerid][1] = CreatePlayerTextDraw(playerid, 318.000000, 177.000000, "SA: Sandbox");
	PlayerTextDrawAlignment(playerid, RegTextDraw[playerid][1], 2);
	PlayerTextDrawBackgroundColor(playerid, RegTextDraw[playerid][1], 255);
	PlayerTextDrawFont(playerid, RegTextDraw[playerid][1], 1);
	PlayerTextDrawLetterSize(playerid, RegTextDraw[playerid][1], 0.319999, 1.399999);
	PlayerTextDrawColor(playerid, RegTextDraw[playerid][1], -1);
	PlayerTextDrawSetOutline(playerid, RegTextDraw[playerid][1], 0);
	PlayerTextDrawSetProportional(playerid, RegTextDraw[playerid][1], 1);
	PlayerTextDrawSetShadow(playerid, RegTextDraw[playerid][1], 0);
	PlayerTextDrawSetSelectable(playerid, RegTextDraw[playerid][1], 0);

	RegTextDraw[playerid][2] = CreatePlayerTextDraw(playerid, 318.000000, 198.000000, GetName(playerid));
	PlayerTextDrawAlignment(playerid, RegTextDraw[playerid][2], 2);
	PlayerTextDrawBackgroundColor(playerid, RegTextDraw[playerid][2], 255);
	PlayerTextDrawFont(playerid, RegTextDraw[playerid][2], 1);
	PlayerTextDrawLetterSize(playerid, RegTextDraw[playerid][2], 0.290000, 0.799999);
	PlayerTextDrawColor(playerid, RegTextDraw[playerid][2], -1);
	PlayerTextDrawSetOutline(playerid, RegTextDraw[playerid][2], 0);
	PlayerTextDrawSetProportional(playerid, RegTextDraw[playerid][2], 1);
	PlayerTextDrawSetShadow(playerid, RegTextDraw[playerid][2], 0);
	PlayerTextDrawUseBox(playerid, RegTextDraw[playerid][2], 1);
	PlayerTextDrawBoxColor(playerid, RegTextDraw[playerid][2], 877226495);
	PlayerTextDrawTextSize(playerid, RegTextDraw[playerid][2], 0.000000, 71.000000);
	PlayerTextDrawSetSelectable(playerid, RegTextDraw[playerid][2], 0);

	RegTextDraw[playerid][3] = CreatePlayerTextDraw(playerid, 318.000000, 211.000000, "Password");
	PlayerTextDrawAlignment(playerid, RegTextDraw[playerid][3], 2);
	PlayerTextDrawBackgroundColor(playerid, RegTextDraw[playerid][3], 255);
	PlayerTextDrawFont(playerid, RegTextDraw[playerid][3], 1);
	PlayerTextDrawLetterSize(playerid, RegTextDraw[playerid][3], 0.290000, 0.799999);
	PlayerTextDrawColor(playerid, RegTextDraw[playerid][3], -1);
	PlayerTextDrawSetOutline(playerid, RegTextDraw[playerid][3], 0);
	PlayerTextDrawSetProportional(playerid, RegTextDraw[playerid][3], 1);
	PlayerTextDrawSetShadow(playerid, RegTextDraw[playerid][3], 0);
	PlayerTextDrawUseBox(playerid, RegTextDraw[playerid][3], 1);
	PlayerTextDrawBoxColor(playerid, RegTextDraw[playerid][3], 877226495);
	PlayerTextDrawTextSize(playerid, RegTextDraw[playerid][3], 10.000000, 71.000000);
	PlayerTextDrawSetSelectable(playerid, RegTextDraw[playerid][3], 1);

	RegTextDraw[playerid][4] = CreatePlayerTextDraw(playerid, 318.000000, 229.000000, "Auth");
	PlayerTextDrawAlignment(playerid, RegTextDraw[playerid][4], 2);
	PlayerTextDrawBackgroundColor(playerid, RegTextDraw[playerid][4], 255);
	PlayerTextDrawFont(playerid, RegTextDraw[playerid][4], 1);
	PlayerTextDrawLetterSize(playerid, RegTextDraw[playerid][4], 0.370000, 1.099999);
	PlayerTextDrawColor(playerid, RegTextDraw[playerid][4], -1);
	PlayerTextDrawSetOutline(playerid, RegTextDraw[playerid][4], 0);
	PlayerTextDrawSetProportional(playerid, RegTextDraw[playerid][4], 1);
	PlayerTextDrawSetShadow(playerid, RegTextDraw[playerid][4], 0);
	PlayerTextDrawUseBox(playerid, RegTextDraw[playerid][4], 1);
	PlayerTextDrawBoxColor(playerid, RegTextDraw[playerid][4], 877226495);
	PlayerTextDrawTextSize(playerid, RegTextDraw[playerid][4], 15.000000, 71.000000);
	PlayerTextDrawSetSelectable(playerid, RegTextDraw[playerid][4], 1);
	return 1;
}

public OnPlayerFinishedDownloading(playerid, virtualworld)
{
	if(!PlayerData[playerid][pLogged])
	{
        new DB_Query[115];
		GetPlayerName(playerid, PlayerData[playerid][pName], MAX_PLAYER_NAME + 1);
		Corrupt_Check[playerid]++;

		mysql_format(MySQL, DB_Query, sizeof(DB_Query), "SELECT * FROM `Accounts` WHERE `Name` = '%e' LIMIT 1", PlayerData[playerid][pName]);
		mysql_tquery(MySQL, DB_Query, "OnPlayerDataCheck", "ii", playerid, Corrupt_Check[playerid]);
		TogglePlayerSpectating(playerid, true);
		SelectTextDraw(playerid, 0x767777FF);
		
		for(new i; i != 100; i++)
		{
		    SCM(playerid, -1, "");
		}
    }
    return 1;
}

public OnPlayerDisconnect(playerid, reason)
{
    Corrupt_Check[playerid]++;
	new DB_Query[256];
	mysql_format(MySQL, DB_Query, sizeof(DB_Query), "UPDATE `Accounts` SET `Admin` = %d, `Premium` = %d, `BanTime` = %d, `MuteTime` = %d WHERE `ID` = %d LIMIT 1",
	PlayerData[playerid][pAdmin], PlayerData[playerid][pPremium], PlayerData[playerid][pBanTime], PlayerData[playerid][pMuteTime], PlayerData[playerid][pID]);
	mysql_tquery(MySQL, DB_Query);
	if(cache_is_valid(PlayerData[playerid][pPlayer_Cache]))
	{
		cache_delete(PlayerData[playerid][pPlayer_Cache]);
		PlayerData[playerid][pPlayer_Cache] = MYSQL_INVALID_CACHE;
	}

	PlayerData[playerid][pLogged] = false;
	return 1;
}

public OnPlayerSpawn(playerid)
{
	new plwd = GetPlayerVirtualWorld(playerid);
	if(plwd >= 10)
	{
	    SetPlayerPos(playerid, WorldData[plwd - 10][spawnX], WorldData[plwd - 10][spawnY], WorldData[plwd - 10][spawnZ]);
	    SetPlayerFacingAngle(playerid, WorldData[plwd - 10][spawnA]);
	}
	return 1;
}

public OnPlayerDeath(playerid, killerid, reason)
{
	return 1;
}

public OnVehicleSpawn(vehicleid)
{
	return 1;
}

public OnVehicleDeath(vehicleid, killerid)
{
	return 1;
}

public OnPlayerText(playerid, text[])
{
	return 1;
}

public OnPlayerCommandText(playerid, cmdtext[])
{
	return 0;
}

public OnPlayerEnterVehicle(playerid, vehicleid, ispassenger)
{
	return 1;
}

public OnPlayerExitVehicle(playerid, vehicleid)
{
	return 1;
}

public OnPlayerStateChange(playerid, newstate, oldstate)
{
	return 1;
}

public OnPlayerEnterCheckpoint(playerid)
{
	return 1;
}

public OnPlayerLeaveCheckpoint(playerid)
{
	return 1;
}

public OnPlayerEnterRaceCheckpoint(playerid)
{
	return 1;
}

public OnPlayerLeaveRaceCheckpoint(playerid)
{
	return 1;
}

public OnRconCommand(cmd[])
{
	return 1;
}

public OnPlayerRequestSpawn(playerid)
{
    if(!PlayerData[playerid][pLogged]) return 0;
	return 1;
}

public OnObjectMoved(objectid)
{
	return 1;
}

public OnPlayerObjectMoved(playerid, objectid)
{
	return 1;
}

public OnPlayerPickUpPickup(playerid, pickupid)
{
	return 1;
}

public OnVehicleMod(playerid, vehicleid, componentid)
{
	return 1;
}

public OnVehiclePaintjob(playerid, vehicleid, paintjobid)
{
	return 1;
}

public OnVehicleRespray(playerid, vehicleid, color1, color2)
{
	return 1;
}

public OnPlayerSelectedMenuRow(playerid, row)
{
	return 1;
}

public OnPlayerExitedMenu(playerid)
{
	return 1;
}

public OnPlayerInteriorChange(playerid, newinteriorid, oldinteriorid)
{
	return 1;
}

public OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
	return 1;
}

public OnRconLoginAttempt(ip[], password[], success)
{
	return 1;
}

public OnPlayerUpdate(playerid)
{
	return 1;
}

public OnPlayerStreamIn(playerid, forplayerid)
{
	if(SPData[forplayerid][NametagsDisable] == true) ShowPlayerNameTagForPlayer(forplayerid, playerid, false);
	return 1;
}

public OnPlayerStreamOut(playerid, forplayerid)
{
	return 1;
}

public OnVehicleStreamIn(vehicleid, forplayerid)
{
	return 1;
}

public OnVehicleStreamOut(vehicleid, forplayerid)
{
	return 1;
}

public OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
	switch(dialogid)
	{
		case 0:
		{
		    if(response)
			{
				format(TimedPassword[playerid], 129, inputtext);
			}
		}
	}
	return 1;
}

public OnPlayerClickPlayer(playerid, clickedplayerid, source)
{
	return 1;
}

public OnPlayerClickTextDraw(playerid, Text:clickedid)
{
    if(!PlayerData[playerid][pLogged] && clickedid == Text: INVALID_TEXT_DRAW) SelectTextDraw(playerid, 0x767777FF);
    if(PagenatorShowed[playerid] && clickedid == Text: INVALID_TEXT_DRAW) HidePagenatorForPlayer(playerid);
    return 1;
}

public OnPlayerGiveDamage(playerid, damagedid, Float:amount, weaponid, bodypart)
{
	new buf[128];
	format(buf, sizeof buf, "GiveDamage: playerid - %d | damagedid - %d | amount - %f | weaponid - %d | bodypart - %d", playerid, damagedid, amount, weaponid, bodypart);
	SCMTA(-1, buf);
	return 1;
}

public OnPlayerTakeDamage(playerid, issuerid, Float:amount, weaponid, bodypart)
{
    new buf[128];
	format(buf, sizeof buf, "GiveDamage: playerid - %d | issuerid - %d | amount - %f | weaponid - %d | bodypart - %d", playerid, issuerid, amount, weaponid, bodypart);
	SCMTA(-1, buf);
    return 1;
}

public OnPlayerClickPlayerTextDraw(playerid, PlayerText:playertextid)
{
	if(playertextid == RegTextDraw[playerid][3])
	{
	    ShowPlayerDialog(playerid, 0, DIALOG_STYLE_INPUT, "{FFFFFF}Ïàðîëü", "{FFFFFF}Ââåäèòå ïàðîëü:", "Ãîòîâî", "Îòìåíà");
	}
    if(playertextid == RegTextDraw[playerid][4])
    {
         if(PlayerData[playerid][pFinded]) // LOG
         {
            WP_Hash(TimedPassword[playerid], 129, TimedPassword[playerid]);
            
			if(strcmp(TimedPassword[playerid], PlayerData[playerid][pPassword]) == 0)
			{
				cache_set_active(PlayerData[playerid][pPlayer_Cache]);

            	cache_get_value_int(0, "ID", PlayerData[playerid][pID]);
        		cache_get_value_int(0, "Admin", PlayerData[playerid][pAdmin]);
        		cache_get_value_int(0, "Premium", PlayerData[playerid][pPremium]);
        		cache_get_value_int(0, "BanTime", PlayerData[playerid][pBanTime]);
        		cache_get_value_int(0, "MuteTime", PlayerData[playerid][pMuteTime]);

				cache_delete(PlayerData[playerid][pPlayer_Cache]);
				PlayerData[playerid][pPlayer_Cache] = MYSQL_INVALID_CACHE;
				PlayerData[playerid][pLogged] = true;
				CallLocalFunction("OnPlayerLogged", "d", playerid);
			}
			else SCM(playerid, -1, "Íåâåðíûé ïàðîëü!");
         }
         else // REG
         {
            if(strlen(TimedPassword[playerid]) < 2 || strlen(TimedPassword[playerid]) > 16) SCM(playerid, -1, "Äëèíà ïàðîëÿ äîëæíà áûòü: îò 2 äî 16 ñèìâîëîâ!");
			else
			{
		    	WP_Hash(PlayerData[playerid][pPassword], 129, TimedPassword[playerid]);

		    	new DB_Query[512];
		    	mysql_format(MySQL, DB_Query, sizeof(DB_Query), "INSERT INTO `Accounts` (`Name`, `Password`, `Admin`, `Premium`, `BanTime`, `MuteTime`)\
		    	VALUES ('%e', '%s', '0', '0', '0', '0')", PlayerData[playerid][pName], PlayerData[playerid][pPassword]);
		     	mysql_tquery(MySQL, DB_Query, "OnPlayerRegister", "d", playerid);
		     }
         }
    }
    
    // Pagenator
    
    if(playertextid == PagenatorHUD[playerid][2] && PagenatorPage[playerid] > 0) PagenatorPage[playerid]--, UpdatePagenator(playerid);
	if(playertextid == PagenatorHUD[playerid][3] && PagenatorPage[playerid] < MaximumPagesType[PagenatorType[playerid]]) PagenatorPage[playerid]++, UpdatePagenator(playerid);
	
	if(playertextid == PagenatorCell[playerid][0]) CallPagenatorFunc(playerid, 0);
    if(playertextid == PagenatorCell[playerid][1]) CallPagenatorFunc(playerid, 1);
    if(playertextid == PagenatorCell[playerid][2]) CallPagenatorFunc(playerid, 2);
    if(playertextid == PagenatorCell[playerid][3]) CallPagenatorFunc(playerid, 3);
    if(playertextid == PagenatorCell[playerid][4]) CallPagenatorFunc(playerid, 4);
    if(playertextid == PagenatorCell[playerid][5]) CallPagenatorFunc(playerid, 5);
    if(playertextid == PagenatorCell[playerid][6]) CallPagenatorFunc(playerid, 6);
    if(playertextid == PagenatorCell[playerid][7]) CallPagenatorFunc(playerid, 7);
    if(playertextid == PagenatorCell[playerid][8]) CallPagenatorFunc(playerid, 8);
    if(playertextid == PagenatorCell[playerid][9]) CallPagenatorFunc(playerid, 9);
    if(playertextid == PagenatorCell[playerid][10]) CallPagenatorFunc(playerid, 10);
    if(playertextid == PagenatorCell[playerid][11]) CallPagenatorFunc(playerid, 11);
    if(playertextid == PagenatorCell[playerid][12]) CallPagenatorFunc(playerid, 12);
    if(playertextid == PagenatorCell[playerid][13]) CallPagenatorFunc(playerid, 13);
    if(playertextid == PagenatorCell[playerid][14]) CallPagenatorFunc(playerid, 14);
    if(playertextid == PagenatorCell[playerid][15]) CallPagenatorFunc(playerid, 15);
    if(playertextid == PagenatorCell[playerid][16]) CallPagenatorFunc(playerid, 16);
    if(playertextid == PagenatorCell[playerid][17]) CallPagenatorFunc(playerid, 17);
    return 1;
}

public OnPlayerCheat(playerid, const code)
{
	if(code == 7) return 1;
	new buf[128];
	if(GetPlayerVirtualWorld(playerid) < 10)
	{
	    format(buf, sizeof buf, "Àíòè-×èò îòñîåäèíèë îò ñåðâåðà èãðîêà %s(%d). (Êîä: ¹%d)", GetName(playerid), playerid, code);
	    SCMTA(-1, buf);
		SetTimerEx("OnTimerKickFix", 1000, false, "d", playerid);
	}
	else
	{
	    if(WorldData[GetPlayerVirtualWorld(playerid) - 10][AC])
	    {
	        if(!IsWorldAdmin(playerid))
	        {
		        format(buf, sizeof buf, "Àíòè-×èò èñêëþ÷èë èç ìèðà èãðîêà %s(%d). (Êîä: ¹%d)", GetName(playerid), playerid, code);
		        SendMesWorld(GetPlayerVirtualWorld(playerid), -1, buf);
		        SetTimerEx("OnTimerLeaveWorFix", 1000, false, "d", playerid);
	        }
	    }
	}
	return 1;
}

forward OnTimerKickFix(playerid);
public OnTimerKickFix(playerid)
{
	Kick(playerid);
	return 1;
}

forward OnTimerLeaveWorFix(playerid);
public OnTimerLeaveWorFix(playerid)
{
	SetPlayerVirtualWorld(playerid, 0);
	SPData[playerid][Moder] = -1;
    HidePagenatorForPlayer(playerid);
    ResetPlayerMoney(playerid);
    ResetPlayerWeapons(playerid);
	return 1;
}

SendMesWorld(worldid, colorid, const message[])
{
	foreach(new i: Player)
	{
	    if(worldid == GetPlayerVirtualWorld(i)) SCM(i, colorid, message);
	}
	return 1;
}

IsPlayerPremium(playerid)
{
	if(PlayerData[playerid][pPremium] >= gettime()) return 1;
	return 0;
}

GetCWorldOwnerPlayer(cworld)
{
	new ownerid = WorldData[cworld][OwnerID];
	foreach(new i: Player)
	{
	    if(ownerid == PlayerData[i][pID]) return i;
	}
	return -1;
}

/*
forward OnCheatDetected(playerid, ip_address[], type, code);
public OnCheatDetected(playerid, ip_address[], type, code)
{
	if(GetPlayerVirtualWorld(playerid) < 10 || WorldData[GetPlayerVirtualWorld(playerid) - 10][AC])
	{
	    if(type) BlockIpAddress(ip_address, 0);
	    else
	    {
	        switch(code)
	        {
	            case 5, 6, 11, 22: return 1;
	            case 14:
	            {
	                new a = GetPlayerMoney(playerid);
	                ResetPlayerMoney(playerid);
	                GivePlayerMoney(playerid, a);
	                return 1;
	            }
	            case 32:
	            {
	                new Float:x, Float:y, Float:z;
	                AntiCheatGetPos(playerid, x, y, z);
	                SetPlayerPos(playerid, x, y, z);
	                return 1;
	            }
	            case 40: SendClientMessage(playerid, -1, MAX_CONNECTS_MSG);
	            case 41: SendClientMessage(playerid, -1, UNKNOWN_CLIENT_MSG);
	            default:
	            {
	                new strtmp[sizeof KICK_MSG];
	                format(strtmp, sizeof strtmp, KICK_MSG, code);
	                SendClientMessage(playerid, -1, strtmp);
	            }
	        }
	        AntiCheatKickWithDesync(playerid, code);
	    }
    }
    return 1;
}*/

forward OnWorldTimer();
public OnWorldTimer()
{
	for(new i; i != MAX_CWORLDS; i++)
	{
	    if(WorldData[i][OwnerID] != -1)
	    {
			if(FindPlayerByGID(WorldData[i][OwnerID]) != -1 && GetPlayerVirtualWorld(FindPlayerByGID(WorldData[i][OwnerID])) == (i + 10))
			{
			    WorldData[i][LastTimeOwner] = gettime();
			}
			else
			{
			    if(!IsWorldOwnerOnline(i))
			    {
			        WorldData[i] = clear_worlddata;
			        WorldData[i][OwnerID] = -1;
			        ClearWorldPasses(i);
			        ClearWorldObjects(i + 10);
			        ClearWorldVehicles(i + 10);
			        ClearWorldLabels(i);
					ClearWorldActors(i);
					ClearWorldScenarios(i);
			        foreach(new j: Player)
			        {
			            if((i + 10) == GetPlayerVirtualWorld(j))
			            {
			                SetPlayerVirtualWorld(j, 0);
			                HidePagenatorForPlayer(j);
			                SCM(j, -1, "Äàííûé ìèð áûë óäàëåí: âëàäåëåö äîëãî îòñóòñòâîâàë â íåì.");
			            }
			        }
			    }
			}
		}
	}
	return 1;
}

forward OnPlayerCommandReceived(playerid, cmd[], params[], flags);
public OnPlayerCommandReceived(playerid, cmd[], params[], flags)
{
	if(!PlayerData[playerid][pLogged]) return 0;
	return 1;
}

forward OnPlayerDataCheck(playerid, corrupt_check);
public OnPlayerDataCheck(playerid, corrupt_check)
{
	if (corrupt_check != Corrupt_Check[playerid]) return Kick(playerid);

	for(new i; i != 5; i++)
	{
	    PlayerTextDrawShow(playerid, RegTextDraw[playerid][i]);
	}
	if(cache_num_rows() > 0)
	{
		cache_get_value(0, "Password", PlayerData[playerid][pPassword], 129);
		PlayerData[playerid][pPlayer_Cache] = cache_save();
		PlayerData[playerid][pFinded] = true;
	}
	else
	{
		PlayerData[playerid][pFinded] = false;
		PlayerTextDrawSetString(playerid, RegTextDraw[playerid][4], "Reg");
	}
	return 1;
}

forward OnPlayerRegister(playerid);
public OnPlayerRegister(playerid)
{
    PlayerData[playerid][pLogged] = true;
    for(new i; i != 5; i++)
    {
        PlayerTextDrawHide(playerid, RegTextDraw[playerid][i]);
    }
    
    TogglePlayerSpectating(playerid, false);
    SetNoneSpawn(playerid);
    SpawnPlayer(playerid);
    CancelSelectTextDraw(playerid);
    return 1;
}

forward OnPlayerLogged(playerid);
public OnPlayerLogged(playerid)
{
    PlayerData[playerid][pLogged] = true;
    for(new i; i != 5; i++)
    {
        PlayerTextDrawHide(playerid, RegTextDraw[playerid][i]);
    }
    
    TogglePlayerSpectating(playerid, false);
    SetNoneSpawn(playerid);
    SpawnPlayer(playerid);
    CancelSelectTextDraw(playerid);
    return 1;
}

CMD:lc(playerid)
{
	return SetPlayerPos(playerid, -5230.2314,-328.9046,16.1266);
}

CMD:vc(playerid)
{
	return SetPlayerPos(playerid, 5020.9219,830.9516,5.1817);
}

CMD:sa(playerid)
{
	return SetPlayerPos(playerid, 0.0, 0.0, 5.0);
}

GetName(playerid)
{
	new name[MAX_PLAYER_NAME + 1];
	GetPlayerName(playerid, name, sizeof name);
	return name;
}

CMD:nameoff(playerid)
{
    foreach(new i: Player)
    {
        ShowPlayerNameTagForPlayer(playerid, i, false);
    }
	SPData[playerid][NametagsDisable] = true;
	return 1;
}

CMD:nameon(playerid)
{
    foreach(new i: Player)
    {
        ShowPlayerNameTagForPlayer(playerid, i, true);
    }
	SPData[playerid][NametagsDisable] = false;
	return 1;
}

GivePlayerVehicle(playerid, vehicleid, color1, color2)
{
	new Float: X, Float: Y, Float: Z, Float: A;
	GetPlayerPos(playerid, X, Y, Z);
	GetPlayerFacingAngle(playerid, A);
	new vehid = CreateVehicle(vehicleid, X, Y, Z, A, color1, color2, -1);
	SetVehicleVirtualWorld(vehid, GetPlayerVirtualWorld(playerid));
	LinkVehicleToInterior(vehid, GetPlayerInterior(playerid));
	PutPlayerInVehicle(playerid, vehid, 0);
	VehLock[vehid] = PlayerData[playerid][pID];
	return 1;
}

SetNoneSpawn(playerid)
{
	SetSpawnInfo(playerid, NO_TEAM, random(311), float(randomEx(2245, 2390)), float(randomEx(557, 596)), 7.7802, 0.0, 0, 0, 0, 0, 0, 0);
	return 1;
}

randomEx(min, max){ return (random(max - min) + min); }

// ----------------------OBJECT_SYSTEM-------------------------------------

IsValidObjectModel(modelid)
{
	if(modelid >= 321 && modelid <= 328 || modelid >= 330 && modelid <= 331)
		return 1;
	else if(modelid >= 333 && modelid <= 339 || modelid >= 341 && modelid <= 373)
		return 1;

	else if(modelid >= 615 && modelid <= 661 || modelid == 664)
		return 1;
	else if(modelid >= 669 && modelid <= 698 || modelid >= 700 && modelid <= 792)
		return 1;
	else if(modelid >= 800 && modelid <= 906 || modelid >= 910 && modelid <= 964)
		return 1;
	else if(modelid >= 966 && modelid <= 998 || modelid >= 1000 && modelid <= 1193)
		return 1;
	else if(modelid >= 1207 && modelid <= 1325 || modelid >= 1327 && modelid <= 1572)
		return 1;
	else if(modelid >= 1574 && modelid <= 1698 || modelid >= 1700 && modelid <= 2882)
		return 1;
	else if(modelid >= 2885 && modelid <= 3135 || modelid >= 3167 && modelid <= 3175)
		return 1;
	else if(modelid == 3178 || modelid == 3187 || modelid == 3193 || modelid == 3214)
		return 1;
	else if(modelid == 3221 || modelid >= 3241 && modelid <= 3244)
		return 1;
	else if(modelid == 3246 || modelid >= 3249 && modelid <= 3250)
		return 1;
	else if(modelid >= 3252 && modelid <= 3253 || modelid >= 3255 && modelid <= 3265)
		return 1;
	else if(modelid >= 3267 && modelid <= 3347 || modelid >= 3350 && modelid <= 3415)
		return 1;
	else if(modelid >= 3417 && modelid <= 3428 || modelid >= 3430 && modelid <= 3609)
		return 1;
	else if(modelid >= 3612 && modelid <= 3783 || modelid >= 3785 && modelid <= 3869)
		return 1;
	else if(modelid >= 3872 && modelid <= 3882 || modelid >= 3884 && modelid <= 3888)
		return 1;
	else if(modelid >= 3890 && modelid <= 3973 || modelid >= 3975 && modelid <= 4541)
		return 1;
	else if(modelid >= 4550 && modelid <= 4762 || modelid >= 4806 && modelid <= 5084)
		return 1;
	else if(modelid >= 5086 && modelid <= 5089 || modelid >= 5105 && modelid <= 5375)
		return 1;
	else if(modelid >= 5390 && modelid <= 5682 || modelid >= 5703 && modelid <= 6010)
		return 1;
	else if(modelid >= 6035 && modelid <= 6253 || modelid >= 6255 && modelid <= 6257)
		return 1;
	else if(modelid >= 6280 && modelid <= 6347 || modelid >= 6349 && modelid <= 6525)
		return 1;
	else if(modelid >= 6863 && modelid <= 7392 || modelid >= 7415 && modelid <= 7973)
		return 1;
	else if(modelid >= 7978 && modelid <= 9193 || modelid >= 9205 && modelid <= 9267)
		return 1;
	else if(modelid >= 9269 && modelid <= 9478 || modelid >= 9482 && modelid <= 10310)
		return 1;
	else if(modelid >= 10315 && modelid <= 10744 || modelid >= 10750 && modelid <= 11417)
		return 1;
	else if(modelid >= 11420 && modelid <= 11753 || modelid >= 12800 && modelid <= 13563)
		return 1;
	else if(modelid >= 13590 && modelid <= 13667 || modelid >= 13672 && modelid <= 13890)
		return 1;
	else if(modelid >= 14383 && modelid <= 14528 || modelid >= 14530 && modelid <= 14554)
		return 1;
	else if(modelid == 14556 || modelid >= 14558 && modelid <= 14643)
		return 1;
	else if(modelid >= 14650 && modelid <= 14657 || modelid >= 14660 && modelid <= 14695)
		return 1;
	else if(modelid >= 14699 && modelid <= 14728 || modelid >= 14735 && modelid <= 14765)
		return 1;
	else if(modelid >= 14770 && modelid <= 14856 || modelid >= 14858 && modelid <= 14883)
		return 1;
	else if(modelid >= 14885 && modelid <= 14898 || modelid >= 14900 && modelid <= 14903)
		return 1;
	else if(modelid >= 15025 && modelid <= 15064 || modelid >= 16000 && modelid <= 16790)
		return 1;
	else if(modelid >= 17000 && modelid <= 17474 || modelid >= 17500 && modelid <= 17974)
		return 1;
	else if(modelid == 17976 || modelid == 17978 || modelid >= 18000 && modelid <= 18036)
		return 1;
	else if(modelid >= 18038 && modelid <= 18102 || modelid >= 18104 && modelid <= 18105)
		return 1;
	else if(modelid == 18109 || modelid == 18112 || modelid >= 18200 && modelid <= 18859)
		return 1;
	else if(modelid >= 18860 && modelid <= 19274 || modelid >= 19275 && modelid <= 19595)
		return 1;
	else if(modelid >= 19596 && modelid <= 19999)
		return 1;
	else
		return 0;
}

GetDynamicObjectVirtualWorld(objectid)
{
	if(IsValidDynamicObject(objectid)) return Streamer_GetIntData(STREAMER_TYPE_OBJECT, objectid, E_STREAMER_WORLD_ID);
	return -345;
}

GetDynamicObjectModel(objectid)
{
	if(IsValidDynamicObject(objectid)) return Streamer_GetIntData(STREAMER_TYPE_OBJECT, objectid, E_STREAMER_MODEL_ID);
	return -345;
}

GetWorldCountObj(cworld)
{
	new count;
	for(new i; i != 100000; i++)
	{
	    if((cworld + 10) == GetDynamicObjectVirtualWorld(i)) count++;
	}
	return count;
}

ClearWorldObjects(worldid)
{
	for(new i; i != 100000; i++)
	{
	    if(IsValidDynamicObject(i))
	    {
	        if(worldid == GetDynamicObjectVirtualWorld(i)) DestroyDynamicObject(i);
	    }
	}
	return 1;
}

CMD:obj(playerid, params[])
{
	if(sscanf(params, "d", params[0])) return SCM(playerid, -1, "/obj [Object Model]");
	if(IsWorldAdmin(playerid))
	{
	    if(GetWorldCountObj(GetPlayerVirtualWorld(playerid) - 10) < ((IsPlayerPremium(GetCWorldOwnerPlayer(GetPlayerVirtualWorld(playerid) - 10))) ? (MAX_CWORLD_PREM_OBJ) : (MAX_CWORLD_OBJ)))
	    {
			if(!IsValidObjectModel(params[0])) return SCM(playerid, -1, "Äàííîé ìîäåëè íå ñóùåñòâóåò èëè îíà íåäîñòóïíà!");
			new Float: X, Float: Y, Float: Z;
			GetPlayerPos(playerid, X, Y, Z);
            new objid = CreateDynamicObject(params[0], X, Y, Z, 0.0, 0.0, 0.0, GetPlayerVirtualWorld(playerid));
            EditDynamicObject(playerid, objid);
            new buf[128];
            format(buf, sizeof buf, "Ñîçäàí îáúåêò ¹%d!", objid);
            SCM(playerid, -1, buf);
		}
	    else SCM(playerid, -1, "Ïðåâûøåí ëèìèò îáúåêòîâ!");
	}
	return 1;
}

CMD:editobj(playerid, params[])
{
    if(IsWorldAdmin(playerid))
	{
		if(sscanf(params, "d", params[0])) return SCM(playerid, -1, "/editobj [Object ID]");
	    if(IsValidDynamicObject(params[0]) && (GetDynamicObjectVirtualWorld(params[0]) == GetPlayerVirtualWorld(playerid)))
	    {
            EditDynamicObject(playerid, params[0]);
		}
	}
	return 1;
}

CMD:delobj(playerid, params[])
{
    if(sscanf(params, "d", params[0])) return SCM(playerid, -1, "/delobj [Object ID]");
	if(IsWorldAdmin(playerid))
	{
	    if(IsValidDynamicObject(params[0]) && (GetDynamicObjectVirtualWorld(params[0]) == GetPlayerVirtualWorld(playerid)))
	    {
            DestroyDynamicObject(params[0]);
            new buf[128];
            format(buf, sizeof buf, "Óäàëåí îáúåêò ¹%d!", params[0]);
            SCM(playerid, -1, buf);
		}
	}
	return 1;
}

// ----------------------OBJECT_SYSTEM-------------------------------------

/*SaveObjects(playerid, slotid)
{
	new buf[64], nametmp[8], saving_data[128], wd = GetPlayerVirtualWorld(playerid), pds, modelid, Float: X, Float: Y, Float: Z, Float: rotX, Float: rotY, Float: rotZ;
	format(buf, sizeof buf, "worlds/objects/%d_%d.ini", PlayerData[playerid][pID], slotid);
	if(!dini_Exists(buf))
	{
	    dini_Create(buf);
	    for(new i; i != 100000; i++)
	    {
	        if(wd == GetDynamicObjectVirtualWorld(i))
	        {
	            pds++;
	            modelid = GetDynamicObjectModel(i);
				GetDynamicObjectPos(i, X, Y, Z);
				GetDynamicObjectRotation(i, rotX, rotY, rotZ);
	        	format(nametmp, sizeof nametmp, "%d", pds);
	        	format(saving_data, sizeof saving_data, "%d %f %f %f %f %f %f", modelid, X, Y, Z, rotX, rotY, rotZ);
	        	dini_Set(buf, nametmp, saving_data);
	        }
	    }
	}
	else
	{
	
	}
	return 1;
}

LoadObjects(playerid, slotid)
{
	return 1;
}*/
