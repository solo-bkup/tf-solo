TFSOLO.WorldMaps.TestClass <- class extends TFSOLO.WorldMap
{
	Name = "Test World Map"
	constructor() { 
		local Node1 = TFSOLO.WorldMapNode("Dustbowl","cp_dustbowl","",null)
		local Node2 = TFSOLO.WorldMapNode("Viaduct","koth_viaduct","",TFSOLO.Cutscenes.Test)
		local Node3 = TFSOLO.WorldMapNode("Upward","pl_upward","",null)
		
		Node1.PosX = "cs-0.5-150"
		Node1.Icon = "cyoa/cyoa_icon_spy"
		Node1.PlayerClass = "spy"
		
		Node2.Icon = "cyoa/cyoa_icon_scout"
		Node2.PlayerClass = "scout"
		Node2.Tooltip = "This one starts a cutscene."
		
		Node3.Icon = "cyoa/cyoa_icon_soldier"
		Node3.PosX = "cs-0.5+150"
		Node3.PlayerClass = "soldier"
		
		Nodes.push(Node1)
		Nodes.push(Node2)
		Nodes.push(Node3)
	}
}
TFSOLO.WorldMaps.Test <- TFSOLO.WorldMaps.TestClass()