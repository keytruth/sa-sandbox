#if !defined IsValidVehicle
    native IsValidVehicle(vehicleid);
#endif

new VehLock[MAX_VEHICLES];

public OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
	if(IsPlayerInAnyVehicle(playerid))
	{
	    if(GetPlayerVehicleSeat(playerid) == 0)
	    {
	        new vehid = GetPlayerVehicleID(playerid);
	        if(newkeys & KEY_ACTION) SetEngineVehicle(vehid, !GetEngineVehicle(vehid));
	        if(newkeys & KEY_FIRE) SetLightsVehicle(vehid, !GetLightsVehicle(vehid));
	    }
	}

	#if defined sandveh_OnPlayerKeyStateChange
    	return sandveh_OnPlayerKeyStateChange(playerid, newkeys, oldkeys);
    #else
        return 1;
    #endif
}
#if defined _ALS_OnPlayerKeyStateChange
	#undef OnPlayerKeyStateChange
#else
	#define _ALS_OnPlayerKeyStateChange
#endif
#define OnPlayerKeyStateChange sandveh_OnPlayerKeyStateChange
#if defined sandveh_OnPlayerKeyStateChange
forward sandveh_OnPlayerKeyStateChange(playerid, newkeys, oldkeys);
#endif

SetEngineVehicle(vehicleid, toggle)
{
	if(IsValidVehicle(vehicleid))
	{
	    new engine, lights, alarm, doors, bonnet, boot, objective;
	    GetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective);
	    SetVehicleParamsEx(vehicleid, toggle, lights, alarm, doors, bonnet, boot, objective);
	}
	return 1;
}

GetEngineVehicle(vehicleid)
{
	if(IsValidVehicle(vehicleid))
	{
	    new engine, lights, alarm, doors, bonnet, boot, objective;
	    GetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective);
	    return engine;
	}
	return -1;
}

SetLockVehicle(vehicleid, toggle)
{
	if(IsValidVehicle(vehicleid))
	{
	    new engine, lights, alarm, doors, bonnet, boot, objective;
	    GetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective);
	    SetVehicleParamsEx(vehicleid, engine, lights, alarm, toggle, bonnet, boot, objective);
	}
	return 1;
}

GetLockVehicle(vehicleid)
{
	if(IsValidVehicle(vehicleid))
	{
	    new engine, lights, alarm, doors, bonnet, boot, objective;
	    GetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective);
	    return doors;
	}
	return -1;
}

SetLightsVehicle(vehicleid, toggle)
{
	if(IsValidVehicle(vehicleid))
	{
	    new engine, lights, alarm, doors, bonnet, boot, objective;
	    GetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective);
	    SetVehicleParamsEx(vehicleid, engine, toggle, alarm, doors, bonnet, boot, objective);
	}
	return 1;
}

GetLightsVehicle(vehicleid)
{
	if(IsValidVehicle(vehicleid))
	{
	    new engine, lights, alarm, doors, bonnet, boot, objective;
	    GetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective);
	    return lights;
	}
	return -1;
}

ClearWorldVehicles(worldid)
{
	foreach(new i: Vehicle)
	{
	    if(worldid == GetVehicleVirtualWorld(i)) DestroyVehicle(i);
	}
	return 1;
}

CMD:veh(playerid, params[])
{
    if(IsPlayerInAnyVehicle(playerid)) return SCM(playerid, -1, "Недоступно, сидя в транспорте!");
    if(GetWorldVehCount(GetPlayerVirtualWorld(playerid)) >= ((IsPlayerPremium(GetCWorldOwnerPlayer(GetPlayerVirtualWorld(playerid) - 10))) ? (MAX_WORLD_PREM_VEH) : (MAX_WORLD_VEH))) return SCM(playerid, -1, "Превышен лимит транспорта в этом мире!");
    if(IsWorldModer(playerid) || IsWorldAdmin(playerid))
	{
		if(sscanf(params, "d", params[0]))
		{
		    ShowPagenatorForPlayer(playerid, 2);
		}
		else
		{
			if(params[0] >= 400 && params[0] <= 611)
			{
				new Float: X, Float: Y, Float: Z, Float: A;
				GetPlayerPos(playerid, X, Y, Z);
				GetPlayerFacingAngle(playerid, A);
				new vehid = CreateVehicle(params[0], X, Y, Z, A, 1, 1, -1);
				SetVehicleVirtualWorld(vehid, GetPlayerVirtualWorld(playerid));
				LinkVehicleToInterior(vehid, GetPlayerInterior(playerid));
				PutPlayerInVehicle(playerid, vehid, 0);
				VehLock[vehid] = PlayerData[playerid][pID];
			}
		}
	}
	return 1;
}

CMD:delveh(playerid, params[])
{
	if(IsWorldModer(playerid) || IsWorldAdmin(playerid))
	{
		if(sscanf(params, "d", params[0]) && IsPlayerInAnyVehicle(playerid))
		{
		    VehLock[GetPlayerVehicleID(playerid)] = -1;
			DestroyVehicle(GetPlayerVehicleID(playerid));
		}
		else
		{
			if(IsValidVehicle(params[0]) && (GetVehicleVirtualWorld(params[0]) == GetPlayerVirtualWorld(playerid))) VehLock[params[0]] = -1, DestroyVehicle(params[0]);
		}
	}
	return 1;
}

CMD:vehcolor(playerid, params[])
{
	if(sscanf(params, "d", params[0], params[1])) return SCM(playerid, -1, "/vehcolor [color1](0-255) [color2](0-255)");
	if(IsWorldModer(playerid) || IsWorldAdmin(playerid))
	{
	    if(IsPlayerInAnyVehicle(playerid) && ((params[0] >= 0 && params[0] <= 255) && (params[1] >= 0 && params[1] <= 255)))
	    {
	        new vehid = GetPlayerVehicleID(playerid);
			ChangeVehicleColor(vehid, params[0], params[1]);
	    }
	}
	return 1;
}

CMD:eject(playerid, params[])
{
	new vehid = GetPlayerVehicleID(playerid);
	if(vehid != 0 && (GetPlayerVehicleSeat(playerid) == 0))
	{
	    if(sscanf(params, "d", params[0])) return SCM(playerid, -1, "/eject [ID]");
	    if(playerid == params[0]) return SCM(playerid, -1, "Нельзя выкинуть из машины самого себя!");
		if(IsPlayerConnected(params[0]))
		{
		    if(vehid == GetPlayerVehicleID(params[0]))
		    {
				RemovePlayerFromVehicle(params[0]);
				SCM(playerid, -1, "Вы выкинули игрока из машины!");
				SCM(params[0], -1, "Вас выкинули из машины!");
		    }
		}
	}
	return 1;
}

CMD:lock(playerid)
{
	foreach(new i: Vehicle)
	{
	    new Float: X, Float: Y, Float: Z;
	    GetVehiclePos(i, X, Y, Z);
	    if(IsPlayerInRangeOfPoint(playerid, 3.0, X, Y, Z))
	    {
	        if(VehLock[i] == PlayerData[playerid][pID])
			{
			    if(GetLockVehicle(i))
			    {
			    	SetLockVehicle(i, false);
					GameTextForPlayer(playerid, "~g~UNLOCKED!", 1000, 3);
				}
				else
				{
				    SetLockVehicle(i, true);
				    GameTextForPlayer(playerid, "~r~LOCKED!", 1000, 3);
				}
			}
	    }
	}
	return 1;
}
