#include "generate_cfa_class.hpp"
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include "TLeafObject.h"

int main(int argc, char *argv[]){
  if(argc>1){
    TFile inFile(argv[1],"read");
    if(inFile.IsOpen() && !inFile.IsZombie()){
      std::ofstream cppFile("src/cfa.cpp"), hppFile("inc/cfa.hpp");

      {TTree t("a","b");}//Magically make ROOT link things correctly...

      TChain *chainA(static_cast<TChain*>(inFile.Get("configurableAnalysis/eventA"))), *chainB(static_cast<TChain*>(inFile.Get("configurableAnalysis/eventB")));

      if(chainA!=NULL && chainB!=NULL){
        hppFile << "#ifndef H_CFA\n";
        hppFile << "#define H_CFA\n\n";
        hppFile << "#include <vector>\n";
        hppFile << "#include <string>\n";
        hppFile << "#include \"TChain.h\"\n";
        hppFile << "#include \"TBranch.h\"\n\n";

        hppFile << "class cfA{\n";
        hppFile << "protected:\n";
        hppFile << "  cfA(const std::string&, const bool);\n";
        hppFile << "  TChain chainA, chainB;\n";
        hppFile << "  TChain* GetChainA();\n";
        hppFile << "  TChain* GetChainB();\n";
        hppFile << "  std::string GetSampleName() const;\n";
        hppFile << "  int GetTotalEntries() const;\n";
        hppFile << "  int GetEntry(const unsigned int);\n";
        hppFile << "  void SetFile(const std::string&, const bool);\n\n";

        hppFile << "  std::string sampleName;\n";
        hppFile << "  int totalEntries;\n";
        hppFile << "  short cfAVersion;\n\n";
        hppFile << "  void GetVersion();\n";
        hppFile << "  void AddFiles(const std::string&, const bool);\n";
        hppFile << "  void CalcTotalEntries();\n";
        hppFile << "  void PrepareNewChains();\n";
        hppFile << "  void InitializeA();\n";
        hppFile << "  void InitializeB();\n\n";
    
        PrintLeaves(chainA, hppFile);
        PrintBranches(chainA, hppFile);
        PrintLeaves(chainB, hppFile);
        PrintBranches(chainB, hppFile);
    
        hppFile << "};\n\n";
        hppFile << "#endif" << std::endl;
    
        cppFile << "#include \"cfa.hpp\"\n";
        cppFile << "#include <vector>\n";
        cppFile << "#include <string>\n";
        cppFile << "#include <fstream>\n";
        cppFile << "#include <sstream>\n";
        cppFile << "#include \"TChain.h\"\n";
        cppFile << "#include \"TBranch.h\"\n\n";
        cppFile << "cfA::cfA(const std::string& fileIn, const bool isList):\n";
        cppFile << "  chainA(\"eventA\"),\n";
        cppFile << "  chainB(\"eventB\"),\n";
        cppFile << "  sampleName(fileIn),\n";
        cppFile << "  totalEntries(0),\n";
        cppFile << "  cfAVersion(-1),\n";
        PrintNullInit(chainA, cppFile);
        PrintBranchInit(chainA, cppFile);
        cppFile << ",\n";
        PrintNullInit(chainB, cppFile);
        PrintBranchInit(chainB, cppFile);
        cppFile << "{\n";
        cppFile << "  GetVersion();\n";
        cppFile << "  AddFiles(fileIn, isList);\n";
        cppFile << "  PrepareNewChains();\n";
        cppFile << "}\n\n";

        cppFile << "void cfA::GetVersion(){\n";
        cppFile << "  size_t pos(sampleName.rfind(\"_v\"));\n";
        cppFile << "  if(pos!=std::string::npos && pos<sampleName.size()-2){\n";
        cppFile << "    std::istringstream iss(sampleName.substr(pos+2));\n";
        cppFile << "    iss >> cfAVersion;\n";
        cppFile << "    if(iss.fail() || iss.bad()){\n";
        cppFile << "      cfAVersion=-1;\n";
        cppFile << "    }\n";
        cppFile << "  }\n";
        cppFile << "}\n\n";

        cppFile << "void cfA::PrepareNewChains(){\n";
        cppFile << "  InitializeA();\n";
        cppFile << "  InitializeB();\n";
        cppFile << "  CalcTotalEntries();\n";
        cppFile << "}\n\n";

        cppFile << "void cfA::AddFiles(const std::string& fileIn, const bool isList){\n";
        cppFile << "  if(isList){\n";
        cppFile << "    std::ifstream infile(fileIn.c_str());\n";
        cppFile << "    std::string file(\"\");\n";
        cppFile << "    while(infile >> file){\n";
        cppFile << "      chainA.Add((file+\"/configurableAnalysis/eventA\").c_str());\n";
        cppFile << "      chainB.Add((file+\"/configurableAnalysis/eventB\").c_str());\n";
        cppFile << "    }\n";
        cppFile << "    infile.close();\n";
        cppFile << "  }else{\n";
        cppFile << "    chainA.Add((fileIn+\"/configurableAnalysis/eventA\").c_str());\n";
        cppFile << "    chainB.Add((fileIn+\"/configurableAnalysis/eventB\").c_str());\n";
        cppFile << "  }\n";
        cppFile << "}\n\n";

        cppFile << "void cfA::SetFile(const std::string& fileIn, const bool isList){\n";
        cppFile << "  chainA.Reset(); chainB.Reset();\n";
        cppFile << "  AddFiles(fileIn, isList);\n";
        cppFile << "}\n\n";

        cppFile << "int cfA::GetEntry(const unsigned int entryIn){\n";
        cppFile << "  return chainA.GetEntry(entryIn)+chainB.GetEntry(entryIn);\n";
        cppFile << "}\n\n";

        cppFile << "void cfA::CalcTotalEntries(){\n";
        cppFile << "  const int nEntriesA(chainA.GetEntries()), nEntriesB(chainB.GetEntries());\n";
        cppFile << "  if (nEntriesA!=nEntriesB){\n";
        cppFile << "    totalEntries=-1;\n";
        cppFile << "  }else{\n";
        cppFile << "    totalEntries=nEntriesA;\n";
        cppFile << "  }\n";
        cppFile << "}\n\n";

        cppFile << "TChain* cfA::GetChainA(){\n";
        cppFile << "  return &chainA;\n";
        cppFile << "}\n\n";

        cppFile << "TChain* cfA::GetChainB(){\n";
        cppFile << "  return &chainB;\n";
        cppFile << "}\n\n";

        cppFile << "std::string cfA::GetSampleName() const{\n";
        cppFile << "  return sampleName;\n";
        cppFile << "}\n\n";

        cppFile << "int cfA::GetTotalEntries() const{\n";
        cppFile << "  return totalEntries;\n";
        cppFile << "}\n\n";

        cppFile << "void cfA::InitializeA(){\n";
        PrintSetNull(chainA, cppFile);
        PrintSetBranchAddressA(chainA, cppFile);
        cppFile << "}\n\n";

        cppFile << "void cfA::InitializeB(){\n";
        PrintSetNull(chainB, cppFile);
        PrintSetBranchAddressB(chainB, cppFile);
        cppFile << "}\n\n";
      }else{
        std::cout << "Warning in " << argv[0] << ": one or both of chainA and chainB are NULL (" << chainA << " and " << chainB << ").\n";
      }

      inFile.Close();
      cppFile.close();
      hppFile.close();
    }else{
      std::cout << "Warning in " << argv[0] << ": Could not open " << argv[1] << ".\n";
    }
  }
}

