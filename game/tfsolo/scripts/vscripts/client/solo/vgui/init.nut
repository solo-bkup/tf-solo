printl("[TFSOLO] VGUI Init")

TFSOLO.PlayerData <- {
	TeamSelected = 0
}

TFSOLO.ConfigKV <- FileToKeyValues("cfg/solo/solo_config.txt")

TFSOLO.VguiEventTag <- UniqueString()
getroottable()[TFSOLO.VguiEventTag] <- {
	OnScriptHook_solopanel_command = function(params)
	{
	}
}
TFSOLO.VguiEventTable <- getroottable()[TFSOLO.VguiEventTag]
__CollectGameEventCallbacks(TFSOLO.VguiEventTable)

IncludeScript("client/solo/vgui/screens/screen.nut")