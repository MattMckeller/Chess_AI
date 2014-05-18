
void AI::IDDLMM(boardState* nodeOrig, int depth, int QS_Depth, bool maximizingPlayer, double a, double b){
	double bestVal = 0; //Tracks best heuristic values calculated yet
	int moverid = playerID(); //The player moving is us, so call playerID() to return our playerID
	int fileFromBest, rankFromBest, fileToBest, rankToBest; //The best moves to and from coordinates
	clock_t t; //Used to keep track offthe time we started at
	clock_t t2; // Used for tracking the duration of the current depth
	clock_t t3; 
	vector<double> time; //Holds the times it took for each level of depth we search in order to calculate future depths
	double estimatedTime;
	
	possibleMovesStruct* possibleMovesMax = new possibleMovesStruct;
	allPossibleMoves(nodeOrig, possibleMovesMax, moverid); //Stores all possible moves for the given boardstate into possibleMovesMax

	map<string, int>::iterator it1;
	t = clock(); //Time we start the algorithm
	
	if (possibleMovesMax->availiableMoves.size() > 0){
		for (int i = 1; i <= depth; i++){
			delete possibleMovesMax;
			possibleMovesMax = new possibleMovesStruct;
			allPossibleMoves(nodeOrig, possibleMovesMax, moverid);
			if (possibleMovesMax->availiableMoves.size() > 0){
				//Calculating time for this depth
				t2 = clock();
				for (int j = possibleMovesMax->availiableMoves.size() - 1; j >= 0; j--){
					double newVal;
					boardState* nodeNew = new boardState;
					
					*(nodeNew) = *(nodeOrig);
					
					int fileFrom = possibleMovesMax->availiableMoves.top().fileFrom;
					int rankFrom = possibleMovesMax->availiableMoves.top().rankFrom;
					int fileTo = possibleMovesMax->availiableMoves.top().fileTo;
					int rankTo = possibleMovesMax->availiableMoves.top().rankTo;

					if (!repeatedMovesLoss(fileFrom, rankFrom, fileTo, rankTo)){
						nodeNew->unofficialMove(fileFrom, rankFrom, fileTo, rankTo, moverid, 0, 2);
						newVal = DLMM(nodeNew, i - 1, QS_Depth, false, bestVal, b);
					}

					else{
						newVal = bestVal;
						possibleMovesMax->availiableMoves.pop();
					}
					if (newVal <= bestVal){
						possibleMovesMax->availiableMoves.pop();
						// Move on 
					}
					else{
						bestVal = newVal;
						it1 = myHistoryTable.find(possibleMovesMax->availiableMoves.top().historyTableVal);
						fileFromBest = fileFrom;
						rankFromBest = rankFrom;
						fileToBest = fileTo;
						rankToBest = rankTo;
						possibleMovesMax->availiableMoves.pop();		
					}
					delete nodeNew;
					possibleMovesMax->availiableMoves.pop();
				}

				//History table updates and inserts
				int currentCount = it1->second;
				string currentString = it1->first;
				myHistoryTable.erase(currentString);
				myHistoryTable[currentString] = currentCount + 1;

				/*
				// Time Heuristic area
				*/
				
				//End time for this depth -> pushes it on the time vector
				t2 = clock() - t2;

				double tempTime = ((double)t2 / CLOCKS_PER_SEC); //Time in sec
				double estTimePerNode;
				
				time.push_back(tempTime);

				double estBranch;
				if (time.size() > 3){
					for (int n = time.size() - 1; n > 2; n--){
						//Adds up the factor difference between the times
						//i.e. 15 sec turn 1 45 sec turn 2 135 sec turn 3
						// => 0+ 135/45 = 3 + 45/15 = 6 
						estBranch += time[n] / time[n - 1];
					}

					// Dividing by the number of calculations done to get us an estimated branching factor
					estBranch = (estBranch / (time.size() - 3));
					
					for (int n = time.size() - 1; n >= 2; n--){
						//Estimates the time per node by taking the time for a specific level
						// and dividing it by the branching factor ^ branch level it was at
						estTimePerNode += time[n] / (pow(estBranch, n + 1));
					}

					//Divides by the number of calculations done / the size of the time array to get an average
					estTimePerNode = (estTimePerNode / (time.size() - 2));

					// Gets the estimated time for the next layer
					estimatedTime = (estTimePerNode)* pow(estBranch, i + 1);
					
					t3 = clock() - t;
					double timeSinceStart = ((double)t3 / CLOCKS_PER_SEC);

					//If we have spent more than x seconds on this move then quit where x is a scaling value from 1 to 5 based on time left
					double timeToSpend = 4 * (double)(((players[moverid].time()) / 900)) + 1;
					if (t3 >= timeToSpend){
						i = depth + 1;
					}
					//If making the next move is estimated to take longer than x seconds for this turn then quit and return 
					// where x is a scaling value from 1 to 5 based on time left
					else if (timeSinceStart + estimatedTime >= (timeToSpend + .5)){
						i = depth + 1;
					}
					estimatedTime = 0;
					estTimePerNode = 0;
					estBranch = 0;
				}
				else{ //Used for depths < 4
					// Estimates that the time will be the time it took for this round * the number of availiable moves
					// as we assume here the branching factor will be about the same
					
					estimatedTime = (tempTime * (possibleMovesMax->availiableMoves.size()));

					t3 = clock() - t;
					double timeSinceStart = ((double)t3 / CLOCKS_PER_SEC);
					double timeToSpend = 4 * (double)(players[moverid].time() / 900) + 1;

					//If we have spent more than x seconds on this move then quit where x is a scaling value from 1 to 5 based on time left
					if (timeSinceStart >= timeToSpend){
						i = depth + 1;
					}

					//If making the next move is estimated to take longer than x seconds for this turn then quit and return 
					// where x is a scaling value from 1 to 5 based on time left 
					else if (timeSinceStart + estimatedTime >= (timeToSpend + .5)){
						i = depth + 1;
					}
				}
			}

			for (int i = 0; i < pieces.size(); i++){
				//Make the move
				if (pieces[i].file() == fileFromBest && pieces[i].rank() == 9 - rankFromBest){
					pieces[i].move(fileToBest, 9 - rankToBest, int('Q'));
					i = pieces.size();
				}
			}
		}
	}
	else{
		//Error - do an illegal move and view log
		Piece piecetoMove;
		piecetoMove = pieces[0];
		piecetoMove.move(50, 50, int('Q'));
	}
	return;
}

