class BLGadgetComponentClass : SCR_GadgetComponentClass
{}

class BLGadgetComponent : SCR_GadgetComponent
{
	override void UpdateVisibility(EGadgetMode mode)	// Stops the gadgetcomponent making rounds invisible during reloads
    {
		if(mode != EGadgetMode.IN_SLOT)
		{
			super.UpdateVisibility(mode);
		}
    }
}