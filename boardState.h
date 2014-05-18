/*
	Struct: boardState
	Used to create nodes of the different chess board states

	Function: void unofficialMove(int fileFrom, int rankFrom, int fileTo, int rankTo, int playerID, int replacementPiece, int replacementOwner)
	Makes a move on this instance of a board, not on the actual game. Used for checking if a move is legal/beneficial etc.
		Parameters: 
		Files/Ranks -> the position of the piece on the board that is moving, and the location it is going to
		playerID -> the ID of the player making the move
		replacementPiece -> If there is a piece swap happening what is the new piece on this spot
		replacementOwner -> Who owns the new piece on this spot
*/

struct boardState{
	int board[9][9]; //2D array of the board's pieces 
	int boardOwner[9][9]; //2D array of the owner of each position
	int TurnsToStalemate;
	bool QS_State; // Quiescent state bool, if it == true, then this state has had something important happen such as a piece capture

	//Overloaded equals operator
	boardState& operator=(const boardState& rhs){
		for (int i = 1; i <= 8; i++){
			for (int j = 1; j <= 8; j++){
				this->board[i][j] = rhs.board[i][j];
				this->boardOwner[i][j] = rhs.boardOwner[i][j];
			}
		}
		this->TurnsToStalemate = rhs.TurnsToStalemate;
		this->QS_State = rhs.QS_State;
		return *this;
	}

	void unofficialMove(int fileFrom, int rankFrom, int fileTo, int rankTo, int playerID, int replacementPiece, int replacementOwner){
		//Utilities//
		//Checking to see if it is a quiescent state
		if (this->boardOwner[fileTo][rankTo] != 2 && this->boardOwner[fileTo][rankTo] != this->boardOwner[fileFrom][rankFrom]){
			QS_State = true;
		}
		else{
			QS_State = false;
		}

		//Stalemate tracker
		if ((this->boardOwner[fileTo][rankTo] != 2 && this->boardOwner[fileTo][rankTo] != this->boardOwner[fileFrom][rankFrom]) || board[fileFrom][rankFrom] == 'P'){
			TurnsToStalemate = 100;
		}
		else{
			TurnsToStalemate -= 1;
		}
		//End utilities//

		//The move//
		//Makes a move on the move
		this->board[fileTo][rankTo] = this->board[fileFrom][rankFrom];
		this->boardOwner[fileTo][rankTo] = playerID;
		this->board[fileFrom][rankFrom] = replacementPiece;
		this->boardOwner[fileFrom][rankFrom] = replacementOwner;
		//End of move//
	}	
};
