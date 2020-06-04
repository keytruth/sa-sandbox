CMD:skin(playerid, params[])
{
	if(sscanf(params, "d", params[0]))
	{
	    if(!PagenatorShowed[playerid])
	    {
	        ShowPagenatorForPlayer(playerid, 1);
	    }
	}
	else
	{
		if(!(params[0] >= 0 && params[0] <= 311)) return 1;
		if(params[0] == 74) return 1;
		SetPlayerSkin(playerid, params[0]);
	}
	return 1;
}

CMD:gun(playerid, params[])
{
	if(IsWorldModer(playerid) || IsWorldAdmin(playerid))
	{
		if(sscanf(params, "dd", params[0], params[1]))
		{
		    if(!PagenatorShowed[playerid])
		    {
		        ShowPagenatorForPlayer(playerid, 0);
		    }
		}
		else
		{
			if(params[0] >= 1 && params[1] <= 46)
			{
			    if(!(params[1] >= 19 && params[1] <= 21))
			    {
			        GivePlayerWeapon(playerid, params[0], params[1]);
			    }
			}
		}
	}
	return 1;
}

CMD:givegun(playerid, params[])
{
	if(sscanf(params, "ddd", params[0], params[1], params[2])) return SCM(playerid, -1, "/givegun [ID] [Gun ID] [Ammo]");
	if(IsWorldModer(playerid) || IsWorldAdmin(playerid))
	{
	    if(IsPlayerConnected(params[0]) && (GetPlayerVirtualWorld(params[0]) == GetPlayerVirtualWorld(playerid)))
	    {
	        if(params[1] >= 1 && params[1] <= 46)
	        {
				if(!(params[1] >= 19 && params[1] <= 21))
				{
					GivePlayerWeapon(params[0], params[1], params[2]);
				}
			}
		}
	}
	return 1;
}

CMD:rm(playerid)
{
	ResetPlayerMoney(playerid);
	return 1;
}

CMD:rw(playerid)
{
	ResetPlayerWeapons(playerid);
	return 1;
}
