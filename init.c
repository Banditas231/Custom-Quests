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

class WeeklyQuestProgressData
{
	string PlayerId;
	string WeekKey;
	int CompletedThisWeek;
	bool RewardClaimed;

	void WeeklyQuestProgressData(string playerId = "", string weekKey = "")
	{
		PlayerId = playerId;
		WeekKey = weekKey;
		CompletedThisWeek = 0;
		RewardClaimed = false;
	}
}

class CustomMission: MissionServer

{
	protected const string WEEKLY_PROGRESS_FILE = "$profile:IronZone_WeeklyQuestProgress.json";
	protected const int WEEKLY_HIGH_THRESHOLD = 3;
	protected const int WEEKLY_MID_THRESHOLD = 2;
	protected const int WEEKLY_LOW_THRESHOLD = 1;
	//! Weekly reset schedule aligned to Expansion defaults (Wednesday 08:00)
	//! Weekday mapping: 0=Sunday, 1=Monday, 2=Tuesday, 3=Wednesday, 4=Thursday, 5=Friday, 6=Saturday
	protected const int WEEKLY_RESET_DAY = 3;
	protected const int WEEKLY_RESET_HOUR = 8;
	protected const int WEEKLY_RESET_MINUTE = 0;
	//! If needed, set local time offset in seconds (e.g. UTC+2 = 7200). Keep 0 if not needed.
	protected const int WEEKLY_RESET_TIMEZONE_OFFSET_SECONDS = 0;
	protected ref map<string, ref WeeklyQuestProgressData> m_WeeklyQuestProgress;

	void CustomMission()
	{
		m_WeeklyQuestProgress = new map<string, ref WeeklyQuestProgressData>;
		LoadWeeklyQuestProgress();
	}

	//! MMG vests: `CustomTypes/MM/*.xml` type names — prefix match (excludes MMG_tt_vest_admin).
	protected bool IronZone_IsMMGVestType(string t)
	{
		if (t == "MMG_tt_vest_admin")
			return false;
		if (t.IndexOf("MMG_JPC_Vest_") == 0)
			return true;
		if (t.IndexOf("MMG_tt_Vest_") == 0)
			return true;
		if (t.IndexOf("MMG_tt_vest_") == 0)
			return true;
		if (t.IndexOf("MMG_chestrig_") == 0)
			return true;
		if (t.IndexOf("mmg_chestrig_") == 0)
			return true;
		if (t.IndexOf("MMG_MK_III_Armor_") == 0)
			return true;
		if (t.IndexOf("MMG_MK_V_Armor_") == 0)
			return true;
		return false;
	}

	//! Vanilla + mission custom vests: `GetType()` — prefix/exact like MMG (`db/types.xml` + `CustomTypes/Hunterz_Animals` Crocodile). NOT Market JSON.
	protected bool IronZone_IsDbAndCustomVestType(string t)
	{
		if (t == "ReflexVest")
			return true;
		if (t == "PoliceVest")
			return true;
		if (t == "SmershVest")
			return true;
		if (t == "Chestplate")
			return true;
		if (t == "HuntingVest")
			return true;
		if (t == "PlateCarrierVest")
			return true;
		if (t.IndexOf("PressVest_") == 0)
			return true;
		if (t.IndexOf("UKAssVest_") == 0)
			return true;
		if (t.IndexOf("HighCapacityVest_") == 0)
			return true;
		if (t.IndexOf("HuntingVest_") == 0)
			return true;
		if (t.IndexOf("LeatherStorageVest_") == 0)
			return true;
		if (t.IndexOf("PlateCarrierVest_") == 0)
			return true;
		if (t.IndexOf("CrocodileVest_") == 0)
			return true;
		return false;
	}

