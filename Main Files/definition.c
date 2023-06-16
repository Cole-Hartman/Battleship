#include "library.h"

/******************************************************************************
*Function: PrintFileContents
*Description: Prints the contents of a .txt file
*Input: const char* filename
*Output: void
******************************************************************************/
void PrintFileContents(const char* filename)
{
	////ALL ASCII ART IS FROM https://patorjk.com/software/taag/#p=display&f=Doom&t=RULES ///////////////////////////////////////////

	FILE* file;
	char line[100];

	file = fopen(filename, "r");

	while (fgets(line, sizeof(line), file) != NULL)
	{
		printf("%s", line);
	}

	fclose(file);
}

/******************************************************************************
*Function: MainLoop
*Description: Houses the main game framework and includes loops that determine certain game functionality
*Input: void
*Output: void
******************************************************************************/
void MainLoop(void)
{
	Stats P1;
	Stats P2;

	FILE* outFile;
	outFile = fopen("Battleship.log", "w");

	//Initialize stats struct values
	P1.num_hits = 0;
	P1.num_misses = 0;
	P1.total_shots = 0;
	P1.hits_misses_ratio = 0;
	P2.num_hits = 0;
	P2.num_misses = 0;
	P2.total_shots = 0;
	P2.hits_misses_ratio = 0;

	char P1board[rows][cols] = { '\0' };
	char P2board[rows][cols] = { '\0' };
	char P2displayboard[rows][cols] = { '\0' };
	int PlayerTurn = 0, Winner = 0;

	GenerateGameBoard(P1board, rows, cols); //Create board for P1
	GenerateGameBoard(P2board, rows, cols); //Create board for P2
	GenerateGameBoard(P2displayboard, rows, cols); //Create display board for P2

	//Create board for P2 (robot)
	Ship carrier = { "carrier", 5, 'c' };
	Ship battleship = { "battleship", 4, 'b' };
	Ship cruiser = { "cruiser", 3, 'r' };
	Ship submarine = { "submarine", 3, 's' };
	Ship destroyer = { "destroyer", 2, 'd' };
	PlaceShipsRandom(P2board, &carrier); 
	PlaceShipsRandom(P2board, &battleship); 
	PlaceShipsRandom(P2board, &cruiser); 
	PlaceShipsRandom(P2board, &submarine); 
	PlaceShipsRandom(P2board, &destroyer); 
	
	IntroScreen();
	PlaceShipsChoice(P1board); //Allow the user to place ships
	
	PlayerTurn = rand() % 2 + 1;
	if (PlayerTurn == 1) //Won't exit either if statement until winner is determined
	{
		do
		{
			system("cls");
			printf("     Your turn!\n");
			printf("\n    PLAYER 2 BOARD:\n");

			P1Shoot(P2board, P2displayboard, &Winner, &P1, &P2, outFile);

			system("cls");
			printf("    Player 2 turn\n");
			printf("\n      YOUR BOARD:\n");
			P2Shoot(P1board, &Winner, &P1, &P2, outFile);

		} while (Winner != 1 && Winner != 2); 
	}
	else if (PlayerTurn == 2)
	{
		do
		{
			system("cls");
			printf("    Player 2 turn\n");
			printf("\n      YOUR BOARD:\n");
			P2Shoot(P1board, &Winner, &P1, &P2, outFile);

			system("cls");
			printf("     Your turn!\n");
			printf("\n    PLAYER 2 BOARD:\n");
			P1Shoot(P2board, P2displayboard, &Winner, &P1, &P2, outFile);

		} while (Winner != 1 && Winner != 2);
	}

	//If you miss 0 times don't divide by 0
	if (P1.num_misses == 0)
	{
		P1.hits_misses_ratio = (P1.num_hits * 100);
	}
	else
	{
		P1.hits_misses_ratio = ((P1.num_hits / P1.num_misses) * 100);
	}

	if (P2.num_misses == 0)
	{
		P2.hits_misses_ratio = (P2.num_hits * 100);
	}
	else
	{
		P2.hits_misses_ratio = ((P2.num_hits / P2.num_misses) * 100);
	}


	if (Winner == 1)
	{
		fprintf(outFile, "\n\n\nPlayer1 wins, Plyaer2 loses\n\n");
		fflush(outFile);
		fprintf(outFile, "*** Player1 Stats ***\n");
		fprintf(outFile, "Number Hits: %d\n", P1.num_hits);
		fprintf(outFile, "Number Misses: %d\n", P1.num_misses);
		fprintf(outFile, "Total Shots: %d\n", P1.total_shots);
		fprintf(outFile, "Hit/Miss Ratio: %.0lf%%\n", P1.hits_misses_ratio);

		fprintf(outFile, "\n\n*** Player2 Stats ***\n");
		fprintf(outFile, "Number Hits: %d\n", P2.num_hits);
		fprintf(outFile, "Number Misses: %d\n", P2.num_misses);
		fprintf(outFile, "Total Shots: %d\n", P2.total_shots);
		fprintf(outFile, "Hit/Miss Ratio: %.0lf%%\n", P2.hits_misses_ratio);

		printf("\nYou have sunk all of the enemies ships! YOU WIN.\n");
		printf("\nTo end the game ");
		system("pause");
		exit(1);
	}
	else if (Winner == 2)
	{
		fprintf(outFile, "\n\nPlayer2 wins, Plyaer1 loses\n\n");
		fflush(outFile);
		fprintf(outFile, "*** Player1 Stats ***\n");
		fprintf(outFile, "Number Hits: %d\n", P1.num_hits);
		fprintf(outFile, "Number Misses: %d\n", P1.num_misses);
		fprintf(outFile, "Total Shots: %d\n", P1.total_shots);
		fprintf(outFile, "Hit/Miss Ratio: %.0lf%%\n", P1.hits_misses_ratio);

		fprintf(outFile, "\n\n*** Player2 Stats ***\n");
		fprintf(outFile, "Number Hits: %d\n", P2.num_hits);
		fprintf(outFile, "Number Misses: %d\n", P2.num_misses);
		fprintf(outFile, "Total Shots: %d\n", P2.total_shots);
		fprintf(outFile, "Hit/Miss Ratio: %.0lf%%\n", P2.hits_misses_ratio);

		printf("\nYour enemy has sunk all your ships :( GAME OVER\n");
		printf("\nTo end the game ");
		system("pause");
		exit(1);
	}

	fclose(outFile);
}

