TFSOLO.Screens <- {}
TFSOLO.Screens.Active <- null

TFSOLO.Screens.EventTag <- UniqueString()
getroottable()[TFSOLO.Screens.EventTag] <- {
	OnScriptHook_solopanel_command = function(params)
	{
		if (params.command == "solopanel_opened")
		{
			if (TFSOLO.Screens.Active == null)
			{
				printl("Spawning VGUI")
				TFSOLO.Screens.Test.Enter()
			}
		}
	}
}
TFSOLO.Screens.EventTable <- getroottable()[TFSOLO.Screens.EventTag]
__CollectGameEventCallbacks(TFSOLO.Screens.EventTable)

TFSOLO.Screen <- class
{
	Name = "BaseScreen"
	
	constructor()
    { 

    }
	
	function Reset()
	{
		
	}
	function Enter()
	{
		SoloPanel.ClearAllScriptPanels()
		TFSOLO.Screens.Active = this
		OnEnter()
	}
	function OnEnter()
	{
		
	}
	function Exit()
	{
		OnExit()
	}
	function OnExit()
	{
	}
}

IncludeScript("client/solo/vgui/screens/test.nut")
IncludeScript("client/solo/vgui/screens/teamselect.nut")

