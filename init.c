void main()
{
	//INIT ECONOMY--------------------------------------
	Hive ce = CreateHive();
	if ( ce )
		ce.InitOffline();

	//DATE RESET AFTER ECONOMY INIT-------------------------
	int year, month, day, hour, minute;
	int reset_month = 3, reset_day = 1;
	GetGame().GetWorld().GetDate(year, month, day, hour, minute);

	if ((month == reset_month) && (day < reset_day))
	{
		GetGame().GetWorld().SetDate(year, reset_month, reset_day, hour, minute);
	}
	else
	{
		if ((month == reset_month + 1) && (day > reset_day))
		{
			GetGame().GetWorld().SetDate(year, reset_month, reset_day, hour, minute);
		}
		else
		{
			if ((month < reset_month) || (month > reset_month + 1))
			{
				GetGame().GetWorld().SetDate(year, reset_month, reset_day, hour, minute);
			}
		}
	}
}

class CustomMission: MissionServer
{
	//! Quest Events Medic Investigate Hospital
#ifdef EXPANSIONMODQUESTS
    override void Expansion_OnQuestStart(ExpansionQuest quest)
    {
        switch (quest.GetQuestConfig().GetID())
        {
            //! Quest 131 - Survivors - Field Medic Investigate Hospital
            case 131:
            {
                if (!ExpansionQuestModule.GetModuleInstance().TempQuestHolderExists(4000))
                    SpawnQuestHolder_4000();
            }
            break;
            //! Quest 402 - Survivors - Field Medic
			      case 402:
			      {
			      	if (!ExpansionQuestModule.GetModuleInstance().IsOtherQuestInstanceActive(8))
			      		ExpansionQuestModule.GetModuleInstance().DeleteQuestHolder(4000, ExpansionQuestNPCType.AI);
			      }
			      break;
            //! Quest 132 - Good Person, Investigate School
            case 132:
            {
                if (!ExpansionQuestModule.GetModuleInstance().TempQuestHolderExists(4001))
                    SpawnQuestHolder_4001();
            }
            break;
            //! Quest 403 - Survivors - Good Person
			      case 403:
			      {
			      	if (!ExpansionQuestModule.GetModuleInstance().IsOtherQuestInstanceActive(8))
			      		ExpansionQuestModule.GetModuleInstance().DeleteQuestHolder(4001, ExpansionQuestNPCType.AI);
			      }
			      break;
            //! Quest 133 - The Lost Survivor - NEW QUEST
            case 133:
            {
                if (!ExpansionQuestModule.GetModuleInstance().TempQuestHolderExists(4003))
                    SpawnQuestHolder_4003();
            }
            break;
            //! Quest 406 - The Lost Survivor - NEW QUEST
			      case 406:
			      {
			      	if (!ExpansionQuestModule.GetModuleInstance().IsOtherQuestInstanceActive(8))
			      		ExpansionQuestModule.GetModuleInstance().DeleteQuestHolder(4003, ExpansionQuestNPCType.AI);
			      }
			      break;
        }
    }

    override void Expansion_OnQuestContinue(ExpansionQuest quest)
    {
        auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

        switch (quest.GetQuestConfig().GetID())
        {
            //! Quest 131 - Survivors - AI Escort quest chain
            //! Spawn a new quest-giver NPC on the given location
            case 131:
            {
                if (!ExpansionQuestModule.GetModuleInstance().TempQuestHolderExists(4000))
                    SpawnQuestHolder_4000();
            }
            break;
            //! Quest 132--Good Person, Investigate School
            //! Spawn a new quest-giver NPC on the given location
            case 132:
            {
                if (!ExpansionQuestModule.GetModuleInstance().TempQuestHolderExists(4001))
                    SpawnQuestHolder_4001();
            }
            break;
            //! Quest 133--The Lost Survivor - NEW QUEST
            //! Spawn a new quest-giver NPC on the given location
            case 133:
            {
                if (!ExpansionQuestModule.GetModuleInstance().TempQuestHolderExists(4003))
                    SpawnQuestHolder_4003();
            }
            break;
        }
    }

