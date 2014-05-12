#ifndef INJSON2012_H
#define INJSON2012_H

#include <vector>
#include <string>

std::vector<std::vector<int> > MakeVRunLumi(std::string input);
bool inJSON(std::vector<std::vector<int> > VRunLumi, int Run, int LS);
void CheckVRunLumi(std::vector<std::vector<int> > VRunLumi);
void CheckVRunLumi2(std::vector<std::vector<int> > VVRunLumi);

#endif //INJSON2012_H