	//! Quest 911: do not repair these vest slots (economy: grenades, utility, radio, compass, chemlights).
	protected bool IronZone_IsVestAttachmentExcludedFromRepair(EntityAI att)
	{
		if (!att)
			return true;
		if (att.IsKindOf("Grenade_Base"))
			return true;
		if (att.IsKindOf("Compass"))
			return true;
		if (att.IsKindOf("OrienteeringCompass"))
			return true;
		if (att.IsKindOf("PersonalRadio"))
			return true;
		if (att.IsKindOf("BaseRadio"))
			return true;
		if (att.IsKindOf("Chemlight_ColorBase"))
			return true;
		string t = att.GetType();
		if (t.Length() >= 10 && t.IndexOf("Chemlight_") == 0)
			return true;
		return false;
	}

	//! Quest 911: types-based only (GetType matches db/MM types). No Market JSON strings, no fuzzy Contains.
	protected bool IronZone_IsVestRepairItem(EntityAI ent)
	{
		if (!ent)
			return false;
		if (ent.IsKindOf("Backpack_Base"))
			return false;
		string t = ent.GetType();
		if (IronZone_IsMMGVestType(t))
			return true;
		if (ent.IsKindOf("Vest_Base"))
			return true;
		if (IronZone_IsDbAndCustomVestType(t))
			return true;
		return false;
	}

	//! Quest 913: true if Health/Shock/Blood not at max for this zone (incl. global).
	protected bool IronZone_PlayerVitalNotFull(PlayerBase player, string zone, string htype)
	{
		if (!player)
			return false;
		float mx = player.GetMaxHealth(zone, htype);
		if (mx <= 0)
			return false;
		return player.GetHealth(zone, htype) < mx;
	}

	//! Quest 913: InediaPain — ar bent vienoje galūnėje pakankamas skausmas (refund / „reikia klinikos“).
	//! Jei kompiliatorius neranda GetPainPercentForLimb — žr. InediaPain pain manager .c (AddPainPercentForLimb simetrija).
	protected bool IronZone_InediaPain_PlayerNeedsMedicalTreatment(PlayerBase player)
	{
		if (!player)
			return false;
		auto ironInediaMgr = player.InediaPain_GetPainManager();
		if (!ironInediaMgr)
			return false;
		array<string> ironInediaLimbs = new array<string>;
		ironInediaLimbs.Insert("head");
		ironInediaLimbs.Insert("arms");
		ironInediaLimbs.Insert("legs");
		ironInediaLimbs.Insert("torso");
		foreach (string ironL : ironInediaLimbs)
		{
			float ironP = ironInediaMgr.GetPainPercentForLimb(ironL);
			if (ironP > 0.5)
				return true;
		}
		return false;
	}

	//! Quest 913: InediaPain — [Medicine guide]: lūžiai → gilios žaizdos → skausmas (neigiamas %).
	protected void IronZone_InediaPain_ApplyQuestTreatment(PlayerBase player)
	{
		if (!player)
			return;
		auto ironInediaMgr = player.InediaPain_GetPainManager();
		if (!ironInediaMgr)
			return;
		array<string> ironInediaLimbs = new array<string>;
		ironInediaLimbs.Insert("head");
		ironInediaLimbs.Insert("arms");
		ironInediaLimbs.Insert("legs");
		ironInediaLimbs.Insert("torso");
		int ironIdx;
		for (ironIdx = 0; ironIdx < ironInediaLimbs.Count(); ironIdx++)
			ironInediaMgr.DeactivateBreakForLimb(ironInediaLimbs.Get(ironIdx));
		for (ironIdx = 0; ironIdx < ironInediaLimbs.Count(); ironIdx++)
			ironInediaMgr.DeactivateDeepWoundForLimb(ironInediaLimbs.Get(ironIdx));
		for (ironIdx = 0; ironIdx < ironInediaLimbs.Count(); ironIdx++)
			ironInediaMgr.AddPainPercentForLimb(ironInediaLimbs.Get(ironIdx), -100.0);
	}

