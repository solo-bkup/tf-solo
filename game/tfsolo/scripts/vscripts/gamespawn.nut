printl("[TFSOLO] Game Init")
TFSOLO <- {}

function IncludeScript( name, scope = null )
{
	if ( scope == null )
	{
		scope = this;
	}
	return ::DoIncludeScript( name, scope );
}

IncludeScript("client/util.nut")
ClearGameEventCallbacks();

IncludeScript("client/savedata.nut")

local TFSOLO_EventTag = UniqueString()
getroottable()[TFSOLO_EventTag] <- {
	OnGameEvent_player_death = function(params)
	{
	}
	
	OnGameEvent_localplayer_changeteam = function(params)
	{
	}
}
local TFSOLO_EventTable = getroottable()[TFSOLO_EventTag]
__CollectGameEventCallbacks(TFSOLO_EventTable)
foreach (n,f in TFSOLO_EventTable)
{
	TFSOLO_EventTable[n] = f.bindenv(this)
}