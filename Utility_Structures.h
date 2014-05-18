
///This struct represents a piece on our "fake" boards, the ones with moves that havn't actually been made yet.
struct fakePiece{
	int file(){ return file_f; }
	int rank(){ return rank_f; }
	int type(){ return type_f; }
	int owner(){ return owner_f; }
	int file_f;
	int rank_f;
	int type_f;
	int owner_f;

	fakePiece& operator=(const fakePiece& rhs){
		file_f = rhs.file_f;
		rank_f = rhs.rank_f;
		type_f = rhs.type_f;
		owner_f = rhs.owner_f;

		return *this;
	}

	
};

///Holds a possible move that could be made
struct moveLocation{
	int fileTo, rankTo, fileFrom, rankFrom;
	string historyTableVal;
	void setupWithArray(int theMove[4], map<string, int> theHT){
		fileFrom = theMove[0];
		rankFrom = theMove[1];
		fileTo = theMove[2];
		rankTo = theMove[3];
		historyTableVal.push_back(fileFrom); historyTableVal.push_back(rankFrom); historyTableVal.push_back(fileTo); historyTableVal.push_back(rankTo);
	}
};

//Used for sorting priority queue
struct CompareNodes
{
	bool operator()(const moveLocation lhs, const moveLocation rhs) const
	{
		it = myHistoryTable.find(possibleMovesMax->availiableMoves.top().historyTableVal);
		int currentCountLHS = it->second;
		it = myHistoryTable.find(possibleMovesMax->availiableMoves.top().historyTableVal);
		int currentCountRHS = it->second;
		return (lhs.historyTableCount > rhs.historyTableCount);
	}
};

///A priority queue structure for recreation at different levels of our search algorithms
struct possibleMovesStruct{
	std::priority_queue<moveLocation, std::vector<moveLocation>, CompareNodes> availiableMoves;
};