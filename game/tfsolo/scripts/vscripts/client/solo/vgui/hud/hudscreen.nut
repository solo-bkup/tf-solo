TFSOLO.HudScreen <- class
{
	Name = "BaseHudScreen"
	
	constructor()
    { 

    }
	
	function Enter()
	{
		TFSOLO.HudScreens.Active = this
		OnEnter()
	}
	function OnEnter()
	{
		
	}
	function OnEvent(key, value)
	{
		
	}
	function OnThink()
	{
		
	}
	function _tostring() return this.Name
}

IncludeScript("client/solo/vgui/hud/test.nut")