#define IsFuncType(%0) !strcmp(func, %0, true)
#define DIALOG_SCENARIO (353)
#define DIALOG_SCENARIO_LINE (354)
#define DIALOG_SCENARIO_TYPE (355)
#define DIALOG_SCENARIO_EDITRANGE (356)
#define DIALOG_SCENARIO_CONDITION (357)
#define DIALOG_SCENARIO_CONDTYPE (358)
#define DIALOG_SCENARIO_CONDITION1 (359)
#define DIALOG_SCENARIO_CONDITION2 (360)

#define MAX_CODE_LEN (256)
#define PARAMS_LEN (128)
#define MAX_TEXT_LEN (128)

#define IsScenarioExists(%0,%1) (ScenarioData[%0][%1][range] != -1.0)
#define IsPlayerInScenario(%0,%1) (IsPlayerInRangeOfPoint(%0,ScenarioData[GetPlayerVirtualWorld(%0)-10][%1][range],ScenarioData[GetPlayerVirtualWorld(%0)-10][%1][posX],ScenarioData[GetPlayerVirtualWorld(%0)-10][%1][posY],ScenarioData[GetPlayerVirtualWorld(%0)-10][%1][posZ]))

enum eScenario
{
	Float: posX,
	Float: posY,
	Float: posZ,
	Float: range,

	Line1[MAX_CODE_LEN],
	Line2[MAX_CODE_LEN],
	Line3[MAX_CODE_LEN],
	Line4[MAX_CODE_LEN],
	Line5[MAX_CODE_LEN],
    Line6[MAX_CODE_LEN],
	Line7[MAX_CODE_LEN],
	Line8[MAX_CODE_LEN],
	Line9[MAX_CODE_LEN],
	Line10[MAX_CODE_LEN],
	
	Text[MAX_TEXT_LEN],
	typeactivation,  /*
	                    0 - button [N]
	                    1 - button [Y]
	                    2 - death
	                    3 - kill
	                    4 - onshot
	                    5 - take damage
	                    6 - give damage
	                    7 - sit in veh
	                    8 - everysecond
	                */

	typecondition, /*
					  0 - N/A
					  1 - ==
					  2 - >
					  3 - <
					  4 - >=
					  5 - <=
					  6 - !=
	                */
	                
	condition1,
	condition2
}
new ScenarioData[MAX_CWORLDS][MAX_CW_PREM_SCENARIOS][eScenario];
new clear_scenario[eScenario];

new EverySecondTimer;

new scenarTypeName[][] =
{
	"Кнопка [N]",
	"Кнопка [Y]",
    "Смерть",
    "Убийство",
    "Выстрел",
	"Получение урона",
	"Нанесение урона",
	"Сесть в транспорт"
};

new ConditionTypeName[][] =
{
	"N/A",
	"==",
    ">",
    "<",
    ">=",
	"<=",
	"!="
};

public OnGameModeInit()
{
	for(new i; i != MAX_CWORLDS; i++)
	{
		for(new j; j != MAX_CW_PREM_SCENARIOS; j++)
		{
			ScenarioData[i][j] = clear_scenario;
			ScenarioData[i][j][range] = -1.0;
		}
	}
	print("Scenario Areas System loaded!");
	EverySecondTimer = SetTimer("OnEverySecondTimer", 1000, true);

    #if defined sandscenario_OnGameModeInit
    	return sandscenario_OnGameModeInit();
    #else
        return 1;
    #endif
}
#if defined _ALS_OnGameModeInit
	#undef OnGameModeInit
#else
	#define _ALS_OnGameModeInit
#endif
#define OnGameModeInit sandscenario_OnGameModeInit
#if defined sandscenario_OnGameModeInit
forward sandscenario_OnGameModeInit();
#endif

public OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
	if(dialogid == DIALOG_SCENARIO)
	{
		if(response)
		{
		    if(IsWorldAdmin(playerid))
		    {
				new custwd = (GetPlayerVirtualWorld(playerid) - 10);
				if(listitem >= 0 && listitem <= 9)
				{
					SPData[playerid][SelSceLine] = (listitem + 1);
					ShowPlayerDialog(playerid, DIALOG_SCENARIO_LINE, DIALOG_STYLE_INPUT, "{FFFFFF}Изменение строки кода", "{FFFFFF}Введите код, согласно синтаксису:", "Готово", "Отмена");
				}
				if(listitem == 10) GetPlayerPos(playerid, ScenarioData[custwd][SPData[playerid][SelSce]][posX], ScenarioData[custwd][SPData[playerid][SelSce]][posY], ScenarioData[custwd][SPData[playerid][SelSce]][posZ]);
				if(listitem == 11) ShowPlayerDialog(playerid, DIALOG_SCENARIO_EDITRANGE, DIALOG_STYLE_INPUT, "{FFFFFF}Изменение радиуса активации", "{FFFFFF}Введите радиус активации(0.1 - 9999.0):", "Готово", "Отмена");
				if(listitem == 12) ShowPlayerDialog(playerid, DIALOG_SCENARIO_TYPE, DIALOG_STYLE_LIST, "{FFFFFF}Изменение типа активации", "{FFFFFF}Кнопка [N]\nКнопка [Y]\nСмерть\nУбийство\nВыстрел\nПолучение урона\nНанесение урона\nСесть в транспорт\nЕжесекундно", "Выбрать", "Отмена");
				if(listitem == 13) ScenarioData[custwd][SPData[playerid][SelSce]][range] = -1.0;
				if(listitem == 14) ShowScenarioCondition(playerid, SPData[playerid][SelSce]);
			}
		}
	}
	if(dialogid == DIALOG_SCENARIO_LINE)
	{
	    if(response)
	    {
	        if(IsWorldAdmin(playerid))
	        {
	            new custwd = (GetPlayerVirtualWorld(playerid) - 10);
				switch(SPData[playerid][SelSceLine])
				{
				    case 1: format(ScenarioData[custwd][SPData[playerid][SelSce]][Line1], MAX_CODE_LEN, inputtext);
				    case 2: format(ScenarioData[custwd][SPData[playerid][SelSce]][Line2], MAX_CODE_LEN, inputtext);
				    case 3: format(ScenarioData[custwd][SPData[playerid][SelSce]][Line3], MAX_CODE_LEN, inputtext);
				    case 4: format(ScenarioData[custwd][SPData[playerid][SelSce]][Line4], MAX_CODE_LEN, inputtext);
				    case 5: format(ScenarioData[custwd][SPData[playerid][SelSce]][Line5], MAX_CODE_LEN, inputtext);
				    case 6: format(ScenarioData[custwd][SPData[playerid][SelSce]][Line6], MAX_CODE_LEN, inputtext);
				    case 7: format(ScenarioData[custwd][SPData[playerid][SelSce]][Line7], MAX_CODE_LEN, inputtext);
				    case 8: format(ScenarioData[custwd][SPData[playerid][SelSce]][Line8], MAX_CODE_LEN, inputtext);
				    case 9: format(ScenarioData[custwd][SPData[playerid][SelSce]][Line9], MAX_CODE_LEN, inputtext);
				    case 10: format(ScenarioData[custwd][SPData[playerid][SelSce]][Line10], MAX_CODE_LEN, inputtext);
				}
	        }
	    }
	}
	if(dialogid == DIALOG_SCENARIO_EDITRANGE)
	{
	    if(response)
	    {
	        if(IsWorldAdmin(playerid))
	        {
	            if(strval(inputtext) >= 0.1 && strval(inputtext) <= 9999.0)
	            {
	                ScenarioData[GetPlayerVirtualWorld(playerid) - 10][SPData[playerid][SelSce]][range] = strval(inputtext);
	            }
	        }
	    }
	}
	if(dialogid == DIALOG_SCENARIO_TYPE)
	{
	    if(response)
		{
		    if(IsWorldAdmin(playerid))
		    {
				ScenarioData[GetPlayerVirtualWorld(playerid) - 10][SPData[playerid][SelSce]][typeactivation] = listitem;
		    }
		}
	}
	if(dialogid == DIALOG_SCENARIO_CONDITION)
	{
	    if(response)
	    {
	        if(IsWorldAdmin(playerid))
	        {
	            if(listitem == 0) ShowPlayerDialog(playerid, DIALOG_SCENARIO_CONDTYPE, DIALOG_STYLE_LIST, "{FFFFFF}Тип условия", "{FFFFFF}N/A\nРавно (==)\nБольше (>)\nМеньше (<)\nБольше/Равно (>=)\nМеньше/Равно (<=)\nНе равно (!=)", "Выбрать", "Отмена");
	            if(listitem == 1) ShowPlayerDialog(playerid, DIALOG_SCENARIO_CONDITION1, DIALOG_STYLE_INPUT, "{FFFFFF}Значение 1", "{FFFFFF}Введите значение:", "Выбрать", "Отмена");
	            if(listitem == 2) ShowPlayerDialog(playerid, DIALOG_SCENARIO_CONDITION2, DIALOG_STYLE_INPUT, "{FFFFFF}Значение 2", "{FFFFFF}Введите значение:", "Выбрать", "Отмена");
	        }
	    }
	}
	if(dialogid == DIALOG_SCENARIO_CONDTYPE)
	{
		if(response)
		{
		    if(IsWorldAdmin(playerid))
		    {
		        ScenarioData[GetPlayerVirtualWorld(playerid) - 10][SPData[playerid][SelSce]][typecondition] = listitem;
		    }
		}
	}
	if(dialogid == DIALOG_SCENARIO_CONDITION1)
	{
		if(response)
		{
		    if(IsWorldAdmin(playerid))
		    {
				ScenarioData[GetPlayerVirtualWorld(playerid) - 10][SPData[playerid][SelSce]][condition1] = strval(inputtext);
		    }
		}
	}
	if(dialogid == DIALOG_SCENARIO_CONDITION2)
	{
		if(response)
		{
		    if(IsWorldAdmin(playerid))
		    {
		        ScenarioData[GetPlayerVirtualWorld(playerid) - 10][SPData[playerid][SelSce]][condition2] = strval(inputtext);
		    }
		}
	}
	
	#if defined sandscenario_OnDialogResponse
    	return sandscenario_OnDialogResponse(playerid, dialogid, response, listitem, inputtext);
    #else
        return 1;
    #endif
}
#if defined _ALS_OnDialogResponse
	#undef OnDialogResponse
