TFSOLO.InitSaveData <- function()
{
	local kv = Solo.GetSaveData()
	kv.SetInt("Credits",10000)
	local itemsKV = kv.FindKey("UnlockedItems", true)
	local armoryKV = kv.FindKey("Armory", true)
	local botpresetsKV = kv.FindKey("BotPresets", true)
	local campaignsKV = kv.FindKey("Campaigns", true)
	local mapsKV = kv.FindKey("Maps", true)
	local genericKV = kv.FindKey("Generic", true)
}

TFSOLO.InitSaveData()