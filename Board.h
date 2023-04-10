#pragma once

#include "Attack.h"
#include <iostream>
#include <cstdlib>
#include <list>
#include <ctime>
#include <fstream>

using namespace std;


class Board {
    const char Cross = 'x';
    const char Zero = 'o';
    const char Empty = 'e';
    const char Border = 'b';

    list<Cell> cellsX;
    list<Cell> cellsO;
    list<Attack> attackX;
    list<Attack> attackO;
    int maxDirectionX = 1;
    int maxDirectionO = 1;
    int n;
    int step=0;
    unsigned int start_time =  clock();
    unsigned int end_time = clock();
    double time = (double)(end_time - start_time) / CLOCKS_PER_SEC;


    bool equalCell(Cell *cell, int x, int y, char st) {
        return cell->x == x && cell->y == y && cell->status == st;
    }

    bool equalCellToCell(Cell *cell, Cell cell1) {
        return cell->x == cell1.x && cell->y == cell1.y && cell->status == cell1.status;
    }

    void assigmentCell(Cell *cell, Cell cell1) {
        cell->x = cell1.x;
        cell->y = cell1.y;
        cell->status = cell1.status;
    }

    char haveCellAndCell(int x, int y, Cell *cell) {
        step++;
        if (x >= n || y >= n || x < 0 || y < 0)
            return Border;

        for (auto item: cellsX) {
            if (equalCell(&item, x, y, Cross)) {
                *cell = item;
                return Cross;
            }
        }
        for (auto item: cellsO) {
            if (equalCell(&item, x, y, Zero)) {
                *cell = item;
                return Zero;
            }
        }

        return Empty;
    }

    char haveCell(int x, int y) {

        if (x >= n || y >= n || x < 0 || y < 0)
            return Border;

        for (auto item: cellsX) {
            step++;
            if (equalCell(&item, x, y, Cross))
                return Cross;
        }

        for (auto item: cellsO) {
            step++;
            if (equalCell(&item, x, y, Zero))
                return Zero;
        }
        return Empty;
    }

    void checkDirection(Cell cell, int dx, int dy, int *count, int *x, int *y) {
        //проверяет сколько крестиков/ноликов в одной из сторон от данной клетки, возвращает последнюю клетку
        int i = 0;
        Cell value = cell;
        Cell temp = cell;
        for (i = 1; i < 5; i++) {
            if (haveCellAndCell(cell.x + i * dx, cell.y + i * dy, &temp) != cell.status) {
                break;
            }
            assigmentCell(&value, temp);
            *count = i;
        }
        (*x) = value.x;
        (*y) = value.y;

    }

    bool removeAttack(Attack *attack, char st) {
        //step++;
        if (st == Cross) {
            auto iter = attackX.begin();
            while (iter != attackX.end()) {
                if ((*iter).equal(attack)) {
                    delete (*iter).begin;
                    delete (*iter).end;
                    attackX.erase(iter);
                    return true;
                }
                ++iter;
            }
        }
        if (st == Zero) {
            auto iter = attackO.begin();
            while (iter != attackO.end()) {
                if ((*iter).equal(attack)) {
                    attackO.erase(iter);
                    return true;
                }
                ++iter;
            }
        }
        return false;
    }

    void deleteAttack(Cell *cell, int dx, int dy, int count1, int count2) {
        if (count1 > 0) {
            if (cell->status == Cross)
                for (auto item: attackX) {
                    if (equalCell(item.begin, cell->x - count1 * dx, cell->y - count1 * dy, Cross) &&
                        item.dx == dx && item.dy == dy) {
                        removeAttack(&item, Cross);
                        break;
                    }
                    if (equalCell(item.end, cell->x - count1 * dx, cell->y - count1 * dy, Cross) &&
                        item.dx == dx && item.dy == dy) {
                        removeAttack(&item, Cross);
                        break;
                    }

                }
            else if (cell->status == Zero)
                for (auto item: attackO) {
                    if (equalCell(item.begin, cell->x - count1 * dx, cell->y - count1 * dy, Zero) &&
                        item.dx == dx && item.dy == dy) {
                        removeAttack(&item, Zero);
                        break;
                    }
                    if (equalCell(item.end, cell->x - count1 * dx, cell->y - count1 * dy, Zero) &&
                        item.dx == dx && item.dy == dy) {
                        removeAttack(&item, Zero);
                        break;
                    }
                }

        }

        if (count2 > 0) {
            if (cell->status == Cross)
                for (auto item: attackX) {
                    if (equalCell(item.begin, cell->x + count2 * dx, cell->y + count2 * dy, Cross) && item.dx == dx &&
                        item.dy == dy) {
                        removeAttack(&item, Cross);
                        break;
                    }
                    if (equalCell(item.end, cell->x + count2 * dx, cell->y + count2 * dy, Cross) &&
                        item.dx == dx && item.dy == dy) {
                        removeAttack(&item, Cross);
                        break;
                    }
                }
            else if (cell->status == Zero)
                for (auto item: attackO) {
                    if (equalCell(item.begin, cell->x + count2 * dx, cell->y + count2 * dy, Zero) && item.dx == dx &&
                        item.dy == dy) {
                        removeAttack(&item, Zero);
                        break;
                    }
                    if (equalCell(item.end, cell->x + count2 * dx, cell->y + count2 * dy, Zero) &&
                        item.dx == dx && item.dy == dy) {
                        removeAttack(&item, Zero);
                        break;
                    }
                }
        }
    }