double AI::DLMM(boardState* nodeOrig, int depth, int QS_Depth, bool maximizingPlayer, double alpha, double beta){
	bool terminal = false;
	double newAlpha = alpha;
	double newBeta = beta;
	double newVal = alpha;
	int moverid = playerID();
	map<string, int>::iterator it1;

	if (!maximizingPlayer){
		if (moverid == 0){ moverid = 1; }
		else{ moverid = 0; }
	}
	
	vector<fakePiece> blank;
	if (kingInCheckmate(nodeOrig, moverid)){
		double retVal = calcHeuristic(moverid, nodeOrig, blank);
		return (retVal);
	}

	else if ((depth == 0 && nodeOrig->QS_State == false) || (depth == 0 && QS_Depth == 0)){
		double retVal = calcHeuristic(moverid, nodeOrig, blank);
		return (retVal);
	}

	else if (maximizingPlayer){
		newBeta = beta;
		newAlpha = alpha;
		newVal = alpha;

		possibleMovesStruct* possibleMovesMax = new possibleMovesStruct;
		allPossibleMoves(nodeOrig, possibleMovesMax, moverid);

		if (possibleMovesMax->availiableMoves.size() > 0){			
			int size = possibleMovesMax->availiableMoves.size() - 1;
			for (int i = size; i >= 0; i--){				
				boardState* nodeNew = new boardState;
				*(nodeNew) = *(nodeOrig);
				
				int fileFrom = possibleMovesMax->availiableMoves.top().fileFrom;
				int rankFrom = possibleMovesMax->availiableMoves.top().rankFrom;
				int fileTo = possibleMovesMax->availiableMoves.top().fileTo;
				int rankTo = possibleMovesMax->availiableMoves.top().rankTo;
				
				nodeNew->unofficialMove(fileFrom, rankFrom, fileTo, rankTo, moverid, 0, 2);
				
				if (depth != 0){
					newVal = DLMM(nodeNew, depth, QS_Depth - 1, false, newAlpha, newBeta);
				}
				else{
					newVal = DLMM(nodeNew, depth - 1, QS_Depth, false, newAlpha, newBeta);
				}

				if (newVal <= newAlpha){ // Fail low
					
					possibleMovesMax->availiableMoves.pop();
				}
				else if (newVal >= newBeta){ //Fail high -> prune because fail high on max
					size = -1;
					it1 = myHistoryTable.find(possibleMovesMax->availiableMoves.top().historyTableVal);
					while (possibleMovesMax->availiableMoves.size() > 0){
						possibleMovesMax->availiableMoves.pop();
					}
				}
				else{ // New best value/alpha
					newAlpha = newVal;
					it1 = myHistoryTable.find(possibleMovesMax->availiableMoves.top().historyTableVal);
					possibleMovesMax->availiableMoves.pop();
				}
				delete nodeNew;
			}
			delete possibleMovesMax;

			//History table updates and inserts
			int currentCount = it1->second;
			string currentString = it1->first;
			myHistoryTable.erase(currentString);
			myHistoryTable[currentString] = currentCount + 1;
			return newAlpha;
		}
		else{  
			return alpha;
		}
	}
	else{ //Min player
		newAlpha = alpha;
		newBeta = beta;
		
		if (alpha != 0){
			newBeta = alpha;
		}
		else{
			newBeta = 1000;
		}
		newVal = 0;
		possibleMovesStruct* possibleMovesMax = new possibleMovesStruct;
		allPossibleMoves(nodeOrig, possibleMovesMax, moverid);
		
		if (possibleMovesMax->availiableMoves.size() > 0){
			
			int size = possibleMovesMax->availiableMoves.size() - 1;
			for (int i = size; i >= 0; i--){
				boardState* nodeNew = new boardState;
				*(nodeNew) = *(nodeOrig);
				
				int fileFrom = possibleMovesMax->availiableMoves.top().fileFrom;
				int rankFrom = possibleMovesMax->availiableMoves.top().rankFrom;
				int fileTo = possibleMovesMax->availiableMoves.top().fileTo;
				int rankTo = possibleMovesMax->availiableMoves.top().rankTo;
				
				nodeNew->unofficialMove(fileFrom, rankFrom, fileTo, rankTo, moverid, 0, 2);
				if (depth == 0){
					newVal = DLMM(nodeNew, depth, QS_Depth - 1, true, newAlpha, newBeta);
				}
				else{
					newVal = DLMM(nodeNew, depth - 1, QS_Depth, true, newAlpha, newBeta);
				}
				
				if (newVal <= newAlpha){ // Fail low on min -> prune
					size = -1;
					it1 = myHistoryTable.find(possibleMovesMax->availiableMoves.top().historyTableVal);
					while (possibleMovesMax->availiableMoves.size() > 0){
						possibleMovesMax->availiableMoves.pop();
					}

				}
				else if (newVal >= newBeta){ //Fail high on min -> dont use this one
					
					possibleMovesMax->availiableMoves.pop();
				}
				else{ //We found a new value which is lower than the highest beta
					
					newBeta = newVal;
					it1 = myHistoryTable.find(possibleMovesMax->availiableMoves.top().historyTableVal);
					possibleMovesMax->availiableMoves.pop();
				}
				delete nodeNew;
			}
			delete possibleMovesMax;

			//History table updates and inserts
			int currentCount = it1->second;
			string currentString = it1->first;
			myHistoryTable.erase(currentString);
			myHistoryTable[currentString] = currentCount + 1;

			return (newBeta);
		}
		else{
			return (beta);
		}
	}


}

