#ifndef __TQ_TERMINAL
#define __TQ_TERMINAL

#include <string>
#include <vector>

namespace Terminal {
void help();
void run();
void printMenu();
void printCity();
void printRoute(int tp);
std::string strfTime(int t);
void printResult(std::vector<int>, int);
} // namespace Terminal

#endif
