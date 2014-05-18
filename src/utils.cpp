#include "utils.hpp"

#include <sstream>
#include <string>
#include "TGraph.h"
#include "TH1.h"
#include "TH1D.h"
#include "TChain.h"

std::string fix_width(const long double number, const std::streamsize width){
  std::ostringstream oss("");
  std::ios_base::fmtflags opts((std::ios::showpoint & (~std::ios::showpos)) | std::ios::right | std::ios::dec | std::ios::fixed);
  oss.fill('0');
  oss.flags(opts);
  oss.precision(width);
  oss << number;
  std::string printed_num(oss.str());
  std::string::size_type decimal_location(printed_num.find('.'));
  printed_num.resize((static_cast<std::string::size_type>(width)>decimal_location)
                     ?width:(decimal_location+1u));
  if(printed_num.size()>0 && printed_num.at(printed_num.size()-1)=='.'){
    printed_num.resize(printed_num.size()-1);
    printed_num=" "+printed_num;
  }
  return printed_num;
}

void get_count_and_uncertainty(TTree& tree,
                               const std::string& cut,
                               double& count,
                               double& uncertainty){
  const std::string hist_name("temp");
  TH1D temp(hist_name.c_str(), "", 1, -1.0, 1.0);
  tree.Project(hist_name.c_str(), "0.0", cut.c_str());
  count=temp.IntegralAndError(1,1,uncertainty);
}

void add_point(TGraph& graph, const double x, const double y){
  graph.SetPoint(graph.GetN(), x, y);
}

double get_maximum(const TH1& h){
  return h.GetBinContent(h.GetMaximumBin());
}

double get_maximum(const TGraph& g){
  if(g.GetN()<=0){
    return 0.0;
  }else{
    double x(0.0), y(0.0), max(0.0);
    g.GetPoint(1, x, max);
    for(unsigned short i(2); i<=g.GetN(); ++i){
      g.GetPoint(1, x, y);
      if(y>max) max=y;
    }
    return max;
  }
}

double get_minimum(const TH1& h){
  return h.GetBinContent(h.GetMinimumBin());
}

double get_minimum(const TGraph& g){
  if(g.GetN()<=0){
    return 0.0;
  }else{
    double x(0.0), y(0.0), min(0.0);
    g.GetPoint(1, x, min);
    for(unsigned short i(2); i<=g.GetN(); ++i){
      g.GetPoint(1, x, y);
      if(y<min) min=y;
    }
    return min;
  }
}

double get_minimum_positive(const TH1& h){
  if(h.GetNbinsX()==0){
    return 0.0;
  }else{
    double min(h.GetBinContent(1));
    for(unsigned short bin(2); bin<=h.GetNbinsX(); ++bin){
      if(h.GetBinContent(bin)<min && h.GetBinContent(bin)>0.0){
        min=h.GetBinContent(bin);
      }
    }
    return min;
  }
}

double get_minimum_positive(const TGraph& g){
  if(g.GetN()==0){
    return 0.0;
  }else{
    double x(0.0), y(0.0), min(0.0);
    g.GetPoint(1, x, min);
    for(unsigned short i(2); i<=g.GetN(); ++i){
      g.GetPoint(1, x, y);
      if(y<min && y>0.0) min=y;
    }
    return min;
  }
}

void normalize(TH1& h){
  h.Scale(1.0/h.Integral("width"));
}