/******************************************************************************
*Function: IntroScreen
*Description: Prints the into screen when game first starts
*Input: void
*Output: void
******************************************************************************/
void IntroScreen(void)
{
	PrintFileContents("BATTLESHIP.txt");
	printf("\n\nRules of the Game: \n");
	printf("1. This is a two player game.\n");
	printf("2. Player1 is you and Player2 is the computer.\n");
	printf("3. The ships come in different sizes\n");
	printf("4. Before the game starts, you must place your ships manually or at random\n");
	printf("5. Players take turns guessing the location of their opponent's ships by entering the row and column.\n");
	printf("6. If the guessed square contains a part of an opponent's ship, it will display a *\n");
	printf("7. If the guessed square misses, it will be display a M\n");
	printf("8. The came continues until one player has sunk all of their opponent's ships.\n");

	printf("\nTo start the game ");
	system("pause");
	system("cls");
}

/******************************************************************************
*Function: GenerateGameBoard
*Description: Initialized the game board with ~ symbols
*Input: char board[10][10], int num_rows, int num_cols
*Output: void
******************************************************************************/
void GenerateGameBoard(char board[10][10], int num_rows, int num_cols)
{
	int row_index = 0, col_index = 0;

	for (; row_index < num_rows; ++row_index) //Controls the row that we're visiting
	{
		for (col_index = 0; col_index < num_cols; ++col_index) //Controls the column that we're visiting
		{
			board[row_index][col_index] = '~';
		}
	}
}

/******************************************************************************
*Function: PrintBoard
*Description: Prints a 2d array
*Input: char board[10][10], int num_rows, int num_cols
*Output: void
******************************************************************************/
void PrintBoard(char board[10][10], int num_rows, int num_cols) //Sorts through board array and prints values  
{
	int row_index = 0, col_index = 0;

	printf("%3d%2d%2d%2d%2d%2d%2d%2d%2d%2d\n",0,1,2,3,4,5,6,7,8,9);

	for (; row_index < num_rows; ++row_index) //Controls the row that we're visiting
	{
		printf("%-2d", row_index);
		for (col_index = 0; col_index < num_cols; ++col_index) //Controls the column that we're visiting
		{
			printf("%-2c", board[row_index][col_index]); //Print board at index...
		}

		putchar('\n');
	}
}

