#include "TestUtility.hh"
#include <algorithm>

bool containsDuplicate(std::vector<int> &vectorToCheck)
{
  std::sort(vectorToCheck.begin(), vectorToCheck.end());
  for(unsigned i=0; i<vectorToCheck.size()-1; ++i)
  {
    if(vectorToCheck[i] == vectorToCheck[i+1])
    {
      return true;
    }
  }
  return false;
}