void AI::allPossibleMoves(boardState* nodeOrig, possibleMovesStruct* possibleMoves, int playerid){
	vector<fakePiece> piecesOnBoard;
	findPiecesOnBoard(nodeOrig, piecesOnBoard); // Creates a vector of all pieces on the board with their file rank type and owner
	
	if (piecesOnBoard.size() >= 1){
		for (int pos = 0; pos < piecesOnBoard.size(); pos++){
			bool myPiece = false;
			bool skip = false;
			fakePiece piecetoMove;

			//Check if creating a new possibleMovesStruct and checking it's size returns 0 
			while (!myPiece && !skip){
				if (pos < piecesOnBoard.size()){
					piecetoMove = piecesOnBoard[pos];
				}
				else{
					if (possibleMoves->availiableMoves.size() == 0){
						return;
					}
					else if (pos >= possibleMoves->availiableMoves.size()){
						skip = true;
						return;
					}
				}
				if (piecetoMove.owner() == playerid){
					myPiece = true;
				}
				else{
					skip = true;
				}
			}
			if (!skip){
				int typeofPiece = piecetoMove.type();
				std::pair<int, int>* position = new std::pair<int, int>;
				position->first = piecetoMove.file();
				position->second = piecetoMove.rank();

				switch (piecetoMove.type()) {
				case 'K':
					moveKing(possibleMoves, position, playerid, nodeOrig);
					break;
				case 'Q':
					moveQueen(possibleMoves, position, playerid, nodeOrig);
					break;
				case 'B':
					moveBishop(possibleMoves, position, playerid, nodeOrig);
					break;
				case 'N':
					moveKnight(possibleMoves, position, playerid, nodeOrig);
					break;
				case 'R':
					moveRook(possibleMoves, position, playerid, nodeOrig);
					break;
				case 'P':
					movePawn(possibleMoves, position, playerid, nodeOrig);
					break;
				default: 
					break;
				}
				delete position;
			}
		}

	}
	return;
}


