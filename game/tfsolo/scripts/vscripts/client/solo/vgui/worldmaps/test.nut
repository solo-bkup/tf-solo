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
	
	function OnEnter()
	{
		local kvRegionLink = {
			ControlName		="EditablePanel"
			fieldName		="Link"
			xpos			="cs-0.5-50"
			ypos			="cs-0.5+100"
			ControlSettings ="Resource/UI/quests/cyoa/QuestMapRegionLink.res"
		}
		local LinkPanel = SoloPanel.CreatePanelRoot(kvRegionLink)
		LinkPanel.SetDialogVariable("link_region_name", "Test Region Link")
		LinkPanel.SetDialogVariable("completed", "Completed: None so far")
		LinkPanel.SetDialogVariable("available", "Available: Also none")
		LinkPanel.SetControlVisible("ActiveLabel", false, true)
		local LinkButton = SoloPanel.FindPanel(LinkPanel, "LinkRegionNameButton")
		SoloPanel.AddActionSignalTargetForPanel(LinkButton)
		LinkButton.SetCommand("close")
		local nTall = LinkButton.GetTall()
		LinkButton.SizeToContents()
		LinkButton.SetTall(nTall)
		
		SoloPanel.SetDrawGrid(true)
		local PosX = Nodes[1].Panel.GetXPos()
		local PosY = Nodes[1].Panel.GetYPos()
		local SizeX = Nodes[1].Panel.GetWide()
		local SizeY = Nodes[1].Panel.GetTall()
		PosX += SizeX / 2.0
		PosY += (SizeY / 2.0) - 46
		SoloPanel.SetActiveCirclePos(PosX, PosY)
	}
}
TFSOLO.WorldMaps.Test <- TFSOLO.WorldMaps.TestClass()