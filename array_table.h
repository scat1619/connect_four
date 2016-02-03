#ifndef ARRAY_TABLE_H
#define ARRAY_TABLE_H

using Column = std::array<Cell, ROWS>;
using Table = std::array<Column, COLUMNS>;

Table table;
std::array<int, COLUMNS> empty;

void init()
{
    for (auto& column : table)
        for (auto& cell: column)
            cell = Cell::Empty;

    for (auto& e : empty)
        e = 0;
}

void printTable(const Table& t)
{
    for (auto& column : t)
    {
        for (auto& cell: column)
            std::cout << cell << ' ';
        std::cout <<  std::endl;
    }
    std::cout <<  std::endl;
}

static int depth = 0;

int makeMove(int col, Cell tok)
{
    auto row = empty[col];
    if (row == ROWS)
    {
        return -1;
    }
    else
    {
        table[col][row] = tok;
        empty[col]++; depth++;
        return row;
    }
}

void revertMove(int col)
{
    auto row = --empty[col];
    assert(row < ROWS && row >= 0);
    depth--;
    table[col][row] = Cell::Empty;
}

bool victory(Cell tok, int r, int c)
{
    int m = 0;
    for (int i = 0; i < ROWS; ++i)
    {
        if (table[c][i] == tok)
        {
            m+=1;
        }
        else
            m = 0;
        if (m >= GAME_SIZE) return true;
    }
    m = 0;
    for (int i = 0; i < COLUMNS; ++i)
    {
        if (table[i][r] == tok)
        {
            m+=1;
        }
        else
            m = 0;
        if (m >= GAME_SIZE) return true;
    }
    m = 0;
    for (int i = -std::min(c,r); i < std::min(COLUMNS-c, ROWS-r); ++i)
    {
        if (table[c+i][r+i] == tok)
            m+=1;
        else
            m = 0;
        if (m >= GAME_SIZE) return true;
    }
    m = 0;
    for (int i = -std::min(c,ROWS - r - 1); i < std::min(COLUMNS-c, r+1); ++i)
    {
        if (table[c+i][r-i] == tok)
            m+=1;
        else
            m = 0;
        if (m >= GAME_SIZE) return true;
    }
    return false;
}

Cell otherPlayer(Cell tok)
{
    if (tok == Cell::Red)
        return Cell::Blue;
    else
        return Cell::Red;
}

std::uint64_t hashTable(const Table& table)
{
    int hash1 = 0;
    for (int c = 0; c < COLUMNS; ++c)
    {
        for (int r = 0; r < ROWS; ++r)
        {
            hash1 *= 3;
            hash1 += (int)table[c][r];
        }
    }

    return hash1;
}


#endif /* ARRAY_TABLE_H */
