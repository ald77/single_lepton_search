#include <iostream>
#include <string>
#include <unistd.h>
#include "reduced_tree_maker.hpp"
#include "weights.hpp"

int main(int argc, char *argv[]){
  std::string inFilename("");
  bool iscfA(false);
  int c(0);
  while((c=getopt(argc, argv, "i:c"))!=-1){
    switch(c){
    case 'i':
      inFilename=optarg;
      break;
    case 'c':
      iscfA=true;
      break;
    default:
      break;
    }
  }
  
  std::string outFilename("");
  if(iscfA){
    outFilename="reduced_trees/"+inFilename+".root";
    inFilename="/net/cms2/cms2r0/cfA/"+inFilename+"/cfA_"+inFilename+"*.root";
  }else{
    std::string baseName(inFilename);
    size_t pos(baseName.find(".root"));
    if(pos!=std::string::npos){
      baseName.erase(pos);
    }
    pos=baseName.rfind("/");
    if(pos!=std::string::npos){
      if(pos!=baseName.size()-1){
        baseName.erase(0,pos+1);
      }else{
        baseName.append("file_name_ended_with_slash");
      }
    }
    outFilename="reduced_trees/"+baseName+".root";
    std::cout << inFilename << "\n" << baseName << "\n" << outFilename << "\n";
  }

  WeightCalculator w(19399);
  ReducedTreeMaker rtm(inFilename, false, w.GetWeight(inFilename));
  rtm.MakeReducedTree(outFilename);
}
