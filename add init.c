If you want extraction quests to work, then you need add this to your init.c

--------------------------
class CustomMission: MissionServer
{
//This is the script to TP GMT to Traders ower map
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
-------------------------
Thanks MACCA for sharing the settings.
