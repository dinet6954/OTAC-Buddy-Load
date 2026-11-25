class BLBeltComponentClass : ScriptComponentClass
{}

class BLBeltComponent : ScriptComponent
{
	[Attribute("5", desc: "Caliber")]
	string m_iCaliber;
	
	string GetAmmoType()	// Used purely to verify ammo compatibility, make sure to set caliber as it is mentioned in ammo type, otherwise this wont work
	{
		return m_iCaliber;
	}
}