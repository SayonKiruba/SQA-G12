Create Master Test Table

Requirements:

* should ask for the new username (as a text line)  
  * Success:  
    * System asks for the new user name  
* then should ask for the type of user (admin or full-standard, buy-standard, sell-standard)  
  * Success  
    * System asks for the type of user  
* should save this information to the daily transaction file  
  * Success  
    * System saves information to the daily transaction file

Constraints:

* privileged transaction \- only accepted when logged in as admin user   
  * Success  
    * For privileged transactions: allows admins users  
    * For privileged transactions: does not allow any other users  
* new user name is limited to at most 15 characters  
  * Success  
    * For new user name: \<=15 character username is allowed  
    * For new user name: \>15 character username is rejected  
* new user names must be different from all other current users  
  * Success  
    * For new user name: unique username is allowed  
    * For new user name: recurring username is rejected  
* maximum credit can be 999,999  
  * Success  
    * When loading credits: balance of \<= 999,999 is allowed  
    * When loading credits: balance of \> 999,999 is rejected

| Test Name | Test Intention |
| ----- | ----- |
| test-create-01 | During user creation: test system to ensure an admin can successfully create a new user.  |
| test-create-02 | During the username process: test system to ensure the system asks for the new user's username.  |
| test-create-03 | During the user-type process: test system to ensure the system asks for the new user's account type.   |
| test-create-04 | During user creation: test system to ensure the created user's information is correctly saved to the Daily Transaction File.  |
| test-create-05 | For privileged transactions: test system to ensure CREATE is rejected when the current user is not an admin.  |
| test-create-06 | During user creation: test system to ensure a username of exactly 15 characters is accepted.   |
| test-create-07 | During user creation: test system to reject a username longer than 15 characters.  |
| test-create-08 | For new user name: test system to make sure  unique username is allowed |
| test-create-09 | During user creation: test system to reject a username that already belongs to a current user.  |
| test-create-10 | During user creation: test system to ensure the maximum allowed credit of 999,999 is accepted.  |
| test-create-11 | During user creation: test system to reject credit greater than 999,999.   |

Delete Master Test Table

Requirements

* cancel any games for sale and remove the user account.  
  * Success  
    * Cancel all games for sale under user & Removes the user account  
* should ask for the username (as a text line)  
  * Success  
    * System asks for the user name  
* should save this information for the daily transaction file  
  * Success  
    * System saves the deletion process into the daily transaction files

Constraints:

* privileged transaction \- only accepted when logged in as admin user  
  * Success  
    * For privileged transactions: deletion method is only allowed for admin users  
    * For privileged transactions: the deletion method is not allowed for other any other users  
* username must be the name of an existing user but not the name of the current user  
  * Success  
    * During deletion: the entered username \[for deletion\] must be the name of an already existing user but not the name of the current user  
    * During deletion: the system rejects unique usernames that aren't in the system or the name of the current user  
* no further transactions should be accepted on a deleted user’s available inventory of games.  
  * Success  
    * When a user is deleted: no further transactions is accepted on users available inventory  
    *  When a user is delete: any sort of transaction attempted on users available inventory is rejected (same thing as above, thus only 1 test is needed)

| Test Name | Test Intention |
| ----- | ----- |
| test-delete-01 | For canceling/removing user account: test system to ensure all games for sale under user are canceled and user account is removed |
| test-delete-02 | During the username process: test system to ensure system asks for user name |
| test-delete-03 | During the deletion process: tests system to ensure deletion information is saved to the daily transaction file.  |
| test-delete-04 | For privileged transactions: test system to make sure deletion method is only allowed for admin users |
| test-delete-05 | For privileged transactions: test system to make sure deletion method is not allowed for any other users |
| test-delete-06 | During deletion: test system to make sure entered username is an existing user but not the current user |
| test-delete-07 | During deletion: test system to reject usernames that aren't in the system. |
| test-delete-08 | During deletion: test system to ensure the current user cannot delete their own account.  |
| test-delete-09 | When a user is deleted: test system to ensure any transaction attempted on the user's available inventory is rejected.  |

Logout Master Test Table

Requirement:

* end a Front End session  
  * Success:  
    * When ending the frontend session: test system to ensure it correctly logs out users  
* should write out the daily transaction file (see below) and stop accepting any transactions except login  
  * Success  
    * When logging out of the front end session: test system to ensure it writes the logout process correctly in the daily transaction file   
    * For the transactions: ensure system does not allow any other transactions after an account has logged out, with the sole exception of login (for when user comes back)

Constraints:

* should only be accepted when logged in  
  * Success:  
    * When logging out of the front end session: ensure that the system only logs out user only if they are logged in  
    * When Logging out of the front end session: ensure that the system does not go through with the logout procedure if user was already logged out  
* no transaction other than login should be accepted after a logout

| Test Name | Test Intention |
| ----- | ----- |
| test-logout-01 | When ending the frontend session: test system to ensure it correctly logs out users |
| test-logout-02 | When logging out of the front end session: test system to ensure it writes the logout process correctly in the daily transaction file |
| test-logout-03 | For transactions after logout: test system to ensure it stops accepting any transactions except login |
| test-logout-05 | When logging out: test system to ensure logout is rejected if the user is not logged in.  |

Directories

* /tests  
  * /logout  
    * /test-logout-01  
      * input.txt  
      * expected.txt  
      * daily-transaction.txt  
    * /test-logout-02  
      * input.txt  
      * expected.txt  
      * daily-transaction.txt  
    * /test-logout-03  
      * input.txt  
      * expected.txt  
      * daily-transaction.txt  
    * /test-logout-04  
      * input.txt  
      * expected.txt  
      * daily-transaction.txt  
    * /test-logout-05  
      * input.txt  
      * expected.txt  
      * daily-transaction.txt

  * /create  
    * test-create-01  
    * test-create-02  
    * test-create-03  
    * test-create-04  
    * test-create-05  
    * test-create-06  
    * test-create-07  
    * test-create-08  
    * test-create-09  
    * test-create-10  
    * test-create-11

  * /delete  
    * test-delete-01  
    * test-delete-02  
    * test-delete-03  
    * test-delete-04  
    * test-delete-05  
    * test-delete-06  
    * test-delete-07  
    * test-delete-08  
    * test-delete-09