	//! Quest 913: player needs service (refund if false).
	protected bool IronZone_PlayerNeedsMedicalTreatment(PlayerBase player)
	{
		if (!player || !player.IsAlive())
			return false;
		if (player.GetTotalAgentCount() > 0)
			return true;
		if (player.GetBleedingSourceCount() > 0)
			return true;
		if (player.GetBrokenLegs() != eBrokenLegs.NO_BROKEN_LEGS)
			return true;
		if (player.IsUnconscious())
			return true;
		ModifiersManager ironZoneMM = player.GetModifiersManager();
		if (ironZoneMM)
		{
			if (ironZoneMM.IsModifierActive(eModifiers.MDF_BROKEN_LEGS))
				return true;
			if (ironZoneMM.IsModifierActive(eModifiers.MDF_BROKEN_ARMS))
				return true;
		}
		array<string> ironHtCheck = new array<string>;
		ironHtCheck.Insert("Health");
		ironHtCheck.Insert("Shock");
		ironHtCheck.Insert("Blood");
		int ci;
		for (ci = 0; ci < ironHtCheck.Count(); ci++)
		{
			if (IronZone_PlayerVitalNotFull(player, "", ironHtCheck.Get(ci)))
				return true;
		}
		DamageZoneMap ironZoneNeedMap = new DamageZoneMap();
		if (DamageSystem.GetDamageZoneMap(player, ironZoneNeedMap))
		{
			array<string> ironZList = ironZoneNeedMap.GetKeyArray();
			int vi;
			for (vi = 0; vi < ironZList.Count(); vi++)
			{
				string vz = ironZList.Get(vi);
				for (ci = 0; ci < ironHtCheck.Count(); ci++)
				{
					if (IronZone_PlayerVitalNotFull(player, vz, ironHtCheck.Get(ci)))
						return true;
				}
			}
		}
		if (IronZone_InediaPain_PlayerNeedsMedicalTreatment(player))
			return true;
		return false;
	}

	//! Quest 913: diseases cleared, bleeding stopped, limbs/script fractures cleared, full Health/Blood/Shock (ResetAllZones), wake; no food/water/stamina.
	protected void IronZone_ApplyMedicalQuestTreatment(PlayerBase player)
	{
		if (!player)
			return;
		if (player.GetBleedingManagerServer())
			player.GetBleedingManagerServer().RemoveAllSources();
		player.RemoveAllAgents();
		DamageSystem.ResetAllZones(player);
		ModifiersManager ironZoneMm2 = player.GetModifiersManager();
		if (ironZoneMm2)
		{
			if (ironZoneMm2.IsModifierActive(eModifiers.MDF_BROKEN_LEGS))
				ironZoneMm2.DeactivateModifier(eModifiers.MDF_BROKEN_LEGS);
			if (ironZoneMm2.IsModifierActive(eModifiers.MDF_BROKEN_ARMS))
				ironZoneMm2.DeactivateModifier(eModifiers.MDF_BROKEN_ARMS);
		}
		if (player.GetBrokenLegs() != eBrokenLegs.NO_BROKEN_LEGS)
			player.SetBrokenLegs(eBrokenLegs.NO_BROKEN_LEGS);
		if (player.IsUnconscious())
			DayZPlayerSyncJunctures.SendPlayerUnconsciousness(player, false);
		IronZone_InediaPain_ApplyQuestTreatment(player);
	}

