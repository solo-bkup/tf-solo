TFSOLO.StartMission <- function()
{
	
	SendToConsole("disconnect;wait;wait;maxplayers 32;progress_enable;map " + TFSOLO.PlayerData.Map)
	SoloPanel.ForceClose()
}