    override void Expansion_OnQuestCancel(ExpansionQuest quest)
    {
        auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

        switch (quest.GetQuestConfig().GetID())
        {
            //! Quest 131 - Survivors - AI Escort quest chain
            case 131:
            {
                if (!ExpansionQuestModule.GetModuleInstance().IsOtherQuestInstanceActive(131))
                    ExpansionQuestModule.GetModuleInstance().DeleteQuestHolder(4000, ExpansionQuestNPCType.AI);
            }
            break;
            case 402:
			      {
			      	  if (!ExpansionQuestModule.GetModuleInstance().IsOtherQuestInstanceActive(8))
			      	  	  ExpansionQuestModule.GetModuleInstance().DeleteQuestHolder(4000, ExpansionQuestNPCType.AI);
			      }
			      break;
            //! Quest 132--Good Person, Investigate School
            case 132:
            {
                if (!ExpansionQuestModule.GetModuleInstance().IsOtherQuestInstanceActive(132))
                    ExpansionQuestModule.GetModuleInstance().DeleteQuestHolder(4001, ExpansionQuestNPCType.AI);
            }
            break;
            case 403:
			      {
			      	  if (!ExpansionQuestModule.GetModuleInstance().IsOtherQuestInstanceActive(8))
			      	  	  ExpansionQuestModule.GetModuleInstance().DeleteQuestHolder(4001, ExpansionQuestNPCType.AI);
			      }
			      break;
            //!The Lost Survivor - NEW QUEST
            case 133:
            {
                if (!ExpansionQuestModule.GetModuleInstance().IsOtherQuestInstanceActive(133))
                    ExpansionQuestModule.GetModuleInstance().DeleteQuestHolder(4003, ExpansionQuestNPCType.AI);
            }
            break;
            case 406:
			      {
			      	  if (!ExpansionQuestModule.GetModuleInstance().IsOtherQuestInstanceActive(8))
			      	  	  ExpansionQuestModule.GetModuleInstance().DeleteQuestHolder(4003, ExpansionQuestNPCType.AI);
			      }
			      break;
        }
    }
    //! Quest 131 - Survivors - AI Escort quest chain
    protected void SpawnQuestHolder_4000()
    {
        ExpansionTempQuestHolder questHolderEscort = new ExpansionTempQuestHolder(4000, "ExpansionQuestNPCAIFrida", "Marina Sidorova", "There is nothing to do here for you...");
        if (!questHolderEscort)
            return;

        questHolderEscort.SetNPCEmoteID(EmoteConstants.ID_EMOTE_SITA);
        questHolderEscort.SetLoadoutName("SurvivorLoadout");
        ExpansionTempQuestHolderPosition questHolderEscortPos = new ExpansionTempQuestHolderPosition("5991.19 307.92 7692.06", "-166.768 0 -0");
        ExpansionQuestModule.GetModuleInstance().SpawnQuestHolder(questHolderEscort, questHolderEscortPos);
    }
    //! Quest 132--Good Person, Investigate School
    protected void SpawnQuestHolder_4001()
    {
        ExpansionTempQuestHolder questHolderEscort = new ExpansionTempQuestHolder(4001, "ExpansionQuestNPCAIJudy", "Elena Petrova", "There is nothing to do here for you...");
        if (!questHolderEscort)
            return;

        questHolderEscort.SetNPCEmoteID(EmoteConstants.ID_EMOTE_SITA);
        questHolderEscort.SetLoadoutName("PlayerFemaleSuitLoadout");
        ExpansionTempQuestHolderPosition questHolderEscortPos = new ExpansionTempQuestHolderPosition("3230.55 210.046 13025.7", "140.698 0 -0");
        ExpansionQuestModule.GetModuleInstance().SpawnQuestHolder(questHolderEscort, questHolderEscortPos);
    }
    //!The Lost Survivor - NEW QUEST
    protected void SpawnQuestHolder_4003()
    {
        ExpansionTempQuestHolder questHolderEscort = new ExpansionTempQuestHolder(4003, "ExpansionQuestNPCAIHassan", "Hassan", "There is nothing to do here for you...");
        if (!questHolderEscort)
            return;

        questHolderEscort.SetNPCEmoteID(EmoteConstants.ID_EMOTE_SITA);
        questHolderEscort.SetLoadoutName("SurvivorLoadout");
        ExpansionTempQuestHolderPosition questHolderEscortPos = new ExpansionTempQuestHolderPosition("13854.64 30.53 2889.72", "62.99 -0 -0");
        ExpansionQuestModule.GetModuleInstance().SpawnQuestHolder(questHolderEscort, questHolderEscortPos);
    }
  //! ADD THIS AFTER EVERYTHING ELSE IN THIS CLASS
	//! Quest Events
	override void Expansion_OnQuestCompletion(ExpansionQuest quest)
    {
        bool shouldCompleteNormally = true;
        
        switch (quest.GetQuestConfig().GetID())
        {
            case 910: //! If quest ID is 910 then this will be executed on quest completion
            {
                PlayerBase player = quest.GetPlayer();
                if (player)
                {
					array<EntityAI> attachmentsItems;

                    //! Check if player has an item in hands
                    EntityAI handItem = player.GetHumanInventory().GetEntityInHands();
                    
                    //! Validate that player has a weapon in hands
                    if (!handItem)
                    {
                        //! No item in hands - return money and send warning message
                        ReturnMoneyToPlayer(player, 25000); //! Return the money that was taken
                        SendQuestMessage(player, "ERROR: You don't have a weapon in your hands! Quest cannot be completed. Money refunded.");
                        shouldCompleteNormally = false;
                    }
                    else
                    {
                        //! Check if the item is a weapon using IsKindOf method
                        bool isWeapon = false;
                        
                        //! Try using IsKindOf to check if item is a weapon
                        if (handItem.IsKindOf("Weapon_Base"))
                        {
                            isWeapon = true;
                        }
                        //! If IsKindOf doesn't work, we'll accept any item in hands as valid
                        //! Player is responsible for having weapon in hands
                        
                        if (!isWeapon)
                        {
                            //! Item in hands is not a weapon - return money and send warning message
                            ReturnMoneyToPlayer(player, 25000); //! Return the money that was taken
                            SendQuestMessage(player, "ERROR: No weapon in your hands! Quest cannot be completed. Money refunded.");
                            shouldCompleteNormally = false;
                        }
                        else
                        {
                            //! Check if weapon and all attachments are pristine (fully repaired)
                            float weaponHealth = handItem.GetHealth("","");
                            float weaponMaxHealth = handItem.GetMaxHealth("","");
                            bool weaponIsPristine = (weaponHealth >= weaponMaxHealth);
                            bool allAttachmentsPristine = true;
                            bool hasAttachments = false;
                            
                            //! Check attachments if weapon has inventory
                            if (handItem.GetInventory() && handItem.GetInventory().CountInventory() > 0)
                            {
                                hasAttachments = true;
                                attachmentsItems = new array<EntityAI>;
                                attachmentsItems.Reserve(handItem.GetInventory().CountInventory());
                                handItem.GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, attachmentsItems);

                                foreach (EntityAI checkAttachment: attachmentsItems)
                                {
                                    if (checkAttachment)
                                    {
                                        float attachHealth = checkAttachment.GetHealth("","");
                                        float attachMaxHealth = checkAttachment.GetMaxHealth("","");
                                        if (attachHealth < attachMaxHealth)
                                        {
                                            allAttachmentsPristine = false;
                                            break; //! Found damaged attachment, no need to check further
                                        }
                                    }
                                }
                            }
                            
                            //! If weapon and all attachments are pristine, return money and don't complete quest
                            if (weaponIsPristine && allAttachmentsPristine)
                            {
                                ReturnMoneyToPlayer(player, 25000); //! Return the money that was taken
                                SendQuestMessage(player, "ERROR: Your weapon and all attachments are already in perfect condition! Quest cannot be completed. Money refunded.");
                                shouldCompleteNormally = false;
                            }
                            else
                            {
                                //! Weapon or attachments are damaged - proceed with repair (even if broken)
                                bool wasRepaired = false;
                                
                                //! Repair weapon (even if broken) - check if it needs repair
                                if (weaponHealth < weaponMaxHealth)
                                {
                                    handItem.SetFullHealth();
                                    wasRepaired = true;
                                }

                                //! Repair attachments (even if broken)
                                if (handItem.GetInventory() && handItem.GetInventory().CountInventory() > 0)
                                {
                                    if (!hasAttachments || attachmentsItems.Count() == 0)
                                    {
                                        attachmentsItems = new array<EntityAI>;
                                        attachmentsItems.Reserve(handItem.GetInventory().CountInventory());
                                        handItem.GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, attachmentsItems);
                                    }

                                    foreach (EntityAI attachment: attachmentsItems)
                                    {
                                        if (attachment)
                                        {
                                            float repairAttachHealth = attachment.GetHealth("","");
                                            float repairAttachMaxHealth = attachment.GetMaxHealth("","");
                                            if (repairAttachHealth < repairAttachMaxHealth)
                                            {
                                                attachment.SetFullHealth();
                                                wasRepaired = true;
                                            }
                                        }
                                    }
                                }
                                
                                if (wasRepaired)
                                {
                                    SendQuestMessage(player, "Weapon successfully repaired!");
                                }
                                else
                                {
                                    //! This should not happen if pristine check works, but just in case
                                    ReturnMoneyToPlayer(player, 25000);
                                    SendQuestMessage(player, "ERROR: Your weapon and all attachments are already in perfect condition! Quest cannot be completed. Money refunded.");
                                    shouldCompleteNormally = false;
                                }
                            }
                        }
                    }
                }
            }
            break;
        }

        //! Only call super if quest should complete normally
        if (shouldCompleteNormally)
        {
            super.Expansion_OnQuestCompletion(quest);
        }
    }
    
    //! Helper function to send messages to player
    void SendQuestMessage(PlayerBase player, string message)
    {
        if (player)
        {
            PlayerIdentity identity = player.GetIdentity();
            if (identity)
            {
                Param1<string> msgParam = new Param1<string>(message);
                GetGame().RPCSingleParam(player, ERPCs.RPC_USER_ACTION_MESSAGE, msgParam, true, identity);
            }
        }
    }
    
    //! Helper function to return money to player (grąžina pinigus žaidėjui kaip stack'intą sumą)
    void ReturnMoneyToPlayer(PlayerBase player, int amount)
    {
        if (!player || amount <= 0)
            return;
            
        //! ExpansionBanknoteHryvnia: 1 banknotas = 1 pinigų vienetas
        //! Amount is 25000, so we need quantity = 25000
        int banknoteCount = amount;
        
        EntityAI createdItem = null;
        ItemBase itemBase = null;
        
        //! Try to create ONE stacked banknote with the correct quantity in player's inventory
        createdItem = player.GetInventory().CreateInInventory("ExpansionBanknoteHryvnia");
        
        if (createdItem)
        {
            //! Cast to ItemBase to set quantity
            itemBase = ItemBase.Cast(createdItem);
            if (itemBase)
            {
                //! Set the quantity to the correct amount (stack the banknotes)
                itemBase.SetQuantity(banknoteCount);
            }
        }
        else
        {
            //! If inventory is full, try to drop near player
            vector playerPos = player.GetPosition();
            vector dropPos = playerPos;
            dropPos[1] = dropPos[1] + 1.0; //! Slightly above ground
            
            createdItem = GetGame().CreateObject("ExpansionBanknoteHryvnia", dropPos, false, true);
            if (createdItem)
            {
                createdItem.SetOrientation(player.GetOrientation());
                
                //! Set quantity for dropped item too
                itemBase = ItemBase.Cast(createdItem);
                if (itemBase)
                {
                    itemBase.SetQuantity(banknoteCount);
                }
            }
        }
    }