	//! Quest Events – 910 weapon, 911 vest, 912 clothes/bag, 913 medical (no temp holder logic)
	//! GitHub copy: no SpawnerBubaku — Expansion_OnQuestStart / OnQuestCancel not overridden.
	override void Expansion_OnQuestCompletion(ExpansionQuest quest)
    {
		PlayerBase questPlayer = quest.GetPlayer();
		if (!questPlayer)
			return;

        bool shouldCompleteNormally = true;
        
        switch (quest.GetQuestConfig().GetID())
        {
            //! Teleports the player after turning in extraction quests
            case 1001:
            {
                array<vector> teleportPositions1 = {"3755.94 402.803 6008.14", "3763.6 403.08 5991.87", "3722.09 399.632 6037.28"};
                vector ori1 = questPlayer.GetOrientation();
                DayZPlayerSyncJunctures.ExpansionTeleport(questPlayer, teleportPositions1.GetRandomElement(), ori1);
            }
            break;
            case 1002:
            {
                array<vector> teleportPositions2 = {"9167.87 236.867 11415.3", "9151.29 233.366 11536.2", "9209.77 233.829 11539.7"};
                vector ori2 = questPlayer.GetOrientation();
                DayZPlayerSyncJunctures.ExpansionTeleport(questPlayer, teleportPositions2.GetRandomElement(), ori2);
            }
            break;
            case 1003:
            {
                array<vector> teleportPositions3 = {"14376.2 6.79 13236.1", "14413.4 3.20859 13288.1"};
                vector ori3 = questPlayer.GetOrientation();
                DayZPlayerSyncJunctures.ExpansionTeleport(questPlayer, teleportPositions3.GetRandomElement(), ori3);
            }
            break;
            case 1004:
            {
                array<vector> teleportPositions4 = {"4416.59 3.06608 2247.98", "4349.93 3.23715 2269.65", "4343.09 3.01606 2266.09"};
                vector ori4 = questPlayer.GetOrientation();
                DayZPlayerSyncJunctures.ExpansionTeleport(questPlayer, teleportPositions4.GetRandomElement(), ori4);
            }
            break;
            case 1005:
            {
                array<vector> teleportPositions5 = {"6519.53 73.4047 3444.91", "6493.89 73.2893 3420.34", "6565.26 72.1474 3353.08"};
                vector ori5 = questPlayer.GetOrientation();
                DayZPlayerSyncJunctures.ExpansionTeleport(questPlayer, teleportPositions5.GetRandomElement(), ori5);
            }
            break;
            case 1006:
            {
                array<vector> teleportPositions6 = {"9974.4 46.3242 2699.85", "10050.6 39.1765 2651.94", "9952.57 48.2364 2715.41"};
                vector ori6 = questPlayer.GetOrientation();
                DayZPlayerSyncJunctures.ExpansionTeleport(questPlayer, teleportPositions6.GetRandomElement(), ori6);
            }
            break;
            case 1007:
            {
                array<vector> teleportPositions7 = {"13018.9 115.593 5908.03", "12962.8 119.708 5917.42", "13043.9 108.047 6034.41"};
                vector ori7 = questPlayer.GetOrientation();
                DayZPlayerSyncJunctures.ExpansionTeleport(questPlayer, teleportPositions7.GetRandomElement(), ori7);
            }
            break;
            case 1008:
            {
                array<vector> teleportPositions8 = {"12413.1 17.4778 9751.19", "12045.2 13.2361 9619.35", "12697.2 5.97264 9383.04"};
                vector ori8 = questPlayer.GetOrientation();
                DayZPlayerSyncJunctures.ExpansionTeleport(questPlayer, teleportPositions8.GetRandomElement(), ori8);
            }
            break;
            case 1009:
            {
                array<vector> teleportPositions9 = {"6335.72 469.763 14860.1", "6380.68 469.483 15082.1", "6906.4 449.582 14978.7"};
                vector ori9 = questPlayer.GetOrientation();
                DayZPlayerSyncJunctures.ExpansionTeleport(questPlayer, teleportPositions9.GetRandomElement(), ori9);
            }
            break;
            case 1010:
            {
                array<vector> teleportPositions10 = {"288.534 525.394 11708.3", "184.534 486.929 12123.2", "491.734 425.529 12108.3"};
                vector ori10 = questPlayer.GetOrientation();
                DayZPlayerSyncJunctures.ExpansionTeleport(questPlayer, teleportPositions10.GetRandomElement(), ori10);
            }
            break;
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
            case 911:
            {
                int q911Cost = 13000;
                EntityAI q911Ent = questPlayer.GetHumanInventory().GetEntityInHands();
                if (!q911Ent)
                {
                    ReturnMoneyToPlayer(questPlayer, q911Cost);
                    SendQuestMessage(questPlayer, "ERROR: You don't have an item in your hands! Quest cannot be completed. Money refunded.");
                    shouldCompleteNormally = false;
                }
                else if (q911Ent.IsKindOf("Weapon_Base"))
                {
                    ReturnMoneyToPlayer(questPlayer, q911Cost);
                    SendQuestMessage(questPlayer, "ERROR: Weapons use the weapon repair service. Money refunded.");
                    shouldCompleteNormally = false;
                }
                else if (!IronZone_IsVestRepairItem(q911Ent))
                {
                    ReturnMoneyToPlayer(questPlayer, q911Cost);
                    SendQuestMessage(questPlayer, "ERROR: This quest is for vests / chest rigs only. Use backpack & clothes repair for other items. Money refunded.");
                    shouldCompleteNormally = false;
                }
                else
                {
                    GameInventory q911Inv = q911Ent.GetInventory();
                    float q911RootH = q911Ent.GetHealth("", "");
                    float q911RootMax = q911Ent.GetMaxHealth("", "");
                    bool q911RootPristine = (q911RootH >= q911RootMax);
                    int q911Ac = 0;
                    if (q911Inv)
                        q911Ac = q911Inv.AttachmentCount();
                    if (q911Ac > 0)
                    {
                        bool q911AllAttPristine = true;
                        for (int q911Vi = 0; q911Vi < q911Ac; q911Vi++)
                        {
                            EntityAI q911Att = q911Inv.GetAttachmentFromIndex(q911Vi);
                            if (q911Att && !IronZone_IsVestAttachmentExcludedFromRepair(q911Att))
                            {
                                if (q911Att.GetHealth("", "") < q911Att.GetMaxHealth("", ""))
                                    q911AllAttPristine = false;
                            }
                        }
                        if (q911RootPristine && q911AllAttPristine)
                        {
                            ReturnMoneyToPlayer(questPlayer, q911Cost);
                            SendQuestMessage(questPlayer, "ERROR: Your vest and attachment pouches are already pristine. Money refunded.");
                            shouldCompleteNormally = false;
                        }
                        else
                        {
                            bool q911DidRepair = false;
                            if (!q911RootPristine)
                            {
                                q911Ent.SetFullHealth();
                                q911DidRepair = true;
                            }
                            for (int q911Vj = 0; q911Vj < q911Ac; q911Vj++)
                            {
                                EntityAI q911Att2 = q911Inv.GetAttachmentFromIndex(q911Vj);
                                if (q911Att2 && !IronZone_IsVestAttachmentExcludedFromRepair(q911Att2))
                                {
                                    if (q911Att2.GetHealth("", "") < q911Att2.GetMaxHealth("", ""))
                                    {
                                        q911Att2.SetFullHealth();
                                        q911DidRepair = true;
                                    }
                                }
                            }
                            if (q911DidRepair)
                            {
                                SendQuestMessage(questPlayer, "Vest successfully repaired!");
                            }
                            else
                            {
                                ReturnMoneyToPlayer(questPlayer, q911Cost);
                                SendQuestMessage(questPlayer, "ERROR: Your vest and attachment pouches are already pristine. Money refunded.");
                                shouldCompleteNormally = false;
                            }
                        }
                    }
                    else
                    {
                        if (q911RootPristine)
                        {
                            ReturnMoneyToPlayer(questPlayer, q911Cost);
                            SendQuestMessage(questPlayer, "ERROR: Your vest is already pristine. Money refunded.");
                            shouldCompleteNormally = false;
                        }
                        else
                        {
                            q911Ent.SetFullHealth();
                            SendQuestMessage(questPlayer, "Vest successfully repaired!");
                        }
                    }
                }
            }
            break;
            case 912:
            {
                int q912Cost = 10000;
                EntityAI q912Ent = questPlayer.GetHumanInventory().GetEntityInHands();
                if (!q912Ent)
                {
                    ReturnMoneyToPlayer(questPlayer, q912Cost);
                    SendQuestMessage(questPlayer, "ERROR: You don't have an item in your hands! Quest cannot be completed. Money refunded.");
                    shouldCompleteNormally = false;
                }
                else if (q912Ent.IsKindOf("Weapon_Base"))
                {
                    ReturnMoneyToPlayer(questPlayer, q912Cost);
                    SendQuestMessage(questPlayer, "ERROR: Weapons use the weapon repair service. Money refunded.");
                    shouldCompleteNormally = false;
                }
                else if (IronZone_IsVestRepairItem(q912Ent))
                {
                    ReturnMoneyToPlayer(questPlayer, q912Cost);
                    SendQuestMessage(questPlayer, "ERROR: Vests and chest rigs use the VEST repair quest. Money refunded.");
                    shouldCompleteNormally = false;
                }
                else
                {
                    ItemBase q912Ib = ItemBase.Cast(q912Ent);
                    bool q912IsBackpack = q912Ent.IsKindOf("Backpack_Base");
                    bool q912IsClothing = q912Ib && q912Ib.IsClothing();
                    if (!q912IsBackpack && !q912IsClothing)
                    {
                        ReturnMoneyToPlayer(questPlayer, q912Cost);
                        SendQuestMessage(questPlayer, "ERROR: Only backpacks and clothing (not vests) can be repaired here. Money refunded.");
                        shouldCompleteNormally = false;
                    }
                    else
                    {
                        float q912H = q912Ent.GetHealth("", "");
                        float q912M = q912Ent.GetMaxHealth("", "");
                        if (q912H >= q912M)
                        {
                            ReturnMoneyToPlayer(questPlayer, q912Cost);
                            SendQuestMessage(questPlayer, "ERROR: Your item is already pristine. Money refunded.");
                            shouldCompleteNormally = false;
                        }
                        else
                        {
                            q912Ent.SetFullHealth();
                            if (q912IsBackpack)
                                SendQuestMessage(questPlayer, "Backpack successfully repaired!");
                            else
                                SendQuestMessage(questPlayer, "Item successfully repaired!");
                        }
                    }
                }
            }
            break;
            case 913:
            {
                int q913Cost = 20000;
                if (IronZone_PlayerNeedsMedicalTreatment(questPlayer))
                {
                    IronZone_ApplyMedicalQuestTreatment(questPlayer);
                    SendQuestMessage(questPlayer, "Treatment complete. Health, blood, shock restored; Inedia fractures, deep wounds and pain cleared. Embedded bullets and internal bleeding need surgery — not removed here. Food, water and stamina unchanged.");
                }
                else
                {
                    ReturnMoneyToPlayer(questPlayer, q913Cost);
                    SendQuestMessage(questPlayer, "ERROR: You do not need medical treatment right now. Money refunded.");
                    shouldCompleteNormally = false;
                }
            }
            break;
        }

        //! Only call super if quest should complete normally
        if (shouldCompleteNormally)
        {
			UpdateWeeklyQuestProgress(questPlayer);
            super.Expansion_OnQuestCompletion(quest);
        }
    }