/******************************************************************************
*Function: PlaceShipsChoice
*Description: Prompts the user whether they wan't to manually or automatically place ships
*Input: char board[10][10]
*Output: void
******************************************************************************/
void PlaceShipsChoice(char board[10][10])
{
	int place_ships_choice = 0, invalid_entry = 0;

	Ship carrier = { "carrier", 5, 'c' };
	Ship battleship = { "battleship", 4, 'b' };
	Ship cruiser = { "cruiser", 3, 'r' };
	Ship submarine = { "submarine", 3, 's' };
	Ship destroyer = { "destroyer", 2, 'd' };

	do
	{
		invalid_entry = 0;

		PrintFileContents("SHIP.txt");
		printf("\n\n1. Place ships manually\n");
		printf("2. Place ships at random\n");
		scanf("%d", &place_ships_choice);

		system("cls");

		if (place_ships_choice == 1) //Manually
		{
			PlaceShipsManually(board, &carrier);
			PlaceShipsManually(board, &battleship);
			PlaceShipsManually(board, &cruiser);
			PlaceShipsManually(board, &submarine);
			PlaceShipsManually(board, &destroyer);
		}
		else if (place_ships_choice == 2) //Random
		{
			
			PlaceShipsRandom(board, &carrier); //carrier
			PlaceShipsRandom(board, &battleship); //battleship
			PlaceShipsRandom(board, &cruiser); //cruiser
			PlaceShipsRandom(board, &submarine); //submarine
			PlaceShipsRandom(board, &destroyer); //destroyer
		}
		else
		{
			printf("\nInvalid entry, try again.\n");
			invalid_entry = 1;
		}
	} while (invalid_entry == 1);
}

/******************************************************************************
*Function: PlaceShipsRandom
*Description: Places ships on 2d array randomly
*Input: char board[10][10], Ship* ship
*Output: void
******************************************************************************/
void PlaceShipsRandom(char board[10][10], Ship* ship)
{
	Dir direction; //direction enum
	Bool success = TRUE; //bool enum

	direction = rand() % 2; 

	
	int row = 0, column = 0;

	if (direction == HORIZ)
	{
		do {
			success = TRUE;

			row = rand() % 10;
			column = rand() % (10 - ship->size + 1); //generates column based off the ships length that is for sure within the bounds of the array

			for (int i = 0; i < ship->size; i++) //Checks if the location of where the ship will be placed horizontaly is empty or not
			{
				if (board[row][i + column] != '~')
				{
					success = FALSE;
				}

			}
		} while (success == FALSE); 

		for (int i = 0; i < ship->size; i++) //Places the ship onto the board using it's symbol
		{
			board[row][column + i] = ship->symbol;
		}
	}

	else if (direction == VERT)
	{
		do {
			success = TRUE;

			row = rand() % (10 - ship->size + 1); //generates row based off the ships length that is for sure within the bounds of the array
			column = rand() % 10; 

			for (int i = 0; i < ship->size; i++) //Checks if the location of where the ship will be placed vertically is empty or not
			{
				if (board[i + row][column] != '~')
				{
					success = FALSE;
				}

			}
		} while (success == FALSE);

		for (int i = 0; i < ship->size; i++) //Places the ship onto the board using it's symbol
		{
			board[row + i][column] = ship->symbol;
		}
	}
}

/******************************************************************************
*Function: PlaceShipsManually
*Description: Allows the user to place ships manually by entering a starting point and direction
*Input: char board[10][10], Ship* ship
*Output: void
******************************************************************************/
void PlaceShipsManually(char board[10][10], Ship* ship) //REMEMBER THIS FUNCTION WILL BE CALLED ONCE PER SHIP
{
	Dir direction = HORIZ;
	Valid check = VALID;
	Bool success = TRUE;


	do
	{
		check = VALID;
		system("cls");
		PrintBoard(board, rows, cols);
		printf("\nDo you want to place the %s horizontally or vertically?\n", ship->name);
		printf("0. Horizontally\n1. Vertically\n\n2. Exit\n\n");
		scanf("%d", &direction);
		if (direction != 1 && direction != 0 && direction != 2)
		{
			check = INVALID;
		}
	} while (check == INVALID);

	if (direction == HORIZ)
	{
		int max_col = (9 - ship->size + 1);
		int input_row = 0, input_col = 0;

		do //If check doesn't pass
		{
			system("cls");
			PrintBoard(board, rows, cols);

			do //If user doesn't enter valid numbers
			{
				check = VALID;
				printf("Select a starting row (0-9) and a starting colum (0-%d)\n", max_col);
				printf("Enter values seperated by comma i.e. x,y\n");
				scanf("%d,%d", &input_row, &input_col);
				if (input_row < 0 || input_row > 9 || input_col < 0 || input_col > max_col)
				{
					check = INVALID;
				}
			} while (check == INVALID);


			success = TRUE;
			for (int i = 0; i < ship->size; i++) //CHECK
			{
				if (board[input_row][i + input_col] != '~')
				{
					success = FALSE;
					printf("\nThis starting point will place the %s on top of another ship!\n", ship->name);
					system("pause");
					break;
				}

			}
		} while (success == FALSE);

		for (int i = 0; i < ship->size; i++) //PLACES
		{
			board[input_row][input_col + i] = ship->symbol;
		}

	}


	if (direction == VERT)
	{
		int max_row = (9 - ship->size + 1);
		int input_row = 0, input_col = 0;

		do //If check doesn't pass
		{
			system("cls");
			PrintBoard(board, rows, cols);

			do //If user doesn't enter valid numbers
			{
				check = VALID;
				system("cls");
				PrintBoard(board, rows, cols);
				printf("Select a starting row (0-%d) and a starting colum (0-9)\n", max_row);
				printf("Enter values seperated by comma i.e. x,y\n");
				scanf("%d,%d", &input_row, &input_col);
				if (input_row < 0 || input_row > max_row || input_col < 0 || input_col > 9)
				{
					check = INVALID;
				}
			} while (check == INVALID);


			success = TRUE;
			for (int i = 0; i < ship->size; i++) //CHECK
			{
				if (board[input_row][i + input_col] != '~')
				{
					success = FALSE;
					printf("\nThis starting point will place the %s on top of another ship!\n", ship->name);
					system("pause");
					break;
				}

			}
		} while (success == FALSE);

		for (int i = 0; i < ship->size; i++) //PLACES
		{
			board[input_row + i][input_col] = ship->symbol;
		}

	}

	if (direction == 2)
	{
		printf("Good Game :)");
		exit(1);
	}

}

