#include <array>
#include <iostream>
#include <unordered_map>

static constexpr int ROWS = 2;
static constexpr int COLUMNS = 3;
static constexpr int GAME_SIZE = 2;

enum class Cell : char
{
    Empty,
    Red,
    Blue
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

void init()
{
    for (auto& column : table)
        for (auto& cell: column)
            cell = Cell::Empty;
}

void printTable()
{
    for (auto& column : table)
    {
        for (auto& cell: column)
            std::cout << cell << ' ';
        std::cout <<  std::endl;
    }
    std::cout <<  std::endl;
}

static int moves = 0;

int makeMove(int col, Cell tok)
{
    auto & column = table[col];
    auto it = column.begin();
    
    while (it != column.end() && *it != Cell::Empty)
        ++it;
        
    if (it == column.end())
        return -1;
    else
    {
        moves += 1;
        *it = tok;
        return std::distance(column.begin(), it);
    }
}

void revertMove(int col)
{
    auto & column = table[col];
    auto it = column.begin();
    
    while (it != column.end() && *it != Cell::Empty)
        ++it;

    moves -= 1;
    *(--it) = Cell::Empty;
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


Cell exploreTree(Cell tok)
{
    std::array<Cellm,
    int nomove = 0;
    for (int c = 0; c < COLUMNS; ++c)
    {
        auto r = makeMove(c, tok);
        if (r < 0)
        {
            nomove += 1;
            continue;
        }
        if (victory(tok, r, c))
        {
            revertMove(c);
            return tok;
        }
        else
        {
            auto r = exploreTree(otherPlayer(tok));
            if (r == tok)
            {
                std::cout << tok << " shortcut" << std::endl;
                revertMove(c);
                return r;
            }
        }
        revertMove(c);
    }
    if (nomove == 0)
        return Cell::Empty;
}

int main(int argc, char *argv[])
{
    init();
    
    std::cout << exploreTree(Cell::Red) << std::endl;
    
    return 0;
}