    bool pushAttack(Cell cell, int dx, int dy) {
        int count1 = 0, count2 = 0;
        int sizeEmpty = 0;

        int x1 = 0, y1 = 0, x2 = 0, y2 = 0;

        checkDirection(cell, -dx, -dy, &count1, &x1, &y1);
        checkDirection(cell, dx, dy, &count2, &x2, &y2);

        if (count1 > 0 || count2 > 0) {
            deleteAttack(&cell, dx, dy, count1, count2);

            Cell *begin = new Cell;
            Cell *end = new Cell;
            begin->x = x1;
            begin->y = y1;
            begin->status = cell.status;
            end->x = x2;
            end->y = y2;
            end->status = cell.status;

            if (haveCell(begin->x - dx, begin->y - dy) == Empty)
                sizeEmpty++;
            if (haveCell(end->x + dx, end->y + dy) == Empty)
                sizeEmpty++;

            Attack newAttack(count1 + count2 + 1, begin, end, dx, dy, sizeEmpty);

            if (cell.status == Cross) {
                attackX.push_back(newAttack);
                if (maxDirectionX < newAttack.length)
                    maxDirectionX = newAttack.length;
            } else {
                attackO.push_back(newAttack);
                if (maxDirectionO < newAttack.length)
                    maxDirectionO = newAttack.length;
            }
            return true;
        }
        return false;
    }

    void checkAllBoard() {
        if (maxDirectionX > 3)
            for (auto item: attackX) {
                if (item.length == 5 || (item.length == 4 && item.boardEmptyCells == 2)) {
                    end_time = clock();
                    time = (double)(end_time - start_time)/ CLOCKS_PER_SEC;
                    drawBoard();

                    cout << item.begin->x << " " << item.begin->y << endl;
                    cout<<"Steps:"<<step<<"  Time:"<<time<<endl;
                    cout << "Crosses WON!" << endl;
                   // return true;
                    exit(0);
                }
            }
        if (maxDirectionO > 3)
            for (auto item: attackO) {
                if (item.length == 5 || (item.length == 4 && item.boardEmptyCells == 2)) {
                    end_time = clock();
                    time = (double)(end_time - start_time)/ CLOCKS_PER_SEC;
                    drawBoard();
                    cout << item.begin->x << " " << item.begin->y << endl;
                    cout<<"Steps:"<<step<<"  Time:"<<time<<endl;
                    cout << "Zero WON!" << endl;
                    //return true;
                    exit(0);
                }
            }


    }

    Cell statusCells(int x, int y, int *i, char st) {
        Cell cell;
        cell.status = Zero;
        step++;

        if (haveCell(x + 1, y + 1) == st) {
            (*i)++;
            cell.x = x + 1;
            cell.y = y + 1;
        }
        if (haveCell(x, y + 1) == st) {
            (*i)++;
            cell.x = x;
            cell.y = y + 1;
        }
        if (haveCell(x - 1, y + 1) == st) {
            (*i)++;
            cell.x = x - 1;
            cell.y = y + 1;
        }
        if (haveCell(x + 1, y) == st) {
            (*i)++;
            cell.x = x + 1;
            cell.y = y;
        }
        if (haveCell(x - 1, y) == st) {
            (*i)++;
            cell.x = x - 1;
            cell.y = y;
        }
        if (haveCell(x + 1, y - 1) == st) {
            (*i)++;
            cell.x = x + 1;
            cell.y = y - 1;
        }
        if (haveCell(x, y - 1) == st) {
            (*i)++;
            cell.x = x;
            cell.y = y - 1;
        }
        if (haveCell(x - 1, y - 1) == st) {
            (*i)++;
            cell.x = x - 1;
            cell.y = y - 1;
        }
        return cell;
    }

