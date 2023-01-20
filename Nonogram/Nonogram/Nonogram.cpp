/**
*
* Solution to course project # <1>
* Introduction to programming course
* Faculty of Mathematics and Informatics of Sofia University
* Winter semester 2022/2023
*
* @author Alexander Spiridonov
* @idnumber 4MI0600117
* @compiler VC
*
* <main>
*
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <random>
using namespace std;

const int MAX_WRONG_ANSWERS = 3;
const int NUM_LEVELS = 3;
const int LEVEL_SIZES[NUM_LEVELS] = { 3, 5, 10 };
const int NUM_PICTURES_PER_LEVEL = 1;

enum CellState { UNKNOWN, FILLED, BLANK };

struct PlayerProfile {
	string username;
	int currentLevel;
	int wrongAnswers;
	int currFilled;
	vector<vector<CellState>> currentGrid;
};

vector<vector<vector<int>>> pictures =
{
	{
	   { 0,1,1},
	   { 0,1,0},
	   { 1,1,1},
   },
   {
	   { 0,0,0,1,1},
	   { 1,0,1,1,1},
	   { 1,1,0,0,1},
	   { 0,0,0,1,1},
	   { 0,1,0,1,1},
   },
	{
		{ 0,0,0,1,1,1,1,0,0,0},
		{ 0,0,1,1,0,0,1,1,0,0},
		{ 0,1,1,0,0,0,0,1,1,0},
		{ 0,1,1,0,0,0,0,1,1,0},
		{ 0,1,1,0,0,0,0,1,1,0},
		{ 0,0,1,1,0,0,1,1,0,0},
		{ 0,0,0,1,1,1,1,0,0,0},
		{ 0,0,1,1,1,1,1,1,0,0},
		{ 0,1,1,1,1,1,1,1,1,0},
		{ 1,1,1,1,1,1,1,1,1,1},

	}
};

vector<vector<vector<int>>> hints =
{
	{
	   { 2 },
	   { 1 },
	   { 3 },
	   { 1 },
	   { 3 },
	   { 1,1 },
	},
   {
	   { 2},
	   { 1,3},
	   { 2 ,1},
	   { 2},
	   { 1,2},
	   { 2},
	   { 1,1},
	   { 1},
	   { 2,2},
	   { 5},
   },
	{
		{4},
		{2, 2},
		{2, 2},
		{2, 2},
		{2, 2},
		{2, 2},
		{4},
		{6},
		{8},
		{10},
		{1},
		{3, 2},
		{5, 3},
		{2, 5},
		{1, 4},
		{1, 4},
		{2, 5},
		{5, 3},
		{3, 2},
		{1},

	}
};

PlayerProfile currentPlayer;


PlayerProfile loadProgress(string username) {
	ifstream inputFile(username + ".txt");
	PlayerProfile profile;
	profile.username = username;

	inputFile >> profile.currentLevel >> profile.wrongAnswers >> profile.currFilled;
	int gridSize = LEVEL_SIZES[profile.currentLevel];
	profile.currentGrid.resize(gridSize, vector<CellState>(gridSize));
	for (int i = 0; i < gridSize; i++) {
		for (int j = 0; j < gridSize; j++) {
			int cell;
			inputFile >> cell;
			profile.currentGrid[i][j] = (CellState)cell;
		}
	}
	for (int i = 0; i < gridSize; i++) {
		for (int j = 0; j < gridSize; j++) {
			if (profile.currentGrid[i][j] == FILLED)
				profile.currFilled++;
		}
	}
	inputFile.close();//
	return profile;
}


bool login() {
	cout << "Login" << endl;
	cout << "Enter your username: ";
	string username;
	cin >> username;

	// Check if the username exists in the profiles folder
	ifstream inputFile(username + ".txt");
	if (inputFile.good()) {
		currentPlayer = loadProgress(username);
		return true;
	}
	else {
		return false;
	}
}

void registerPlayer() {
	cout << "Enter your username: ";
	string username;
	cin >> username;

	ofstream stream(username + ".txt");
	int gridSize = LEVEL_SIZES[0];
	stream << 0 << " " << 0 << " " << 0 << endl;
	for (int i = 0; i < gridSize; i++) {
		for (int j = 0; j < gridSize; j++) {
			stream << (int)UNKNOWN << " ";
		}
		stream << endl;
	}
	stream.close();
}
void saveProgress(PlayerProfile profile) {
	ofstream outputFile(profile.username + ".txt");
	outputFile << profile.currentLevel << " " << profile.wrongAnswers << " " << profile.currFilled << endl;
	int gridSize = LEVEL_SIZES[profile.currentLevel];
	for (int i = 0; i < gridSize; i++) {
		for (int j = 0; j < gridSize; j++) {
			outputFile << (int)profile.currentGrid[i][j] << " ";
		}
		outputFile << endl;
	}
	outputFile.close();
}




void displayMenu() {
	cout << "Welcome, " << currentPlayer.username << "!" << endl;
	cout << "1. Continue last attempt" << endl;
	cout << "2. Start over at level " << currentPlayer.currentLevel << endl;
	cout << "3. Start over at a different level" << endl;
	cout << "4. Exit" << endl;
}

void displayStartMenu() {
	cout << "1. Register" << endl;
	cout << "2. Login" << endl;
}

void resetLevel(int level) {
	PlayerProfile profile;
	currentPlayer.currentLevel = level;
	currentPlayer.wrongAnswers = 0;
	currentPlayer.currFilled = 0;

	int gridSize = LEVEL_SIZES[level];
	currentPlayer.currentGrid = vector<vector<CellState>>(gridSize);
	for (int i = 0; i < gridSize; i++)
	{
		for (int j = 0; j < gridSize; j++)
		{
			currentPlayer.currentGrid[i].push_back(UNKNOWN);
		}
	}
	ofstream outputFile(profile.username + ".txt");
	outputFile << profile.currentLevel << " " << 0 << endl;

	for (int i = 0; i < gridSize; i++) {
		for (int j = 0; j < gridSize; j++) {
			outputFile << 0 << " ";
		}
		outputFile << endl;
	}
	outputFile.close();
}

void playLevel(int level) {
	int gridSize = LEVEL_SIZES[level];
	int maxWrongAnswers = MAX_WRONG_ANSWERS;
	int maxFilled = 0;
	//int currFilled = 0;
	for (int i = 0; i < pictures[currentPlayer.currentLevel].size(); i++)
	{
		for (int j = 0; j < pictures[currentPlayer.currentLevel][i].size(); j++)
		{
			if (pictures[currentPlayer.currentLevel][i][j] == (int)FILLED)
				maxFilled++;
		}
	}

	while (currentPlayer.wrongAnswers < maxWrongAnswers) {
		// Print the grid
		for (int i = 0; i < gridSize; i++) {
			for (int j = 0; j < gridSize; j++) {
				char c;
				if (currentPlayer.currentGrid[i][j] == UNKNOWN) {
					c = '?';
				}
				else if (currentPlayer.currentGrid[i][j] == BLANK) {
					c = ' ';
				}
				else {
					c = '#';
				}
				cout << c << " ";
			}
			cout << endl;
		}

		// Print the row and column clues
		for (int i = 0; i < gridSize; i++) {
			cout << "Row " << i + 1 << " clues: ";
			// print clues for row i
			for (int j = 0; j < hints[currentPlayer.currentLevel][i].size(); j++)
			{
				cout << hints[currentPlayer.currentLevel][i][j] << " ";
			}
			cout << endl;
		}
		for (int j = 0; j < gridSize; j++) {
			cout << "Column " << j + 1 << " clues: ";
			//print clues for column j
			for (int i = 0; i < hints[currentPlayer.currentLevel][j + gridSize].size(); i++)
			{
				cout << hints[currentPlayer.currentLevel][j + gridSize][i] << " ";
			}
			cout << endl;
		}
		cout << "Enter the coordinates of the cell you want to guess (i j): ";
		int i, j;
		cin >> i >> j;
		i--; // Convert the 1-indexed coordinates to 0-indexed
		j--;
		cout << "Is the cell (" << i + 1 << ", " << j + 1 << ") full or blank? (f/b): ";
		char guess;
		cin >> guess;
		if (guess == 'f') {
			if (pictures[currentPlayer.currentLevel][i][j] != FILLED) {
				currentPlayer.wrongAnswers++;
				cout << "Wrong! You have " << maxWrongAnswers - currentPlayer.wrongAnswers << " tries left." << endl;
			}
			else {
				if (currentPlayer.currentGrid[i][j] != FILLED) {
					currentPlayer.currFilled++;
					currentPlayer.currentGrid[i][j] = FILLED;

				}
				// check if the row/column is completed
				bool rowCompleted = true, colCompleted = true;
				for (int k = 0; k < gridSize; k++) {
					if (currentPlayer.currentGrid[i][k] != FILLED) {
						rowCompleted = false;
					}
					if (currentPlayer.currentGrid[k][j] != FILLED) {
						colCompleted = false;
					}
				}
				// if row/column is completed, mark the remaining cells as empty
				if (rowCompleted) {
					for (int k = 0; k < gridSize; k++) {
						if (currentPlayer.currentGrid[i][k] == UNKNOWN) {
							currentPlayer.currentGrid[i][k] = BLANK;
						}
					}
				}
				if (colCompleted) {
					for (int k = 0; k < gridSize; k++) {
						if (currentPlayer.currentGrid[k][j] == UNKNOWN) {
							currentPlayer.currentGrid[k][j] = BLANK;
						}
					}
				}
				//saveProgress(currentPlayer);
				// check if the player has completed the picture
				bool completed = true;
				for (int i = 0; i < gridSize; i++) {
					for (int j = 0; j < gridSize; j++) {
						saveProgress(currentPlayer);
						if (currentPlayer.currentGrid[i][j] == UNKNOWN || maxFilled == currentPlayer.currFilled) {
							completed = false;
							break;
						}
					}
					if (!completed) {
						break;
					}

				}

				if (completed || maxFilled == currentPlayer.currFilled) {
					cout << "Congratulations! You completed level " << level << "." << endl;


					if (level == NUM_LEVELS) {
						cout << "You have completed the game!" << endl;
						saveProgress(currentPlayer);

						return;
					}
					else {
						currentPlayer.currentLevel++;
						resetLevel(currentPlayer.currentLevel);
						playLevel(currentPlayer.currentLevel);
						return;
					}
				}
			}
		}
		else {
			currentPlayer.currentGrid[i][j] = BLANK;
		}
	}
	cout << "You have exceeded the maximum number of wrong answers. You lose!" << endl;
	saveProgress(currentPlayer);
}

int main() {
	displayStartMenu();
	cin.clear();
	string option;
	cin >> option;
	if (option[0] == '1')
	{
		registerPlayer();
	}

	if (login()) {
		displayMenu();
		int choice;
		cin >> choice;
		if (choice == 1) {
			// Continue last attempt
			loadProgress(currentPlayer.username);
			playLevel(currentPlayer.currentLevel);
		}
		else if (choice == 2) {
			// Start over at current level
			resetLevel(currentPlayer.currentLevel);
			playLevel(currentPlayer.currentLevel);
		}
		else if (choice == 3) {
			// Start over at a different level
			cout << "Enter the level you want to start over at (0-" << NUM_LEVELS - 1 << "): ";
			int level;
			cin >> level;
			resetLevel(level);
			playLevel(level);
		}
		else if (choice == 4) {
			// Exit
			saveProgress(currentPlayer);
			return 0;
		}
		else {
			cout << "Invalid choice. Exiting game." << endl;
			return 0;
		}
	}
	else {
		cout << "Invalid username. Exiting game." << endl;
		return 0;
	}
}