void PrintLeaves(TChain *theChain, std::ofstream &theFile){
  for(int i(0); i<theChain->GetListOfLeaves()->GetSize(); ++i){
    std::string typeName(static_cast<TLeafObject*>((theChain->GetListOfLeaves()->At(i)))->GetTypeName());
    std::string varName(static_cast<TLeaf*>((theChain->GetListOfLeaves()->At(i)))->GetBranch()->GetName());
    bool nonSimp(false);
    for(unsigned long j(typeName.find("vector")); j!=std::string::npos; j=typeName.find("vector",j+6)){
      typeName.replace(j,6,"std::vector");
      nonSimp=true;
    }
    for(unsigned long j(typeName.find("string")); j!=std::string::npos; j=typeName.find("string",j+6)){
      typeName.replace(j,6,"std::string");
      nonSimp=true;
    }
    if(nonSimp){
      theFile << "  " << typeName << " *" << varName << ";\n";
    }else{
      theFile << "  " << typeName << " " << varName << ";\n";
    }
  }
}

void PrintBranches(TChain *theChain, std::ofstream &theFile){
  for(int i(0); i<theChain->GetListOfLeaves()->GetSize(); ++i){
    theFile << "  TBranch *b_" << static_cast<TLeaf*>(theChain->GetListOfLeaves()->At(i))->GetBranch()->GetName() << ";\n";
  }
}

