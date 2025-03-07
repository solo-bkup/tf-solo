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
ClearGameEventCallbacks()

IncludeScript("client/savedata.nut")

TFSOLO.CoreEventTag <- UniqueString()
getroottable()[TFSOLO.CoreEventTag] <- {
	OnGameEvent_player_death = function(params)
	{
	}
	
	OnGameEvent_localplayer_changeteam = function(params)
	{
	}
}
TFSOLO.CoreEventTable <- getroottable()[TFSOLO.CoreEventTag]
__CollectGameEventCallbacks(TFSOLO.CoreEventTable)
foreach (n,f in TFSOLO.CoreEventTable)
{
	TFSOLO.CoreEventTable[n] = f.bindenv(this)
}