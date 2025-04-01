TFSOLO.Hud <- {}
TFSOLO.Hud.IsActive <- 0
TFSOLO.Hud.ActiveFile <- ""
TFSOLO.Hud.IsSetup <- function()
{
	return ( (TFSOLO.Hud.IsActive != 0) || ("SoloHUD" in getroottable()) )
}

TFSOLO.Hud.OnThink <- function()
{
	
}

TFSOLO.Hud.OnEvent <- function(key, value)
{
	
}

TFSOLO.Hud.EventTag <- UniqueString()
getroottable()[TFSOLO.Hud.EventTag] <- {
	OnScriptHook_solohud_init = function(params)
	{
		TFSOLO.Hud.IsActive = 1
		TFSOLO.Hud.ActiveFile <- SoloHUD.GetResFile()
	}
	
	OnScriptHook_LevelShutdownPostEntity = function(params)
	{
		// Clean up panel on map exit
		if (!TFSOLO.Hud.IsSetup()) return;
		TFSOLO.Hud.IsActive = 0
		SoloHUD.ResetResFile()
		SoloHUD.ReinitializeEverything()
	}
	
	OnGameEvent_solohud_file_changed = function(params)
	{
		if (!TFSOLO.Hud.IsSetup()) return;
		TFSOLO.Hud.ActiveFile <- SoloHUD.GetResFile()
	}
	
	OnGameEvent_solohud_int = function(params)
	{
		if (!TFSOLO.Hud.IsSetup()) return;
		SoloHUD.SetDialogVariableInt(params.key, params.value)
	}
	
	OnGameEvent_solohud_float = function(params)
	{
		if (!TFSOLO.Hud.IsSetup()) return;
		SoloHUD.SetDialogVariableFloat(params.key, params.value)
	}
	
	OnGameEvent_solohud_string = function(params)
	{
		if (!TFSOLO.Hud.IsSetup()) return;
		SoloHUD.SetDialogVariable(params.key, params.value)
	}
	
	OnGameEvent_solohud_event = function(params)
	{
		if (!TFSOLO.Hud.IsSetup()) return;
		TFSOLO.Hud.OnEvent(params.key, params.value)
	}
	
	OnGameEvent_solohud_think = function(params)
	{
		TFSOLO.Hud.OnThink()
	}
}
TFSOLO.Hud.EventTable <- getroottable()[TFSOLO.Hud.EventTag]
__CollectGameEventCallbacks(TFSOLO.Hud.EventTable)