/******************************************************************************
*Function: P1Shoot
*Description: prompts the user to shoot and proccesses the decision
*Input: char board[10][10], char board2[10][10], int* Winner_ptr, Stats* P1, Stats* P2, FILE* outFile
*Output: void
******************************************************************************/
void P1Shoot(char board[10][10], char board2[10][10], int* Winner_ptr, Stats* P1, Stats* P2, FILE* outFile)
{
	int RowTarget = 0, ColTarget = 0;
	static int HitCarrier = 0, HitBattleship = 0, HitCruiser = 0, HitSubmarine = 0, HitDestroyer = 0, SinkCount = 0;

	PrintBoard(board2, rows, cols);
	printf("\nEnter a row and column to target\n");
	printf("Enter values seperated by comma i.e. x,y\n");
	scanf("%d,%d", &RowTarget, &ColTarget);

	P1->total_shots += 1;

	//Input validation: within board range
	do
	{
		if (RowTarget < 0 || RowTarget > 9 || ColTarget < 0 || ColTarget > 9)
		{

			printf("\nEnter values within the board (0-9)!\n");
			system("pause");

			system("cls");
			printf("     Your turn!\n");
			printf("\n    PLAYER 2 BOARD:\n");

			PrintBoard(board2, rows, cols);
			printf("\nEnter a row and column to target\n");
			printf("Enter values seperated by comma i.e. x,y\n");
			scanf("%d,%d", &RowTarget, &ColTarget);
		}
	} while (RowTarget < 0 || RowTarget > 9 || ColTarget < 0 || ColTarget > 9);

	//Check if coord has been previously selected
	do
	{
		if (board2[RowTarget][ColTarget] == '*' || board2[RowTarget][ColTarget] == 'm')
		{
			printf("\nYou have already targeted this spot!\n");
			system("pause");

			system("cls");
			printf("     Your turn!\n");
			printf("\n    PLAYER 2 BOARD:\n");

			PrintBoard(board2, rows, cols);
			printf("\nEnter a row and column to target\n");
			printf("Enter values seperated by comma i.e. x,y\n");
			scanf("%d,%d", &RowTarget, &ColTarget);
		}
	} while (board2[RowTarget][ColTarget] == '*' || board2[RowTarget][ColTarget] == 'm'); 

	PrintFileContents("MISSILE.txt");
	printf("\nMissile launched...");
	system("pause");

	//check for hit
	if (board[RowTarget][ColTarget] == 'c' && board2[RowTarget][ColTarget] == '~') //carrier
	{
		
		P1->num_hits += 1;
		fprintf(outFile, "Player1: %d,%d hit\n", RowTarget, ColTarget);
		fflush(outFile);

		HitCarrier += 1;
		board2[RowTarget][ColTarget] = '*';

		system("cls");
		printf("     Your turn!\n");
		printf("\n    PLAYER 2 BOARD:\n");

		PrintBoard(board2, rows, cols);
		printf("\nYou hit the enemies carrier!\n");
		system("pause");
	}
	else if (board[RowTarget][ColTarget] == 'b' && board2[RowTarget][ColTarget] == '~') //battleship
	{
		P1->num_hits += 1;
		fprintf(outFile, "Player1: %d,%d hit\n", RowTarget, ColTarget);
		fflush(outFile);

		HitBattleship += 1;
		board2[RowTarget][ColTarget] = '*';

		system("cls");
		printf("     Your turn!\n");
		printf("\n    PLAYER 2 BOARD:\n");
		
		PrintBoard(board2, rows, cols);
		printf("\nYou hit the enemies Battleship!\n");
		system("pause");
	}
	else if (board[RowTarget][ColTarget] == 'r' && board2[RowTarget][ColTarget] == '~') //cruiser
	{
		P1->num_hits += 1;
		fprintf(outFile, "Player1: %d,%d hit\n", RowTarget, ColTarget);
		fflush(outFile);

		HitCruiser += 1;
		board2[RowTarget][ColTarget] = '*';

		system("cls");
		printf("     Your turn!\n");
		printf("\n    PLAYER 2 BOARD:\n");

		PrintBoard(board2, rows, cols);
		printf("\nYou hit the enemies Cruiser!\n");
		system("pause");
	}
	else if (board[RowTarget][ColTarget] == 's' && board2[RowTarget][ColTarget] == '~') //submarine
	{
		P1->num_hits += 1;
		fprintf(outFile, "Player1: %d,%d hit\n", RowTarget, ColTarget);
		fflush(outFile);

		HitSubmarine += 1;
		board2[RowTarget][ColTarget] = '*';

		system("cls");
		printf("     Your turn!\n");
		printf("\n    PLAYER 2 BOARD:\n");

		PrintBoard(board2, rows, cols);
		printf("\nYou hit the enemies Submarine!\n");
		system("pause");
	}
	else if (board[RowTarget][ColTarget] == 'd' && board2[RowTarget][ColTarget] == '~') //destroyer
	{
		P1->num_hits += 1;
		fprintf(outFile, "Player1: %d,%d hit\n", RowTarget, ColTarget);
		fflush(outFile);

		HitDestroyer += 1;
		board2[RowTarget][ColTarget] = '*';

		system("cls");
		printf("     Your turn!\n");
		printf("\n    PLAYER 2 BOARD:\n");

		PrintBoard(board2, rows, cols);
		printf("\nYou hit the enemies Destroyer!\n");
		system("pause");
	}
	else
	{
		P1->num_misses += 1;
		fprintf(outFile, "Player1: %d,%d miss\n", RowTarget, ColTarget);
		fflush(outFile);

		board2[RowTarget][ColTarget] = 'm';

		system("cls");
		printf("     Your turn!\n");
		printf("\n    PLAYER 2 BOARD:\n");

		PrintBoard(board2, rows, cols);
		printf("\nYou missed, prepare your defense!\n");
		system("pause");
	}

	//Search for sink
	if (HitCarrier == 5)
	{
		fprintf(outFile, "Player1: Sunk the enemies Carrier!\n");
		fflush(outFile);

		HitCarrier = 0;
		SinkCount += 1;
		system("cls");
		printf("     Your turn!\n");
		printf("\n    PLAYER 2 BOARD:\n");
		PrintBoard(board2, rows, cols);
		printf("\nYou have sunk the enemies Carrier!\n");
		system("pause");
	}
	else if (HitBattleship == 4)
	{
		fprintf(outFile, "Player1: Sunk the enemies Battleship!\n");
		fflush(outFile);

		SinkCount += 1;
		HitBattleship = 0;
		system("cls");
		printf("     Your turn!\n");
		printf("\n    PLAYER 2 BOARD:\n");
		PrintBoard(board2, rows, cols);
		printf("\nYou have sunk the enemies Battleship!\n");
		system("pause");
	}
	else if (HitCruiser == 3)
	{
		fprintf(outFile, "Player1: Sunk the enemies Cruiser!\n");
		fflush(outFile);

		SinkCount += 1;
		HitCruiser = 0;
		system("cls");
		printf("     Your turn!\n");
		printf("\n    PLAYER 2 BOARD:\n");
		PrintBoard(board2, rows, cols);
		printf("\nYou have sunk the enemies Cruiser!\n");
		system("pause");
	}
	else if (HitSubmarine == 3)
	{
		fprintf(outFile, "Player1: Sunk the enemies Submarine!\n");
		fflush(outFile);

		SinkCount += 1;
		HitSubmarine = 0;
		system("cls");
		printf("     Your turn!\n");
		printf("\n    PLAYER 2 BOARD:\n");
		PrintBoard(board2, rows, cols);
		printf("\nYou have sunk the enemies Submarine!\n");
		system("pause");
	}
	else if (HitDestroyer == 2)
	{
		fprintf(outFile, "Player1: Sunk the enemies Destroyer!\n");
		fflush(outFile);

		SinkCount += 1;
		HitDestroyer = 0;
		system("cls");
		printf("     Your turn!\n");
		printf("\n    PLAYER 2 BOARD:\n");
		PrintBoard(board2, rows, cols);
		printf("\nYou have sunk the enemies Destroyer!\n");
		system("pause");
	}

	//Check if P1 won
	if (SinkCount == 5)
	{
		*Winner_ptr = 1;
	}


}