	void UpdateWeeklyQuestProgress(PlayerBase player)
	{
		if (!player)
			return;

		PlayerIdentity identity = player.GetIdentity();
		if (!identity)
			return;

		string playerId = identity.GetId();
		if (playerId == "")
			return;

		string currentWeekKey = GetCurrentWeekKey();
		ref WeeklyQuestProgressData data;

		if (!m_WeeklyQuestProgress.Find(playerId, data) || !data)
		{
			data = new WeeklyQuestProgressData(playerId, currentWeekKey);
			m_WeeklyQuestProgress.Set(playerId, data);
		}

		if (data.WeekKey != currentWeekKey)
		{
			GrantWeeklyReward(player, data);
			data.WeekKey = currentWeekKey;
			data.CompletedThisWeek = 0;
			data.RewardClaimed = false;
		}

		data.CompletedThisWeek++;
		SaveWeeklyQuestProgress();

		SendQuestMessage(player, string.Format("Weekly quest progress: %1 completed this week.", data.CompletedThisWeek));
	}

	void GrantWeeklyReward(PlayerBase player, WeeklyQuestProgressData data)
	{
		if (!player || !data)
			return;

		if (data.RewardClaimed || data.CompletedThisWeek <= 0)
			return;

		if (data.CompletedThisWeek >= WEEKLY_HIGH_THRESHOLD)
		{
			ReturnMoneyToPlayer(player, 50000);
			SendQuestMessage(player, "Weekly reward (HIGH): +50000.");
		}
		else if (data.CompletedThisWeek >= WEEKLY_MID_THRESHOLD)
		{
			ReturnMoneyToPlayer(player, 25000);
			SendQuestMessage(player, "Weekly reward (MID): +25000.");
		}
		else if (data.CompletedThisWeek >= WEEKLY_LOW_THRESHOLD)
		{
			ReturnMoneyToPlayer(player, 10000);
			SendQuestMessage(player, "Weekly reward (LOW): +10000.");
		}

		data.RewardClaimed = true;
	}

