/*
  Produces reduced_trees from cfA format ntuples.
  Input: cfA format .root file (file path given with -i option)
  Output: reduced_tree format .root file (file path may optionally be specified with -o option)
  Options:
  -i: Set input file name. Only one file path accepted, but may contain wildcards.
  -c: Denotes that input name is only a cfA ntuple name and program should intelligently figure out the full path
  -o: Explicitly set output file name (automatically determined if not set)
*/

#include <iostream>
#include <string>
#include <unistd.h>
#include "reduced_tree_maker.hpp"
#include "weights.hpp"

int main(int argc, char *argv[]){
  std::string inFilename("");
  bool iscfA(false);
  bool explicit_outfile(false);
  std::string outFilename("");

  int c(0);
  while((c=getopt(argc, argv, "i:o:c"))!=-1){
    switch(c){
    case 'i':
      inFilename=optarg;
      break;
    case 'c':
      iscfA=true;
      break;
    case 'o':
      explicit_outfile=true;
      outFilename=optarg;
    default:
      break;
    }
  }
  
  if(iscfA){
    if(!explicit_outfile) outFilename="reduced_trees/"+inFilename+".root";
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
    if(!explicit_outfile) outFilename="reduced_trees/"+baseName+".root";
    std::cout << inFilename << "\n" << baseName << "\n" << outFilename << "\n";
  }

  WeightCalculator w(19399);
  ReducedTreeMaker rtm(inFilename, false, w.GetWeight(inFilename));
  rtm.MakeReducedTree(outFilename);
}
