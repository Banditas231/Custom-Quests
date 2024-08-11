void main() 
{
  //INIT WEATHER BEFORE ECONOMY INIT------------------------
  Weather weather = g_Game.GetWeather();

  weather.MissionWeather(false); // false = use weather controller from Weather.c

  weather.GetOvercast().Set(Math.RandomFloatInclusive(0.4, 0.6), 1, 0);
  weather.GetRain().Set(0, 0, 1);
  weather.GetFog().Set(Math.RandomFloatInclusive(0.05, 0.1), 1, 0);

  //INIT ECONOMY--------------------------------------
  Hive ce = CreateHive();
  if (ce)
    ce.InitOffline();

  //DATE RESET AFTER ECONOMY INIT-------------------------
  int year, month, day, hour, minute;
  int reset_month = 5, reset_day = 20;
  GetGame().GetWorld().GetDate(year, month, day, hour, minute);

  if ((month == reset_month) && (day < reset_day)) {
    GetGame().GetWorld().SetDate(year, reset_month, reset_day, hour, minute);
  } else {
    if ((month == reset_month + 1) && (day > reset_day)) {
      GetGame().GetWorld().SetDate(year, reset_month, reset_day, hour, minute);
    } else {
      if ((month < reset_month) || (month > reset_month + 1)) {
        GetGame().GetWorld().SetDate(year, reset_month, reset_day, hour, minute);
      }
    }
  }
};

class CustomMission: MissionServer
{
////This is the script to TP GMT to Traders ower map
	override void Expansion_OnQuestCompletion(ExpansionQuest quest)
  {
   auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);   
      PlayerBase player = quest.GetPlayer();
      if (!player)
          return;
      