#endif

  /* Set global variables to use in the following functions */

  bool rndHealthClothing = true; // set this to false if you want your spawned clothing items to be pristine instead of random between low end badly damaged and high end of worn

  /* Define the chosen career and Generate the starting gear */

  /*******************************************************/
  /* EDITING OF ITEMS IS PERFORMED IN THIS FUNCTION ONLY */
  /*******************************************************/

  void SetCareerPath(PlayerBase player) {

    /* grab a random career on spawn */
    int rndCareer = Math.RandomInt(0, 99); // comment out if you are testing a specific character role **be sure to comment out next line or you will get an error on load**
    //int rndCareer = 90; // comment out if you are running all available character roles, set number and uncomment if testing for a specific role **be sure to comment out previous line if testing or you will get an error on load**

    /* set gloabal variables for strict processing requirements */
    autoptr TStringArray playerGear;
    autoptr TStringArray allItems;
    autoptr TStringArray beltAttachments;
    string itemWeapon;
    autoptr TStringArray itemWeaponAttachments;

    /* check rndCareer value and set appropriate loadout */
    if (rndCareer < 5) {

      /* prisoner */

      /* Build Loadout */
      /* this list should include ALL items you want your spawned characters to wear */
      playerGear = {
        "BushlatPoliceJacket_Blue",
        "CargoPants_Blue",
        "BeanieHat_Black",
        "AthleticShoes_Blue"
      };
      allItems = {
        "BandageDressing",
        "greenbellpepper",
        "Apple",
        "Matchbox",
        "BoneKnife"
      };
      beltAttachments = {};
      itemWeapon = "";
      itemWeaponAttachments = {};

      /* send items to build loadout */
      attachLoadout(player, playerGear, allItems, beltAttachments);
      attachWeapon(player, itemWeapon, itemWeaponAttachments);

    } else if (5 <= rndCareer && rndCareer < 15) {

      /* businessman */

      /* Build Loadout */
      /* this list should include ALL items you want your spawned characters to wear */
      playerGear = {
        "NavyUniformJacket",
        "NavyUniformPants",
        "BeanieHat_Black",
        "AthleticShoes_Black"
      };
      allItems = {
        "BandageDressing",
        "greenbellpepper",
        "Apple",
        "Matchbox",
        "BoneKnife"
      };
      beltAttachments = {};
      itemWeapon = "";
      itemWeaponAttachments = {};

      /* send items to build loadout */
      attachLoadout(player, playerGear, allItems, beltAttachments);
      attachWeapon(player, itemWeapon, itemWeaponAttachments);

    } else if (15 <= rndCareer && rndCareer < 25) {

      /* civilian */

      /* Build Loadout */
      /* this list should include ALL items you want your spawned characters to wear */
      playerGear = {
        "RidersJacket_Black",
        "Jeans_Black",
        "BeanieHat_Black",
        "AthleticShoes_Black"
      };
      allItems = {
        "BandageDressing",
        "greenbellpepper",
        "Apple",
        "Matchbox",
        "BoneKnife"
      };
      beltAttachments = {};
      itemWeapon = "";
      itemWeaponAttachments = {};

      /* send items to build loadout */
      attachLoadout(player, playerGear, allItems, beltAttachments);
      attachWeapon(player, itemWeapon, itemWeaponAttachments);

    } else if (25 <= rndCareer && rndCareer < 35) {

      /* industrial */

      /* Build Loadout */
      /* this list should include ALL items you want your spawned characters to wear */
      playerGear = {
        "JumpsuitJacket_Green",
        "JumpsuitPants_Green",
        "BeanieHat_Green",
        "AthleticShoes_Green"
      };
      allItems = {
        "BandageDressing",
        "greenbellpepper",
        "Apple",
        "Matchbox",
        "BoneKnife"
      };
      beltAttachments = {};
      itemWeapon = "";
      itemWeaponAttachments = {};

      /* send items to build loadout */
      attachLoadout(player, playerGear, allItems, beltAttachments);
      attachWeapon(player, itemWeapon, itemWeaponAttachments);

    } else if (35 <= rndCareer && rndCareer < 45) {

      /* athlete */

      /* Build Loadout */
      /* this list should include ALL items you want your spawned characters to wear */
      playerGear = {
        "WoolCoat_Red",
        "Jeans_Brown",
        "BeanieHat_Red",
        "AthleticShoes_Black"
      };
      allItems = {
        "BandageDressing",
        "greenbellpepper",
        "Apple",
        "Matchbox",
        "BoneKnife"
      };
      beltAttachments = {};
      itemWeapon = "";
      itemWeaponAttachments = {};

      /* send items to build loadout */
      attachLoadout(player, playerGear, allItems, beltAttachments);
      attachWeapon(player, itemWeapon, itemWeaponAttachments);

    } else if (45 <= rndCareer && rndCareer < 55) {

      /* farmer */

      /* Build Loadout */
      /* this list should include ALL items you want your spawned characters to wear */
      playerGear = {
        "WoolCoat_Green",
        "Jeans_Green",
        "BeanieHat_Green",
        "AthleticShoes_Green"
      };
      allItems = {
        "BandageDressing",
        "greenbellpepper",
        "Apple",
        "Matchbox",
        "BoneKnife"
      };
      beltAttachments = {};
      itemWeapon = "";
      itemWeaponAttachments = {};

      /* send items to build loadout */
      attachLoadout(player, playerGear, allItems, beltAttachments);
      attachWeapon(player, itemWeapon, itemWeaponAttachments);

    } else if (55 <= rndCareer && rndCareer < 65) {

      /* medic */

      /* Build Loadout */
      /* this list should include ALL items you want your spawned characters to wear */
      playerGear = {
        "ParamedicJacket_Blue",
        "ParamedicPants_Blue",
        "BeanieHat_Blue",
        "AthleticShoes_Blue"
      };
      allItems = {
        "BandageDressing",
        "greenbellpepper",
        "Apple",
        "Matchbox",
        "BoneKnife"
      };
      beltAttachments = {};
      itemWeapon = "";
      itemWeaponAttachments = {};

      /* send items to build loadout */
      attachLoadout(player, playerGear, allItems, beltAttachments);
      attachWeapon(player, itemWeapon, itemWeaponAttachments);

    } else if (65 <= rndCareer && rndCareer < 75) {

      /* hunter */

      /* Build Loadout */
      /* this list should include ALL items you want your spawned characters to wear */
      playerGear = {
        "HuntingJacket_Brown",
        "HunterPants_Brown",
        "BeanieHat_Brown",
        "AthleticShoes_Brown"
      };
      allItems = {
        "BandageDressing",
        "greenbellpepper",
        "Apple",
        "Matchbox",
        "BoneKnife"
      };
      beltAttachments = {};
      itemWeapon = "";
      itemWeaponAttachments = {};

      /* send items to build loadout */
      attachLoadout(player, playerGear, allItems, beltAttachments);
      attachWeapon(player, itemWeapon, itemWeaponAttachments);

    } else if (75 <= rndCareer && rndCareer < 85) {

      /* firefighter */

      /* Build Loadout */
      /* this list should include ALL items you want your spawned characters to wear */
      playerGear = {
        "FirefighterJacket_Black",
        "FirefightersPants_Black",
        "BeanieHat_Black",
        "AthleticShoes_Grey"
      };
      allItems = {
        "BandageDressing",
        "greenbellpepper",
        "Apple",
        "Matchbox",
        "BoneKnife"
      };
      beltAttachments = {};
      itemWeapon = "";
      itemWeaponAttachments = {};

      /* send items to build loadout */
      attachLoadout(player, playerGear, allItems, beltAttachments);
      attachWeapon(player, itemWeapon, itemWeaponAttachments);

    } else if (85 <= rndCareer && rndCareer < 95) {

      /* police */

      /* Build Loadout */
      /* this list should include ALL items you want your spawned characters to wear */
      playerGear = {
        "WoolCoat_BlueCheck",
        "Jeans_Blue",
        "BeanieHat_Blue",
        "AthleticShoes_Blue"
      };
      allItems = {
        "BandageDressing",
        "greenbellpepper",
        "Apple",
        "Matchbox",
        "BoneKnife"
      };
      beltAttachments = {};
      itemWeapon = "";
      itemWeaponAttachments = {};

      /* send items to build loadout */
      attachLoadout(player, playerGear, allItems, beltAttachments);
      attachWeapon(player, itemWeapon, itemWeaponAttachments);

    } else if (95 <= rndCareer) {

      /* military */

      /* Build Loadout */
      /* this list should include ALL items you want your spawned characters to wear */
      playerGear = {
        "WoolCoat_Black",
        "Jeans_Black",
        "BeanieHat_Black",
        "AthleticShoes_Black"
      };
      allItems = {
        "BandageDressing",
        "greenbellpepper",
        "Apple",
        "Matchbox",
        "BoneKnife"
      };
      beltAttachments = {};
      itemWeapon = "";
      itemWeaponAttachments = {};

      /* send items to build loadout */
      attachLoadout(player, playerGear, allItems, beltAttachments);
      attachWeapon(player, itemWeapon, itemWeaponAttachments);

    }
  }

  /************************************************************/
  /* THERE SHOULD BE NO NEED TO EDIT ANYTHING BELOW THIS LINE */
  /************************************************************/

  /* function to loop array and attach items to player */
  void attachLoadout(PlayerBase player, TStringArray playerGear, TStringArray playerItems, TStringArray beltItems) {

    /* Loops through the array and puts last item in your hands - typically the weapon */
    // define our total number of items to attach
    int gearCount = playerGear.Count();
    int weaponSlot = (gearCount - 1);

    // loop through the array and attach items
    foreach(string j: playerGear) {
      player.GetInventory().CreateInInventory(j);
    }

    /* set up items for each possible inventory slot/clothing slot */
    EntityAI itemsArray[11];
    itemsArray[0] = player.FindAttachmentBySlotName("Armband");
    itemsArray[1] = player.FindAttachmentBySlotName("Backpack");
    itemsArray[2] = player.FindAttachmentBySlotName("Body");
    itemsArray[3] = player.FindAttachmentBySlotName("Eyewear");
    itemsArray[4] = player.FindAttachmentBySlotName("Feet");
    itemsArray[5] = player.FindAttachmentBySlotName("Gloves");
    itemsArray[6] = player.FindAttachmentBySlotName("Headgear");
    itemsArray[7] = player.FindAttachmentBySlotName("Hips");
    itemsArray[8] = player.FindAttachmentBySlotName("Legs");
    itemsArray[9] = player.FindAttachmentBySlotName("Mask");
    itemsArray[10] = player.FindAttachmentBySlotName("Vest");

    /* set random health on starting items if configured */
    if (rndHealthClothing == true) {
      if (itemsArray) {
        foreach(int z, EntityAI ia: itemsArray) {
          if (itemsArray[z]) {
            SetRandomHealth(ia);
          }
        }
      }
    }

    /* drop provided items into the inventory slots */
    if (playerItems) {
      foreach(string pi: playerItems) {
        player.GetInventory().CreateInInventory(pi);
      }
    }

    /* attach belt items if provided */
    if (beltItems) {
      foreach(string bi: beltItems) {
        itemsArray[7].GetInventory().CreateAttachment(bi);
      }
    }
  }

  /* attach gun and provided ammo */
  void attachWeapon(PlayerBase player, string playerWeapon, TStringArray weaponAttachments) {
    int weaponAttachmentCount = weaponAttachments.Count();
    bool magAdded = false;

    /* put first inventory in hands - assumed to be the gun */
    EntityAI weapon = player.GetHumanInventory().CreateInHands(playerWeapon);

    /* verify weaponAttachments has items before looping through */
    if (weaponAttachmentCount > 0) {

      /* create attachments as the rest of the items */
      foreach(int wa, string was: weaponAttachments) {

        string weaponStart = weaponAttachments[wa].Substring(0, 3);
        //string weaponLower = weaponStart.ToLower();

        /* verify if the string starts with "mag" and drop in inventory if adding more than 1 */
        if (weaponStart == "Mag" && magAdded == false) {
          magAdded = true;
          weapon.GetInventory().CreateAttachment(was);
        } else if (weaponStart == "Mag" && magAdded == true) {
          player.GetInventory().CreateInInventory(was);
        } else {
          weapon.GetInventory().CreateAttachment(was);
        }
      }
    }
  }

  void SetRandomHealth(EntityAI itemEnt) {
    if (itemEnt) {
      float rndHlt = Math.RandomFloat(0.45, 0.65);
      itemEnt.SetHealth01("", "", rndHlt);
    }
  }

  override PlayerBase CreateCharacter(PlayerIdentity identity, vector pos, ParamsReadContext ctx, string characterName) {
    Entity playerEnt;
    playerEnt = GetGame().CreatePlayer(identity, characterName, pos, 0, "NONE");
    Class.CastTo(m_player, playerEnt);

    GetGame().SelectPlayer(identity, m_player);

    return m_player;
  }

  // Check if player name is invalid/banned
  bool IsInvalidPlayerName(string playerName) {
    string nameLower = playerName;
    nameLower.ToLower();
    
    // List of invalid names (case-insensitive)
    TStringArray invalidNames = {
      "survivor",
      "123",
      "test",
      "test123",
      "player",
      "user",
      "admin",
      "administrator",
      "mod",
      "moderator",
      "gm",
      "game master",
      "dev",
      "developer",
      "",
      "1",
      "12",
      "abc",
      "12345",
      "qwerty",
      "asdf",
      "player1",
      "player2",
      "newplayer"
    };
    
    // Check if name matches any invalid name
    foreach(string invalidName: invalidNames) {
      if (nameLower == invalidName) {
        return true;
      }
    }
    
    // Check if name is too short (less than 3 characters)
    if (playerName.Length() < 3) {
      return true;
    }
    
    return false;
  }

  override void StartingEquipSetup(PlayerBase player, bool clothesChosen) 
  {
    // Start By Removing all default items regardess of name or state
    player.RemoveAllItems();

    // Get Player Name
    string characterName = player.GetIdentity().GetName();
    
    // Check if player name is invalid - REMOVE GEAR AND SEND MESSAGE (will need to reconnect with proper name)
    if (IsInvalidPlayerName(characterName)) {
      // Remove all items - player spawns naked
      player.RemoveAllItems();
      
      // Send warning message to player
      string kickMessage = "INVALID NAME: Your character name '" + characterName + "' is not allowed! You will spawn NAKED. Please disconnect and reconnect with a proper name (minimum 3 characters, not 'Survivor', '123', etc.)!";
      GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(SendInvalidNameMessage, 2000, false, player, kickMessage);
      
      return;
    } else {
      SetCareerPath(player);
    }
  }
  
  // Send message to player about invalid name
  void SendInvalidNameMessage(PlayerBase player, string message) {
    if (player) {
      PlayerIdentity identity = player.GetIdentity();
      if (identity) {
        Param1<string> msgParam = new Param1<string>(message);
        GetGame().RPCSingleParam(player, ERPCs.RPC_USER_ACTION_MESSAGE, msgParam, true, identity);
      }
    }
  }
};

Mission CreateCustomMission(string path) {
  return new CustomMission();
}