void PrintNullInit(TChain *theChain, std::ofstream &theFile){
  for(int i(0); i<theChain->GetListOfLeaves()->GetSize(); ++i){
    const std::string typeName(static_cast<TLeafObject*>(theChain->GetListOfLeaves()->At(i))->GetTypeName());
    if(typeName.find("string")==std::string::npos){
      theFile << "  " << static_cast<TLeaf*>(theChain->GetListOfLeaves()->At(i))->GetBranch()->GetName() << "(0),\n";
    }else{
      theFile << "  " << static_cast<TLeaf*>(theChain->GetListOfLeaves()->At(i))->GetBranch()->GetName() << "(0),\n";
    }
  }  
}

void PrintSetNull(TChain *theChain, std::ofstream &theFile){
  for(int i(0); i<theChain->GetListOfLeaves()->GetSize(); ++i){
    const std::string typeName(static_cast<TLeafObject*>(theChain->GetListOfLeaves()->At(i))->GetTypeName());
    if(true || typeName.find("string")!=std::string::npos || typeName.find("vector")!=std::string::npos){
      theFile << "  " << static_cast<TLeaf*>(theChain->GetListOfLeaves()->At(i))->GetBranch()->GetName() << "=0;\n";
    }
  }
}

void PrintBranchInit(TChain *theChain, std::ofstream &theFile){
  const int theSize(theChain->GetListOfLeaves()->GetSize());
  for(int i(0); i<theSize-1; ++i){
    theFile << "  b_" << static_cast<TLeaf*>(theChain->GetListOfLeaves()->At(i))->GetBranch()->GetName() << "(),\n";
  }
  if(theChain->GetListOfLeaves()->GetSize()!=0){
    theFile << "  b_" << static_cast<TLeaf*>(theChain->GetListOfLeaves()->At(theSize-1))->GetBranch()->GetName() << "()";
  }
}

void PrintSetBranchAddressA(TChain *theChain, std::ofstream &theFile){
  for(int i(0); i<theChain->GetListOfLeaves()->GetSize(); ++i){
    const std::string name(static_cast<TLeaf*>(theChain->GetListOfLeaves()->At(i))->GetBranch()->GetName());
    theFile << "  chainA.SetBranchAddress(\"" << name << "\", &" << name << ", &b_" << name << ");\n";
  }  
}

void PrintSetBranchAddressB(TChain *theChain, std::ofstream &theFile){
  for(int i(0); i<theChain->GetListOfLeaves()->GetSize(); ++i){
    const std::string name(static_cast<TLeaf*>(theChain->GetListOfLeaves()->At(i))->GetBranch()->GetName());
    theFile << "  chainB.SetBranchAddress(\"" << name << "\", &" << name << ", &b_" << name << ");\n";
  }  
}
