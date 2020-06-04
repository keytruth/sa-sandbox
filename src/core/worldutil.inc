CMD:tp(playerid, params[])
{
	if(IsWorldModer(playerid) || IsWorldAdmin(playerid))
	{
	    if(sscanf(params, "d", params[0])) return SCM(playerid, -1, "/tp [ID]");
		if(!IsPlayerConnected(params[0])) return 1;
		if(GetPlayerVirtualWorld(playerid) == GetPlayerVirtualWorld(params[0]))
		{
			new Float: X, Float: Y, Float: Z, Float: A;
			GetPlayerPos(params[0], X, Y, Z);
			GetPlayerFacingAngle(params[0], A);
			SetPlayerPos(playerid, X, Y, Z);
			SetPlayerFacingAngle(playerid, A);
			SetPlayerInterior(playerid, GetPlayerInterior(params[0]));
		}
	}
	return 1;
}

CMD:bring(playerid, params[])
{
	if(IsWorldModer(playerid) || IsWorldAdmin(playerid))
	{
	    if(sscanf(params, "d", params[0])) return SCM(playerid, -1, "/bring [ID]");
		if(!IsPlayerConnected(params[0])) return 1;
		if(GetPlayerVirtualWorld(playerid) == GetPlayerVirtualWorld(params[0]))
		{
			new Float: X, Float: Y, Float: Z, Float: A;
			GetPlayerPos(playerid, X, Y, Z);
			GetPlayerFacingAngle(playerid, A);
			SetPlayerPos(params[0], X, Y, Z);
			SetPlayerFacingAngle(params[0], A);
			SetPlayerInterior(params[0], GetPlayerInterior(playerid));
		}
	}
	return 1;
}

CMD:sethp(playerid, params[])
{
	if(IsWorldModer(playerid) || IsWorldAdmin(playerid))
	{
		if(sscanf(params, "dd", params[0], params[1])) return SCM(playerid, -1, "/sethp [ID] [Health]");
		if(!IsPlayerConnected(params[0])) return 1;
		if(GetPlayerVirtualWorld(playerid) == GetPlayerVirtualWorld(params[0]))
		{
		    new Float: hp = float(params[1]);
		    if(hp >= 0.0 && hp <= 100.0) SetPlayerHealth(params[0], hp);
		}
	}
	return 1;
}

CMD:setarm(playerid, params[])
{
	if(IsWorldModer(playerid) || IsWorldAdmin(playerid))
	{
		if(sscanf(params, "dd", params[0], params[1])) return SCM(playerid, -1, "/setarm [ID] [Armour]");
		if(!IsPlayerConnected(params[0])) return 1;
		if(GetPlayerVirtualWorld(playerid) == GetPlayerVirtualWorld(params[0]))
		{
		    new Float: arm = float(params[1]);
		    if(arm >= 0.0 && arm <= 100.0) SetPlayerArmour(params[0], arm);
		}
	}
	return 1;
}

CMD:spawn(playerid)
{
	if(GetPlayerVirtualWorld(playerid) == 0)
	{
	    new tmpskin = GetPlayerSkin(playerid);
	    SetNoneSpawn(playerid);
		SpawnPlayer(playerid);
		SetPlayerSkin(playerid, tmpskin);
	}
	else if(GetPlayerVirtualWorld(playerid) >= 10)
	{
	    new customwd = (GetPlayerVirtualWorld(playerid) - 10);
	    SetPlayerPos(playerid, WorldData[customwd][spawnX], WorldData[customwd][spawnY], WorldData[customwd][spawnZ]);
		SetPlayerFacingAngle(playerid, WorldData[customwd][spawnA]);
	}
	return 1;
}

CMD:money(playerid, params[])
{
	if(IsWorldAdmin(playerid))
	{
		if(sscanf(params, "d", params[0])) return SCM(playerid, -1, "/money [Деньги]");
		GivePlayerMoney(playerid, params[0]);
	}
	return 1;
}

CMD:pay(playerid, params[])
{
	if(sscanf(params, "dd", params[0], params[1])) return SCM(playerid, -1, "/pay [ID] [Money]");
	if(playerid == params[0]) return SCM(playerid, -1, "Нельзя передать деньги самому себе!");
	if(params[1] > 0)
	{
	    if(params[1] <= GetPlayerMoney(playerid))
	    {
			if(IsPlayerConnected(params[0]))
			{
				if((GetPlayerInterior(playerid) == GetPlayerInterior(params[0])) && (GetPlayerVirtualWorld(playerid) == GetPlayerVirtualWorld(params[0])))
				{
				    new Float: X, Float: Y, Float: Z;
					GetPlayerPos(params[0], X, Y, Z);
					if(IsPlayerInRangeOfPoint(playerid, 5.0, X, Y, Z))
					{
					    new buf[128];
					    format(buf, sizeof buf, "Вы передали $%d игроку %s(%d).", params[1], GetName(params[0]), params[0]);
					    SCM(playerid, -1, buf);
					    format(buf, sizeof buf, "%s(%d) передал Вам $%d.", GetName(params[0]), params[0], params[1]);
					    SCM(params[0], -1, buf);
						GivePlayerMoney(playerid, -params[1]);
						GivePlayerMoney(params[0], params[1]);
					}
			    }
			}
		}
	}
	return 1;
}
