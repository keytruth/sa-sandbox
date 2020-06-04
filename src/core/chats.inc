static const Float: dist = 15.0;

public OnPlayerText(playerid, text[])
{
	new buf[128];
    if(text[0] == '!' && PlayerData[playerid][pLogged])
    {
        if(GetPVarInt(playerid, "LastMesTime") < gettime() - 2)
        {
	        SetPVarInt(playerid, "LastMesTime", gettime());
	        text[0] = ' ';
	        format(buf, sizeof buf, "%s(%d): {87d3fd}%s", GetName(playerid), playerid, text);
	        SendClientMessageToAll(-1, buf);
        }
    }
    else if(text[0] == '@' && PlayerData[playerid][pLogged])
    {
        text[0] = ' ';
        format(buf, sizeof buf, "%s(%d): {87dd84}%s", GetName(playerid), playerid, text);
        new id = GetPlayerVirtualWorld(playerid);
        foreach(new i: Player)
        {
			if(id == GetPlayerVirtualWorld(i)) SendClientMessage(i, -1, buf);
        }
    }
    else
    {
        if(PlayerData[playerid][pLogged])
        {
	        format(buf, sizeof buf, "%s сказал: %s", GetName(playerid), text);
	        new intid = GetPlayerInterior(playerid), worldid = GetPlayerVirtualWorld(playerid), Float: X, Float: Y, Float: Z;
	        GetPlayerPos(playerid, X, Y, Z);
	        new colorid = -1;
			foreach(new i: Player)
			{
				if(GetPlayerVirtualWorld(i) == worldid && GetPlayerInterior(i) == intid)
				{
					new Float: tmpdist = GetPlayerDistanceFromPoint(i, X, Y, Z);
					if(tmpdist <= dist)
					{
						if(tmpdist >= (dist / 2)) colorid = 0xc5c6c5FF;
						else if(GetPlayerDistanceFromPoint(i, X, Y, Z) <= (dist / 2)) colorid = -1;
						SendClientMessage(i, colorid, buf);
					}
				}
			}
		}
    }
    #if defined sandchats_OnPlayerText
    	return 0;
    #else
        return 1;
    #endif
}
#if defined _ALS_OnPlayerText
	#undef OnPlayerText
#else
	#define _ALS_OnPlayerText
#endif
#define OnPlayerText sandchats_OnPlayerText
#if defined sandchats_OnPlayerText
forward sandchats_OnPlayerText(playerid, text[]);
#endif

CMD:me(playerid, params[])
{
	if(sscanf(params, "s[128]", params[0])) return 0;
	new buf[128];
	format(buf, sizeof buf, "%s %s", GetName(playerid), params[0]);
	new intid = GetPlayerInterior(playerid), worldid = GetPlayerVirtualWorld(playerid), Float: X, Float: Y, Float: Z;
 	GetPlayerPos(playerid, X, Y, Z);
	foreach(new i: Player)
	{
		if(GetPlayerVirtualWorld(i) == worldid && GetPlayerInterior(i) == intid && GetPlayerDistanceFromPoint(i, X, Y, Z) <= dist) SendClientMessage(i, 0xDD90FFFF, buf);
	}
	return 1;
}

CMD:do(playerid, params[])
{
	if(sscanf(params, "s[128]", params[0])) return 0;
	new buf[128];
	format(buf, sizeof buf, "%s (%s)", params[0], GetName(playerid));
	new intid = GetPlayerInterior(playerid), worldid = GetPlayerVirtualWorld(playerid), Float: X, Float: Y, Float: Z;
 	GetPlayerPos(playerid, X, Y, Z);
	foreach(new i: Player)
	{
		if(GetPlayerVirtualWorld(i) == worldid && GetPlayerInterior(i) == intid && GetPlayerDistanceFromPoint(i, X, Y, Z) <= dist) SendClientMessage(i, 0xDD90FFFF, buf);
	}
	return 1;
}

CMD:pm(playerid, params[])
{
	if(sscanf(params, "ds[128]", params[0], params[1])) return 0;
	if(!IsPlayerConnected(params[0])) return 0;
	new buf[128];
	format(buf, sizeof buf, "PM от %s(%d): %s", GetName(playerid), playerid, params[1]);
	SendClientMessage(params[0], 0xFFFF00AA, buf);
	format(buf, sizeof buf, "PM для %s(%d): %s", GetName(params[0]), params[0], params[1]);
	SendClientMessage(playerid, 0xFFFF00AA, buf);
	return 1;
}
