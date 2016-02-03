#ifndef BIT_TABLE_H
#define BIT_TABLE_H

struct BitTable
{
  constexpr static int Rows, Columns;
  
  void init()
  {
    data[0]=data[1]=0;
  }

  void makeMove(int col)
  {
    1 << col 
  }

  std::uint64_t data[2];
};


#endif /* BIT_TABLE_H */
