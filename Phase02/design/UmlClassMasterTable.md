| Class | Method | Description |
| --- | --- | --- |
| FrontEnd | -------------------------------------- | Controls program execution, handles user commands, and manages login/logout sessions |
|  | run | Runs the main program loop and accepts commands |
|  | processCommand | Directs commands to the appropriate functions |
|  | login | Validates a username and begins a session |
|  | logout | Ends a session and triggers transaction file output |
| TransactionManager | -------------------------------------- | Processes transactions and manages users, games, and ownership information in memory |
|  | loadData | Loads user, game, and ownership objects using FileManager |
|  | beginSession | Initializes session-specific information |
|  | processTransaction | Directs transaction commands to their corresponding functions |
|  | createUser | Validates and creates a new user account |
|  | deleteUser | Validates and removes an existing user account |
|  | sellGame | Validates and records a game listing |
|  | buyGame | Processes a purchase and updates account balances and ownership |
|  | refund | Transfers a valid refund amount between accounts |
|  | addCredit | Adds credit to an account within the permitted limits |
|  | listGames | Displays all games currently available for sale using the stored game objects |
|  | listAccounts | Displays all active user accounts using the stored user objects |
|  | findUser | Searches for a user by username |
|  | findGame | Searches for an available game by name |
|  | ownsGame | Checks whether a user already owns a game |
| FileManager | -------------------------------------- | Reads the three data files, creates objects from their records, and writes daily transaction files |
|  | readUsers | Reads the Current User Accounts File and returns User objects |
|  | readGames | Reads the Available Games File and returns Game objects |
|  | readCollections | Reads the Game Collection File and returns GameOwnership objects |
|  | saveTransaction | Stores a formatted transaction string in memory |
|  | writeDailyTransactions | Writes a session's transaction records to a TXT file |
|  | clearTransactions | Clears saved transaction records after writing the file |
| User | -------------------------------------- | Represents a user account and stores its type, balance, and session credit information |
|  | hasPermission | Checks whether an account can perform a transaction |
|  | adjustCredit | Updates an account's credit balance within valid limits |
|  | resetSessionCredit | Resets the session's added-credit counter |
| Game | -------------------------------------- | Represents a game listing with its name, seller, price, and listing session |
| GameOwnership | -------------------------------------- | Represents the relationship between a game and its owner |