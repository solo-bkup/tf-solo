TFSOLO.WorldMaps <- {}
TFSOLO.WorldMaps.Active <- null
TFSOLO.WorldMaps.ActiveNode <- null

TFSOLO.WorldMapNode <- class
{
	Name = "Base Node"
	Map = ""
	Icon = ""
	PosX = "cs-0.5"
	PosY = "cs-0.5"
	Cutscene = null
	Panel = null
	
	constructor() { }
	constructor(aname, amap, aicon) { 
		Name = aname
		Map = amap
		Icon = aicon
	}
	constructor(aname, amap, aicon, acts) { 
		Name = aname
		Map = amap
		Icon = aicon
		Cutscene = acts
	}
	function _tostring() return this.Name
}

TFSOLO.WorldMap <- class
{
	function Enter()
	{
		TFSOLO.WorldMaps.Active = this
		TFSOLO.Screens.MapSelect.Enter()
		OnEnter()
	}
	function OnEnter()
	{
		
	}
	
	Name = "Base World Map"
	Nodes = []
	
	
	constructor() { }
	function _tostring() return this.Name
}

IncludeScript("client/solo/vgui/worldmaps/test.nut")