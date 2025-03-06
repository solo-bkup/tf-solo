printl("[TFSOLO] Game Init")

function IncludeScript( name, scope = null )
{
	if ( scope == null )
	{
		scope = this;
	}
	return ::DoIncludeScript( name, scope );
}

IncludeScript("client/util.nut")