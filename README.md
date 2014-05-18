Chess_AI
========

Snippets from my chess AI which implements the MiniMax algorithm along with some singular extensions for efficiency

The Algorithm
========

<b>Full algorithm:</b> <i>TLHTQSID-ABDLMM</i>: Time limited history table quiescent search iterative deepening - alpha beta depth limited mini max

MM: Minimax
  http://en.wikipedia.org/wiki/Minimax
  Basically it takes recursively looks at our possible moves, then the enemies possible moves from those potential moves,
  and again our move, etc. Selecting the one that would eventually lead to us being in the best position.
  
  While recursively looking it wants to find the best heurstic value( the value which determines how good of a position
  we are in) assuming that the enemy player will also make his best move for the depth we search. 


AB: Alpha-Beta pruning

  When searching down the tree of moves we can cut off certain branches depending on the values we have already obtained
  from previous levels. For example if we found a move we could make which would give us a value of 5, and the next branch
  we see that the enemy can make a move which puts us in a board position of value less than 5 there is no point in
  exploring the rest of the possible moves the enemy could make at that point because he already has one worse than we 
  are allowing him to make.


DL: Depth limited

  The algorithm will only go down to x amount of depths.
  
QS: Quiescent search

  If the state we are observing at the bottom level of the tree( i.e. depth == 0) is a state where something important
  happened, like a piece being captured, then we probably want to look down a few more levels to see if more pieces are
  taken/if we are still in a good position after the move.
  
    Real example: 
    
      Our queen takes a pawn, but is now in the open and vulnerable to capture. If this move is the last one 
      looked at then our value may be higher than it should be because we are now up a piece. However, if we look
      a few moves deeper we may see our queen being captures and now this is no longer a good move. This is why 
      we implement quiescent search

HT: History table
  
    The history table is a table stored in memory that records the moves previously selected at different depths
    and quiescent states.
    
    This combined with iterative deepening(i.e. start searching at level 1, then next time do 1 then 2, 1 then 2 then 3)
    allows for more efficient pruning from alpha beta.
    
TL: Time limited
  
    Based on the time taken at each previous level of searching we go further down the tree or stop at our recently
    finished level.
  
  
  