    Cell commonCell(int *max, char status) {
        int i = 0;
        Cell cell;
        if (status == Cross)
            for (auto item: cellsX) {
                if (haveCell(item.x + 1, item.y + 1) == Empty) {
                    statusCells(item.x + 1, item.y + 1, &i, Cross);
                    if (i > (*max)) {
                        (*max) = i;
                        cell.x = item.x + 1;
                        cell.y = item.y + 1;
                    }
                }
                if (haveCell(item.x, item.y + 1) == Empty) {
                    i = 0;
                    statusCells(item.x, item.y + 1, &i, Cross);
                    if (i > (*max)) {
                        (*max) = i;
                        cell.x = item.x;
                        cell.y = item.y + 1;
                    }
                }
                if (haveCell(item.x - 1, item.y + 1) == Empty) {
                    i = 0;
                    statusCells(item.x - 1, item.y + 1, &i, Cross);
                    if (i > (*max)) {
                        (*max) = i;
                        cell.x = item.x - 1;
                        cell.y = item.y + 1;
                    }
                }
                if (haveCell(item.x + 1, item.y) == Empty) {
                    i = 0;
                    statusCells(item.x + 1, item.y, &i, Cross);
                    if (i > (*max)) {
                        (*max) = i;
                        cell.x = item.x + 1;
                        cell.y = item.y;
                    }
                }
                if (haveCell(item.x - 1, item.y) == Empty) {
                    i = 0;
                    statusCells(item.x - 1, item.y, &i, Cross);
                    if (i > (*max)) {
                        (*max) = i;
                        cell.x = item.x - 1;
                        cell.y = item.y;
                    }
                }
                if (haveCell(item.x + 1, item.y - 1) == Empty) {
                    i = 0;
                    statusCells(item.x + 1, item.y - 1, &i, Cross);
                    if (i > (*max)) {
                        (*max) = i;
                        cell.x = item.x + 1;
                        cell.y = item.y - 1;
                    }
                }
                if (haveCell(item.x, item.y - 1) == Empty) {
                    i = 0;
                    statusCells(item.x, item.y - 1, &i, Cross);
                    if (i > (*max)) {
                        (*max) = i;
                        cell.x = item.x;
                        cell.y = item.y - 1;
                    }
                }
                if (haveCell(item.x - 1, item.y - 1) == Empty) {
                    i = 0;
                    statusCells(item.x - 1, item.y - 1, &i, Cross);
                    if (i > (*max)) {
                        (*max) = i;
                        cell.x = item.x - 1;
                        cell.y = item.y - 1;
                    }
                }
            }
        else
            for (auto item: cellsO) {
                if (haveCell(item.x + 1, item.y + 1) == Empty) {
                    i = 0;
                    statusCells(item.x + 1, item.y + 1, &i, Zero);
                    if (i > (*max)) {
                        (*max) = i;
                        cell.x = item.x + 1;
                        cell.y = item.y + 1;
                    }
                }
                if (haveCell(item.x, item.y + 1) == Empty) {
                    i = 0;
                    statusCells(item.x, item.y + 1, &i, Zero);
                    if (i > (*max)) {
                        (*max) = i;
                        cell.x = item.x;
                        cell.y = item.y + 1;
                    }
                }
                if (haveCell(item.x - 1, item.y + 1) == Empty) {
                    i = 0;
                    statusCells(item.x - 1, item.y + 1, &i, Zero);
                    if (i > (*max)) {
                        (*max) = i;
                        cell.x = item.x - 1;
                        cell.y = item.y + 1;
                    }
                }
                if (haveCell(item.x + 1, item.y) == Empty) {
                    i = 0;
                    statusCells(item.x + 1, item.y, &i, Zero);
                    if (i > (*max)) {
                        (*max) = i;
                        cell.x = item.x + 1;
                        cell.y = item.y;
                    }
                }
                if (haveCell(item.x - 1, item.y) == Empty) {
                    i = 0;
                    statusCells(item.x - 1, item.y, &i, Zero);
                    if (i > (*max)) {
                        (*max) = i;
                        cell.x = item.x - 1;
                        cell.y = item.y;
                    }
                }
                if (haveCell(item.x + 1, item.y - 1) == Empty) {
                    i = 0;
                    statusCells(item.x + 1, item.y - 1, &i, Zero);
                    if (i > (*max)) {
                        (*max) = i;
                        cell.x = item.x + 1;
                        cell.y = item.y - 1;
                    }
                }
                if (haveCell(item.x, item.y - 1) == Empty) {
                    i = 0;
                    statusCells(item.x, item.y - 1, &i, Zero);
                    if (i > (*max)) {
                        (*max) = i;
                        cell.x = item.x;
                        cell.y = item.y - 1;
                    }
                }
                if (haveCell(item.x - 1, item.y - 1) == Empty) {
                    i = 0;
                    statusCells(item.x - 1, item.y - 1, &i, Zero);
                    if (i > (*max)) {
                        (*max) = i;
                        cell.x = item.x - 1;
                        cell.y = item.y - 1;
                    }
                }
            }

        cell.status = Zero;
        return cell;
    }