#else
	#define _ALS_OnDialogResponse
#endif
#define OnDialogResponse sandscenario_OnDialogResponse
#if defined sandscenario_OnDialogResponse
forward sandscenario_OnDialogResponse(playerid, dialogid, response, listitem, inputtext[]);
#endif

public OnPlayerDeath(playerid, killerid, reason)
{
	if(GetPlayerVirtualWorld(playerid) >= 10)
	{
		new scustwd = (GetPlayerVirtualWorld(playerid) - 10);
		new k_scustwd = (GetPlayerVirtualWorld(killerid) - 10);
		
	    for(new i; i != MAX_CW_PREM_SCENARIOS; i++)
	    {
	        if(IsPlayerInScenario(playerid, i))
	        {
				if(IsScenarioExists(scustwd, i) && CheckScenarioCondition(scustwd, i))
				{
				    if(ScenarioData[scustwd][i][typeactivation] == 2)
				    {
				        ExecuteAllScenario(playerid, scustwd, i);
					}
					else if(ScenarioData[scustwd][i][typeactivation] == 3)
				    {
				        if(GetPlayerVirtualWorld(killerid) == scustwd)
				        {
				            ExecuteAllScenario(killerid, k_scustwd, i);
						}
					}
				}
			}
	    }
    }

	#if defined sandscenario_OnPlayerDeath
    	return sandscenario_OnPlayerDeath(playerid, killerid, reason);
    #else
        return 1;
    #endif
}
#if defined _ALS_OnPlayerDeath
	#undef OnPlayerDeath
#else
	#define _ALS_OnPlayerDeath
#endif
#define OnPlayerDeath sandscenario_OnPlayerDeath
#if defined sandscenario_OnPlayerDeath
forward sandscenario_OnPlayerDeath(playerid, killerid, reason);
#endif

public OnPlayerGiveDamage(playerid, damagedid, Float:amount, weaponid, bodypart)
{
    if(GetPlayerVirtualWorld(playerid) >= 10)
	{
		new scustwd = (GetPlayerVirtualWorld(playerid) - 10);

	    for(new i; i != MAX_CW_PREM_SCENARIOS; i++)
	    {
	        if(IsPlayerInScenario(playerid, i))
	        {
				if(IsScenarioExists(scustwd, i) && CheckScenarioCondition(scustwd, i))
				{
				    if(ScenarioData[scustwd][i][typeactivation] == 6)
				    {
	                    ExecuteAllScenario(playerid, scustwd, i);
					}
				}
			}
	    }
    }

	#if defined sandscenario_OnPlayerGiveDamage
    	return sandscenario_OnPlayerGiveDamage(playerid, damagedid, amount, weaponid, bodypart);
    #else
        return 1;
    #endif
}
#if defined _ALS_OnPlayerGiveDamage
	#undef OnPlayerGiveDamage
#else
	#define _ALS_OnPlayerGiveDamage
#endif
#define OnPlayerGiveDamage sandscenario_OnPlayerGiveDamage
#if defined sandscenario_OnPlayerGiveDamage
forward sandscenario_OnPlayerGiveDamage(playerid, damagedid, Float:amount, weaponid, bodypart);
#endif

