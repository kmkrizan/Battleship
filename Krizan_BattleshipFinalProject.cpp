/* Final Project: Battleship 
Author: Kristina Krizan
Date last modified: 5/7/18
*/
#include <iostream>
#include <iomanip>
#include <fstream> 
#include <string>
#include <ctime>
using namespace std;

//characters displayed on baord
const char OCEAN = '~';
const char EMPTY = ' ';
const char HIT = 'X';
const char AIRCRAFT = 'A';
const char BATTLESHIP = 'B';
const char CRUISER = 'C';
const char SUB = 'S';
const char DESTROYER = 'D';

//lengths of ships
const int AIRCRAFT_LEN = 5;
const int BATTLESHIP_LEN = 4;
const int CRUISER_LEN = 3;
const int SUB_LEN = 3;
const int DESTROYER_LEN = 2;

//board dimensions
const int MAX_ROWS = 10;
const int MAX_COL = 10;

//player types 
const int PLAYER = 0;
const int COMPUTER = 1;

//allows computer to keep track of previous hits
bool sink = false;
int sinkRow = 0;
int sinkCol = 0;
int sinkDirection = 0;
int sinkOffset = 0;

//This function initializes the board to blanks 
//accepts a pointer to 2-D array so that both boards can be initialized
void initializeBoard(char(&board)[MAX_ROWS][MAX_COL]) {
	for(int row = 0; row<MAX_ROWS; row++){
		for(int col = 0; col<MAX_COL; col++){
			board[row][col] = ' ';
		}
	}
}

//displays the game boards from the player's perspective 
//accepts a pointer to 2-D array and the type of board to control the user's perspective
void displayGameBoard(char(&boardPlayer)[MAX_ROWS][MAX_COL], int type) {

	char cols[] = "ABCDEFGHIJKLMNO";          //first row
	cout << "     ";
	for(int col = 0; col<MAX_COL; col++){
		cout << cols[col] << " ";
	}
	cout << endl;

	cout << "    -";
	for(int col = 0; col<MAX_COL; col++){    //second row 
		cout << "--";
	}
	cout << "-";
	cout << endl;

	for(int row = 0; row<MAX_ROWS; row++){
		if(row<9)
			cout << " ";
		cout << row + 1 << "  |";
		for(int col = 0; col<MAX_COL; col++){
			if(boardPlayer[row][col] == EMPTY){
				cout << EMPTY << " ";
			}
			else if(boardPlayer[row][col] == AIRCRAFT
				|| boardPlayer[row][col] == BATTLESHIP
				|| boardPlayer[row][col] == CRUISER
				|| boardPlayer[row][col] == SUB
				|| boardPlayer[row][col] == DESTROYER){
				if(type == PLAYER){      //player can see ships
					cout << boardPlayer[row][col] << " ";
				}
				if(type == COMPUTER){    //computer cannot see player's ships
					cout << EMPTY << " ";
				}
			}
			else if(boardPlayer[row][col] == tolower(AIRCRAFT)
				|| boardPlayer[row][col] == tolower(BATTLESHIP)
				|| boardPlayer[row][col] == tolower(CRUISER)
				|| boardPlayer[row][col] == tolower(SUB)
				|| boardPlayer[row][col] == tolower(DESTROYER)){
				cout << HIT << " ";
			}
			else if(boardPlayer[row][col] == OCEAN)
				cout << OCEAN << " ";
		}
		cout << "|" << endl;

	}
	cout << "    -";              //last row 
	for(int col = 0; col<MAX_COL; col++){
		cout << "--";
	}
	cout << "-";
	cout << endl << endl;
}

