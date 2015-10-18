#include <array>
#include <vector>
#include <iostream>
#include <unordered_map>
#include <assert.h>

static constexpr int ROWS = 4;
static constexpr int COLUMNS = 6;
static constexpr int GAME_SIZE = 4;

enum class Cell : char
{
    Empty = 0,
    Red   = 1,
    Blue  = 2
};

std::ostream& operator<<(std::ostream& os, const Cell& c)
{
    switch (c)
    {
    case Cell::Empty: os << "Empty"; break;
    case Cell::Red:   os << "Red";   break;
    case Cell::Blue:  os << "Blue";  break;
    }

    return os;
}

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

std::unordered_map<std::uint64_t, std::pair<Table, Cell>> solved;

static int iter = 0;

Cell exploreTree(Cell tok)
{
    int nomove = 0;
    bool canDraw = false;
    
    for (int c = 0; c < COLUMNS; ++c)
    {
        iter++;
        auto r = makeMove(c, tok);
        auto h = hashTable(table);

        if (r < 0)
        {
            nomove += 1;
            continue;
        }

        auto it = solved.find(h);
        if (it != solved.end())
        {
            if (it->second.first == table)
            {
                auto rt = it->second.second;
                revertMove(c);
                if (rt == tok)
                {
                    return rt;
                }
                else if (rt == Cell::Empty)
                {
                    canDraw = true;
                }
                continue;
            }
        }

        if (victory(tok, r, c))
        {
            revertMove(c);
            return tok;
        }
        else
        {
            auto rt = exploreTree(otherPlayer(tok));
            if (depth < 0.90 * ROWS * COLUMNS) 
                solved[h] = std::make_pair(table, rt);
            if (depth == 1)
            {
                std::cout << tok << " s  " << c << " " << rt << std::endl;
            }
            revertMove(c);
            if (rt == tok)
            {
                return tok;
            }
            else if (rt == Cell::Empty)
            {
                canDraw = true;
            }
        }
    }

    if (canDraw || nomove == COLUMNS)
    {
        return Cell::Empty;
    }
    
    return otherPlayer(tok);
}

int main(int argc, char *argv[])
{
    init();
    auto winner = exploreTree(Cell::Red);
    std::cout << "Done " <<  winner
              <<  " after " << iter << " moves" <<  std::endl;

    return 0;
}