void AI::moveBishop(possibleMovesStruct* possibleMoves, std::pair<int, int>* position, int playerid, boardState* boardForMoves){
	int file = position->first;
	int rank = position->second;
	int enemyID;
	if (playerid == 0){ enemyID = 1; }
	else{ enemyID = 0; }
	boardState* tempBoard = new boardState;
	*(tempBoard) = *(boardForMoves);

	bool UL, UR, DL, DR;
	UL = UR = DL = DR = false;
	for (int i = 1; i <= 7; i++){

		///////////////////////
		/* Diagonal movement */
		///////////////////////

		//Checking down and right
		if (file + i <= 8 && rank + i <= 8 && DR == false){
			//If the spot is empty or if there is an enemy there we can move
			if (boardForMoves->board[file + i][rank + i] == 0 || boardForMoves->boardOwner[file + i][rank + i] == enemyID){
				tempBoard->unofficialMove(file, rank, file + i, rank + i, playerid, 0, 2);
				//If the move does not cause a check then push the move onto the possibleMoves vector
				if (!kingInCheck(tempBoard, playerid)){ int theMove[4] = { file, rank, file + i, rank + i }; moveLocation loc; loc.setupWithArray(theMove, myHistoryTable); 
					possibleMoves->availiableMoves.push(loc); if (boardForMoves->boardOwner[file + i][rank + i] == enemyID){ DR = true; } }
				tempBoard->unofficialMove(file + i, rank + i, file, rank, playerid, boardForMoves->board[file + i][rank + i], boardForMoves->boardOwner[file + i][rank + i]);
				if (boardForMoves->boardOwner[file + i][rank + i] == enemyID){ DR = true; }
			}
			else{
				//We cannot move if there is one of our pieces there
				DR = true;
			}
		}

		//Checking down and left
		if (file - i >= 1 && rank + i <= 8 && DL == false){
			//If the spot is empty or if there is an enemy there we can move
			if (boardForMoves->board[file - i][rank + i] == 0 || boardForMoves->boardOwner[file - i][rank + i] == enemyID){
				tempBoard->unofficialMove(file, rank, file - i, rank + i, playerid, 0, 2);
				//If the move does not cause a check then push the move onto the possibleMoves vector
				if (!kingInCheck(tempBoard, playerid)){ int theMove[4] = { file, rank, file - i, rank + i }; moveLocation loc; loc.setupWithArray(theMove, myHistoryTable); 
					possibleMoves->availiableMoves.push(loc); if (boardForMoves->boardOwner[file - i][rank + i] == enemyID){ DL = true; } }
				tempBoard->unofficialMove(file - i, rank + i, file, rank, playerid, boardForMoves->board[file - i][rank + i], boardForMoves->boardOwner[file - i][rank + i]);
				if (boardForMoves->boardOwner[file - i][rank + i] == enemyID){ DL = true; }
			}
			else{
				//We cannot move if there is one of our pieces there
				DL = true;
			}
		}

		//Checking up and left
		if (file - i >= 1 && rank - i >= 1 && UL == false){
			//If the spot is empty or if there is an enemy there we can move
			if (boardForMoves->board[file - i][rank - i] == 0 || boardForMoves->boardOwner[file - i][rank - i] == enemyID){
				tempBoard->unofficialMove(file, rank, file - i, rank - i, playerid, 0, 2);
				//If the move does not cause a check then push the move onto the possibleMoves vector
				if (!kingInCheck(tempBoard, playerid)){ int theMove[4] = { file, rank, file - i, rank - i }; moveLocation loc; loc.setupWithArray(theMove, myHistoryTable); 
					possibleMoves->availiableMoves.push(loc); if (boardForMoves->boardOwner[file - i][rank - i] == enemyID){ UL = true; } }
				tempBoard->unofficialMove(file - i, rank - i, file, rank, playerid, boardForMoves->board[file - i][rank - i], boardForMoves->boardOwner[file - i][rank - i]);
				if (boardForMoves->boardOwner[file - i][rank - i] == enemyID){ UL = true; }
			}
			else{
				//We cannot move if there is one of our pieces there
				UL = true;
			}
		}

		//Checking up and right
		if (file + i <= 8 && rank - i >= 1 && UR == false){
			//If the spot is empty or if there is an enemy there we can move
			if (boardForMoves->board[file + i][rank - i] == 0 || boardForMoves->boardOwner[file + i][rank - i] == enemyID){
				tempBoard->unofficialMove(file, rank, file + i, rank - i, playerid, 0, 2);
				//If the move does not cause a check then push the move onto the possibleMoves vector
				if (!kingInCheck(tempBoard, playerid)){ int theMove[4] = { file, rank, file + i, rank - i }; moveLocation loc; loc.setupWithArray(theMove, myHistoryTable); 
					possibleMoves->availiableMoves.push(loc); if (boardForMoves->boardOwner[file + i][rank - i] == enemyID){ UR = true; } }
				tempBoard->unofficialMove(file + i, rank - i, file, rank, playerid, boardForMoves->board[file + i][rank - i], boardForMoves->boardOwner[file + i][rank - i]);
				if (boardForMoves->boardOwner[file + i][rank - i] == enemyID){ UR = true; }
			}
			else{
				//We cannot move if there is one of our pieces there
				UR = true;
			}
		}


	}
	delete tempBoard;
}