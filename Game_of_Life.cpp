
#include <iostream>
#include "conio.h"
#include "fstream"

using namespace std;

//struct LOCATION
//{
//	int x;
//	int y;
//};
	int x = 0;
	int y = 0;
	int* liveCells;
	int* _liveLocations;
	static const int ROWS = 4;
	static const int COLUMNS = 4;
	int** grid;
	int** neighborsCountGrid;
	//LOCATION* liveCells;

	int generations;
	int numberOfInitialLiveCells;
	//LOCATION* _liveLocations;
	int lastLiveCell = 0;

	void Game(int genrs, int numOfIniLiveCells, int* liveLoc);	
	void Start();
	void PopulateGrid();
	void PopulateLiveCells();
	bool IsLiveLocation(int location);
	bool IsLocationOutOfBounds(int location);
	int CountLiveNeighbors(int location);
	void InsertLiveCell(int location);
	void DeleteLiveCell(int location);
	void AddNeighbors();
	void PropagateCells();
	void Display();

int main(int argc, char* argv[])
	{
	if (argc == 1)											// If file name is not passed.
		{
			cout << "You must provide the file name for Initial Grid." << endl;
			_getch();
			return 0;
		}

	int genrs = 0, numOfIniLiveCells = 0; int* liveLoc = new int[numOfIniLiveCells];
		/*int generations = 0,
			numberOfInitialLiveCells = 0;
		LOCATION* liveLocations = new LOCATION[numberOfInitialLiveCells];*/
		ifstream file;

		try
		{
			file.open(argv[argc - 1], ios::in);					// Open the file.

			if (!file.is_open())								// If file cannot be opened.
			{
				cout << argv[argc - 1] << " not found." << endl;
				_getch();
				return 0;
			}

			if (file.peek() == -1)								// If file is empty.
			{
				cout << argv[argc - 1] << " is empty." << endl;
				_getch();
				return 0;
			}

			file >> genrs;								// Read number of generations from the file.
			file >> numOfIniLiveCells;					// Read number of initial live cells from the file.
			liveLoc = new int[numOfIniLiveCells];

			for (int i = 0; i < numOfIniLiveCells; i++)
			{
				file >> liveLoc[i];						// Read (x, y) coordiantes of live cells.
				file >> liveLoc[i];
			}

			file.close();										// Close the file.
		}
		catch (const std::exception&)
		{
			cout << "Whoops! Something went wrong." << endl;
			_getch();
			return 0;
		}

		Game(genrs,numOfIniLiveCells,liveLoc);
		Start();

		_getch();
	}

void Game(int genrs, int numOfIniLiveCells, int* liveLoc)
{
	generations = genrs;
	numberOfInitialLiveCells = numOfIniLiveCells;
	_liveLocations = liveLoc;

	grid = new int*[ROWS];
	neighborsCountGrid = new int*[ROWS];
	liveCells = new int[16];
	PopulateGrid();												// Populate grid with 0's and 1's
	PopulateLiveCells();	
									// Populate UnInit Array (Array of Locations of Live Cells)
}

