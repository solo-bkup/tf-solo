TFSOLO.Screens.MapSelect <- class extends TFSOLO.Screen
{
	Name = "MapSelect"
	constructor() {}
	
	function OnEnter()
	{
		
	}
}

TFSOLO.VguiMapSelectEventTag <- UniqueString()
getroottable()[TFSOLO.VguiMapSelectEventTag] <- {
	OnScriptHook_solopanel_command = function(params)
	{
		if (TFSOLO.Screens.Active != TFSOLO.Screens.MapSelect) return;
	}
}
TFSOLO.VguiMapSelectEventTable <- getroottable()[TFSOLO.VguiMapSelectEventTag]
__CollectGameEventCallbacks(TFSOLO.VguiMapSelectEventTable)