//displays the actual characters stored in boards
//used for debugging 
void displayBoard(char(&boardPlayer)[MAX_ROWS][MAX_COL]) {
	char cols[] = "ABCDEFGHIJKLMNO";
	cout << "     ";
	for(int col = 0; col<MAX_COL; col++){
		cout << cols[col];
	}
	cout << endl;

	cout << "    -";
	for(int col = 0; col<MAX_COL; col++){
		cout << "-";
	}
	cout << "-";
	cout << endl;
	for(int row = 0; row<MAX_ROWS; row++){
		if(row<9)
			cout << " ";
		cout << row + 1 << "  |";
		for(int col = 0; col<MAX_COL; col++)
			cout << boardPlayer[row][col];
		cout << "|" << endl;
	}
	cout << "    -";
	for(int col = 0; col<MAX_COL; col++){
		cout << "-";
	}
	cout << "-";
	cout << endl;
}

//verifies if the coordinates entered by user are acceptable for placement of ship 
//if valid, ship is placed on board using given coordinates
//accepts a pointer to 2-D array to be used by player and computer
bool addShip(char(&board)[MAX_ROWS][MAX_COL], int row, int col, int dir, char ship, int len) {
	bool added = true;
	int c;
	int r;

	switch(dir){
	case 0:          //Up
		if((row - (len-1))<0){                    //checks if coordinates are out of range
			added = false;
		}
		for(r = row; r>(row - len); r--){         //checks if a ship is already in coordinates
			if(board[r][col] != ' '){
				added = false;
			}
		}
		if(added){
			for (r = row; r>(row - len); r--){
				board[r][col] = ship;
			}
		}
		break;
	case 2:          //Right
		if((col + len)>MAX_COL){                 //checks if coordinates are out of range
			added = false;
		}
		for(c = col; c<(col + len); c++){        //checks if  a ship is already in coordinates
			if(board[row][c] != ' '){
				added = false;
			}
		}
		if(added){
			for(c = col; c<(col + len); c++){
				board[row][c] = ship;
			}
		}
		break;
	case 1:          //Down
		if((row + len)>MAX_ROWS){                 //checks if coordinates are out of range
			added = false;
		}
		for(r = row; r<(row + len); r++){         //checks if a ship is already in coordinates
			if(board[r][col] != ' '){
				added = false;
			}
		}
		if(added){
			for(r = row; r<(row + len); r++){
				board[r][col] = ship;
			}
		}
		break;
	case 3:          //Left
		if((col - (len-1))<0){                    //checks if coordinates are out of range
			added = false;
		}
		for(c = col; c>(col - len); c--){         //checks if a ship is already in coordinates
			if(board[row][c] != ' '){
				added = false;
			}
		}
		if(added){
			for(c = col; c>(col - len); c--){
				board[row][c] = ship;
			}
		}
		break;
	}

	return added;
}

//allows user to add all types of ships and asks for a direction to place the ships
//accepts a pointer to 2-D array to be used by player and computer
void loadShip(char(&boardPlayer)[MAX_ROWS][MAX_COL], char ship, int len) {
	int row;
	int col;
	int dir;
	char ch;
	bool valid = false;
	char maxCol = 'A' + MAX_COL - 1;

	displayGameBoard(boardPlayer, PLAYER);

	do{
		do{
			switch (ship) {
			case 'A':
				cout << "Enter coordinates for your Aircraft Carrier-length of 5.\n";
				break;
			case 'B':
				cout << "Enter coordinates for your Battleship-length of 4.\n";
				break;
			case 'C':
				cout << "Enter coordinates for your Cruiser-length of 3.\n";
				break;
			case 'S':
				cout << "Enter coordinates for your Submarine-length of 3.\n";
				break;
			case 'D':
				cout << "Enter coordinates for your Destroyer-length 2.\n";
				break;
			}
			cout << "Enter a number 1-" << MAX_ROWS << " for rows and a letter A-" << maxCol << " for columns: ";
			cin >> row >> ch;
			cin.clear();
			cin.ignore(256, '\n');      //prevents program from going into an infinite loop 
			row--;
			col = (int)(toupper(ch) - 65);
			if ((row >= 0 && row<MAX_ROWS) && (col >= 0 && col<MAX_COL)) {
				valid = true;
			}
			else {
				cout << endl << "INVALID COORDINATES.\n";
			}
		} while (!valid);
		valid = false;
		do{
			cout << "Enter a direction. U-up, D-down, L-left, R-right: ";
			cin >> ch;
			cout << endl;
		
			switch(toupper(ch)){
			case 'U':
				dir = 0;
				valid = true;
				break;
			case 'R':
				dir = 2;
				valid = true;
				break;
			case 'D':
				dir = 1;
				valid = true;
				break;
			case 'L':
				dir = 3;
				valid = true;
				break;
			default:
				cout << "INVALID DIRECTION.\n";
			}
		}while(!valid);
		valid = false;
		valid = addShip(boardPlayer, row, col, dir, ship, len);
		if(!valid){
			cout << "SHIP CANNOT BE PLACED\n";
		}
	}while(!valid);
}

