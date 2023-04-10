
struct Cell {
    int x;
    int y;
    char status;
};

class Attack {
public:
    int length = 0;
    Cell *begin;
    Cell *end;
    int dx = 0;
    int dy = 0;
    int boardEmptyCells = 0;

    bool equal(Attack *attack) {
        return this->length == attack->length && this->begin == attack->begin && this->end == attack->end;
    }

    void assigment(Attack attack){
        this->length = attack.length;
        this->begin = attack.begin;
        this->end = attack.end;
        this->dx = attack.dx;
        this->dy = attack.dy;
        this->boardEmptyCells = attack.boardEmptyCells;
    }
    void decrease(){
        this->boardEmptyCells--;
    }

    Attack(int length, Cell *begin, Cell *end, int dx, int dy, int emptyCells) {
        this->length = length;
        this->begin = begin;
        this->end = end;
        this->dx = dx;
        this->dy = dy;
        this->boardEmptyCells = emptyCells;
    }
};