public OnPlayerTakeDamage(playerid, issuerid, Float:amount, weaponid, bodypart)
{
    if(GetPlayerVirtualWorld(playerid) >= 10)
	{
		new scustwd = (GetPlayerVirtualWorld(playerid) - 10);

	    for(new i; i != MAX_CW_PREM_SCENARIOS; i++)
	    {
	        if(IsPlayerInScenario(playerid, i))
	        {
				if(IsScenarioExists(scustwd, i) && CheckScenarioCondition(scustwd, i))
				{
				    if(ScenarioData[scustwd][i][typeactivation] == 5)
				    {
	                    ExecuteAllScenario(playerid, scustwd, i);
					}
				}
			}
	    }
    }

	#if defined sandscenario_OnPlayerTakeDamage
    	return sandscenario_OnPlayerTakeDamage(playerid, issuerid, amount, weaponid, bodypart);
    #else
        return 1;
    #endif
}
#if defined _ALS_OnPlayerTakeDamage
	#undef OnPlayerTakeDamage
#else
	#define _ALS_OnPlayerTakeDamage
#endif
#define OnPlayerTakeDamage sandscenario_OnPlayerTakeDamage
#if defined sandscenario_OnPlayerTakeDamage
forward sandscenario_OnPlayerTakeDamage(playerid, issuerid, Float:amount, weaponid, bodypart);
#endif

public OnPlayerWeaponShot(playerid, weaponid, hittype, hitid, Float:fX, Float:fY, Float:fZ)
{
    if(GetPlayerVirtualWorld(playerid) >= 10)
	{
		new scustwd = (GetPlayerVirtualWorld(playerid) - 10);

	    for(new i; i != MAX_CW_PREM_SCENARIOS; i++)
	    {
	        if(IsPlayerInScenario(playerid, i))
	        {
				if(IsScenarioExists(scustwd, i) && CheckScenarioCondition(scustwd, i))
				{
				    if(ScenarioData[scustwd][i][typeactivation] == 4)
				    {
						ExecuteAllScenario(playerid, scustwd, i);
					}
				}
			}
	    }
    }

	#if defined sandscenario_OnPlayerWeaponShot
    	return sandscenario_OnPlayerWeaponShot(playerid, weaponid, hittype, hitid, fX, fY, fZ);
    #else
        return 1;
    #endif
}
#if defined _ALS_OnPlayerWeaponShot
	#undef OnPlayerWeaponShot
#else
	#define _ALS_OnPlayerWeaponShot
#endif
#define OnPlayerWeaponShot sandscenario_OnPlayerWeaponShot
#if defined sandscenario_OnPlayerWeaponShot
forward sandscenario_OnPlayerWeaponShot(playerid, weaponid, hittype, hitid, Float:fX, Float:fY, Float:fZ);
#endif

public OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
    if(GetPlayerVirtualWorld(playerid) >= 10)
	{
	    if(newkeys & KEY_NO)
	    {
			new scustwd = (GetPlayerVirtualWorld(playerid) - 10);
		    for(new i; i != MAX_CW_PREM_SCENARIOS; i++)
		    {
		        if(IsPlayerInScenario(playerid, i))
	        	{
					if(IsScenarioExists(scustwd, i) && CheckScenarioCondition(scustwd, i))
					{
					    if(ScenarioData[scustwd][i][typeactivation] == 0)
					    {
					        ExecuteAllScenario(playerid, scustwd, i);
						}
					}
				}
		    }
	    }
	    if(newkeys & KEY_YES)
	    {
			new scustwd = (GetPlayerVirtualWorld(playerid) - 10);
		    for(new i; i != MAX_CW_PREM_SCENARIOS; i++)
		    {
		        if(IsPlayerInScenario(playerid, i))
	        	{
					if(IsScenarioExists(scustwd, i) && CheckScenarioCondition(scustwd, i))
					{
					    if(ScenarioData[scustwd][i][typeactivation] == 1)
					    {
					        ExecuteAllScenario(playerid, scustwd, i);
						}
					}
				}
		    }
	    }
    }
    
	#if defined sandsce_OnPlayerKeyStateChange
    	return sandsce_OnPlayerKeyStateChange(playerid, newkeys, oldkeys);
    #else
        return 1;
    #endif
}
#if defined _ALS_OnPlayerKeyStateChange
	#undef OnPlayerKeyStateChange
#else
	#define _ALS_OnPlayerKeyStateChange
