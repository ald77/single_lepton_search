#ifndef H_REDUCED_TREE_MAKER
#define H_REDUCED_TREE_MAKER

#include <string>
#include "event_handler.hpp"

class ReducedTreeMaker : public EventHandler{
public:
  ReducedTreeMaker(const std::string& in_file_name,
                   const bool is_list,
                   const double weight_in=1.0);

  void MakeReducedTree(const std::string& out_file_name);
};

#endif