//automatically adds computer's ships to board ensuring that ships are in range and do not overlap
void loadShipAuto(char(&boardComputer)[MAX_ROWS][MAX_COL], char ship, int len) {
	int row;
	int col;
	int dir;
	bool added = false;

	do{
		row = rand() % MAX_ROWS;
		col = rand() % MAX_COL;
		dir = rand() % 4;
		added = addShip(boardComputer, row, col, dir, ship, len);
	}while (added == false);
}

//stores data in board for placement of user's ships
void loadBoardPlayer(char(&boardPlayer)[MAX_ROWS][MAX_COL]) {
	loadShip(boardPlayer, AIRCRAFT, AIRCRAFT_LEN);
	loadShip(boardPlayer, BATTLESHIP, BATTLESHIP_LEN);
	loadShip(boardPlayer, CRUISER, CRUISER_LEN);
	loadShip(boardPlayer, SUB, SUB_LEN);
	loadShip(boardPlayer, DESTROYER, DESTROYER_LEN);
	displayGameBoard(boardPlayer, PLAYER);
}

//stores data in board for placement of computer's ship
void loadBoardComputer(char(&boardComputer)[MAX_ROWS][MAX_COL]) {
	loadShipAuto(boardComputer, AIRCRAFT, AIRCRAFT_LEN);
	loadShipAuto(boardComputer, BATTLESHIP, BATTLESHIP_LEN);
	loadShipAuto(boardComputer, CRUISER, CRUISER_LEN);
	loadShipAuto(boardComputer, SUB, SUB_LEN);
	loadShipAuto(boardComputer, DESTROYER, DESTROYER_LEN);
}

//determines if the ship is sunk and returns true if sunk 
//accepts a pointer to 2-D array to be used by player and computer
bool checkSunk(char(&board)[MAX_ROWS][MAX_COL], char ship) {
	bool shipSunk = true;
	ship = toupper(ship);
	for(int row = 0; row<MAX_ROWS; row++){
		for(int col = 0; col<MAX_COL; col++){
			if(board[row][col] == ship)
				shipSunk = false;
		}
	}
	return shipSunk;
}

