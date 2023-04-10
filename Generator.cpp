#include "Generator.h"

using namespace std;


bool cellCross(int *n, Board *board) {
    int x = rand() % *n;
    int y = rand() % *n;
    return (*board).pushCell(x, y);
}

 void play(int k) {
    srand(time(NULL));
    int n = k;
    Board board(n);
    while (board.fullBoard() != true)
        cellCross(&n, &board);
}