    void decreaseEmptyCells(Cell cell, int dx, int dy) {
        //step++;
        if (cell.status == Zero) {
            auto iter = attackX.begin();
            while (iter != attackX.end()) {
                if (((*iter).begin->x == cell.x && (*iter).begin->y == cell.y) ||
                    ((*iter).end->x == cell.x && (*iter).end->y == cell.y))
                    if ((*iter).dx == dx && (*iter).dy == dy)
                        (*iter).decrease();

                ++iter;
            }
        }

        if (cell.status == Cross) {
            auto iter = attackO.begin();
            while (iter != attackO.end()) {
                if (((*iter).begin->x == cell.x && (*iter).begin->y == cell.y) ||
                    ((*iter).end->x == cell.x && (*iter).end->y == cell.y))
                    if ((*iter).dx == dx && (*iter).dy == dy)
                        (*iter).decrease();
                ++iter;
            }
        }
    }

    void decrease(Cell cell) {
        Cell cell1;
        cell1.status = cell.status;
        cell1.x = cell.x - 1;
        cell1.y = cell.y - 1;
        decreaseEmptyCells(cell1, 1, 1);

        cell1.x++;
        decreaseEmptyCells(cell1, 0, 1);
        cell1.x++;
        decreaseEmptyCells(cell1, -1, 1);

        cell1.y++;
        decreaseEmptyCells(cell1, 1, 0);
        cell1.x -= 2;
        decreaseEmptyCells(cell1, 1, 0);

        cell1.y++;
        decreaseEmptyCells(cell1, -1, 1);
        cell1.x++;
        decreaseEmptyCells(cell1, 0, 1);
        cell1.x++;
        decreaseEmptyCells(cell1, 1, 1);
    }