#endif
#define OnPlayerKeyStateChange sandsce_OnPlayerKeyStateChange
#if defined sandsce_OnPlayerKeyStateChange
forward sandsce_OnPlayerKeyStateChange(playerid, newkeys, oldkeys);
#endif

public OnPlayerStateChange(playerid, newstate, oldstate)
{
    if(GetPlayerVirtualWorld(playerid) >= 10)
	{
		if(newstate == PLAYER_STATE_DRIVER || newstate == PLAYER_STATE_PASSENGER) // MAYBE BUG!!!!!!
		{
			new scustwd = (GetPlayerVirtualWorld(playerid) - 10);
		    for(new i; i != MAX_CW_PREM_SCENARIOS; i++)
		    {
		        if(IsPlayerInScenario(playerid, i))
	        	{
					if(IsScenarioExists(scustwd, i) && CheckScenarioCondition(scustwd, i))
					{
					    if(ScenarioData[scustwd][i][typeactivation] == 7)
					    {
					        ExecuteAllScenario(playerid, scustwd, i);
						}
					}
				}
		    }
		}
    }
    
    #if defined sandsce_OnPlayerStateChange
    	return sandsce_OnPlayerStateChange(playerid, newstate, oldstate);
    #else
        return 1;
    #endif
}
#if defined _ALS_OnPlayerStateChange
	#undef OnPlayerStateChange
#else
	#define _ALS_OnPlayerStateChange
#endif
#define OnPlayerStateChange sandsce_OnPlayerStateChange
#if defined sandsce_OnPlayerStateChange
forward sandsce_OnPlayerStateChange(playerid, newstate, oldstate);
#endif

ExecuteAllScenario(playerid, cworld, sa_id)
{
    if(cworld == (GetPlayerVirtualWorld(playerid) - 10)) ParseString(playerid, sa_id, ScenarioData[cworld][sa_id][Line1]);
	if(cworld == (GetPlayerVirtualWorld(playerid) - 10)) ParseString(playerid, sa_id, ScenarioData[cworld][sa_id][Line2]);
	if(cworld == (GetPlayerVirtualWorld(playerid) - 10)) ParseString(playerid, sa_id, ScenarioData[cworld][sa_id][Line3]);
	if(cworld == (GetPlayerVirtualWorld(playerid) - 10)) ParseString(playerid, sa_id, ScenarioData[cworld][sa_id][Line4]);
	if(cworld == (GetPlayerVirtualWorld(playerid) - 10)) ParseString(playerid, sa_id, ScenarioData[cworld][sa_id][Line5]);
	if(cworld == (GetPlayerVirtualWorld(playerid) - 10)) ParseString(playerid, sa_id, ScenarioData[cworld][sa_id][Line6]);
	if(cworld == (GetPlayerVirtualWorld(playerid) - 10)) ParseString(playerid, sa_id, ScenarioData[cworld][sa_id][Line7]);
	if(cworld == (GetPlayerVirtualWorld(playerid) - 10)) ParseString(playerid, sa_id, ScenarioData[cworld][sa_id][Line8]);
	if(cworld == (GetPlayerVirtualWorld(playerid) - 10)) ParseString(playerid, sa_id, ScenarioData[cworld][sa_id][Line9]);
	if(cworld == (GetPlayerVirtualWorld(playerid) - 10)) ParseString(playerid, sa_id, ScenarioData[cworld][sa_id][Line10]);
	return 1;
}

CheckScenarioCondition(cworld, sa_id)
{
	switch(ScenarioData[cworld][sa_id][typecondition])
	{
		case 0: return 1;
	    case 1: return (ScenarioData[cworld][sa_id][condition1] == ScenarioData[cworld][sa_id][condition2]);
	    case 2: return (ScenarioData[cworld][sa_id][condition1] > ScenarioData[cworld][sa_id][condition2]);
	    case 3: return (ScenarioData[cworld][sa_id][condition1] < ScenarioData[cworld][sa_id][condition2]);
	    case 4: return (ScenarioData[cworld][sa_id][condition1] >= ScenarioData[cworld][sa_id][condition2]);
	    case 5: return (ScenarioData[cworld][sa_id][condition1] <= ScenarioData[cworld][sa_id][condition2]);
	    case 6: return (ScenarioData[cworld][sa_id][condition1] != ScenarioData[cworld][sa_id][condition2]);
	}
	return 0;
}

ParseString(playerid, sa_id, const string[])
{
	new parstr[2][PARAMS_LEN];
	str_split(string, parstr, ':');
	Execute(playerid, sa_id, parstr[0], parstr[1]);
	return 1;
}