//updates the board internally based off what user input for coordinates 
//if HIT, will make lowercase letter
//if MISS, will put '~'
//if hit/miss again, board does not change
//outputs a message displaying results of fire
char fire(int x, int y, char(&boardPlayer)[MAX_ROWS][MAX_COL], int type) {
	int row = x;
	int col = y;
	char status = ' ';

	if(boardPlayer[row][col] == EMPTY){
		if (type == 0) {
			cout << "\nYOU MISS\n";
		}
		else{
			cout << "COMPUTER MISS\n\n";
		}
		boardPlayer[row][col] = OCEAN;
	}
	else if(boardPlayer[row][col] == AIRCRAFT
		|| boardPlayer[row][col] == BATTLESHIP
		|| boardPlayer[row][col] == CRUISER
		|| boardPlayer[row][col] == SUB
		|| boardPlayer[row][col] == DESTROYER){
		boardPlayer[row][col] = tolower(boardPlayer[row][col]);
		status = tolower(boardPlayer[row][col]);
		if(checkSunk(boardPlayer, boardPlayer[row][col])){
			status = toupper(boardPlayer[row][col]);
			if(type == 0){
				cout << "\nYOU SUNK THE ";
			}
			else{
				cout << "\nCOMPUTER SUNK ";
			}
			switch(status){
			case 'A':
				cout << "AIRCRAFT CARRIER\n\n";
				break;
			case 'B':
				cout << "BATTLESHIP\n\n";
				break;
			case 'C':
				cout << "CRUISER\n\n";
				break;
			case 'S':
				cout << "SUBMARINE\n\n";
				break;
			case 'D':
				cout << "DESTROYER\n\n";
				break;
			}
		}
		else{
			if(type == 0){
				cout << "\nYOU HIT\n";
			}
			else{
				cout << "COMPUTER HIT\n\n";
			}
		}
	}
	else if(boardPlayer[row][col] == tolower(AIRCRAFT)
		|| boardPlayer[row][col] == tolower(BATTLESHIP)
		|| boardPlayer[row][col] == tolower(CRUISER)
		|| boardPlayer[row][col] == tolower(SUB)
		|| boardPlayer[row][col] == tolower(DESTROYER)){
		if(type == 0) {
			cout << "\nYOU HIT AGAIN\n";
		}
		else{
			cout << "\nCOMPUTER HIT AGAIN\n";
		}
	}
	else if(boardPlayer[row][col] == OCEAN){
		if(type == 0){
			cout << "\nYOU MISS AGAIN\n";
		}
		else{
			cout << "\nCOMPUTER MISS AGAIN\n";
		}
	}

	return status;
}

//uses results of fire to record when a ship has been sunk
void shipStatus(char status, bool &aSunk, bool &bSunk, bool &cSunk, bool &sSunk, bool &dSunk){
	switch (status) {
	case 'A':
		aSunk = true;
		break;
	case 'B':
		bSunk = true;
		break;
	case 'C':
		cSunk = true;
		break;
	case 'S':
		sSunk = true;
		break;
	case 'D':
		dSunk = true;
		break;
	}
}

//takes random guess to fire at player's ships
//if a player's ship has been hit, searches up, down, left, and right until ship is sunk 
void computerGuess(int &row, int &col){
	if(sink){
		sinkOffset++;
		switch(sinkDirection){
		case 0:          //Up
			row = sinkRow - sinkOffset;
			col = sinkCol;
			if(row<0){                       //checks if coordinates are out of range
				sinkOffset = 0;
				sinkDirection++;
				computerGuess(row, col);
			}
			break;
		case 2:          //Right
			row = sinkRow;
			col = sinkCol + sinkOffset;
			if(col>MAX_COL){                 //checks if coordinates are out of range
				sinkOffset = 0;
				sinkDirection++;
				computerGuess(row, col);
			}
			break;
		case 1:          //Down
			row = sinkRow + sinkOffset;
			col = sinkCol;
			if(row>MAX_ROWS){                 //checks if coordinates are out of range
				sinkOffset = 0;
				sinkDirection++;
				computerGuess(row, col);
			}
			break;
		case 3:          //Left
			row = sinkRow;
			col = sinkCol - sinkOffset;
			if(col<0){                       //checks if coordinates are out of range
				cout << "\nThis Should Never Happen\n";
				row = rand() % MAX_ROWS;
				col = rand() % MAX_COL;
				sink = false;
				sinkRow = 0;
				sinkCol = 0;
				sinkDirection = 0;
				sinkOffset = 0;
			}
			break;
		}
	} 
	else{
		row = rand() % MAX_ROWS;
		col = rand() % MAX_COL;
	}
}