      switch (quest.GetQuestConfig().GetID())
      {
          //! Teleports the player after tuning-in quest with ID 1001 Destination SZ Trader
          case 1001:
          {
              array<vector> teleportPositions1 = {"3755.94 402.803 6008.14", "3763.6 403.08 5991.87", "3722.09 399.632 6037.28"};
              vector ori1 = player.GetOrientation();
              DayZPlayerSyncJunctures.ExpansionTeleport(player, teleportPositions1.GetRandomElement(), ori1);
          }
          break;            
      
	//! Teleports the player after tuning-in quest with ID 1002 Destination Black Market
          case 1002:
          {
              array<vector> teleportPositions2 = {"9167.87 236.867 11415.3", "9151.29 233.366 11536.2", "9209.77 233.829 11539.7"};
              vector ori2 = player.GetOrientation();
              DayZPlayerSyncJunctures.ExpansionTeleport(player, teleportPositions2.GetRandomElement(), ori2);
          }
		break;
		//! Teleports the player after tuning-in quest with ID 1003 Destination Fishing Trader
          case 1003:
          {
              array<vector> teleportPositions3 = {"14376.2 6.79 13236.1", "14413.4 3.20859 13288.1"};
              vector ori3 = player.GetOrientation();
              DayZPlayerSyncJunctures.ExpansionTeleport(player, teleportPositions3.GetRandomElement(), ori3);
          }
		break;
		//! Teleports the player after tuning-in quest with ID 1004 Destination Boat Trader
          case 1004:
          {
              array<vector> teleportPositions4 = {"4416.59 3.06608 2247.98", "4349.93 3.23715 2269.65", "4343.09 3.01606 2266.09"};
              vector ori4 = player.GetOrientation();
              DayZPlayerSyncJunctures.ExpansionTeleport(player, teleportPositions4.GetRandomElement(), ori4);
          }
		break;
    //! Teleports the player after tuning-in quest with ID 1005 Destination Chernogorsk
          case 1005:
          {
              array<vector> teleportPositions5 = {"6519.53 73.4047 3444.91", "6493.89 73.2893 3420.34", "6565.26 72.1474 3353.08"};
              vector ori5 = player.GetOrientation();
              DayZPlayerSyncJunctures.ExpansionTeleport(player, teleportPositions5.GetRandomElement(), ori5);
          }
		break;
    //! Teleports the player after tuning-in quest with ID 1006 Destination Elektrozavodsk
          case 1006:
          {
              array<vector> teleportPositions6 = {"9974.4 46.3242 2699.85", "10050.6 39.1765 2651.94", "9952.57 48.2364 2715.41"};
              vector ori6 = player.GetOrientation();
              DayZPlayerSyncJunctures.ExpansionTeleport(player, teleportPositions6.GetRandomElement(), ori6);
          }
		break;
    //! Teleports the player after tuning-in quest with ID 1007 Destination Solnecny
          case 1007:
          {
              array<vector> teleportPositions7 = {"13018.9 115.593 5908.03", "12962.8 119.708 5917.42", "13043.9 108.047 6034.41"};
              vector ori7 = player.GetOrientation();
              DayZPlayerSyncJunctures.ExpansionTeleport(player, teleportPositions7.GetRandomElement(), ori7);
          }
		break;
    //! Teleports the player after tuning-in quest with ID 1008 Destination Berezino
          case 1008:
          {
              array<vector> teleportPositions8 = {"12413.1 17.4778 9751.19", "12045.2 13.2361 9619.35", "12697.2 5.97264 9383.04"};
              vector ori8 = player.GetOrientation();
              DayZPlayerSyncJunctures.ExpansionTeleport(player, teleportPositions8.GetRandomElement(), ori8);
          }
		break;
    //! Teleports the player after tuning-in quest with ID 1009 Destination Krasnoe
          case 1009:
          {
              array<vector> teleportPositions9 = {"6335.72 469.763 14860.1", "6380.68 469.483 15082.1", "6906.4 449.582 14978.7"};
              vector ori9 = player.GetOrientation();
              DayZPlayerSyncJunctures.ExpansionTeleport(player, teleportPositions9.GetRandomElement(), ori9);
          }
		break;
    //! Teleports the player after tuning-in quest with ID 1010 Destination Svyatoy
          case 1010:
          {
              array<vector> teleportPositions10 = {"288.534 525.394 11708.3", "184.534 486.929 12123.2", "491.734 425.529 12108.3"};
              vector ori10 = player.GetOrientation();
              DayZPlayerSyncJunctures.ExpansionTeleport(player, teleportPositions10.GetRandomElement(), ori10);
          }
		break;
	}
      super.Expansion_OnQuestCompletion(quest);
  }
  
	//! Quest Events
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
        }
    }

    override void Expansion_OnQuestCancel(ExpansionQuest quest)
    {
        auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

        switch (quest.GetQuestConfig().GetID())
        {
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
        }
    }

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
        "PrisonUniformJacket",
        "PrisonUniformPants",
        "PrisonerCap",
        "JoggingShoes_White"
      };
      allItems = {
        "greenbellpepper",
        "Apple",
        "BandageDressing"
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
        "ManSuit_Black",
        "SlacksPants_Black",
        "DressShoes_Black"
      };
      allItems = {
        "greenbellpepper",
        "Apple",
        "BandageDressing"
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
        "Hoodie_Blue",
        "Jeans_BlueDark",
        "JoggingShoes_White"
      };
      allItems = {
        "greenbellpepper",
        "Apple",
        "BandageDressing"
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
        "Shirt_BlueCheckBright",
        "CargoPants_Beige",
        "WorkingBoots_Brown",
        "ConstructionHelmet_Orange"
      };
      allItems = {
        "greenbellpepper",
        "Apple",
        "BandageDressing"
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
        "TrackSuitJacket_Red",
        "TrackSuitPants_Red",
        "AthleticShoes_Grey",
        "BaseballCap_Red"
      };
      allItems = {
        "greenbellpepper",
        "Apple",
        "BandageDressing"
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
        "Shirt_GreenCheck",
        "Jeans_Blue",
        "WorkingBoots_Brown"
      };
      allItems = {
        "greenbellpepper",
        "Apple",
        "BandageDressing"
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
        "WorkingBoots_Beige"
      };
      allItems = {
        "greenbellpepper",
        "Apple",
        "BandageDressing"
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
        "HikingBoots_Brown"
      };
      allItems = {
        "greenbellpepper",
        "Apple",
        "BandageDressing"
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
        "WorkingBoots_Grey"
      };
      allItems = {
        "greenbellpepper",
        "Apple",
        "BandageDressing"
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
        "PoliceCap",
        "PoliceJacket",
        "PolicePants",
        "DressShoes_Black"
      };
      allItems = {
        "greenbellpepper",
        "Apple",
        "BandageDressing"
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
        "TacticalShirt_Olive",
        "BDUPants",
        "JungleBoots_Olive"
      };
      allItems = {
        "greenbellpepper",
        "Apple",
        "BandageDressing"
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

  override void StartingEquipSetup(PlayerBase player, bool clothesChosen) {
    // Start By Removing all default items regardess of name or state
    player.RemoveAllItems();

    // Get Player Name
    bool playerNameIsSurvivor = false;
    string characterName = player.GetIdentity().GetName();
    characterName.ToLower();

    // Does player name contain Survivor
    if (characterName.Contains("survivor")) {
      playerNameIsSurvivor = true;
    }

    // If Player is Named Survivor Remove All Gear
    if (playerNameIsSurvivor) {
      player.RemoveAllItems();
      GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(SurvivorDetected, 5000, true, player);
    } else {
      SetCareerPath(player);
    }
  }

  // Send player message if they have Survivor as a name
  protected void SurvivorDetected(PlayerBase player) {
    sendPlayerMessage(player, "Since your name is 'Survivor', you spawn NAKED!");
    sendPlayerMessage(player, "If you give yourself a name, you get starter pack with clothes, backpack, basic gear!");
    sendPlayerMessage(player, "Because we are an Hard Server it is highly recommended that you pick a character name.");
  }

  protected void sendPlayerMessage(PlayerBase player, string message) {
    if ((player) && (message != "")) {
      Param1 < string > Msgparam;
      Msgparam = new Param1 < string > (message);
      GetGame().RPCSingleParam(player, ERPCs.RPC_USER_ACTION_MESSAGE, Msgparam, true, player.GetIdentity());
      GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).Remove(SurvivorDetected);
    }
  }
};

Mission CreateCustomMission(string path) {
  return new CustomMission();
}