/*
	Class: AI
	The class implementing gameplay logic.

	Functios: IDDLMM(boardState* nodeOrig, int depth, int QS_Depth, bool maximizingPlayer, double a, double b)
	Iterative deepening depth limited minimax algorithm, with singular extensions including quiescent search, alpha-beta pruning,
	history table, and a time heuristic.
		Parameters:
		boardState* nodeOrig - The initial boardstate we are given
		int depth - The max basic depth to itterate to
		int QS_Dept - The possible depths past base depths to extend to for quiescent states
		bool maximizingPlayer - Is this our turn
		double a - Starting Alpha value for alpha-beta pruning
		double b - Starting Beta value for alpha-beta pruning

	Function: DLMM()
	Depth limited minimax, in this implementation it is called by IDDLMM

	Functions: void moveKing(), void moveQueen(), ..., movePawn()
	Generates the possible moves for a given piece on a given board at position x,y

	Function: kingInCheck(struct boardState* boardtoCheck, int playerid)
	Returns true if for the passed boardState the king of the player in question is in check

	Function: kingInCheckmate(struct boardState* boardtoCheck, int playerid)
	Returns true if for the passed boardState the king of the player in question is in checkmate

	Function: bool repeatedMovesLoss(int fileFrom, int rankFrom, int fileTo, int rankTo);
	Returns true if the move passed will result in a repeated moves loss, generally used to avoid infinite looping of moves

	Function: double calcHeuristic(int playerid, boardState* boardToCalc, vector<fakePiece> piecesOnBoard)
	Calculates the "value" of the board passed in for the given player id, determines how good of a position the player is in on the board

	Function: void allPossibleMoves(boardState* nodeOrig, possibleMovesStruct* possibleMoves, int playerid)
	Generates all possible moves for a given board and puts them into the possibleMoves struct

	void findPiecesOnBoard(boardState* boardtoCheck, vector<fakePiece> &piecesOnBoard);
	Finds all the pieces on a board and puts them into the vector

*/

class AI : public BaseAI
{
public:
	//Part of framework
	AI(Connection* c);
	virtual const char* username();
	virtual const char* password();
	virtual void init();
	virtual bool run();
	virtual void end();
	
	/*Utility_Structures.h structs defined here*/

	///Minimax/Movement Algorithms
	//void RandomMove(); Random movement used for testing legitimacy of move functions
	void IDDLMM(boardState* nodeOrig, int depth, int QS_Depth, bool maximizingPlayer, double a, double b); //Iterative deepening depth-limited minimax with Quiescent states, alpha beta pruning and history table
	double DLMM(boardState* nodeOrig, int depth, int QS_Depth, bool maximizingPlayer, double alpha, double beta); 
	
	///Move functions
	void moveKing(possibleMovesStruct* possibleMoves, std::pair<int, int>* position, int playerid, boardState* boardForMoves);
	void moveQueen(possibleMovesStruct* possibleMoves, std::pair<int, int>* position, int playerid, boardState* boardForMoves);
	void moveBishop(possibleMovesStruct* possibleMoves, std::pair<int, int>* position, int playerid, boardState* boardForMoves);
	void moveKnight(possibleMovesStruct* possibleMoves, std::pair<int, int>* position, int playerid, boardState* boardForMoves);
	void moveRook(possibleMovesStruct* possibleMoves, std::pair<int, int>* position, int playerid, boardState* boardForMoves);
	void movePawn(possibleMovesStruct* possibleMoves, std::pair<int, int>* position, int playerid, boardState* boardForMoves);
	
	///Utility and check functions
	bool kingInCheck(struct boardState* boardtoCheck, int playerid);
	bool kingInCheckmate(struct boardState* boardtoCheck, int playerid);
	bool repeatedMovesLoss(int fileFrom, int rankFrom, int fileTo, int rankTo);
	double calcHeuristic(int playerid, boardState* boardToCalc, vector<fakePiece> piecesOnBoard); // calculates the heuristic for the playerid passed, 0= white 1 = black
	void allPossibleMoves(boardState* nodeOrig, possibleMovesStruct* possibleMoves, int playerid);
	void findPiecesOnBoard(boardState* boardtoCheck, vector<fakePiece> &piecesOnBoard); // Creates a vector of all pieces on boardtoCheck with their file rank type and owner

private:
	boardState* boardOrig; // representation of the board upon begining of turn
	map<string, int> myHistoryTable;
	map<string, int>::iterator it;

};
