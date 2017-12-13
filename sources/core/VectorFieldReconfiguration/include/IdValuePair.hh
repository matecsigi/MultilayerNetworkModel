#ifndef IDVALUEPAIR_HH
#define IDVALUEPAIR_HH

class IdValuePair
{
public:
  IdValuePair(int id, double value)
  {
    mId = id;
    mValue = value;
  }
  ~IdValuePair(){};

  int mId;
  double mValue;
};

#endif
