printl("[TFSOLO] VGUI Init")

TFSOLO.PlayerData <- {
	TeamSelected = 0
}

TFSOLO.ConfigKV <- FileToKeyValues("cfg/solo/solo_config.txt")

IncludeScript("client/solo/vgui/animations.nut")
IncludeScript("client/solo/vgui/screens/screen.nut")
IncludeScript("client/solo/vgui/cutscenes/cutscene.nut")

TFSOLO.VguiEventTag <- UniqueString()
getroottable()[TFSOLO.VguiEventTag] <- {
	OnScriptHook_solopanel_open = function(params)
	{
		TFSOLO.PlayMenuOpenEffects()
	}
}
TFSOLO.VguiEventTable <- getroottable()[TFSOLO.VguiEventTag]
__CollectGameEventCallbacks(TFSOLO.VguiEventTable)