    bool checkZeroAttack(int *x, int *y, char status) {
        if (status == Zero) {
            int count = maxDirectionO;
            if (count == 4) {
                for (auto item: attackO) {
                    //step++;
                    if (count == item.length && item.boardEmptyCells > 0) {
                        if (haveCell(item.begin->x - item.dx, item.begin->y - item.dy) == Empty) {
                            (*x) = item.begin->x - item.dx;
                            (*y) = item.begin->y - item.dy;
                            return true;
                        } else {
                            (*x) = item.end->x + item.dx;
                            (*y) = item.end->y + item.dy;
                            return true;
                        }
                    }
                }
                count--;
            }
            if (count == 3) {
                for (auto item: attackO) {
                    //step++;
                    if (count == item.length && item.boardEmptyCells > 0) {
                        //step++;
                        if (haveCell(item.begin->x - item.dx, item.begin->y - item.dy) == Empty &&
                            haveCell(item.begin->x - 2 * item.dx, item.begin->y - 2 * item.dy) == Zero) {
                            (*x) = item.begin->x - item.dx;
                            (*y) = item.begin->y - item.dy;
                            return true;
                        } else if (haveCell(item.end->x + item.dx, item.end->y + item.dy) == Empty &&
                                   haveCell(item.end->x + 2 * item.dx, item.end->y + 2 * item.dy) == Zero) {
                            (*x) = item.end->x + item.dx;
                            (*y) = item.end->y + item.dy;
                            return true;
                        }

                        if (item.boardEmptyCells == 2) {
                            //step++;
                            if (haveCell(item.begin->x - 2 * item.dx, item.begin->y - 2 * item.dy) == Empty) {
                                (*x) = item.begin->x - item.dx;
                                (*y) = item.begin->y - item.dy;
                                return true;
                            } else if (haveCell(item.end->x + 2 * item.dx, item.end->y + 2 * item.dy) == Empty) {
                                (*x) = item.end->x + item.dx;
                                (*y) = item.end->y + item.dy;
                                return true;
                            }
                        }
                    }
                    }
                count--;
            }
            if (count == 2 && attackO.size() > 1) {

                //step++;
                for (auto item: attackO) {
                    //step++;
                    if (count == item.length && item.boardEmptyCells > 0) {
                        if (haveCell(item.begin->x - item.dx, item.begin->y - item.dy) == Empty &&
                            haveCell(item.begin->x - 2 * item.dx, item.begin->y - 2 * item.dy) == Zero &&
                            haveCell(item.begin->x - 3 * item.dx, item.begin->y - 3 * item.dy) == Zero) {
                            (*x) = item.begin->x - item.dx;
                            (*y) = item.begin->y - item.dy;
                            return true;
                        } else if (haveCell(item.end->x + item.dx, item.end->y + item.dy) == Empty &&
                                   haveCell(item.end->x + 2 * item.dx, item.end->y + 2 * item.dy) == Zero &&
                                   haveCell(item.end->x + 3 * item.dx, item.end->y + 3 * item.dy) == Zero) {
                            (*x) = item.end->x + item.dx;
                            (*y) = item.end->y + item.dy;
                            return true;
                        }
                    }
                }
            }
        }
        if (status == Cross) {
            int count = maxDirectionX;
            if (count == 3) {
                for (auto item: attackX) {
                    if (count == item.length && item.boardEmptyCells > 0) {
                        if (haveCell(item.begin->x - item.dx, item.begin->y - item.dy) == Empty &&
                            haveCell(item.begin->x - 2 * item.dx, item.begin->y - 2 * item.dy) == Cross) {
                            (*x) = item.begin->x - item.dx;
                            (*y) = item.begin->y - item.dy;
                            return true;
                        } else if (haveCell(item.end->x + item.dx, item.end->y + item.dy) == Empty &&
                                   haveCell(item.end->x + 2 * item.dx, item.end->y + 2 * item.dy) == Cross) {
                            (*x) = item.end->x + item.dx;
                            (*y) = item.end->y + item.dy;
                            return true;
                        }

                    }
                }
                count--;
            }
            if (count == 2 && attackX.size() > 1) {
                for (auto item: attackX) {
                    if (count == item.length && item.boardEmptyCells > 0) {
                        if (haveCell(item.begin->x - item.dx, item.begin->y - item.dy) == Empty &&
                            haveCell(item.begin->x - 2 * item.dx, item.begin->y - 2 * item.dy) == Cross &&
                            haveCell(item.begin->x - 3 * item.dx, item.begin->y - 3 * item.dy) == Cross) {
                            (*x) = item.begin->x - item.dx;
                            (*y) = item.begin->y - item.dy;
                            return true;
                        } else if (haveCell(item.end->x + item.dx, item.end->y + item.dy) == Empty &&
                                   haveCell(item.end->x + 2 * item.dx, item.end->y + 2 * item.dy) == Cross &&
                                   haveCell(item.end->x + 3 * item.dx, item.end->y + 3 * item.dy) == Cross) {
                            (*x) = item.end->x + item.dx;
                            (*y) = item.end->y + item.dy;
                            return true;
                        }
                    }
                }
            }
        }
        return false;
    }

