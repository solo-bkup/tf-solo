TFSOLO.Cutscenes.Test <- class extends TFSOLO.Cutscene
{
	function CutsceneRun()
	{
		local cs = TFSOLO.Screens.Cutscene
		cs.ActorPanel1.SetVisible(false)
		cs.ActorPanel2.SetVisible(false)
		cs.DialogLabel.SetTextConst("dialogue line 1")
		::suspend()
		cs.ActorPanel1.SetVisible(true)
		cs.DialogLabel.SetTextConst("dialogue line 2")
		::suspend()
		cs.ActorPanel2.SetVisible(true)
		cs.DialogLabel.SetTextConst("dialogue line 3")
		::suspend()
		TFSOLO.Cutscenes.Test.Exit()
		return
	}
	function OnExit()
	{
		TFSOLO.Cutscenes.Active = null
		TFSOLO.PlayTransitionScreenEffects()
		TFSOLO.Screens.Test.Enter()
	}
	
	Name = "TestCutscene"
	constructor() { }
}