/// Populates two-dimensional dynamic array "grid".
/// Instantiates two-dimensional dynamic array "neighborsCountGrid".
void PopulateGrid()
{
	for (int i = 0; i < ROWS; i++)
	{
		grid[i] = new int[COLUMNS];
		neighborsCountGrid[i] = new int[COLUMNS];				// Instaniate each row of "neighborsCountGrid" with "COLUMNS" columns.

		for (int j = 0; j < COLUMNS; j++)
		{
			if (IsLiveLocation(i,j)
				grid[i][j] = 1;									// If (i, j) is a live location, then cell (i, j) is 1.
			else
				grid[i][j] = 0;									// Otherwise, cell (i, j) is 0.
		}
	}
}

/// Returns true if location is a live location.
/// Return false, otherwise.
bool IsLiveLocation(LOCATION location)
{
	for (int i = 0; i < numberOfInitialLiveCells; i++)
		if (_liveLocations[i].x == location.x &&
			_liveLocations[i].y == location.y)					// If "_liveLocations" contains "location", then return true.
			return true;

	return false;												// Return false, otherwise.
}

/// Populates UnInit Array "liveCells" with the locations of initial live cells.
void PopulateLiveCells()
{
	for (int i = 0; i < numberOfInitialLiveCells; i++)
		InsertLiveCell(_liveLocations[i]);						// For each live location in "_liveLocations", insert it in liveCells.
}

/// Returns the number of live neighbors of given location.
/// This method uses 8-Neigbor Connectivty.
int CountLiveNeighbors(LOCATION location)
{
	int count = 0;

	if ((!IsLocationOutOfBounds({ (location.x - 1), (location.y - 1) })) &&
		(grid[location.x - 1][location.y - 1] == 1))
		count++;												//	Check neighbor (x - 1, y - 1).

	if ((!IsLocationOutOfBounds({ (location.x - 1), (location.y) })) &&
		(grid[location.x - 1][location.y] == 1))
		count++;												//	Check neighbor (x - 1, y).

	if ((!IsLocationOutOfBounds({ (location.x - 1), (location.y + 1) })) &&
		(grid[location.x - 1][location.y + 1] == 1))
		count++;												//	Check neighbor (x - 1, y + 1).

	if ((!IsLocationOutOfBounds({ (location.x), (location.y - 1) })) &&
		(grid[location.x][location.y - 1] == 1))
		count++;												//	Check neighbor (x, y - 1).

	if ((!IsLocationOutOfBounds({ (location.x), (location.y + 1) })) &&
		(grid[location.x][location.y + 1] == 1))
		count++;												//	Check neighbor (x, y + 1).

	if ((!IsLocationOutOfBounds({ (location.x + 1), (location.y - 1) })) &&
		(grid[location.x + 1][location.y - 1] == 1))
		count++;												//	Check neighbor (x + 1, y - 1).

	if ((!IsLocationOutOfBounds({ (location.x + 1), (location.y) })) &&
		(grid[location.x + 1][location.y] == 1))
		count++;												//	Check neighbor (x + 1, y).

	if ((!IsLocationOutOfBounds({ (location.x + 1), (location.y + 1) })) &&
		(grid[location.x + 1][location.y + 1] == 1))
		count++;												//	Check neighbor (x + 1, y + 1).

	return count;
}

/// Returns true if location is out of bounds, false otherwise.
bool IsLocationOutOfBounds(LOCATION location)
{
	if ((location.x < 0 || location.x >(ROWS - 1)) ||
		(location.y < 0 || location.y >(COLUMNS - 1)))
		return true;

	return false;
}

/// Inserts location of a live cell in "liveCells" array.
void InsertLiveCell(LOCATION location)
{
	try
	{
		liveCells[lastLiveCell++] = location;					// Enter the location in "liveCells" and increment "lastLiveCell" by 1.
	}
	catch (const exception&)
	{
		cout << "Whoops! I think something went wrong." << endl;
	}
}

/// Delete a live cell at location "location".
void DeleteLiveCell(LOCATION location)
{

	for (int i = 0; i < lastLiveCell; i++)						// For All live-cell locations in "liveCells" up to "lastLiveCell - 1".
	{
		if (liveCells[i].x == location.x &&
			liveCells[i].y == location.y)						// If location is found.
		{
			//location = liveCells[i];							// Save the location.

			if (i == (lastLiveCell - 1))						// If location is the last live cell, then decrement "lastLiveCell" by 1.
				lastLiveCell--;
			else
				liveCells[i] = liveCells[lastLiveCell-- - 1];	// Otherwise, move last live cell to index i, and decrement "lastLiveCell" by 1.

			return;
		}
	}
}

/// Starts the game, and simulates the grid up to "generations" generations.
void Start()
{
	cout << "******************* INITIAL STATE *******************" << endl << endl;
	AddNeighbors();												// Count live neighbors of each cell in "grid", and add them in corresponding locations in "neighborsCountGrid".

	Display();													// Display initial grid.	

	for (int i = 0; i < generations; i++)						// Iterate over generations.
	{
		PropagateCells();										// Propagate each cell, and apply the rules of life.

		cout << "******************* GENERATION NO " << i + 1 << " *******************" << endl << endl;

		AddNeighbors();											// Update neighbors.
		Display();												// Display each generation.
	}
}

/// Updates "neighborsCountGrid" in each generation.
void AddNeighbors()
{
	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 0; j < COLUMNS; j++)						// For each cell in "grid", update the corresponding Neighbors Count in "neighborsCountGrid". 
			neighborsCountGrid[i][j] = CountLiveNeighbors({ i, j });
	}
}

/// Propagates the cells and applies the rules of life.
void PropagateCells()
{
	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 0; j < COLUMNS; j++)
		{
			if ((grid[i][j] == 1 &&
				(neighborsCountGrid[i][j] > 3)))				// Death by Over-crowding
			{
				grid[i][j] = 0;
				DeleteLiveCell({ i, j });
			}
			else if ((grid[i][j] == 0) &&
				(neighborsCountGrid[i][j] == 3))				// Birth
			{
				grid[i][j] = 1;
				InsertLiveCell({ i, j });
			}
			else if ((grid[i][j] == 1) &&
				(neighborsCountGrid[i][j] == 2 ||
					neighborsCountGrid[i][j] == 3))				// Survival
			{
				grid[i][j] = 1;
			}
			else												// Death by Lonliness
			{
				if (grid[i][j] == 1)
				{
					grid[i][j] = 0;
					DeleteLiveCell({ i, j });
				}
			}
		}
	}
}

/// Displays the grids and uninit array.
void Display()
{
	cout << "GRID" << endl << endl;

	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 0; j < COLUMNS; j++)
			cout << grid[i][j] << " ";

		cout << endl;
	}

	cout << endl << "NEIGHBORS" << endl << endl;

	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 0; j < COLUMNS; j++)
			cout << neighborsCountGrid[i][j] << " ";

		cout << endl;
	}

	cout << endl << "LIVE CELLS - UNINIT ARRAY" << endl << endl;

	for (int i = 0; i < lastLiveCell; i++)
		cout << "(" << liveCells[i].x << ", " << liveCells[i].y << ")" << "  ";

	cout << endl << endl << "*******************************************************" << endl << endl;
}





