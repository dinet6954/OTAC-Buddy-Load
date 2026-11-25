class BuddyLoadMG : ScriptedUserAction
{	
	override bool CanBeShownScript(IEntity user)
	{
		return CanBePerformedScript(user);
	}

	override bool CanBePerformedScript(IEntity user)	
	{
		ChimeraCharacter gunner = ChimeraCharacter.Cast(GetOwner());
		if (!gunner)
			return false;

		ChimeraCharacter loader = ChimeraCharacter.Cast(user);
		if (!loader)
			return false;
		
		// ^ Fetches character for both parties

		CharacterControllerComponent loaderController = loader.GetCharacterController();
		if (!loaderController || loaderController.GetLifeState() != ECharacterLifeState.ALIVE)
			return false;

		CharacterControllerComponent gunnerController = gunner.GetCharacterController();
		if (!gunnerController || gunnerController.GetLifeState() != ECharacterLifeState.ALIVE)
			return false;
		
		// ^ Fetches character controllers
		
		BaseWeaponManagerComponent wpnComp = BaseWeaponManagerComponent.Cast(gunner.FindComponent(BaseWeaponManagerComponent));
		if (!wpnComp)
			return false;
		
		// ^ Fetches weaponmanagercomponent
		
		BaseWeaponComponent bWpnComp = wpnComp.GetCurrentWeapon();
		if (!bWpnComp)
			return false;
		
		// ^ Fetches weapon
		
		if (bWpnComp.GetWeaponType() != EWeaponType.WT_MACHINEGUN)
    	return false;
		
		// ^ Checks if weapon is an mg
		
		BaseMuzzleComponent muzzle = bWpnComp.GetCurrentMuzzle();
		if (!muzzle)
			return false;
			
		// ^ Checks if muzzle is present (Needed to compare ammo type and round compatibility)
		
		if (muzzle.GetAmmoCount() >= muzzle.GetMaxAmmoCount())
			return false;
		
		// ^ Checks if gun is at max ammo count
		
		BaseMagazineComponent mgBox = muzzle.GetMagazine();
		if(mgBox.GetAmmoCount() == 0)
			return false;
		
		BuddyLoadCompat BLComp = BuddyLoadCompat.Cast(bWpnComp.GetOwner().FindComponent(BuddyLoadCompat));
		if (!BLComp)
			return false;
		
		// ^ Checks for BuddyLoadCompat in order to ensure gun is buddy load capable
		
		if (!bWpnComp.IsReloadPossible())
			return false;
		
		// ^ Redundant check just in case reload is somehow impossible even without max ammo
		
		IEntity ammo = GetHeldAmmo(loaderController);
		if (!ammo)
			return false;
		
		// ^ Check if ammo is held in loader's hand
		
		if (!IsAmmoCompatible(ammo, muzzle))
			return false;
		
		// ^ Check if held ammo is compatible
		
		return true;
	}
	
	override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity)
	{
		ChimeraCharacter gunner = ChimeraCharacter.Cast(pOwnerEntity);
		ChimeraCharacter loader = ChimeraCharacter.Cast(pUserEntity);

		if (!gunner || !loader)
			return;
		
		// Gets gunner and loader, and ends function if either party is null for some reason

		CharacterControllerComponent gunnerController = gunner.GetCharacterController();
		CharacterControllerComponent loaderController = loader.GetCharacterController();

		if (!gunnerController || !loaderController)
			return;
		
		// Same as above but for character controllers

		IEntity ammo = GetHeldAmmo(loaderController);	// Gets held belt for comparison
		BaseWeaponManagerComponent wpnComp = BaseWeaponManagerComponent.Cast(gunner.FindComponent(BaseWeaponManagerComponent));
		BaseWeaponComponent bWpnComp = wpnComp.GetCurrentWeapon();
		BaseMuzzleComponent muzzle = bWpnComp.GetCurrentMuzzle();	// Gets muzzle for comparison
		if (!IsAmmoCompatible(ammo, muzzle))	//Comparison
			return;
		
		BaseMagazineComponent mgBox = muzzle.GetMagazine();	//Gets MG box
		MagazineComponent ammoMag = MagazineComponent.Cast(ammo.FindComponent(MagazineComponent));
		BaseMagazineComponent loadBelt = BaseMagazineComponent.Cast(ammoMag);
		
		int gunAmmo = mgBox.GetAmmoCount();
		int maxAmmo = mgBox.GetMaxAmmoCount();
		int beltAmmo = loadBelt.GetAmmoCount();
		
		int reloadedAmmo = gunAmmo + beltAmmo;	// Calculates total reloaded amount
		if (reloadedAmmo > maxAmmo)	// Calculations if loader tries to load a box with more rounds than the gun can take
		{
			int remainder = reloadedAmmo - maxAmmo;
			
			mgBox.SetAmmoCount(maxAmmo);	// Sets gun ammo to full
			loadBelt.SetAmmoCount(remainder); //Sets belt to remainder
			Print("Remainder: " + remainder);
		}
		else
		{
			mgBox.SetAmmoCount(reloadedAmmo);	
			Print("Reloaded ammo: " + reloadedAmmo);
			SCR_EntityHelper.DeleteEntityAndChildren(ammo); 	//Removes belt from loader
		}
	}

	protected IEntity GetHeldAmmo(CharacterControllerComponent controller)	//Gets held ammo
	{
		IEntity item = controller.GetRightHandItem();
		if (item)
			return item;

		return controller.GetAttachedGadgetAtLeftHandSlot();
	}

	bool IsAmmoCompatible(IEntity ammo, BaseMuzzleComponent muzzle)	//Comparison using ammo type and comparing to set caliber in BLBeltComponent
	{
		if (!ammo || !muzzle)
			return false;
			
		BuddyLoadCompat BLComp = BuddyLoadCompat.Cast(ammo.FindComponent(BuddyLoadCompat));
		if (!BLComp)
			return false;
		
		BaseMagazineComponent gunMag = muzzle.GetMagazine();
		string typeName = gunMag.GetAmmoType();
		
		BLBeltComponent belt = BLBeltComponent.Cast(ammo.FindComponent(BLBeltComponent));
		string ammoType = belt.GetAmmoType();
			
		if(typeName.Contains(ammoType))
			return true;
		else
			return false;
	}
	
	override bool GetActionNameScript(out string outName)
	{
		ChimeraCharacter gunner = ChimeraCharacter.Cast(GetOwner());
		BaseWeaponManagerComponent wpnComp = BaseWeaponManagerComponent.Cast(gunner.FindComponent(BaseWeaponManagerComponent));
		BaseWeaponComponent bWpnComp = wpnComp.GetCurrentWeapon();
		BaseMuzzleComponent muzzle = bWpnComp.GetCurrentMuzzle();
		
		
		int ammo = muzzle.GetAmmoCount();
		int maxAmmo = muzzle.GetMaxAmmoCount();
		
		outName = "Link Belt (" + ammo + "/" + maxAmmo + ")";
		return true;
	}
}