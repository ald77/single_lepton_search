#ifndef H_CLASSMAKER
#define H_CLASSMAKER

#include <fstream>
#include <string>
#include "TFile.h"
#include "TChain.h"
#include "TLeafObject.h"

void PrintLeaves(TChain *, std::ofstream &);
void PrintBranches(TChain *, std::ofstream &);
void PrintNullInit(TChain *, std::ofstream &);
void PrintSetNull(TChain *, std::ofstream &);
void PrintBranchInit(TChain *, std::ofstream &);
void PrintSetBranchAddressA(TChain *, std::ofstream &);
void PrintSetBranchAddressB(TChain *, std::ofstream &);

#endif
