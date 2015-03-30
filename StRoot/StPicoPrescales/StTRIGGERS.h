#ifndef StTRIGGERS_H
#define StTRIGGERS_H

#include <vector>

namespace trigDef
{
  enum trgNumbers{VPDMB,BBCMB,HT0VPDMB,HT1VPDMB,HT2,HT0BBCMBTOF0,HT1BBCMBTOF0,HT2BBCMB,BBCMBTOF0};
  int const triggerArray[] = {370011,370022,370501,370511,370531,370542,370546,370522,370301};
  std::vector<int> const triggers(triggerArray,triggerArray+sizeof(triggerArray)/sizeof(int));
}

#endif