ClearWorldScenarios(cworld)
{
	for(new i; i != MAX_CW_PREM_SCENARIOS; i++)
	{
		ScenarioData[cworld][i][range] = -1.0;
	}
	return 1;
}

GetCountScenariosWorld(cworld)
{
	new count;
	for(new i; i != MAX_CW_PREM_SCENARIOS; i++)
	{
	    if(IsScenarioExists(cworld, i)) count++;
	}
	return count;
}

Execute(playerid, sa_id, func[], params[])
{
	new so_buf[128],
		custwd = (GetPlayerVirtualWorld(playerid) - 10);
		
	str_replace(params, "[TEXT]", ScenarioData[custwd][sa_id][Text]);
	
	if(isnull(func)) return 0;
	else if(IsFuncType("set text"))
	{
	    if(sscanf(params, "s[128]", params[0]))
	    {
	        format(so_buf, sizeof so_buf, "[SA №%d] Ошибка: неверные параметры.", sa_id);
			SCM(playerid, -1, so_buf);
			return 0;
	    }
 		format(ScenarioData[custwd][sa_id][Text], MAX_TEXT_LEN, params[0]);
 		return 1;
	}
	else if(IsFuncType("sum"))
	{
	    if(sscanf(params, "dd", params[0], params[1]))
	    {
	        format(so_buf, sizeof so_buf, "[SA №%d] Ошибка: неверные параметры.", sa_id);
			SCM(playerid, -1, so_buf);
			return 0;
	    }
		format(ScenarioData[custwd][sa_id][Text], MAX_TEXT_LEN, "%d", params[0] + params[1]);
		return 1;
	}
	else if(IsFuncType("sub"))
	{
	    if(sscanf(params, "dd", params[0], params[1]))
	    {
	        format(so_buf, sizeof so_buf, "[SA №%d] Ошибка: неверные параметры.", sa_id);
			SCM(playerid, -1, so_buf);
			return 0;
	    }
		format(ScenarioData[custwd][sa_id][Text], MAX_TEXT_LEN, "%d", params[0] - params[1]);
		return 1;
	}
	else if(IsFuncType("mul"))
	{
	    if(sscanf(params, "dd", params[0], params[1]))
	    {
	        format(so_buf, sizeof so_buf, "[SA №%d] Ошибка: неверные параметры.", sa_id);
			SCM(playerid, -1, so_buf);
			return 0;
	    }
		format(ScenarioData[custwd][sa_id][Text], MAX_TEXT_LEN, "%d", params[0] * params[1]);
		return 1;
	}
	else if(IsFuncType("div"))
	{
	    if(sscanf(params, "dd", params[0], params[1]))
	    {
	        format(so_buf, sizeof so_buf, "[SA №%d] Ошибка: неверные параметры.", sa_id);
			SCM(playerid, -1, so_buf);
			return 0;
	    }
		format(ScenarioData[custwd][sa_id][Text], MAX_TEXT_LEN, "%d", params[0] / params[1]);
		return 1;
	}
	else if(IsFuncType("createveh"))
	{
	    if(GetWorldVehCount(GetPlayerVirtualWorld(playerid)) <= (IsPlayerPremium(GetCWorldOwnerPlayer(GetPlayerVirtualWorld(playerid) - 10))) ? (MAX_WORLD_PREM_VEH) : (MAX_WORLD_VEH))
	    {
		    if(sscanf(params, "ddd", params[0], params[1], params[2]))
		    {
		  		format(so_buf, sizeof so_buf, "[SA №%d] Ошибка: неверные параметры.", sa_id);
				SCM(playerid, -1, so_buf);
				return 0;
		    }
		    GivePlayerVehicle(playerid, params[0], params[1], params[2]);
	    }
	    return 1;
	}
	else if(IsFuncType("setplyhealth"))
	{
	    if(sscanf(params, "d", params[0]))
	    {
	        format(so_buf, sizeof so_buf, "[SA №%d] Ошибка: неверные параметры.", sa_id);
			SCM(playerid, -1, so_buf);
			return 0;
	    }
	    SetPlayerHealth(playerid, params[0]);
	    return 1;
	}
	else if(IsFuncType("setplyarmour"))
	{
	    if(sscanf(params, "d", params[0]))
	    {
	        format(so_buf, sizeof so_buf, "[SA №%d] Ошибка: неверные параметры.", sa_id);
			SCM(playerid, -1, so_buf);
			return 0;
	    }
	    SetPlayerArmour(playerid, params[0]);
	    return 1;
	}
	else if(IsFuncType("setplypos"))
	{
	    if(sscanf(params, "fff", params[0], params[1], params[2]))
	    {
	        format(so_buf, sizeof so_buf, "[SA №%d] Ошибка: неверные параметры.", sa_id);
			SCM(playerid, -1, so_buf);
			return 0;
	    }
	    SetPlayerPos(playerid, params[0], params[1], params[2]);
	    return 1;
	}
	else if(IsFuncType("kick")) // ТАК ЖЕ НУЖНО ОЧИЩАТЬ ПАГИНАТОРЫ ГАНЫ И ПРОЧУЮ ДИЧЬ!!!
	{
	    SetPlayerVirtualWorld(playerid, 0);
	    return 1;
	}
	else if(IsFuncType("message"))
	{
	    if(sscanf(params, "s[128]", params[0]))
	    {
	        format(so_buf, sizeof so_buf, "[SA №%d] Ошибка: неверные параметры.", sa_id);
			SCM(playerid, -1, so_buf);
			return 0;
	    }
	    format(so_buf, sizeof so_buf, "[SA №%d] %s", sa_id, params[0]);
		SCM(playerid, -1, so_buf);
		return 1;
	}
	else if(IsFuncType("gametext"))
	{
	    if(sscanf(params, "s[128]dd", params[0], params[1], params[2]))
	    {
	        format(so_buf, sizeof so_buf, "[SA №%d] Ошибка: неверные параметры.", sa_id);
			SCM(playerid, -1, so_buf);
			return 0;
	    }
 		GameTextForPlayer(playerid, params[0], params[1] * 1000, params[2]);
 		return 1;
	}
	else if(IsFuncType("giveplyweapon"))
	{
	    if(sscanf(params, "dd", params[0], params[1]))
	    {
	        format(so_buf, sizeof so_buf, "[SA №%d] Ошибка: неверные параметры.", sa_id);
			SCM(playerid, -1, so_buf);
			return 0;
	    }
		GivePlayerWeapon(playerid, params[0], params[1]);
		return 1;
	}
	else
	{
	    format(so_buf, sizeof so_buf, "[SA №%d] Ошибка: неизвестная функция.", sa_id);
		SCM(playerid, -1, so_buf);
		return 0;
	}
}

