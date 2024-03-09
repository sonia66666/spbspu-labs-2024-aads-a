#include <iostream>
#include <limits>

#include "constiterator.hpp"
#include "iterator.hpp"
#include "list.hpp"
#include "inputlist.hpp"

int main()
{
  using namespace gladyshev;
  List<std::pair<std::string, List<size_t>>> sequences;
  readSequence(std::cin, sequences);
  if (sequences.empty())
  {
    std::cout << "0\n";
    return 0;
  }
  sequences.reverse();
  for (auto it = sequences.begin(); it != sequences.end(); ++it)
  {
    std::cout << it->first;
    auto temp = it;
    ++temp;
    if (temp != sequences.end())
    {
      std::cout << " ";
    }
  }
  std::cout << "\n";
  bool overflow = false;
  size_t sum = 0;
  size_t counter = 0;
  bool moreData = true;
  List<size_t> listOfNums;
  while (moreData)
  {
    moreData = false;
    for (auto it = sequences.begin(); it != sequences.end(); ++it)
    {
      auto itr = it->second.begin();
      for (size_t i = 0; i < counter; ++i)
      {
        if (itr != it->second.end())
        {
          ++itr;
        }
      }
      if (itr != it->second.end())
      {
         if (moreData)
         {
           std::cout << " ";
         }
         if (std::numeric_limits<size_t>::max() - sum >= *itr)
         {
           sum += *itr;
         }
         else
         {
           overflow = true;
         }
         std::cout << *itr;
         moreData = true;
      }
    }
    if (moreData)
    {
      std::cout << "\n";
      listOfNums.push_front(sum);
      sum = 0;
    }
    ++counter;
  }
  if (overflow)
  {
    std::cerr << "overflow detected\n";
    return 1;
  }
  if (listOfNums.empty())
  {
    std::cout << "0\n";
    return 0;
  }
  listOfNums.reverse();
  for (auto it = listOfNums.begin(); it != listOfNums.end(); ++it)
  {
    std::cout << *it;
    auto temp = it;
    ++temp;
    if (temp != listOfNums.end())
    {
      std::cout << " ";
    }
  }
  std::cout << "\n";
  return 0;
}

