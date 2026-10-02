| Test ID: | Feature | Test Description |
| :--- | :--- | :--- |
| TC01_Valid_Login | Login | Tests to ensure valid a login is accepted |
| TC02_Login_Before_Other_Transaction | Login | Test to ensure that users can do other transactions only after logging in |
| TC03_Double_Login_Rejected | Login | Test to ensure that users do cannot login again if they are already logged in and haven't logged out |
| TC04_Valid_Logout | Logout | Tests to ensure valid logout is accepted |
| TC05_Logout_Before_Login | Logout | Tests to ensure users can only logout if they had initially logged in |
| TC06_Create_Admin | Create | Tests to ensure admin users can create create other admin user accounts |
| TC07_Create_Buy_Standard | Create | Tests to ensure admin users can create buy standard user accounts |
| TC08_Create_Sell_Standard | Create | Tests to ensure admin users can create sell standard user accounts |
| TC09_Create_By_Non_Admin_Rejected | Create | Tests to ensure users who are not admin cannot create user accounts |
| TC10_Create_Username_Too_Long | Create | Tests to ensure users cannot enter too long of a username |
| TC11_Create_Duplicate_Username | Create | Tests to ensure admin users cannot create user accounts with duplicate names |
| TC12_Delete_Existing_Other_User | Delete | Test to ensure admin users can delete other user accounts |
| TC13_Delete_By_Non_Admin_Rejected | Delete | Tests to ensure that users who are not admin cannot delete other user accounts |
| TC14_Delete_Non_Existent_User | Delete | Tests to ensure admin users cannot delete a non existing user account |
| TC15_Delete_Current_User | Delete | Tests to ensure admin users cannot delete their current admin user account |
| TC16_Delete_User_Cancels_Inventory | Delete | Tests to ensure when admin user deletes a user account, their inventory is cancelled and no other transaction on their inventory is allowed |
| TC17_Sell_Valid_Game | Sell | Test to ensure users can only sell valid games |
| TC18_Sell_By_Buy_Standard_Rejected | Sell | Test to ensure buy standard user accounts cannot sell |
| TC19_Sell_Max_Price | Sell | Test to ensure users can sell at max price |
| TC20_Sell_Over_Max_Price | Sell | Tests to ensure users cannot sell at max price |
| TC21_Sell_Max_Game_Length | Sell | Tests to ensure users can sell a game with the max game name length |
| TC22_Sell_Game_Name_Too_Long | Sell | Tests to ensure users cannot sell a game with a name length over the max |
| TC23_Sell_Duplicate_Game_Name | Sell | Tests to ensure that users can sell games with unique names only |
| TC24_Sell_No_Further_Transactions | Sell | Tests to ensure that when users sell a game, no further transactions should be accepted on a new game for sale until the next session |
| TC25_Buy_Existing_Game | Buy | Test to ensure users can only buy existing games |
| TC26_Buy_By_Sell_Standard_Rejected | Buy | Test to ensure that sell standard user accounts cannot buy games |
| TC27_Buy_Nonexistent_Game | Buy | Test to ensure that users can only buy an existing game |
| TC28_Buy_Insufficient_Credit | Buy | Test to ensure that users can only buy the game if they have enough credit |
| TC29_Buy_Already_Owned_Game | Buy | Tests to ensure rejection if the user tries to buy a game they already have a copy of the game in their collection|
| TC30_Refund_Valid | Refund | Tests to ensure valid refund process is accepted |
| TC31_Refund_By_Non_Admin_Rejected | Refund | Test to ensure non admin user accounts cannot complete refunds |
| TC32_Refund_Invalid_Buyer | Refund | Tests to ensure the refund process is rejected if users input invalid buyer |
| TC33_Refund_Invalid_Seller | Refund | Tests to ensure the refund process is rejected if users input invalid seller |
| TC34_Add_Credit_Standard | Add Credit | Tests to ensure a valid add credit process for standard user accounts |
| TC35_Add_Credit_Admin_To_Existing_User | Add Credit | Tests to ensure admin user accounts properly input existing users for add credit process |
| TC36_Add_Credit_Admin_Invalid_User | Add Credit | Tests to ensure admin user accounts cannot add credit to invalid user accounts |
| TC37_Add_Credit_Max_1000 | Add Credit | Tests to ensure valid amount of credit is accepted |
| TC38_Add_Credit_Over_1000 | Add Credit | Tests to ensure over that users can not input over credit max  |
| TC39_Invalid_Command | Command Prompt | Tests to ensure users are inputting valid command prompts |
| TC40_Bad_Input_Does_Not_Crash | Input | Tests to ensure invalid prompts does not crash system|
| TC41_Daily_Transaction_End_Code | Daily Transaction File | Tests the production of transaction codes |
| TC42_Transaction_Record_Create_Format | Daily Transaction File | Tests to ensure valid transaction format for create is accepted |
| TC43_Transaction_Record_Sell_Format | Daily Transaction File | Tests to ensure valid transaction format for sell is accepted |
| TC44_Transaction_Record_Buy_Format | Daily Transaction File | Tests to ensure valid transaction format for buy is accepted |
| TC45_Transaction_Record_Refund_Format | Daily Transaction File | Tests to ensure valid transaction format for refund is accepted |
| TC46_Transaction_Record_Add_Credit_Format | Daily Transaction File | Tests to ensure valid transaction format for add_credit is accepted |
| TC47_Non_Admin_Only_Unprivleged | User Roles | Tests to ensure non admins have access to non privileged transactions |
| TC48_Admin_Allows_All_Transaction_Types | User Roles | Test to ensure admins have access to all privileged transactions |
