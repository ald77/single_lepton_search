#ifndef H_UTILS
#define H_UTILS

#include <string>
#include <cmath>
#include <ios>
#include <vector>
#include "TH1D.h"
#include "TChain.h"
#include "TColor.h"
#include "TGraph.h"
#include "TH1.h"

std::string fix_width(const long double number, const std::streamsize width);

void get_count_and_uncertainty(TTree& tree,
                               const std::string& cut,
                               double& count,
                               double& uncertainty);

void add_point(TGraph& graph, const double x, const double y);
double get_maximum(const TH1& h);
double get_maximum(const TGraph& h);
double get_minimum(const TH1& h);
double get_minimum(const TGraph& h);
double get_minimum_positive(const TH1& h);
double get_minimum_positive(const TGraph& h);
void normalize(TH1& h);

template<typename T>
void setup(TTree& chain, const std::string& name, T& variable){
  chain.SetBranchStatus(name.c_str(), 1);
  chain.SetBranchAddress(name.c_str(), &variable);
}

template<typename T>
void assign_colors(std::vector<T>& items,
                   const unsigned short start,
                   const float lightness,
                   const float saturation){
  const float golden_angle(180.0*(3.0-sqrt(5.0)));
  for(typename std::vector<T>::size_type item_num(0);
      item_num<items.size();
      ++item_num){
    const unsigned short multiplier(item_num+start);
    float r(0.0), g(0.0), b(0.0);
    TColor::HLS2RGB(fmod(multiplier*golden_angle,360.0),
                    lightness, saturation, r, g, b);
    const int color_num(TColor::GetColor(r, g, b));
    T& item(items.at(item_num));
    item.SetFillColor(color_num);
    item.SetLineColor(color_num);
    item.SetMarkerColor(color_num);
  }
}

template<typename T>
void assign_colors(std::vector<T>& items, const unsigned short start){
  assign_colors(items, start, 0.75, 1.00);
}

template<typename T>
void assign_colors(std::vector<T>& items){
  assign_colors(items, 0);
}

template<typename T>
void assign_color(T& item, const unsigned short color){
  item.SetFillColor(color);
  item.SetLineColor(color);
  item.SetMarkerColor(color);
}

template<typename T>
void assign_color(T& item, const float r, const float g, const float b){
  const int color(TColor::GetColor(r, g, b));
  item.SetFillColor(color);
  item.SetLineColor(color);
  item.SetMarkerColor(color);
}

template<typename T>
void get_full_title(const T& histo, std::string& title){
  title=histo.GetTitle();
  title+=";";
  title+=histo.GetXaxis()->GetTitle();
  title+=";";
  title+=histo.GetYaxis()->GetTitle();
}

#endif
