#ifndef TRACE_HH
#define TRACE_HH

#include <iostream>

#ifndef TRACELEVEL
#define TRACELEVEL 1
#endif

#define traceRun(...) tracePrint(1, ##__VA_ARGS__)
#define traceDebug(...) tracePrint(2, ##__VA_ARGS__)

#define tracePrint(level, ...)  \
  if(level > TRACELEVEL) \
  { \
    std::cout<<__VA_ARGS__<<std::endl; \
  }

#endif
