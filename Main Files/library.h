#ifndef LIBRARY_H
#define LIBRARY_H

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>

//REMEMBER TO CLOSE FILE

typedef enum direction
{
	HORIZ, VERT
}Dir;

typedef enum boardsize
{
	rows = 10, cols = 10
}Board;

typedef enum boolean
{
	FALSE, TRUE
}Bool;

typedef enum validchoice
{
	INVALID, VALID
}Valid;

typedef enum ai
{
	MISS, HIT
}AI;

typedef struct ship
{
	char name[11];
	int size;
	char symbol;
}Ship;

typedef struct stats
{
	int num_hits; 
	int num_misses;
	int total_shots;
	double hits_misses_ratio;
}Stats;

/******************************************************************************
*Function: PrintFileContents
*Description: Prints the contents of a .txt file
*Input: const char* filename
*Output: void
******************************************************************************/
void PrintFileContents(const char* filename);

/******************************************************************************
*Function: MainLoop
*Description: Houses the main game framework and includes loops that determine certain game functionality
*Input: void
*Output: void
******************************************************************************/
void MainLoop(void);

/******************************************************************************
*Function: IntroScreen
*Description: Prints the into screen when game first starts
*Input: void
*Output: void
******************************************************************************/
void IntroScreen(void);

/******************************************************************************
*Function: GenerateGameBoard
*Description: Initialized the game board with ~ symbols
*Input: char board[10][10], int num_rows, int num_cols
*Output: void
******************************************************************************/
void GenerateGameBoard(char board[10][10], int num_rows, int num_cols);

/******************************************************************************
*Function: PrintBoard
*Description: Prints a 2d array
*Input: char board[10][10], int num_rows, int num_cols
*Output: void
******************************************************************************/
void PrintBoard(char board[10][10], int num_rows, int num_cols);

/******************************************************************************
*Function: PlaceShipsChoice
*Description: Prompts the user whether they wan't to manually or automatically place ships
*Input: char board[10][10]
*Output: void
******************************************************************************/
void PlaceShipsChoice(char board[10][10]);

/******************************************************************************
*Function: PlaceShipsRandom
*Description: Places ships on 2d array randomly
*Input: char board[10][10], Ship* ship
*Output: void
******************************************************************************/
void PlaceShipsRandom(char board[10][10], Ship* ship);

/******************************************************************************
*Function: PlaceShipsManually
*Description: Allows the user to place ships manually by entering a starting point and direction
*Input: char board[10][10], Ship* ship
*Output: void
******************************************************************************/
void PlaceShipsManually(char board[10][10], Ship* ship);

/******************************************************************************
*Function: P1Shoot
*Description: prompts the user to shoot and proccesses the decision
*Input: char board[10][10], char board2[10][10], int* Winner_ptr, Stats* P1, Stats* P2, FILE* outFile
*Output: void
******************************************************************************/
void P1Shoot(char board[10][10], char board2[10][10], int* Winner_ptr, Stats* P1, Stats* P2, FILE* outFile);

/******************************************************************************
*Function: P2Shoot
*Description: Generates a number for the robot player (player2) and processes the outcome
*Input: char board[10][10], int* Winner_ptr, Stats* P1, Stats* P2, FILE* outFile
*Output: void
******************************************************************************/
void P2Shoot(char board[10][10], int* Winner_ptr, Stats* P1, Stats* P2, FILE* outFile);

  
#endif