str_split(const strsrc[], strdest[][], delimiter)
{
	new i, li, aNum, len;
	while(i <= strlen(strsrc))
	{
	    if(strsrc[i]==delimiter || i==strlen(strsrc))
	    {
	        len = strmid(strdest[aNum], strsrc, li, i, 128);
	        strdest[aNum][len] = 0;
	        li = i+1;
	        aNum++;
	    }
	    i++;
	}
	return 1;
}

str_replace(inputvar[],const searchstring[],const replacestring[],bool:ignorecase=true,bool:replaceall=false)
{
	if(strfind(replacestring,searchstring,ignorecase) != -1) return -1;
	new pos = strfind(inputvar,searchstring,ignorecase);
	if(pos == -1) return 0;
	if(replaceall == true)
	{
		while(strfind(inputvar,searchstring,ignorecase) != -1)
		{
			pos = strfind(inputvar,searchstring,ignorecase);
			strdel(inputvar, pos, pos+strlen(searchstring));
			strins(inputvar, replacestring, pos, strlen(inputvar));
		}
	}
	else
	{
		strdel(inputvar, pos, pos+strlen(searchstring));
		strins(inputvar, replacestring, pos, strlen(inputvar));
	}
	return 1;
}

GetFreeScenario(cworld)
{
	for(new i; i != (IsPlayerPremium(GetCWorldOwnerPlayer(cworld))) ? (MAX_CW_PREM_SCENARIOS) : (MAX_CW_SCENARIOS); i++)
	{
	    if(ScenarioData[cworld][i][range] == -1.0) return i;
	}
	return -1;
}

