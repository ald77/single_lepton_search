#include "in_json_2012.hpp"

//root function to filter cfA using JSON File.
//Usage: #include "/afs/cern.ch/user/w/wto/public/scripts/inJSON2012.h"
//Create Vector of Run and Lumi by calling vector< vector<int> > VRunLumi = MakeVRunLumi("Golden") before the event loop.
//You can print of a list of Lumiblock by calling CheckVRunLumi(VRunLumi);
//Check if your run is inJSON by calling bool inJSON(VRunLumi,run,lumiblock) in the event loop..
//  13Jul files contain most of 2012 A and B runs, while 06Aug files contain
//      five runs only (namely 190782-190949) which suffered from data corruption 
//      (different conditions needed for those runs, the corresponding lumi is 82/pb.)

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

//using namespace std;
std::vector< std::vector<int> > MakeVRunLumi(std::string input){
  std::ifstream orgJSON;
  if(input == "Golden" || input == "Prompt"){
    orgJSON.open("/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions12/8TeV/Prompt/Cert_190456-208686_8TeV_PromptReco_Collisions12_JSON.txt");
  }
  else if(input == "13Jul" || input == "Jul13"){
    orgJSON.open("/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions12/8TeV/Reprocessing/Cert_190456-196531_8TeV_13Jul2012ReReco_Collisions12_JSON_v2.txt");
  }
  else if(input == "06Aug" || input == "Aug06" || input == "Aug6"){
    orgJSON.open("/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions12/8TeV/Reprocessing/Cert_190782-190949_8TeV_06Aug2012ReReco_Collisions12_JSON.txt");
  }
  else if(input == "24Aug" || input == "Aug24"){
    orgJSON.open("/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions12/8TeV/Reprocessing/Cert_198022-198523_8TeV_24Aug2012ReReco_Collisions12_JSON.txt");
  }
  else if(input == "MuonPhys" || input == "Muon"){
    orgJSON.open("/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions12/8TeV/Prompt/Cert_190456-204567_8TeV_PromptReco_Collisions12_JSON_MuonPhys.txt");
  }
  else if(input == "DCS" || input == "DCSOnly"){
    orgJSON.open("/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions12/8TeV/DCSOnly/json_DCSONLY.txt");
  }
  else{
    orgJSON.open(input.c_str());
  }
  std::vector<int> VRunLumi;
  if(orgJSON.is_open()){
    char inChar;
    int inInt;
    std::string str;
    while(!orgJSON.eof()){
      char next = orgJSON.peek();
      if( next == '1' || next == '2' || next == '3' ||
          next == '4' || next == '5' || next == '6' ||
          next == '7' || next == '8' || next == '9' || 
          next == '0'){     
        orgJSON >>inInt;
        VRunLumi.push_back(inInt);        
      }
      else if(next == ' '){
        getline(orgJSON,str,' ');
      }
      else{
        orgJSON>>inChar;
      }
    }
  }//check if the file opened.
  else{
    std::cout<<"Invalid JSON File!\n";
  }
  orgJSON.close();
  if(VRunLumi.size() == 0){
    std::cout<<"No Lumiblock found in JSON file\n";
  }
  std::vector< std::vector<int> > VVRunLumi;
  for(unsigned int i = 0; i+2 < VRunLumi.size();){
    if(VRunLumi[i] > 130000){
      std::vector<int> RunLumi;
      RunLumi.push_back(VRunLumi[i]);
      while(VRunLumi[i+1] < 130000 && i+1 < VRunLumi.size()){
        RunLumi.push_back(VRunLumi[i+1]);
        ++i;
      }
      VVRunLumi.push_back(RunLumi);
      ++i;
    }
  }
  return VVRunLumi;
}

bool inJSON(std::vector< std::vector<int> > VVRunLumi, int Run, int LS){
  bool answer = false;
  if(Run < 120000){
    answer = true;
  }
  else{
    for(unsigned int i = 0; i < VVRunLumi.size();++i){
      if(Run == VVRunLumi[i][0]){
        for(unsigned int j = 1; j+1 < VVRunLumi[i].size();j=j+2){
          if(LS >= VVRunLumi[i][j] && LS <= VVRunLumi[i][j+1]){
            answer = true;
          }
        }
      }
    }
  }
  return answer;
}

void CheckVRunLumi(std::vector< std::vector<int> > VVRunLumi){
  for(unsigned int i = 0; i < VVRunLumi.size();++i){
    std::cout<<"Run:"<<VVRunLumi[i][0]<<" LS: ";
    for(unsigned int j = 1; j+1 < VVRunLumi[i].size();j=j+2){
      std::cout<<VVRunLumi[i][j]<<"-"<<VVRunLumi[i][j+1]<<" ";
    }
    std::cout<<std::endl;
  }
}

void CheckVRunLumi2(std::vector< std::vector<int> > VVRunLumi){
  for(unsigned int i = 0; i < VVRunLumi.size();++i){
    for(unsigned int j = 1; j+1 < VVRunLumi[i].size();j=j+2){
      if(VVRunLumi[i][j] == VVRunLumi[i][j+1]){
        std::cout<<VVRunLumi[i][0]<<" "<<VVRunLumi[i][j]<<std::endl;
      }
      else{
        for(int k=VVRunLumi[i][j];k<=VVRunLumi[i][j+1];++k){
          std::cout<<VVRunLumi[i][0]<<" "<<k<<std::endl;
        }
      }
    }
    std::cout<<std::endl;
  }
}