/******************************************************************************
*Function: P2Shoot
*Description: Generates a number for the robot player (player2) and processes the outcome
*Input: char board[10][10], int* Winner_ptr, Stats* P1, Stats* P2, FILE* outFile
*Output: void
******************************************************************************/
void P2Shoot(char board[10][10], int* Winner_ptr, Stats* P1, Stats* P2, FILE* outFile)
{
	int RowTarget = 0, ColTarget = 0;
	static int HitCarrier = 0, HitBattleship = 0, HitCruiser = 0, HitSubmarine = 0, HitDestroyer = 0, SinkCount = 0;
	static int resetAI = 0;

	//For AI
	static int AI_activate = 0, AI_search = 0, AI_search_level = 0, AI_direction = 0, PreviousRowTarget = 0, PreviousColTarget = 0, AI_found_ship_count = 1, AI_first_level_search_hit_or_miss = 0;

	if (resetAI != 0) //After AI is used once reset the variables for next ship hit
	{
		AI_activate = 0, AI_search = 0, AI_search_level = 0, AI_direction = 0, PreviousRowTarget = 0, PreviousColTarget = 0, AI_found_ship_count = 1, AI_first_level_search_hit_or_miss = 0;
		resetAI = 0;
	}
	
	
	if (AI_activate == HIT) //If on P2's last turn, a ship was hit. CALCULATED SHOT
	{

		//ACTIVATE SEARCH
		if (AI_search_level == 0) //Initial level search around spot hit
		{
			AI_search += 1;

			if (AI_search == 1) //Search 1
			{
				//Search 1 down
				RowTarget = (PreviousRowTarget + 1);
				ColTarget = PreviousColTarget;
				AI_direction = VERT;
			}
			else if (AI_search == 2) //Search 2 (if search 1 was miss)
			{
				//Search 1 left
				RowTarget = PreviousRowTarget;
				ColTarget = (PreviousColTarget - 1);
				AI_direction = HORIZ;
			}
			else if (AI_search == 3) //Search 3 (if search 2 was miss)
			{
				//Search 1 up
				RowTarget = (PreviousRowTarget - 1);
				ColTarget = PreviousColTarget;
				AI_direction = VERT;
			}
			else if (AI_search == 4) //Search 4 (if search 3 was miss)
			{
				//Search 1 right
				RowTarget = PreviousRowTarget;
				ColTarget = (PreviousColTarget + 1);
				AI_direction = HORIZ;
			}
		}
		else if (AI_search_level == 1) //first level search after first search was successful
		{
			//We can determine based off initial hit and hit found from search which direction the ship is
			if (AI_first_level_search_hit_or_miss == 1) //Continually search linearly until end of ship (MISS)
			{
				static int search_increment = 1;
				search_increment += 1;

				if (AI_direction == VERT)
				{
					if (AI_search == 1) //Search down
					{
						RowTarget = (PreviousRowTarget + search_increment);
						ColTarget = PreviousColTarget;
					}
					else if (AI_search == 3) //Search up
					{
						RowTarget = (PreviousRowTarget - search_increment);
						ColTarget = PreviousColTarget;

					}
				}
				else if (AI_direction == HORIZ)
				{
					if (AI_search == 2) //Search left
					{
						RowTarget = PreviousRowTarget;
						ColTarget = (PreviousColTarget - search_increment);
					}
					else if (AI_search == 4) //Search right
					{
						RowTarget = PreviousRowTarget;
						ColTarget = (PreviousColTarget + search_increment);

					}

				}
			}

			if (AI_first_level_search_hit_or_miss != 1)
			{
				if (AI_activate == HIT); //Continue last search until the ship is sunk
				{
					if (AI_first_level_search_hit_or_miss == 0) //Once end of ship is found, search on the other end
					{
						static int search_increment = 0;
						search_increment += 1;

						if (AI_direction == VERT)
						{
							if (AI_search == 1) //If search was previously down, search up
							{
								RowTarget = (PreviousRowTarget - search_increment);
								ColTarget = PreviousColTarget;
							}
							else if (AI_search == 3) //If search was previously up, search down
							{
								RowTarget = (PreviousRowTarget + search_increment);
								ColTarget = PreviousColTarget;

							}
						}
						else if (AI_direction == HORIZ)
						{
							if (AI_search == 2) //If search was previously left, search right
							{
								RowTarget = PreviousRowTarget;
								ColTarget = (PreviousColTarget + search_increment);
							}
							else if (AI_search == 4) //If search was previously right, search left
							{
								RowTarget = PreviousRowTarget;
								ColTarget = (PreviousColTarget - search_increment);

							}

						}

					}
				}
			}
		}

		


	}
	else if (AI_activate == MISS) //If on P2's last turn, no ship was hit. RANDOM SHOT
	{
		
		RowTarget = rand() % 10;
		ColTarget = rand() % 10;

		//Store the last shot in a static variable
		PreviousRowTarget = RowTarget;
		PreviousColTarget = ColTarget; 

	}
	

	


	P2->total_shots += 1;

	//Check if coord has been previously selected
	if (AI_activate == MISS)
	{
		do
		{
			if (board[RowTarget][ColTarget] == '*' || board[RowTarget][ColTarget] == 'm')
			{
				RowTarget = rand() % 10;
				ColTarget = rand() % 10;
				scanf("%d,%d", &RowTarget, &ColTarget);
			}
		} while (board[RowTarget][ColTarget] == '*' || board[RowTarget][ColTarget] == 'm');
	}

	//Search for hit
	if (board[RowTarget][ColTarget] == 'c') //carrier
	{
		P2->num_hits += 1;
		fprintf(outFile, "Player2: %d,%d hit\n", RowTarget, ColTarget);
		fflush(outFile);
		
		if (AI_activate == HIT)
		{
			AI_search_level = 1;
		}

		AI_activate = HIT;
		AI_found_ship_count += 1;

		if (AI_search_level == 1)
		{
			AI_first_level_search_hit_or_miss = HIT;
		}

		HitCarrier += 1;
		board[RowTarget][ColTarget] = '*';

		system("cls");
		printf("    Player 2 turn\n");
		printf("\n      YOUR BOARD:\n");
		PrintBoard(board, rows, cols);
		printf("\nThe enemy has hit your Carrier!\n");

		PrintFileContents("EXPLOSION.txt");
	
		system("pause");
	}
	else if (board[RowTarget][ColTarget] == 'b') //battleship
	{
		P2->num_hits += 1;
		fprintf(outFile, "Player2: %d,%d hit\n", RowTarget, ColTarget);
		fflush(outFile);

		if (AI_activate == HIT)
		{
			AI_search_level = 1;
		}

		AI_activate = HIT;
		AI_found_ship_count += 1;

		if (AI_search_level == 1)
		{
			AI_first_level_search_hit_or_miss = HIT;
		}

		HitBattleship += 1;
		board[RowTarget][ColTarget] = '*';

		system("cls");
		printf("    Player 2 turn\n");
		printf("\n      YOUR BOARD:\n");
		PrintBoard(board, rows, cols);
		printf("\nThe enemy has hit your Battleship!\n");

		PrintFileContents("EXPLOSION.txt");

		system("pause");
	}
	else if (board[RowTarget][ColTarget] == 'r') //cruiser
	{
		P2->num_hits += 1;
		fprintf(outFile, "Player2: %d,%d hit\n", RowTarget, ColTarget);
		fflush(outFile);

		if (AI_activate == HIT)
		{
			AI_search_level = 1;
		}

		AI_activate = HIT;
		AI_found_ship_count += 1;

		if (AI_search_level == 1)
		{
			AI_first_level_search_hit_or_miss = HIT;
		}

		HitCruiser += 1;
		board[RowTarget][ColTarget] = '*';

		system("cls");
		printf("    Player 2 turn\n");
		printf("\n      YOUR BOARD:\n");
		PrintBoard(board, rows, cols);
		printf("\nThe enemy has hit your Cruiser!\n");

		PrintFileContents("EXPLOSION.txt");

		system("pause");
	}
	else if (board[RowTarget][ColTarget] == 's') //submarine
	{
		P2->num_hits += 1;
		fprintf(outFile, "Player2: %d,%d hit\n", RowTarget, ColTarget);
		fflush(outFile);

		if (AI_activate == HIT)
		{
			AI_search_level = 1;
		}

		AI_activate = HIT;
		AI_found_ship_count += 1;

		if (AI_search_level == 1)
		{
			AI_first_level_search_hit_or_miss = HIT;
		}

		HitSubmarine += 1;
		board[RowTarget][ColTarget] = '*';

		system("cls");
		printf("    Player 2 turn\n");
		printf("\n      YOUR BOARD:\n");
		PrintBoard(board, rows, cols);
		printf("\nThe enemy has hit your Submarine!\n");

		PrintFileContents("EXPLOSION.txt");

		system("pause");
	}
	else if (board[RowTarget][ColTarget] == 'd') //destroyer
	{
		P2->num_hits += 1;
		fprintf(outFile, "Player2: %d,%d hit\n", RowTarget, ColTarget);
		fflush(outFile);

		if (AI_activate == HIT)
		{
			AI_search_level = 1;
		}

		AI_activate = HIT;
		AI_found_ship_count += 1;

		if (AI_search_level == 1)
		{
			AI_first_level_search_hit_or_miss = HIT;
		}

		HitDestroyer += 1;
		board[RowTarget][ColTarget] = '*';

		system("cls");
		printf("    Player 2 turn\n");
		printf("\n      YOUR BOARD:\n");
		PrintBoard(board, rows, cols);
		printf("\nThe enemy has hit your Destroyer!\n");

		PrintFileContents("EXPLOSION.txt");

		system("pause");
	}
	else
	{
		P2->num_misses += 1;
		fprintf(outFile, "Player2: %d,%d miss\n", RowTarget, ColTarget);
		fflush(outFile);

		if (AI_search_level == 1) //This should execute after missing on one end for the first time
		{
			AI_first_level_search_hit_or_miss = MISS;
		}

		system("cls");
		printf("    Player 2 turn\n");
		printf("\n      YOUR BOARD:\n");

		board[RowTarget][ColTarget] = 'm';
		PrintBoard(board, rows, cols);
		printf("\nThe enemy missed, your ships are safe\n");
		system("pause");
	}

	//Search for sink
	if (HitCarrier == 5)
	{
		fprintf(outFile, "Player2: Sunk the enemies Carrier!\n");
		fflush(outFile);

		AI_activate = MISS;
		resetAI = 1; //reset AI variables

		HitCarrier = 0;
		SinkCount += 1;
		system("cls");
		printf("    Player 2 turn\n");
		printf("\n      YOUR BOARD:\n");
		PrintBoard(board, rows, cols);
		printf("\nThe enemy has sunk your Carrier!\n");
		system("pause");
	}
	else if (HitBattleship == 4)
	{
		fprintf(outFile, "Player2: Sunk the enemies Battleship!\n");
		fflush(outFile);

		AI_activate = MISS;
		resetAI = 1; //reset AI variables

		HitBattleship = 0;
		SinkCount += 1;
		system("cls");
		printf("    Player 2 turn\n");
		printf("\n      YOUR BOARD:\n");
		PrintBoard(board, rows, cols);
		printf("\nThe enemy has sunk your Battleship!\n");
		system("pause");
	}
	else if (HitCruiser == 3)
	{
		fprintf(outFile, "Player2: Sunk the enemies Cruiser!\n");
		fflush(outFile);

		AI_activate = MISS;
		resetAI = 1; //reset AI variables

		HitCruiser = 0;
		SinkCount += 1;
		system("cls");
		printf("    Player 2 turn\n");
		printf("\n      YOUR BOARD:\n");
		PrintBoard(board, rows, cols);
		printf("\nThe enemy has sunk your Cruiser!\n");
		system("pause");
	}
	else if (HitSubmarine == 3)
	{
		fprintf(outFile, "Player2: Sunk the enemies Submarine!\n");
		fflush(outFile);

		AI_activate = MISS;
		resetAI = 1; //reset AI variables

		HitSubmarine = 0;
		SinkCount += 1;
		system("cls");
		printf("    Player 2 turn\n");
		printf("\n      YOUR BOARD:\n");
		PrintBoard(board, rows, cols);
		printf("\nThe enemy has sunk your Submarine!\n");
		system("pause");
	}
	else if (HitDestroyer == 2)
	{
		fprintf(outFile, "Player2: Sunk the enemies Destroyer!\n");
		fflush(outFile);

		AI_activate = MISS;
		resetAI = 1; //reset AI variables

		HitDestroyer = 0;
		SinkCount += 1;
		system("cls");
		printf("    Player 2 turn\n");
		printf("\n      YOUR BOARD:\n");
		PrintBoard(board, rows, cols);
		printf("\nThe enemy has sunk your Destroyer!\n");
		system("pause");
	}

	//Check if P2 won
	if (SinkCount == 5)
	{
		*Winner_ptr = 2; 
	}


}