//when computer hits a player's ship, puts game in sink mode (computer tries to sink ship)
//in sink mode, if computer misses, computer changes directions
//when computer hits ship, turns off sink mode
void computerCheckGuess(char status, int row, int col){
	string strHit = "abcsd";
	string strSunk = "ABCSD";
	if(sink){
		if(strSunk.find(status) != string::npos){      //Sunk
			sink = false;
			sinkRow = 0;
			sinkCol = 0;
			sinkDirection = 0;
			sinkOffset = 0;
		}
		else if(strHit.find(status) == string::npos){  //Miss
			sinkOffset = 0;
			sinkDirection++;
		}
	}
	else{
		if(strHit.find(status) != string::npos){      //First Hit
			sink = true;
			sinkRow = row;
			sinkCol = col;
			sinkDirection = 0;
			sinkOffset = 0;
		}
	}
}

//main function that runs the Battleship game 
int main() {
	srand((unsigned int)time(0));

	int row = 0;
	char ch;
	int col;
	bool invalidCoordinates = true;
	bool win = false;
	char boardPlayer[MAX_ROWS][MAX_COL];
	char boardComputer[MAX_ROWS][MAX_COL];
	char maxCol = 'A' + MAX_COL - 1;
	bool aPSunk = false;
	bool aCSunk = false;
	bool bPSunk = false;
	bool bCSunk = false;
	bool cPSunk = false;
	bool cCSunk = false;
	bool sPSunk = false;
	bool sCSunk = false;
	bool dPSunk = false;
	bool dCSunk = false;
	char status;
	string strHits = "~abcsd";
	char space = ' ';

	//initializes boards
	initializeBoard(boardPlayer);
	initializeBoard(boardComputer);

	//loads boards
	loadBoardPlayer(boardPlayer);
	loadBoardComputer(boardComputer);

	cout << "\nALL SHIPS HAVE BEEN PLACED. Press enter to begin the game. ";
	cin.ignore();
	cin.get();
	cout << endl << endl;

	//starts game
	do{
		cout << "       COMPUTER'S FLEET\n";
		displayGameBoard(boardComputer, COMPUTER);
		//displayBoard(boardComputer);     //used for debugging 

		invalidCoordinates = true;
		do{           //loops until valid coordinates are entered  
			cout << "Enter a number 1-" << MAX_ROWS << " for rows and a letter A-" << maxCol << " for columns: ";
			cin >> row >> ch;
			row--;
			col = (int)(toupper(ch) - 65);
			if ((row >= 0 && row<MAX_ROWS) && (col >= 0 && col<MAX_COL)){
				invalidCoordinates = false;
			}
			else{
				cout << endl << "INVALID COORDINATES.\n";
			}
		}while (invalidCoordinates);

		//Player fires and checks if win
		status = fire(row, col, boardComputer, PLAYER);
		shipStatus(status, aCSunk, bCSunk, cCSunk, sCSunk, dCSunk);
		if (aCSunk && bCSunk && cCSunk && sCSunk && dCSunk){
			cout << "\nCONGRATULATIONS! YOU WON! You destroyed the enemy's fleet!\n\n";
			win = true;
			continue;
		}
		cout << "Computer's turn. Press enter to continue.\n";
		cin.ignore();
		cin.get();

		//Computer makes guess and checks if guessed before
		do{
			computerGuess(row, col);
			space = boardPlayer[row][col];
		}while (strHits.find(space) != string::npos);

		//Computer fires and checks if win
		status = fire(row, col, boardPlayer, COMPUTER);
		computerCheckGuess(status, row, col);
		shipStatus(status, aPSunk, bPSunk, cPSunk, sPSunk, dPSunk);
		if (aPSunk && bPSunk && cPSunk && sPSunk && dPSunk){
			cout << "\nYOU LOSE. The enemy destroyed your fleet.\n\n";
			win = true;
			continue;
		}
		cout << "          YOUR FLEET\n";
		displayGameBoard(boardPlayer, PLAYER);
		cout << "\nPress enter to continue. ";
		cin.get();
		cout << endl << endl;

	}while (!win);

	return 0;
}