ShowScenarioOptions(playerid, so_id)
{
    SPData[playerid][SelSce] = so_id;
	new buf[1024], custw = (GetPlayerVirtualWorld(playerid) - 10);
	format(buf, sizeof buf, "{FFFFFF}Опция\tПараметр");
	format(buf, sizeof buf, "%s\nЛиния 1\t[ %s ]", buf, ScenarioData[custw][so_id][Line1]);
	format(buf, sizeof buf, "%s\nЛиния 2\t[ %s ]", buf, ScenarioData[custw][so_id][Line2]);
	format(buf, sizeof buf, "%s\nЛиния 3\t[ %s ]", buf, ScenarioData[custw][so_id][Line3]);
	format(buf, sizeof buf, "%s\nЛиния 4\t[ %s ]", buf, ScenarioData[custw][so_id][Line4]);
	format(buf, sizeof buf, "%s\nЛиния 5\t[ %s ]", buf, ScenarioData[custw][so_id][Line5]);
	format(buf, sizeof buf, "%s\nЛиния 6\t[ %s ]", buf, ScenarioData[custw][so_id][Line6]);
	format(buf, sizeof buf, "%s\nЛиния 7\t[ %s ]", buf, ScenarioData[custw][so_id][Line7]);
	format(buf, sizeof buf, "%s\nЛиния 8\t[ %s ]", buf, ScenarioData[custw][so_id][Line8]);
	format(buf, sizeof buf, "%s\nЛиния 9\t[ %s ]", buf, ScenarioData[custw][so_id][Line9]);
	format(buf, sizeof buf, "%s\nЛиния 10\t[ %s ]", buf, ScenarioData[custw][so_id][Line10]);
	format(buf, sizeof buf, "%s\nОбновить позицию\t[ Опционально ]", buf);
	format(buf, sizeof buf, "%s\nИзменить радиус активации\t[ %f ]", buf, ScenarioData[custw][so_id][range]);
	format(buf, sizeof buf, "%s\nТип активации\t[ %s ]", buf, scenarTypeName[ScenarioData[custw][so_id][typeactivation]]);
	format(buf, sizeof buf, "%s\nУдалить данную область\t[ Опционально ]", buf);
	format(buf, sizeof buf, "%s\nРедактировать условие\t[ Опционально ]", buf);
	ShowPlayerDialog(playerid, DIALOG_SCENARIO, DIALOG_STYLE_TABLIST_HEADERS, "{FFFFFF}Сценарная область", buf, "Выбрать", "Отмена");
	return 1;
}

ShowScenarioCondition(playerid, so_id)
{
	SPData[playerid][SelSce] = so_id; // MAYBE BUG!!!!
	new buf[256], custwd = (GetPlayerVirtualWorld(playerid) - 10);
	format(buf, sizeof buf, "{FFFFFF}Опция\t{FFFFFF}Параметр");
	format(buf, sizeof buf, "%s\nТип условия\t[ %s ]", buf, ConditionTypeName[ScenarioData[custwd][so_id][typecondition]]);
	format(buf, sizeof buf, "%s\nЗначение 1\t[ %d ]", buf, ScenarioData[custwd][so_id][condition1]);
	format(buf, sizeof buf, "%s\nЗначение 2\t[ %d ]", buf, ScenarioData[custwd][so_id][condition2]);
	ShowPlayerDialog(playerid, DIALOG_SCENARIO_CONDITION, DIALOG_STYLE_TABLIST_HEADERS, "{FFFFFF}Условие", buf, "Выбрать", "Отмена");
	return 1;
}

CMD:newso(playerid)
{
	if(!IsWorldAdmin(playerid)) return 1;
	if(GetFreeScenario(GetPlayerVirtualWorld(playerid) - 10) == -1) return SCM(playerid, -1, "Превышен лимит сценарных областей!");
	ShowScenarioOptions(playerid, GetFreeScenario(GetPlayerVirtualWorld(playerid) - 10));
	ScenarioData[GetPlayerVirtualWorld(playerid) - 10][GetFreeScenario(GetPlayerVirtualWorld(playerid) - 10)][range] = 1.0;
	return 1;
}

CMD:editso(playerid, params[])
{
	if(IsWorldAdmin(playerid))
	{
	    if(sscanf(params, "d", params[0])) return SCM(playerid, -1, "/editso [ID SO]");
	    new wd = GetPlayerVirtualWorld(playerid);
	    if(IsScenarioExists(wd - 10, params[0]))
	    {
	        ShowScenarioOptions(playerid, params[0]);
	    }
	}
	return 1;
}

forward OnEverySecondTimer();
public OnEverySecondTimer() // OPTIMIZATION СРОЧНО!!!!!!!!!!!!!
{
	foreach(new i: Player)
	{
	    new scustwd = (GetPlayerVirtualWorld(i) - 10);
	    for(new j; j != MAX_CW_PREM_SCENARIOS; j++)
	    {
			if(IsPlayerInScenario(i, j))
	        {
				if(IsScenarioExists(scustwd, j) && CheckScenarioCondition(scustwd, j))
				{
				    if(ScenarioData[scustwd][j][typeactivation] == 0)
				    {
				        ExecuteAllScenario(i, scustwd, j);
					}
				}
			}
	    }
	}
	return 1;
}
