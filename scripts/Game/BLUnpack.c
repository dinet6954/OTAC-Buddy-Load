class BLUnpack : ScriptedUserAction
{
	int ammoType = 0;	//Used to change spawned belt based on the owner object
	
	override void Init(IEntity pOwnerEntity, GenericComponent pManagerComponent)	// Checks the ammo type of the owner box
	{
		MagazineComponent mag = MagazineComponent.Cast(pOwnerEntity.FindComponent(MagazineComponent));
		string typeData = mag.GetAmmoType();
		
		if(typeData.Contains("556x45"))
		{
			ammoType = 1;
		}
		else if(typeData.Contains("762x51"))
		{
			ammoType = 2;
		}
		else if(typeData.Contains("762x54r"))
		{
			ammoType = 3;
		}
	}
	
	override bool CanBeShownScript(IEntity user)	//Runs checks to see if action can be done, and then shows the option if it can
	{
		return CanBePerformedScript(user);
	}
	
	override bool CanBePerformedScript(IEntity user)
	{
		if (!super.CanBePerformedScript(user))
			return false;

		auto pInventoryManager = SCR_InventoryStorageManagerComponent.Cast(user.FindComponent(SCR_InventoryStorageManagerComponent));
		if (!pInventoryManager)
			return false;

		pInventoryManager.SetReturnCodeDefault();

		if (!pInventoryManager.IsAnimationReady())
		{
			return false;
		}

		return true;
	}
	
	override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity)
	{
		switch(ammoType)	// I'm using a switch case to account for different ammo boxes
		{
			case 1:
				Resource prefabResource = Resource.Load("{281C6DAEB20B0A68}Prefabs/Items/Equipment/Ammo Belts/OTAC_AmmoBelt_556x45_50.et");
				Resource prefab2Resource = Resource.Load("{2AA429F0BB987048}Prefabs/Items/Equipment/Ammo Belts/OTAC_AmmoBelt_556x45_Half.et");
				Unpack(pOwnerEntity, prefabResource, prefab2Resource);	//	Moved to its own function for readability's sake
				break;
			case 2:
				Resource prefabResource = Resource.Load("{C2B17649DE8535F8}Prefabs/Items/Equipment/Ammo Belts/OTAC_AmmoBelt_762x51_50.et");
				Resource prefab2Resource = Resource.Load("{07707E45437C25FB}Prefabs/Items/Equipment/Ammo Belts/OTAC_AmmoBelt_762x51_Half.et");
    			Unpack(pOwnerEntity, prefabResource, prefab2Resource);	//	Moved to its own function for readability's sake
				break;
			case 3:
				Resource prefabResource = Resource.Load("{5AFD80D7814AAD04}Prefabs/Items/Equipment/Ammo Belts/OTAC_AmmoBelt_762x54r_50.et");
				Resource prefab2Resource = Resource.Load("{F8E0D9FC6BF7D2F1}Prefabs/Items/Equipment/Ammo Belts/OTAC_AmmoBelt_762x54r_Half.et");
    			Unpack(pOwnerEntity, prefabResource, prefab2Resource);	//	Moved to its own function for readability's sake
				break;
			default:
				return;
		}
		Print("Action Performed!");
	}
	
	void Unpack(IEntity owner, Resource prefab, Resource prefab2)	// Function that spawns the appropriate amount of belts according to the ammo in the box
	{
		vector pos = owner.GetOrigin();	// This block is used to get the box's position so that the belts can be spawned where it was
		EntitySpawnParams params();
		params.TransformMode = ETransformMode.WORLD;
		params.Transform[3] = pos;
		
		MagazineComponent mag = MagazineComponent.Cast(owner.FindComponent(MagazineComponent));	// Gets mag to check ammo
		int ammo = mag.GetAmmoCount();
		
		if(ammo > 150 && ammo <= 200)	// Spawns 4 belts
		{
			for(int i = 0; i < 4; i++)
			{
				if(i != 3)	// Spawns 3 belts with full ammo
				{
					IEntity newEntity = GetGame().SpawnEntityPrefab(prefab, GetGame().GetWorld(), params);
				}
				else	//	If ammo wasn't full, it will spawn the last belt with the remainder ammo (done to prevent ammo duping)
				{
					if(ammo == 200)
					{
						IEntity newEntity = GetGame().SpawnEntityPrefab(prefab, GetGame().GetWorld(), params);
					}
					else
					{
						IEntity newEntity = GetGame().SpawnEntityPrefab(prefab2, GetGame().GetWorld(), params);
						
						MagazineComponent newMag = MagazineComponent.Cast(newEntity.FindComponent(MagazineComponent));
						int remainder = ammo - 150;
						newMag.SetAmmoCount(remainder);
					}
				}
			}
		}
		
		if(ammo > 100 && ammo <= 150)	// Spawns 3 belts
		{
			for(int i = 0; i < 3; i++)
			{
				if(i != 2)	
				{
					IEntity newEntity = GetGame().SpawnEntityPrefab(prefab, GetGame().GetWorld(), params);
				}
				else
				{
					if(ammo == 150)
					{
						IEntity newEntity = GetGame().SpawnEntityPrefab(prefab, GetGame().GetWorld(), params);
					}
					else
					{
						IEntity newEntity = GetGame().SpawnEntityPrefab(prefab2, GetGame().GetWorld(), params);
						
						MagazineComponent newMag = MagazineComponent.Cast(newEntity.FindComponent(MagazineComponent));
						int remainder = ammo - 100;
						newMag.SetAmmoCount(remainder);
					}
				}
			}
		}
		
		if(ammo > 50 && ammo <= 100)	// Spawns 2 belts
		{
			for(int i = 0; i < 2; i++)
			{
				if(i != 1)	
				{
					IEntity newEntity = GetGame().SpawnEntityPrefab(prefab, GetGame().GetWorld(), params);
				}
				else
				{
					if(ammo == 100)
					{
						IEntity newEntity = GetGame().SpawnEntityPrefab(prefab, GetGame().GetWorld(), params);
					}
					else
					{
						IEntity newEntity = GetGame().SpawnEntityPrefab(prefab2, GetGame().GetWorld(), params);
						
						MagazineComponent newMag = MagazineComponent.Cast(newEntity.FindComponent(MagazineComponent));
						int remainder = ammo - 50;
						newMag.SetAmmoCount(remainder);
					}
				}
			}
		}
		
		if(ammo > 0 && ammo <= 50)	// Spawns 1 belt
		{
			if(ammo == 50)
			{
				IEntity newEntity = GetGame().SpawnEntityPrefab(prefab, GetGame().GetWorld(), params);
			}
			else
			{
				IEntity newEntity = GetGame().SpawnEntityPrefab(prefab2, GetGame().GetWorld(), params);
				MagazineComponent newMag = MagazineComponent.Cast(newEntity.FindComponent(MagazineComponent));
						
				int remainder = ammo;
				newMag.SetAmmoCount(remainder);
				newEntity.SetName("" + remainder);
			}
		}
		
		SCR_EntityHelper.DeleteEntityAndChildren(owner);	// Deletes box after unpacking is complete
	}
}