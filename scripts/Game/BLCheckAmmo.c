class BLCheckAmmo : ScriptedUserAction
{
	BLGadgetComponent gadget;
	MagazineComponent mag;
	bool checkingAmmo;
	
	override void Init(IEntity pOwnerEntity, GenericComponent pManagerComponent)
	{
		gadget = BLGadgetComponent.Cast(pOwnerEntity.FindComponent(BLGadgetComponent));
	}
	
	override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity)
	{
		checkingAmmo = true;
		mag = MagazineComponent.Cast(pOwnerEntity.FindComponent(MagazineComponent));
		GetGame().GetCallqueue().CallLater(ResetName, 2000, false);
	}
	
	override bool CanBePerformedScript(IEntity user)
	{	
		if(gadget.GetMode() == EGadgetMode.ON_GROUND)
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	
	override bool CanBeShownScript(IEntity user)
	{
		return CanBePerformedScript(user);
	}
	
	void ResetName()
	{
		checkingAmmo = false;
	}
	
	override bool GetActionNameScript(out string outName)
	{
		if(checkingAmmo)
		{
			int ammo = mag.GetAmmoCount();
			int maxAmmo = mag.GetMaxAmmoCount();
			
			outName = "Check Ammo (" + ammo + "/" + maxAmmo + ")";
		}
		else
		{
			outName = "Check Ammo";
		}		
		return true;
	}
}