	string GetCurrentWeekKey()
	{
		int nowTimestamp = CF_Date.Now().GetTimestamp();
		int localTimestamp = nowTimestamp + WEEKLY_RESET_TIMEZONE_OFFSET_SECONDS;

		int daysSinceEpoch = localTimestamp / 86400;
		int secondsOfDay = localTimestamp % 86400;
		if (secondsOfDay < 0)
			secondsOfDay += 86400;

		//! 1970-01-01 was Thursday
		int currentWeekday = (daysSinceEpoch + 4) % 7;
		if (currentWeekday < 0)
			currentWeekday += 7;

		int resetSeconds = (WEEKLY_RESET_HOUR * 3600) + (WEEKLY_RESET_MINUTE * 60);
		int daysSinceReset = currentWeekday - WEEKLY_RESET_DAY;
		if (daysSinceReset < 0)
			daysSinceReset += 7;

		//! If it is reset day but before reset time, still treat as previous week
		if (daysSinceReset == 0 && secondsOfDay < resetSeconds)
			daysSinceReset = 7;

		int secondsSinceReset = (daysSinceReset * 86400) + (secondsOfDay - resetSeconds);
		int weekStartLocalTimestamp = localTimestamp - secondsSinceReset;
		int weekIndex = weekStartLocalTimestamp / 604800;

		return string.Format("RTW-%1", weekIndex);
	}

	void LoadWeeklyQuestProgress()
	{
		if (!FileExist(WEEKLY_PROGRESS_FILE))
			return;

		ref array<ref WeeklyQuestProgressData> loadedData = new array<ref WeeklyQuestProgressData>;
		JsonFileLoader<array<ref WeeklyQuestProgressData>>.JsonLoadFile(WEEKLY_PROGRESS_FILE, loadedData);

		foreach (ref WeeklyQuestProgressData entry: loadedData)
		{
			if (entry && entry.PlayerId != "")
				m_WeeklyQuestProgress.Set(entry.PlayerId, entry);
		}
	}

	void SaveWeeklyQuestProgress()
	{
		ref array<ref WeeklyQuestProgressData> saveData = new array<ref WeeklyQuestProgressData>;

		foreach (string playerId, ref WeeklyQuestProgressData entry: m_WeeklyQuestProgress)
		{
			if (entry)
				saveData.Insert(entry);
		}

		JsonFileLoader<array<ref WeeklyQuestProgressData>>.JsonSaveFile(WEEKLY_PROGRESS_FILE, saveData);
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