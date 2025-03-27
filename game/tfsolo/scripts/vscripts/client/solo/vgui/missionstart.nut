TFSOLO.StartMission <- function()
{
	
	if (TFSOLO.PlayerData.TeamSelected == 1)
	{
		SendToConsole("mp_humans_must_join_team blue")
	}
	else
	{
		SendToConsole("mp_humans_must_join_team red")
	}
	if (TFSOLO.PlayerData.PlayerClass == "any")
	{
		SendToConsole("mp_humans_must_join_class any")
	}
	else
	{
		SendToConsole("mp_humans_must_join_class " + TFSOLO.PlayerData.PlayerClass)
	}
	SendToConsole("disconnect;wait;wait;maxplayers 32;progress_enable;map " + TFSOLO.PlayerData.Map)
	
	SoloPanel.ForceClose()
}