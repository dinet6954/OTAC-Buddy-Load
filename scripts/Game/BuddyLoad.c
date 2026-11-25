class BuddyLoad : ScriptedUserAction
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
		
		if (bWpnComp.GetWeaponType() != EWeaponType.WT_ROCKETLAUNCHER)
    	return false;
		
		// ^ Checks if weapon is a rocket launcher
		
		BaseMuzzleComponent muzzle = bWpnComp.GetCurrentMuzzle();
		if (!muzzle)
			return false;
			
		// ^ Checks if muzzle is present (Needed to compare ammo type and round compatibility)
		
		if (muzzle.IsCurrentBarrelChambered() || muzzle.GetAmmoCount() >= muzzle.GetMaxAmmoCount())
			return false;
		
		// ^ Checks if round is already chambered
		
		BuddyLoadCompat BLComp = BuddyLoadCompat.Cast(bWpnComp.GetOwner().FindComponent(BuddyLoadCompat));
		if (!BLComp)
			return false;
		
		// ^ Checks for BuddyLoadCompat in order to ensure launcher is buddy load capable
		
		if (!bWpnComp.IsReloadPossible())
			return false;
		
		// ^ Redundant check just in case reload is somehow impossible even without a round chambered
		
		IEntity ammo = GetHeldAmmo(loaderController);
		if (!ammo)
			return false;
		
		// ^ Check if ammo is held in loader's hand
		
		if (!IsAmmoCompatible(ammo, muzzle))
			return false;
		
		// ^ Check if held ammo is compatible
		
		return true;
	}
	
	override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity)	//Performs the action
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

		IEntity ammo = GetHeldAmmo(loaderController);	// Gets held round for comparison
		BaseWeaponManagerComponent wpnComp = BaseWeaponManagerComponent.Cast(gunner.FindComponent(BaseWeaponManagerComponent));
		BaseWeaponComponent bWpnComp = wpnComp.GetCurrentWeapon();
		BaseMuzzleComponent muzzle = bWpnComp.GetCurrentMuzzle();	// Gets muzzle for comparison
		if (!IsAmmoCompatible(ammo, muzzle))	//Comparison
			return;

		//gunnerController.ReloadWeaponWith(ammo);	//Puts round from loader's hand into gunner's gun, Disabled till I find a better approach
		
		BaseMagazineComponent magazine = muzzle.GetMagazine();	//Gets launcher magazine
		magazine.SetAmmoCount(1);	//Adds 1 round to ammo count
		SCR_EntityHelper.DeleteEntityAndChildren(ammo); 	//Removes round from loader
	}

	protected IEntity GetHeldAmmo(CharacterControllerComponent controller)	//Gets held ammo
	{
		IEntity item = controller.GetRightHandItem();
		if (item)
			return item;

		return controller.GetAttachedGadgetAtLeftHandSlot();
	}

	bool IsAmmoCompatible(IEntity ammo, BaseMuzzleComponent muzzle)	//Comparison using MagazineWell, checks if round falls into the same mag well as the launcher
	{
		if (!ammo || !muzzle)
			return false;

		MagazineComponent magazine = MagazineComponent.Cast(ammo.FindComponent(MagazineComponent));
		if (!magazine)
			return false;
		
		BuddyLoadCompat blCompat = BuddyLoadCompat.Cast(ammo.FindComponent(BuddyLoadCompat));
		if (!blCompat)
			return false;

		if (magazine.GetAmmoCount() <= 0)
			return false;

		BaseMagazineWell ammoWell = magazine.GetMagazineWell();
		BaseMagazineWell muzzleWell = muzzle.GetMagazineWell();

		if (!ammoWell || !muzzleWell)
			return false;

		return ammoWell.Type() == muzzleWell.Type();
	}
}