#ifndef H_EVENTHANDLER
#define H_EVENTHANDLER

#include <cfloat>
#include <vector>
#include <string>
#include <utility>
#include <limits>
#include <stdint.h>
#include "TChain.h"
#include "TBranch.h"
#include "TLorentzVector.h"
#include "pu_constants.hpp"
#include "lumi_reweighting_stand_alone.hpp"
#include "cfa.hpp"

class EventHandler : public cfA{
public:
  EventHandler(const std::string &, const bool, const double=1.0, const bool=false);

  void SetScaleFactor(const double);
  void SetScaleFactor(const double, const double, const int);

protected:
  static const double CSVTCut, CSVMCut, CSVLCut;
  double scaleFactor;

  int GetcfAVersion() const;

  void GetEntry(const unsigned int);

  std::vector<double> GetBeta(const std::string which="beta") const;

  double GetNumInteractions() const;
  unsigned short GetNumVertices() const;
  double GetPUWeight(reweight::LumiReWeighting &) const;

  bool isGoodVertex(const unsigned int) const;

  bool PassesJSONCut() const;
  bool PassesPVCut() const;
  bool PassesMETCleaningCut() const;

  bool PassesTrigger(const std::string&) const;

  bool PassesBadJetFilter() const;

  int GetMass(const std::string&) const;

  int GetPBNR() const;
  double GetMinDeltaPhiMET(const unsigned int=std::numeric_limits<unsigned int>::max()) const;

  int GetNumGoodJets() const;
  int GetNumCSVTJets() const;
  int GetNumCSVMJets() const;
  int GetNumCSVLJets() const;
  int GetNumBTaggedJets() const;

  bool isGoodJet(const unsigned int, const bool=true, const double=20.0,
                 const double=2.4, const bool=true) const;
  bool isProblemJet(const unsigned int) const;
  bool jetPassLooseID(const unsigned int) const;

  bool isElectron(const unsigned int, const unsigned short=0, const bool=true) const;
  bool isMuon(const unsigned int, const unsigned short=0, const bool=true) const;
  bool isTau(const unsigned int, const unsigned short=0, const bool=true) const;

  int GetNumElectrons(const unsigned short=0, const bool=true) const;
  int GetNumMuons(const unsigned short=0, const bool=true) const;
  int GetNumTaus(const unsigned short=0, const bool=true) const;

  bool isIsoTrack(const unsigned int, const double=10.0) const;
  bool isQualityTrack(const unsigned int) const;

  int GetNumIsoTracks(const double=10.0) const;
  int NewGetNumIsoTracks(const double=10.0) const;

  double GetElectronRelIso(const unsigned int) const;

  double GetTopPt() const;
  double GetTopPtWeight() const;

  double GetHT(const bool=true, const bool=false) const;
  
  int GetPartonIdBin(const float=0.) const;

  double GetHighestJetPt(const unsigned int=1) const;
  double GetHighestJetCSV(const unsigned int=1) const;
};

#endif
