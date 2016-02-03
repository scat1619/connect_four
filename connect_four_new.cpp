#include <array>
#include <vector>
#include <iostream>
#include <unordered_map>
#include <assert.h>

static constexpr int ROWS = 5;
static constexpr int COLUMNS = 4;
static constexpr int GAME_SIZE = 4;

enum class Cell : char
{
    Empty = 2,
    Red   = 0,
    Blue  = 1
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

//using Column = std::array<Cell, ROWS>;
//using Table = std::array<Column, COLUMNS>;

//Table table;
std::array<int, COLUMNS> empty;

using Table = std::array<std::uint64_t, 2>;
std::array<std::uint64_t, 2> table;

void init()
{
    // for (auto& column : table)
    //     for (auto& cell: column)
    //         cell = Cell::Empty;

    table[0] = table[1] = 0ull;
    
    for (auto& e : empty)
        e = 0;
}

Cell getValue(int c, int r)
{
    std::cout << 'i' << c << ' ' << r << std::endl;
    
    auto t = 1ull << c * COLUMNS + r;

    std::cout << 'c' <<  (table[0] & t) << ' ' << (table[1] & t) << std::endl;

    assert(!((table[0] & t) && (table[1] & t)));

    return (table[0] & t) ? Cell::Red : 
        (table[1] & t) ? Cell::Blue : Cell::Empty;
}


void printTable(const Table& t)
{
    for (int c = 0; c < COLUMNS;  ++c)
    {
        for (int r = 0; r < ROWS; ++r)
        {
            std::cout << getValue(c,r) << ' ';
        }
        std::cout << '\n';
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
        auto t = 1ull << col * COLUMNS + row;
        table[(int)tok] |= t;
        empty[col]++; depth++;
        return row;
    }
}

void revertMove(int col)
{
    auto row = --empty[col];
    assert(row < ROWS && row >= 0);
    depth--;
    auto t = ~(1ull << col * COLUMNS + row);
    table[0] &= t;
    table[1] &= t;
}

bool victory(Cell tok, int r, int c)
{
    int m = 0;
    for (int i = 0; i < ROWS; ++i)
    {
        if (getValue(c, i) == tok)
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
        if (getValue(i, r) == tok)
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
        if (getValue(c+i, r+i) == tok)
            m+=1;
        else
            m = 0;
        if (m >= GAME_SIZE) return true;
    }
    m = 0;
    for (int i = -std::min(c,ROWS - r - 1); i < std::min(COLUMNS-c, r+1); ++i)
    {
        if (getValue(c+i, r-i) == tok)
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
    
    return table[0] * table[1];
}

struct state
{
    Table t_;
    Cell w_;
    int d_;
};

std::unordered_map<std::uint64_t, state> solved;

void sweep(float p = 0.65)
{
    const auto sz = solved.size();
    if (sz < 1024 * 1024 * 1024 / sizeof(state))
        return;
    
    auto it = solved.begin();
    while (it != solved.end())
    {
        if (it->second.d_ > (ROWS * COLUMNS * p))
            it = solved.erase(it);
        else
            ++it;
    }

    if (sz < 2 * solved.size())
        sweep(0.9 * p);
}

// Max score goes first
using MoveOrder = std::array<int, COLUMNS>;
using LevelMoveOrder = std::array<MoveOrder, ROWS * COLUMNS>;

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

        sweep();
        
        auto it = solved.find(h);
        if (it != solved.end())
        {
            auto& s = it->second;
            if (s.t_ == table)
            {
                auto rt = s.w_;
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
            if (depth < 0.95 * ROWS * COLUMNS) 
                solved[h] =  {table, rt, depth};
            if (depth == 1)
            {
                std::cout << tok << " s  " << c << " " << rt << " " << solved.size() << std::endl;
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
