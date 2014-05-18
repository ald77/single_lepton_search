#include <ctime>
#include <stdint.h>
#include <iostream>
#include <string>
#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "utils.hpp"

int main(int argc, char *argv[]){
  for(int arg(1); arg<argc; ++arg){
    TFile file(argv[arg],"read");
    if(file.IsOpen() && !file.IsZombie()){
      TTree *tree(NULL);
      file.GetObject("meta_info", tree);
      if(tree!=NULL){
        std::string* original_file_name(NULL);
        uint16_t reduced_tree_version(0);
        uint16_t utc_creation_year(0);
        uint8_t utc_creation_month(0);
        uint8_t utc_creation_day(0);
        uint8_t utc_creation_hour(0);
        uint8_t utc_creation_minute(0);
        uint8_t utc_creation_second(0);
        int32_t utc_creation_isdst(0);
        uint32_t original_file_entries(0);

        tree->SetBranchStatus("*",false);
        setup(*tree, "original_file_name", original_file_name);
        setup(*tree, "original_file_entries", original_file_entries);
        setup(*tree, "reduced_tree_version", reduced_tree_version);
        setup(*tree, "utc_creation_year", utc_creation_year);
        setup(*tree, "utc_creation_month", utc_creation_month);
        setup(*tree, "utc_creation_day", utc_creation_day);
        setup(*tree, "utc_creation_hour", utc_creation_hour);
        setup(*tree, "utc_creation_minute", utc_creation_minute);
        setup(*tree, "utc_creation_second", utc_creation_second);
        setup(*tree, "utc_creation_isdst", utc_creation_isdst);

        const int num_entries(tree->GetEntries());
        if(num_entries>0){
          if(num_entries>1){
            std::cerr << "Warning: tree meta_info has multiple entries in file " << argv[arg] << '.' << std::endl;
          }
          tree->GetEntry(0);
          struct tm ptm;
          ptm.tm_year=utc_creation_year-1900;
          ptm.tm_mon=utc_creation_month-1;
          ptm.tm_mday=utc_creation_day;
          ptm.tm_hour=utc_creation_hour;
          ptm.tm_min=utc_creation_minute;
          ptm.tm_sec=utc_creation_second;
          ptm.tm_isdst=utc_creation_isdst;

          const time_t time(mktime(&ptm));
          const std::string time_string(ctime(&time));

          std::cout << "   reduced_tree file: " << argv[arg] << '\n'
                    << "    cfA n-tuple file: " << *original_file_name << '\n'
                    << "reduced_tree version: " << reduced_tree_version << '\n'
                    << "            Produced: " << time_string
                    << " cfA n-tuple entries: " << original_file_entries << '\n' << std::endl;
        }else{
          std::cerr << "Error: tree meta_info has no entries in file " << argv[arg] << '.' << std::endl;
        }
      }else{
        std::cerr << "Error: Could not open find tree meta_info in file " << argv[arg] << '.' << std::endl;
      }
      file.Close();
    }else{
      std::cerr << "Error: Could not open file " << argv[arg] << '.' << std::endl;
    }
  }
}