    void attackZero() {
        int x = -1, y = -1;
        Cell temp;
        Cell newCell1;
        Attack attack = (Attack) attackX.front();
        int max = 0;
        int count = 0;

        if (attackO.size() != 0) {

            if (checkZeroAttack(&x, &y, Zero)) {
                Cell newCell;
                newCell.x = x;
                newCell.y = y;
                //cout<<"Zero: x="<<newCell.x<<" y"<<newCell.x<<endl;
                newCell.status = Zero;
                decrease(newCell);
                cellsO.push_back(newCell);
                pushAttack(newCell, 1, 0);
                pushAttack(newCell, 0, 1);
                pushAttack(newCell, 1, 1);
                pushAttack(newCell, -1, 1);
                return;
            }
        }
        if (attackX.size() != 0) {
            if (checkZeroAttack(&x, &y, Cross)) {
                Cell newCell;
                newCell.x = x;
                newCell.y = y;
                //cout<<"Zero: x="<<newCell.x<<" y"<<newCell.x<<endl;
                newCell.status = Zero;
                decrease(newCell);
                cellsO.push_back(newCell);
                pushAttack(newCell, 1, 0);
                pushAttack(newCell, 0, 1);
                pushAttack(newCell, 1, 1);
                pushAttack(newCell, -1, 1);
                return;
            }
        }

        if (attackX.size() != 0) {
            count = maxDirectionX;

            while (count != 1) {
                for (auto item: attackX) {
                    step++;
                    if (count == item.length)
                        if (item.boardEmptyCells > max) {
                            if (count == 3 && item.boardEmptyCells == 1) {
                                if (haveCell(item.begin->x - item.dx, item.begin->y - item.dy) == Empty &&
                                    haveCell(item.begin->x - 2 * item.dx, item.begin->y - 2 * item.dy) != Empty) {
                                    continue;
                                }
                                else if (haveCell(item.end->x + item.dx, item.end->y + item.dy) == Empty &&
                                           haveCell(item.end->x + 2 * item.dx, item.end->y + 2 * item.dy) !=
                                           Empty) {
                                    continue;
                                }
                            }
                            max = item.boardEmptyCells;
                            attack.assigment(item);
                        }
                }

                if (max > 0) {
                    if (haveCell(attack.begin->x - attack.dx, attack.begin->y - attack.dy) == Empty) {
                        x = attack.begin->x - attack.dx;
                        y = attack.begin->y - attack.dy;
                    } else {
                        x = attack.end->x + attack.dx;
                        y = attack.end->y + attack.dy;
                    }
                    Cell newCell;
                    newCell.x = x;
                    newCell.y = y;
                    //cout<<"Zero: x="<<newCell.x<<" y"<<newCell.x<<endl;
                    newCell.status = Zero;
                    decrease(newCell);
                    cellsO.push_back(newCell);
                    pushAttack(newCell, 1, 0);
                    pushAttack(newCell, 0, 1);
                    pushAttack(newCell, 1, 1);
                    pushAttack(newCell, -1, 1);
                    return;
                }

                if (count <= 3 && count <= maxDirectionO) {
                    for (auto item: attackO) {
                        step++;
                        if (count == item.length) {

                            if (item.boardEmptyCells == 2) {
                                if (count == 3) {
                                    //step++;
                                    x = item.begin->x - item.dx;
                                    y = item.begin->y - item.dy;
                                    Cell newCell;
                                    newCell.x = x;
                                    newCell.y = y;
                                    newCell.status = Zero;
                                    decrease(newCell);
                                    cellsO.push_back(newCell);
                                    //cout<<"Zero: x="<<newCell.x<<" y"<<newCell.x<<endl;
                                    pushAttack(newCell, 1, 0);
                                    pushAttack(newCell, 0, 1);
                                    pushAttack(newCell, 1, 1);
                                    pushAttack(newCell, -1, 1);
                                    return;
                                }
                                else if (count == 2) {
                                    //step++;
                                    char st = haveCell(item.begin->x - 2 * item.dx, item.begin->y - 2 * item.dy);
                                    if (st == Empty || st == Zero) {
                                        x = item.begin->x - item.dx;
                                        y = item.begin->y - item.dy;
                                        Cell newCell;
                                        newCell.x = x;
                                        newCell.y = y;
                                        newCell.status = Zero;
                                        decrease(newCell);
                                        cellsO.push_back(newCell);
                                        pushAttack(newCell, 1, 0);
                                        pushAttack(newCell, 0, 1);
                                        pushAttack(newCell, 1, 1);
                                        pushAttack(newCell, -1, 1);
                                        return;
                                    } else {
                                        st = haveCell(item.end->x + 2 * item.dx, item.end->y + 2 * item.dy);
                                        if (st == Empty || st == Zero) {
                                            x = item.end->x + item.dx;
                                            y = item.end->y + item.dy;
                                            Cell newCell;
                                            newCell.x = x;
                                            newCell.y = y;
                                            newCell.status = Zero;
                                            decrease(newCell);
                                            cellsO.push_back(newCell);
                                            pushAttack(newCell, 1, 0);
                                            pushAttack(newCell, 0, 1);
                                            pushAttack(newCell, 1, 1);
                                            pushAttack(newCell, -1, 1);
                                            //cout<<"Zero: x="<<newCell.x<<" y"<<newCell.x<<endl;;
                                            return;
                                        }
                                    }
                                }
                            }
                            else if (item.boardEmptyCells == 1) {
                                if (count == 3) {
                                    if (haveCell(item.begin->x - item.dx, item.begin->y - item.dy) == Empty &&
                                        haveCell(item.begin->x - 2 * item.dx, item.begin->y - 2 * item.dy) ==
                                        Empty) {
                                        x = item.begin->x - item.dx;
                                        y = item.begin->y - item.dy;
                                        Cell newCell;
                                        newCell.x = x;
                                        newCell.y = y;
                                        newCell.status = Zero;
                                        decrease(newCell);
                                        cellsO.push_back(newCell);
                                        pushAttack(newCell, 1, 0);
                                        pushAttack(newCell, 0, 1);
                                        pushAttack(newCell, 1, 1);
                                        pushAttack(newCell, -1, 1);
                                       // cout<<"Zero: x="<<newCell.x<<" y"<<newCell.x<<endl;
                                        return;
                                    }
                                    else if (haveCell(item.end->x + item.dx, item.end->y + item.dy) == Empty &&
                                               haveCell(item.end->x + 2 * item.dx, item.end->y + 2 * item.dy) ==
                                               Empty) {
                                        x = item.end->x + item.dx;
                                        y = item.end->y + item.dy;
                                        Cell newCell;
                                        newCell.x = x;
                                        newCell.y = y;
                                        newCell.status = Zero;
                                        decrease(newCell);
                                        cellsO.push_back(newCell);
                                        //cout<<"Zero: x="<<newCell.x<<" y"<<newCell.x<<endl;
                                        pushAttack(newCell, 1, 0);
                                        pushAttack(newCell, 0, 1);
                                        pushAttack(newCell, 1, 1);
                                        pushAttack(newCell, -1, 1);
                                        return;
                                    }

                                }
                                else if (count == 2) {
                                   // step++;
                                    char st1 = haveCell(item.begin->x - 2 * item.dx, item.begin->y - 2 * item.dy);
                                    char st2 = haveCell(item.begin->x - 3 * item.dx, item.begin->y - 3 * item.dy);

                                    if (haveCell(item.begin->x - item.dx, item.begin->y - item.dy) == Empty) {
                                        if ((st1 == Empty && (st2 == Empty || st2 == Zero)) ||
                                            (st1 == Zero && st2 == Empty)) {
                                            x = item.begin->x - item.dx;
                                            y = item.begin->y - item.dy;
                                            Cell newCell;
                                            newCell.x = x;
                                            newCell.y = y;
                                            newCell.status = Zero;
                                            decrease(newCell);
                                            cellsO.push_back(newCell);
                                            pushAttack(newCell, 1, 0);
                                            pushAttack(newCell, 0, 1);
                                            pushAttack(newCell, 1, 1);
                                            pushAttack(newCell, -1, 1);
                                            //cout<<"Zero: x="<<newCell.x<<" y"<<newCell.x<<endl;
                                            return;
                                        }

                                    } else {
                                        st1 = haveCell(item.end->x + 2 * item.dx, item.end->y + 2 * item.dy);
                                        st2 = haveCell(item.end->x + 2 * item.dx, item.end->y + 2 * item.dy);
                                        if ((st1 == Empty && (st2 == Empty || st2 == Zero)) ||
                                            (st1 == Zero && st2 == Empty)) {
                                            x = item.end->x + item.dx;
                                            y = item.end->y + item.dy;
                                            Cell newCell;
                                            newCell.x = x;
                                            newCell.y = y;
                                            newCell.status = Zero;
                                            decrease(newCell);
                                            cellsO.push_back(newCell);
                                            //cout<<"Zero: x="<<newCell.x<<" y"<<newCell.x<<endl;
                                            pushAttack(newCell, 1, 0);
                                            pushAttack(newCell, 0, 1);
                                            pushAttack(newCell, 1, 1);
                                            pushAttack(newCell, -1, 1);
                                            return;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }

                count--;
            }
        }

        if (max == 0) {
            int k = 0;
            step++;
            assigmentCell(&newCell1, commonCell(&k, Cross));

            if (k > 1) {
                Cell newCell;
                newCell.x = newCell1.x;
                newCell.y = newCell1.y;
                newCell.status = Zero;
                cellsO.push_back(newCell);
                //cout<<"Zero: x="<<newCell.x<<" y"<<newCell.x<<endl;;
                pushAttack(newCell, 1, 0);
                pushAttack(newCell, 0, 1);
                pushAttack(newCell, 1, 1);
                pushAttack(newCell, -1, 1);
                return;
            }
            else {
                k = 0;
                assigmentCell(&newCell1, commonCell(&k, Zero));
                step++;
                if (k > 1) {
                    Cell newCell;
                    newCell.x = newCell1.x;
                    newCell.y = newCell1.y;
                    newCell.status = Zero;
                    //cout<<"Zero: x="<<newCell.x<<" y"<<newCell.x<<endl;
                    cellsO.push_back(newCell);
                    pushAttack(newCell, 1, 0);
                    pushAttack(newCell, 0, 1);
                    pushAttack(newCell, 1, 1);
                    pushAttack(newCell, -1, 1);
                    return;

                } else if (cellsO.size() != 0) {
                    k = 0;
                    for (auto item: cellsO) {
                        temp = statusCells(item.x, item.y, &k, Empty);
                        if (k > max) {
                            max = k;
                            assigmentCell(&newCell1, temp);
                        }
                    }

                    Cell newCell;
                    newCell.x = newCell1.x;
                    newCell.y = newCell1.y;
                    newCell.status = Zero;
                    //cout<<"Zero: x="<<newCell.x<<"y"<<newCell.x<<endl;
                    cellsO.push_back(newCell);
                    pushAttack(newCell, 1, 0);
                    pushAttack(newCell, 0, 1);
                    pushAttack(newCell, 1, 1);
                    pushAttack(newCell, -1, 1);
                    return;
                } else {
                    k = 0;
                    for (auto item: cellsX) {
                        temp = statusCells(item.x, item.y, &k, Empty);
                        if (k > max) {
                            max = k;
                            assigmentCell(&newCell1, temp);
                        }
                    }

                    Cell newCell;
                    newCell.x = newCell1.x;
                    newCell.y = newCell1.y;
                    newCell.status = Zero;
                    //cout<<"Zero: x="<<newCell.x<<"y"<<newCell.x<<endl;
                    cellsO.push_back(newCell);
                    pushAttack(newCell, 1, 0);
                    pushAttack(newCell, 0, 1);
                    pushAttack(newCell, 1, 1);
                    pushAttack(newCell, -1, 1);
                    return;
                }

            }

        }

    }

    void drawBoard() {
        cout << " " << "|";
        for (int i = 0; i < n; i++)
            cout << i << "|";
        cout << endl;
        Cell *temp = &cellsX.front();

        for (int i = 0; i < n; i++) {

            for (int j = 0; j < n; j++)
                cout << "-+";
            cout << "-|";
            cout << endl;

            cout << i << "|";
            for (int j = 0; j < n; j++) {
                char stat = haveCell(j, i);
                if (stat != Empty)
                    cout << stat << "|";
                else cout << " |";
            }
            cout << endl;
        }
        cout << endl << endl;
    }

public:

    bool fullBoard() {
        int m = cellsX.size() + cellsO.size();
        if (m + 1 == n * n && n % 2 == 1) {
            if (attackX.size() != 0) {
                Attack attack = (Attack) attackX.front();
                bool flag = false;
                for (auto item: attackX) {
                    if (item.boardEmptyCells == 0) {
                        flag = true;
                        attack.assigment(item);
                        break;
                    }

                }
                int x, y;
                if (flag) {
                    if (haveCell(attack.begin->x - attack.dx, attack.begin->y - attack.dy) == Empty) {
                        x = attack.begin->x - attack.dx;
                        y = attack.begin->y - attack.dy;
                    } else {
                        x = attack.end->x + attack.dx;
                        y = attack.end->y + attack.dy;
                    }
                    Cell newCell;
                    newCell.x = x;
                    newCell.y = y;
                    newCell.status = Cross;
                    decrease(newCell);
                    cellsO.push_back(newCell);
                    pushAttack(newCell, 1, 0);
                    pushAttack(newCell, 0, 1);
                    pushAttack(newCell, 1, 1);
                    pushAttack(newCell, -1, 1);

                } else m = n * n;
            }
        }
        if (m == n * n) {
            end_time = clock();
            time = (double)(0.01 + end_time - start_time);
//            cout<<"Steps:"<<step<<"  Time:"<<time<<endl;
            drawBoard();
            cout << "friendship won" << endl;
            return true;
            exit(0);
        }
        return false;
    }

   bool pushCell(int x, int y) {

        if (x <= 0 && y <= 0 && x >= n && y >= n) {
            //cout << "Error" << endl << "Repeat the move" << endl;
            return false;
        }

        if (haveCell(x, y) != Empty) {
            //cout << "This cell is busy" << endl << "Repeat the move" << endl;
            return false;
        }

        if(step==0){
            start_time =  clock();
        }
        char st = Cross;
        //cout<<"Cross: x="<<x<<"y"<<y<<endl;
        Cell newCell;
        newCell.x = x;
        newCell.y = y;
        newCell.status = st;
        cellsX.push_back(newCell);
        decrease(newCell);
        pushAttack(newCell, 1, 0);
        pushAttack(newCell, 0, 1);
        pushAttack(newCell, 1, 1);
        pushAttack(newCell, -1, 1);
        checkAllBoard();

        attackZero();
       checkAllBoard();

       return true;
    }

    explicit Board(int n) {
        this->n = n;
    }

    ~

    Board() {
    }

};


