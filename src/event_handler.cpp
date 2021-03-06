#include "event_handler.hpp"
#include <cassert>
#include <climits>
#include <cfloat>
#include <ctime>
#include <cmath>
#include <cstdio>
#include <iomanip>
#include <fstream>
#include <vector>
#include <string>
#include <set>
#include <utility>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <functional>
#include <limits>
#include "TChain.h"
#include "TTree.h"
#include "TBranch.h"
#include "TFile.h"
#include "TObjArray.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TCanvas.h"
#include "pu_constants.hpp"
#include "lumi_reweighting_stand_alone.hpp"
#include "cfa.hpp"
#include "math.hpp"
#include "in_json_2012.hpp"
#include "mt2_bisect.hpp"

const double EventHandler::CSVTCut(0.898);
const double EventHandler::CSVMCut(0.679);
const double EventHandler::CSVLCut(0.244);
const std::vector<std::vector<int> > VRunLumiPrompt(MakeVRunLumi("Golden"));
const std::vector<std::vector<int> > VRunLumi24Aug(MakeVRunLumi("24Aug"));
const std::vector<std::vector<int> > VRunLumi13Jul(MakeVRunLumi("13Jul"));

EventHandler::EventHandler(const std::string &fileName, const bool isList, const double scaleFactorIn, const bool fastMode):
  cfA(fileName, isList),
  scaleFactor(scaleFactorIn),
  beta_(0),
  beta_cached_(false){
  if (fastMode) { // turn off unnecessary branches
    chainA.SetBranchStatus("els_*",0);
    chainA.SetBranchStatus("triggerobject_*",0);
    chainA.SetBranchStatus("standalone_t*",0);
    chainA.SetBranchStatus("L1trigger_*",0);
    chainA.SetBranchStatus("passprescale*",0);
    chainA.SetBranchStatus("jets_AK5PFclean_*",0);
    chainA.SetBranchStatus("softjetUp_*",0);
    chainA.SetBranchStatus("pdfweights_*",0);
    chainA.SetBranchStatus("photon_*",0);
    chainB.SetBranchStatus("Ntcmets",0);
    chainB.SetBranchStatus("tcmets_*",0);
    chainB.SetBranchStatus("Nphotons",0);
    chainB.SetBranchStatus("photons_*",0);
    chainB.SetBranchStatus("Npf_photons",0);
    chainB.SetBranchStatus("pf_photons_*",0);
    chainB.SetBranchStatus("Nmus",0);
    chainB.SetBranchStatus("mus_*",0);
    chainB.SetBranchStatus("Nels",0);
    chainB.SetBranchStatus("els_*",0);
    chainB.SetBranchStatus("Nmets*",0);
    chainB.SetBranchStatus("mets*",0);
    chainB.SetBranchStatus("mets_AK5_et",1);
    chainB.SetBranchStatus("Njets_AK5PFclean",0);
    chainB.SetBranchStatus("jets_AK5PFclean_*",0);
    chainB.SetBranchStatus("Nmc*",0);
    chainB.SetBranchStatus("mc_*",0);
    chainB.SetBranchStatus("Nmc_doc*",1);
    chainB.SetBranchStatus("mc_doc*",1);
  }
  }

void EventHandler::SetScaleFactor(const double crossSection, const double luminosity, int numEntries){
  const int maxEntriesA(chainA.GetEntries()), maxEntriesB(chainB.GetEntries());
  if(maxEntriesA!=maxEntriesB){
    fprintf(stderr,"Error: Chains have different numbers of entries.\n");
  }
  if(maxEntriesA==0 || maxEntriesB==0){
    fprintf(stderr, "Error: Empty chains.\n");
  }
  if(numEntries<0){
    numEntries=maxEntriesA;
  }
  if(numEntries>0 && luminosity>0.0 && crossSection>0.0){
    scaleFactor=luminosity*crossSection/static_cast<double>(numEntries);
  }else{
    scaleFactor=1.0;
  }
}

void EventHandler::GetEntry(const unsigned int entry){
  cfA::GetEntry(entry);
  beta_cached_=false;
}

int EventHandler::GetcfAVersion() const{
  size_t pos(sampleName.rfind("_v"));
  if(pos!=std::string::npos && pos<(sampleName.size()-2)){
    std::istringstream iss(sampleName.substr(pos+2));
    int version(0);
    iss >> version;
    return version;
  }else{
    return 0;
  }
}

std::vector<double> EventHandler::GetBeta(const std::string which) const{
  if(!beta_cached_){
    beta_cached_=true;
    beta_.clear();

    if (GetcfAVersion()<69){
      beta_.resize(jets_AK5PF_pt->size(), 0.0);
    }else{
      for (unsigned int ijet=0; ijet<jets_AK5PF_pt->size(); ++ijet) {
        const float pt(jets_AK5PF_pt->at(ijet));
        const float eta(fabs(jets_AK5PF_eta->at(ijet)));
      
        for(std::vector<std::vector<float> >::const_iterator itr(puJet_rejectionBeta->begin());
            itr != puJet_rejectionBeta->end();
            ++itr){
          int j(0);
          float mypt(0.0);
          float myeta(0.0);
          float result(0.0);
          float tmp1(0.0), tmp2(0.0), tmp3(0.0), tmp4(0.0);
          for(std::vector<float>::const_iterator it(itr->begin());
              it != itr->end();
              ++it, ++j) {
            switch(j%6){
            case 0:
              mypt = *it;
              break;
            case 1:
              myeta = fabs(*it); 
              break;
            case 2:
              tmp1 = *it;
              break;
            case 3:
              tmp2 = *it;
              break;
            case 4:
              tmp3 = *it;
              break;
            case 5:
              tmp4 = *it;
              break;
            default:
              break;
            }
          
            if ( which.compare("beta")==0 )                 result = tmp1; 
            else if ( which.compare("betaStar")==0 )        result = tmp2; 
            else if ( which.compare("betaClassic")==0 )     result = tmp3; 
            else if ( which.compare("betaStarClassic")==0 ) result = tmp4; 
            else result = -5; //Don't assert..
          
          }//vector of info of each jet
          if ( mypt == pt && myeta == eta ) {
            beta_.push_back(result);
            break;
          }     
        }//vector of jets
      } //ijet
    }
  }
  return beta_;
}

void EventHandler::SetScaleFactor(const double scaleFactorIn){
  scaleFactor=scaleFactorIn;
}

bool EventHandler::PassesPVCut() const{
  if(beamSpot_x->size()<1 || pv_x->size()<1) return false;
  const double pv_rho(sqrt(pv_x->at(0)*pv_x->at(0) + pv_y->at(0)*pv_y->at(0)));
  if(pv_ndof->at(0)>4 && fabs(pv_z->at(0))<24. && pv_rho<2.0 && pv_isFake->at(0)==0) return true;
  return false;
  return isGoodVertex(0);
}

bool EventHandler::PassesMETCleaningCut() const{
  for(unsigned int jet(0); jet<jets_AK5PF_pt->size(); ++jet){
    if(isProblemJet(jet)) return false;
  }
  //if(pfTypeImets_et->at(0)>2.0*pfmets_et->at(0)) return false;
  if(pfTypeImets_et->at(0)>2.0*mets_AK5_et->at(0)) return false; // updated 11/14 (JB-F)
  return cschalofilter_decision
    && (hbhefilter_decision || sampleName.find("TChihh")!=std::string::npos || sampleName.find("HbbHbb")!=std::string::npos)
    && hcallaserfilter_decision 
    && ecalTPfilter_decision 
    && trackingfailurefilter_decision 
    && eebadscfilter_decision 
    && (ecallaserfilter_decision || sampleName.find("_v66")!=std::string::npos)
    && greedymuonfilter_decision 
    && inconsistentPFmuonfilter_decision 
    && scrapingVeto_decision
    && PassesBadJetFilter()
    && GetPBNR()>=1;
}

bool EventHandler::PassesTrigger(const std::string& trigger) const{ // just check if a specific trigger fired
  for(unsigned int a=0; a<trigger_name->size(); ++a){
    if(trigger_name->at(a).find(trigger)!=std::string::npos
       && trigger_prescalevalue->at(a)==1 && trigger_decision->at(a)==1){
      return true;
    }
  }
  return false;
}

bool EventHandler::PassesJSONCut() const{
  if(sampleName.find("Run2012")!=std::string::npos){
    if(sampleName.find("PromptReco")!=std::string::npos
       &&!inJSON(VRunLumiPrompt, run, lumiblock)) return false;
    if(sampleName.find("24Aug")!=std::string::npos
       && !inJSON(VRunLumi24Aug, run, lumiblock)) return false;
    if(sampleName.find("13Jul")!=std::string::npos
       && !inJSON(VRunLumi13Jul, run, lumiblock)) return false;
    return true;
  }else{
    return true;
  }
}

bool EventHandler::PassesBadJetFilter() const{
  for(unsigned int i(0); i<jets_AK5PF_pt->size(); ++i){
    if(isGoodJet(i,false,30.0,DBL_MAX) && !isGoodJet(i,true,30.0,DBL_MAX)) return false;
  }
  return true;
}

int EventHandler::GetPBNR() const{
  //RA2 particle-based noise rejection                                                                                                                                           

  bool nhBad=false;
  bool phBad=false;
  for (unsigned int it = 0; it<jets_AK5PF_pt->size(); it++) {
    //cfA version from Keith                                                                                                                                                     
    double NHF = jets_AK5PF_neutralHadE->at(it)/(jets_AK5PF_energy->at(it)*jets_AK5PF_corrFactorRaw->at(it));
    double PEF = jets_AK5PF_photonEnergy->at(it)/(jets_AK5PF_energy->at(it)*jets_AK5PF_corrFactorRaw->at(it));
    if (NHF > 0.9)  nhBad = true;
    if (PEF > 0.95) phBad = true;
  }

  if (nhBad && phBad) return -3;
  else if (phBad) return -2;
  else if (nhBad) return -1;
  return 1;
}

bool EventHandler::PassesLeptonCut() const{
  //Require exactly one loose electron or muon and veto on any other leptons (e, mu, or tau)
  return (GetNumElectrons(1)+GetNumMuons(1)==1)
    && (GetNumElectrons(0)+GetNumMuons(0)==1)
    && (GetNumTaus(0)==0);
}

bool EventHandler::PassesHTCut() const{
  return GetHT()>500.0;
}

bool EventHandler::PassesMETCut() const{
  return pfTypeImets_et->at(0)>250.0;
}

bool EventHandler::PassesNumJetsCut() const{
  return GetNumGoodJets()>=6;
}

bool EventHandler::PassesBTaggingCut() const{
  return GetNumCSVMJets()>=2;
}

bool EventHandler::PassesBaselineSelection() const{
  return PassesJSONCut() && PassesPVCut() && PassesMETCleaningCut()
    && PassesLeptonCut() && PassesHTCut() && PassesMETCut()
    && PassesNumJetsCut() && PassesBTaggingCut();
}

double EventHandler::GetHT(const bool useMET, const bool useLeps) const{
  double HT(0.0);
  if(useMET && pfTypeImets_et->size()>0) HT+=pfTypeImets_et->at(0);
  for(unsigned int i(0); i<jets_AK5PF_pt->size(); ++i){
    if(isGoodJet(i)) HT+=jets_AK5PF_pt->at(i);
  }
  if(useLeps){
    for(unsigned int i(0); i<pf_els_pt->size(); ++i){
      if(isElectron(i)) HT+=pf_els_pt->at(i);
    }
    for(unsigned int i(0); i<pf_mus_pt->size(); ++i){
      if(isMuon(i)) HT+=pf_mus_pt->at(i);
    }
    for(unsigned int i(0); i<taus_pt->size(); ++i){
      if(isTau(i)) HT+=taus_pt->at(i);
    }
  }
  return HT;
}

int EventHandler::GetPartonIdBin(float partonId) const{
  if (fabs(partonId)>=1.&&fabs(partonId)<=3.) return 2;
  else if (fabs(partonId)==4.) return 3;
  else if (fabs(partonId)==5.) return 4;
  else if (fabs(partonId)==21.) return 5;
  else return 1;
}

int EventHandler::GetMass(const std::string& token) const{
  std::string::size_type pos(model_params->find(token));
  if(pos==std::string::npos){
    return -1;
  }else{
    pos+=token.size();
    const std::string::size_type end(model_params->find("_",pos));
    if(end==std::string::npos){
      return -1;
    }else{
      return atoi((model_params->substr(pos, end-pos+1)).c_str());
    }
  }
}

int EventHandler::GetNumGoodJets() const{
  int numGoodJets(0);
  for(unsigned int i(0); i<jets_AK5PF_pt->size(); ++i){
    if(isGoodJet(i)) ++numGoodJets;
  }
  return numGoodJets;
}

int EventHandler::GetNumCSVTJets() const{
  int numPassing(0);
  for(unsigned int i(0); i<jets_AK5PF_pt->size(); ++i){
    if(isGoodJet(i) && jets_AK5PF_btag_secVertexCombined->at(i)>CSVTCut){
      ++numPassing;
    }
  }
  return numPassing;
}

int EventHandler::GetNumCSVMJets() const{
  int numPassing(0);
  for(unsigned int i(0); i<jets_AK5PF_pt->size(); ++i){
    if(isGoodJet(i) && jets_AK5PF_btag_secVertexCombined->at(i)>CSVMCut){
      ++numPassing;
    }
  }
  return numPassing;
}

int EventHandler::GetNumCSVLJets() const{
  int numPassing(0);
  for(unsigned int i(0); i<jets_AK5PF_pt->size(); ++i){
    if(isGoodJet(i) && jets_AK5PF_btag_secVertexCombined->at(i)>CSVLCut){
      ++numPassing;
    }
  }
  return numPassing;
}

double EventHandler::GetMinDeltaPhiMET(const unsigned int maxjets) const{
  std::vector<std::pair<double, double> > jets(0);
  for(unsigned int i(0); i<jets_AK5PF_phi->size(); ++i){
    if(isGoodJet(i, false, 20.0, 5.0, false)){
      jets.push_back(std::make_pair(jets_AK5PF_pt->at(i),jets_AK5PF_phi->at(i)));
    }
  }

  std::sort(jets.begin(), jets.end(), std::greater<std::pair<double, double> >());

  double mindp(DBL_MAX);
  for(unsigned int i(0); i<jets.size() && i<maxjets; ++i){
    const double thisdp(fabs((Math::GetAbsDeltaPhi(jets.at(i).second, pfTypeImets_phi->at(0)))));
    if(thisdp<mindp){
      mindp=thisdp;
    }
  }
  
  return mindp;
}

bool EventHandler::isProblemJet(const unsigned int ijet) const{
  return jets_AK5PF_pt->at(ijet)>50.0
    && fabs(jets_AK5PF_eta->at(ijet))>0.9
    && fabs(jets_AK5PF_eta->at(ijet))<1.9
    && jets_AK5PF_chg_Mult->at(ijet)-jets_AK5PF_neutral_Mult->at(ijet)>=40;
}

bool EventHandler::isGoodJet(const unsigned int ijet, const bool jetid, const double ptThresh, const double etaThresh, const bool doBeta) const{
  if(jets_AK5PF_pt->at(ijet)<ptThresh || fabs(jets_AK5PF_eta->at(ijet))>etaThresh) return false;
  if( jetid && !jetPassLooseID(ijet) ) return false;
  if(GetcfAVersion()<69||sampleName.find("SMS-TChiHH")!=std::string::npos) return true;
  if(doBeta && GetBeta().at(ijet)<0.2) return false;
  return true;
}

bool EventHandler::jetPassLooseID(const unsigned int ijet) const{
  //want the uncorrected energy
  const double jetenergy = jets_AK5PF_energy->at(ijet) * jets_AK5PF_corrFactorRaw->at(ijet);
  const int numConst = static_cast<int>(jets_AK5PF_mu_Mult->at(ijet)+jets_AK5PF_neutral_Mult->at(ijet)+jets_AK5PF_chg_Mult->at(ijet)); //stealing from Keith
  
  if (jetenergy>0.0) {
    const double jet_energy_inv(1.0/jetenergy);
    const double eta(fabs(jets_AK5PF_eta->at(ijet)));
    if (jets_AK5PF_neutralHadE->at(ijet) *jet_energy_inv <= 0.99
        && jets_AK5PF_neutralEmE->at(ijet) * jet_energy_inv <= 0.99
        && numConst >= 2
        && ( eta>=2.4 || ((jets_AK5PF_chgHadE->at(ijet)*jet_energy_inv) > 0.
                          && (jets_AK5PF_chgEmE->at(ijet)*jet_energy_inv) < 0.99
                          && jets_AK5PF_chg_Mult->at(ijet)>0))){
      return true;
    }
  }
  return false;
}

bool EventHandler::isElectron(const unsigned int k,
                              const unsigned short level,
                              const bool use_iso) const{
  //N.B.: cut does not have the fabs(1/E-1/p) and conversion rejection cuts from the EGamma POG!!!
  const double dmax(std::numeric_limits<double>::max());
  double pt_cut(10.0); //Not actually part of the EGamma ID
  double eta_cut(0.007), phi_cut(0.8), sigmaietaieta_cut(0.01), h_over_e_cut(0.15), d0_cut(0.04), dz_cut(0.2), iso_cut(0.15);
  switch(level){
  case 1:
    pt_cut=20.0;
    if(pf_els_isEB->at(k)){
      eta_cut=0.007; phi_cut=0.15; sigmaietaieta_cut=0.01; h_over_e_cut=0.12;
      d0_cut=0.02; dz_cut=0.2; iso_cut=0.15;
    }else if(pf_els_isEE->at(k)){
      eta_cut=0.009; phi_cut=0.10; sigmaietaieta_cut=0.03; h_over_e_cut=0.1;
      d0_cut=0.02; dz_cut=0.2; iso_cut=(pf_els_pt->at(k)>20.0?0.15:0.10);
    }
    break;
  case 2:
    pt_cut=20.0;
    if(pf_els_isEB->at(k)){
      eta_cut=0.004; phi_cut=0.06; sigmaietaieta_cut=0.01; h_over_e_cut=0.12;
      d0_cut=0.02; dz_cut=0.1; iso_cut=0.15;
    }else if(pf_els_isEE->at(k)){
      eta_cut=0.007; phi_cut=0.03; sigmaietaieta_cut=0.03; h_over_e_cut=0.1;
      d0_cut=0.02; dz_cut=0.1; iso_cut=(pf_els_pt->at(k)>20.0?0.15:0.10);
    }
    break;
  case 3:
    pt_cut=20.0;
    if(pf_els_isEB->at(k)){
      eta_cut=0.004; phi_cut=0.03; sigmaietaieta_cut=0.01; h_over_e_cut=0.12;
      d0_cut=0.02; dz_cut=0.1; iso_cut=0.10;
    }else if(pf_els_isEE->at(k)){
      eta_cut=0.005; phi_cut=0.02; sigmaietaieta_cut=0.03; h_over_e_cut=0.1;
      d0_cut=0.02; dz_cut=0.1; iso_cut=(pf_els_pt->at(k)>20.0?0.10:0.07);
    }
    break;
  case 0: //intentionally falling through to default "veto" case
  default:
    pt_cut=10.0;
    if(pf_els_isEB->at(k)){
      eta_cut=0.007; phi_cut=0.8; sigmaietaieta_cut=0.01; h_over_e_cut=0.15;
      d0_cut=0.04; dz_cut=0.2; iso_cut=0.15;
    }else if(pf_els_isEE->at(k)){
      eta_cut=0.01; phi_cut=0.7; sigmaietaieta_cut=0.03; h_over_e_cut=dmax;
      d0_cut=0.04; dz_cut=0.2; iso_cut=0.15;
    }
    break;
  }
  //if(k>pf_else_pt->size()) return false;
  if (fabs(pf_els_scEta->at(k)) >= 2.5 ) return false;
  if (pf_els_pt->at(k) < pt_cut) return false;

  if ( fabs(pf_els_dEtaIn->at(k)) > eta_cut)  return false;
  if ( fabs(pf_els_dPhiIn->at(k)) > phi_cut)  return false;
  if (pf_els_sigmaIEtaIEta->at(k) > sigmaietaieta_cut) return false;
  if (pf_els_hadOverEm->at(k) > h_over_e_cut) return false;

  const double beamx(beamSpot_x->at(0)), beamy(beamSpot_y->at(0)); 
  const double d0(pf_els_d0dum->at(k)-beamx*sin(pf_els_tk_phi->at(k))+beamy*cos(pf_els_tk_phi->at(k)));
  if ( fabs(d0) >= d0_cut ) return false;
  if ( fabs(pf_els_vz->at(k) - pv_z->at(0) ) >= dz_cut ) return false;

  if(GetElectronRelIso(k)>=iso_cut && use_iso) return false;
  return true;
}

double EventHandler::GetElectronRelIso(const unsigned int k) const{
  const double rho(rho_kt6PFJetsForIsolation2012);
  // get effective area from delR=0.3 2011 data table for neutral+gamma based on supercluster eta pf_els_scEta->at(k)
  double AE(0.10); 
  const double abseta(fabs(pf_els_scEta->at(k)));
  if      ( abseta < 1.0 ) AE = 0.13;
  else if ( abseta >=1.0 && abseta <1.479) AE = 0.14;
  else if ( abseta >=1.479&&abseta <2.0)   AE = 0.07;
  else if ( abseta >=2.0 && abseta <2.2) AE = 0.09;
  else if ( abseta >=2.2 && abseta <2.3) AE = 0.11;
  else if ( abseta >=2.3 && abseta <2.4) AE = 0.11;
  else if ( abseta >=2.4 ) AE = 0.14;

  const double eleIso(pf_els_PFphotonIsoR03->at(k) + pf_els_PFneutralHadronIsoR03->at(k) - rho*AE);
  return ( pf_els_PFchargedHadronIsoR03->at(k) + ( eleIso > 0 ? eleIso : 0.0 ) )/pf_els_pt->at(k);
}

bool EventHandler::isMuon(const unsigned int k,
                          const unsigned short level,
                          const bool use_iso) const{
  double pt_thresh(10.0);
  if(level>=1) pt_thresh=20.0;

  if (fabs(pf_mus_eta->at(k)) >= 2.4 ) return false;
  if (pf_mus_pt->at(k) < pt_thresh) return false;
  if ( !pf_mus_id_GlobalMuonPromptTight->at(k)) return false;
  // GlobalMuonPromptTight includes: isGlobal, globalTrack()->normalizedChi2() < 10, numberOfValidMuonHits() > 0
  if ( pf_mus_numberOfMatchedStations->at(k) <= 1 ) return false;
  const double beamx (beamSpot_x->at(0)), beamy(beamSpot_y->at(0));   
  const double d0 = pf_mus_tk_d0dum->at(k)-beamx*sin(pf_mus_tk_phi->at(k))+beamy*cos(pf_mus_tk_phi->at(k));
  const double pf_mus_vz = pf_mus_tk_vz->at(k);
  const double pf_mus_dz_vtx = fabs(pf_mus_vz-pv_z->at(0));
  if (fabs(d0)>=0.2 || pf_mus_dz_vtx>=0.5) return false;
  if ( !pf_mus_tk_numvalPixelhits->at(k)) return false;
  if ( pf_mus_tk_LayersWithMeasurement->at(k) <= 5 ) return false;
  
  double isoNeutral(pf_mus_pfIsolationR04_sumNeutralHadronEt->at(k) + pf_mus_pfIsolationR04_sumPhotonEt->at(k) - 0.5*pf_mus_pfIsolationR04_sumPUPt->at(k));
  if(isoNeutral<0.0) isoNeutral=0.0;
  const double pf_mus_rel_iso((pf_mus_pfIsolationR04_sumChargedHadronPt->at(k) + isoNeutral) / pf_mus_pt->at(k));
  if (pf_mus_rel_iso > 0.2 && use_iso) return false;
  return true;
}

bool EventHandler::isTau(const unsigned int k,
                         const unsigned short level,
                         const bool require_iso) const{
  double pt_cut(20.0);
  if(level>0) pt_cut=30.0;

  if (taus_pt->at(k)<pt_cut) return false;
  if (fabs(taus_eta->at(k)) > 2.4) return false;
  if (require_iso && (taus_byLooseIsolationDeltaBetaCorr->at(k) <= 0)) return false;
  return true;
}

int EventHandler::GetNumElectrons(const unsigned short level, const bool use_iso) const{
  int count(0);
  for(unsigned int i(0); i<pf_els_pt->size(); ++i){
    if(isElectron(i,level, use_iso)) ++count;
  }
  return count;
}

int EventHandler::GetNumMuons(const unsigned short level, const bool use_iso) const{
  int count(0);
  for(unsigned int i(0); i<pf_mus_pt->size(); ++i){
    if(isMuon(i,level,use_iso)) ++count;
  }
  return count;
}

int EventHandler::GetNumTaus(const unsigned short level, const bool use_iso) const{
  int count(0);
  for(unsigned int i(0); i<taus_pt->size(); ++i){
    if(isTau(i,level,use_iso)) ++count;
  }
  return count;
}

bool EventHandler::isIsoTrack(const unsigned int itracks, const double ptThresh) const{
  if(!isQualityTrack(itracks)) return false;
  if (fabs(tracks_vz->at(itracks) - pv_z->at(0) ) >= 0.05) return false;
  if(tracks_pt->at(itracks)<ptThresh) return false;
  double isosum=0;
  for (unsigned int jtracks=0; jtracks<tracks_chi2->size(); jtracks++) {
    if (itracks==jtracks) continue;  //don't count yourself
    if (!isQualityTrack(jtracks)) continue;
    if(Math::GetDeltaR(tracks_phi->at(itracks),tracks_eta->at(itracks),tracks_phi->at(jtracks),tracks_eta->at(jtracks))>0.3) continue;
    //cut on dz of this track
    if ( fabs( tracks_vz->at(jtracks) - pv_z->at(0)) >= 0.05) continue;
    isosum += tracks_pt->at(jtracks);
  }
  if ( isosum / tracks_pt->at(itracks) > 0.05) return false;
  return true;
}

bool EventHandler::isQualityTrack(const unsigned int k) const{
  if (fabs(tracks_eta->at(k))>2.4 ) return false;
  if (!(tracks_highPurity->at(k)>0)) return false;
  return true;  
}

int EventHandler::GetNumIsoTracks(const double ptThresh) const{
  int count(0);
  for(unsigned int i(0); i<tracks_pt->size(); ++i){
    if(isIsoTrack(i,ptThresh)) ++count;
  }
  return count;
}

int EventHandler::NewGetNumIsoTracks(const double ptThresh) const{
  int nisotracks=0;
  if ( GetcfAVersion() < 71 ) return nisotracks;
  for ( unsigned int itrack = 0 ; itrack < isotk_pt->size() ; ++itrack) {
    if ( (isotk_pt->at(itrack) >= ptThresh) &&
         (isotk_iso->at(itrack) /isotk_pt->at(itrack) < 0.1 ) &&
         ( fabs(isotk_dzpv->at(itrack)) <0.1) && //important to apply this; was not applied at ntuple creation
         ( fabs(isotk_eta->at(itrack)) < 2.4 ) //this is more of a sanity check
         ){
      ++nisotracks;
    }
  }
  return nisotracks;
}

double EventHandler::GetNumInteractions() const{
  double npv(-1.0);
  for(unsigned int i(0); i<PU_bunchCrossing->size(); ++i){
    if(PU_bunchCrossing->at(i)==0){
      npv = PU_TrueNumInteractions->at(i);
    }
  }
  return npv;
}

bool EventHandler::isGoodVertex(const unsigned int vertex) const{
  const double pv_rho(sqrt(pv_x->at(vertex)*pv_x->at(vertex) + pv_y->at(vertex)*pv_y->at(vertex)));
  return pv_ndof->at(vertex)>4 && fabs(pv_z->at(vertex))<24. && pv_rho<2.0 && pv_isFake->at(vertex)==0;
}

unsigned short EventHandler::GetNumVertices() const{
  unsigned short num_vertices(0);
  for(unsigned int vertex(0); vertex<pv_x->size(); ++vertex){
    if(isGoodVertex(vertex)) ++num_vertices;
  }
  return num_vertices;
  //return pv_x->size();
}

double EventHandler::GetPUWeight(reweight::LumiReWeighting &lumiWeights) const{
  return lumiWeights.weight(GetNumInteractions());
}

double EventHandler::GetTopPt() const {
  double topPt(-1);
  for(unsigned int i(0); i<mc_doc_id->size(); ++i){
    // look for the *top* (not antitop) pT
    if(mc_doc_id->at(i)==6) topPt = mc_doc_pt->at(i);
    if(topPt>=0) break; //check to see if we're done
  }
  return topPt;
}

double EventHandler::GetTopPtWeight() const{ // New 11/07
  //So called "Official version" in RA2b code
  double topweight(-1.0);

  //official recipe from
  // https://twiki.cern.ch/twiki/bin/viewauth/CMS/TopPtReweighting
  //(2 string comparisons for every event is not so good for performance, but oh well)
  if (sampleName.find("TTJets")!=std::string::npos || sampleName.find("TT_")!=std::string::npos) {
    double topPt(-1);
    double topbarPt(-1);
    for(unsigned int i(0); i<mc_doc_id->size(); ++i){
      // look for the *top* (not antitop) pT
      if(mc_doc_id->at(i)==6) topPt = mc_doc_pt->at(i);
      if(mc_doc_id->at(i)==-6) topbarPt = mc_doc_pt->at(i);
      if(topPt>=0 && topbarPt>=0) break; //check to see if we're done
    }

    //SF(x)=exp(a+bx)
    const double a(0.156); //combined 8 TeV values
    const double b(-0.00137);

    //important choice -- I've decided to use the 400 GeV value for values above 400 GeV
    //an alternative would be to just blindly use the formula
    if (topPt >400) topPt=400;
    if (topbarPt >400) topbarPt=400;

    const double SFt(exp(a + b*topPt));
    const double SFtbar(exp(a + b*topbarPt));

    topweight = sqrt( SFt * SFtbar );
  }

  if(topweight<0) topweight=1;
  return topweight;
}

double EventHandler::GetHighestJetPt(const unsigned int nth_highest) const{
  std::vector<double> pts(0);
  for(unsigned int jet(0); jet<jets_AK5PF_pt->size(); ++jet){
    if(isGoodJet(jet)){
      pts.push_back(jets_AK5PF_pt->at(jet));
    }
  }
  std::sort(pts.begin(), pts.end(), std::greater<double>());
  if(nth_highest<=pts.size()){
    return pts.at(nth_highest-1);
  }else{
    return 0.0;
  }
}

double EventHandler::GetHighestJetCSV(const unsigned int nth_highest) const{
  std::vector<double> csvs(0);
  for(unsigned int jet(0); jet<jets_AK5PF_btag_secVertexCombined->size(); ++jet){
    if(isGoodJet(jet)){
      csvs.push_back(jets_AK5PF_btag_secVertexCombined->at(jet));
    }
  }
  std::sort(csvs.begin(), csvs.end(), std::greater<double>());
  if(nth_highest<=csvs.size()){
    return csvs.at(nth_highest-1);
  }else{
    return 0.0;
  }
}

int EventHandler::GetMass1() const{
  const std::string::size_type p1(model_params->find('_'));
  const std::string::size_type p2(model_params->find('_',p1+1));
  if(p1!=std::string::npos && p2!=std::string::npos){
    return atoi(model_params->substr(p1+1,p2-p1-1).c_str());
  }else{
    return -1;
  }
}

int EventHandler::GetMass2() const{
  const std::string::size_type p1(model_params->find('_'));
  const std::string::size_type p2(model_params->find('_',p1+1));
  const std::string::size_type p3(model_params->find(' ',p2+1));
  if(p2!=std::string::npos && p3!=std::string::npos){
    return atoi(model_params->substr(p2+1,p3-p2-1).c_str());
  }else{
    return -1;
  }
}

double EventHandler::GetMT2(const double test_mass) const{
  double max_pt(-std::numeric_limits<double>::max());
  double child[3]={0.0, pfTypeImets_ex->at(0), pfTypeImets_ey->at(0)};
  for(unsigned ele(0); ele<pf_els_pt->size(); ++ele){
    if(isElectron(ele, 1)){
      const double this_pt(pf_els_pt->at(ele));
      if(this_pt>max_pt){
        max_pt=this_pt;
        child[1]=pf_els_px->at(ele)+pfTypeImets_ex->at(0);
        child[2]=pf_els_py->at(ele)+pfTypeImets_ey->at(0);
      }
    }
  }
  for(unsigned mu(0); mu<pf_mus_pt->size(); ++mu){
    if(isMuon(mu, 1)){
      const double this_pt(pf_mus_pt->at(mu));
      if(this_pt>max_pt){
        max_pt=this_pt;
        child[1]=pf_mus_px->at(mu)+pfTypeImets_ex->at(0);
        child[2]=pf_mus_py->at(mu)+pfTypeImets_ey->at(0);
      }
    }
  }

  const unsigned bad_index(static_cast<unsigned>(-1));
  unsigned index_1(bad_index), index_2(bad_index);
  max_pt=-std::numeric_limits<double>::max();
  double max2_pt(-std::numeric_limits<double>::max());
  for(unsigned jet(0); jet<jets_AK5PF_pt->size(); ++jet){
    const double this_pt(jets_AK5PF_pt->at(jet));
    if(this_pt>max_pt){
      max2_pt=max_pt;
      max_pt=this_pt;
      index_2=index_1;
      index_1=jet;
    }else if(this_pt>max2_pt){
      max2_pt=this_pt;
      index_2=jet;
    }
  }

  if(index_1==bad_index || index_2==bad_index){
    return 0.0;
  }else{
    double jet1[3]={0.0, 0.0, 0.0}, jet2[3]={0.0, 0.0, 0.0};
    jet1[0]=jets_AK5PF_mass->at(index_1);
    jet1[1]=jets_AK5PF_px->at(index_1);
    jet1[2]=jets_AK5PF_py->at(index_1);
    jet2[0]=jets_AK5PF_mass->at(index_2);
    jet2[1]=jets_AK5PF_px->at(index_2);
    jet2[2]=jets_AK5PF_py->at(index_2);

    mt2_bisect::mt2 mt2_calc;
    mt2_calc.set_momenta(jet1, jet2, child);
    mt2_calc.set_mn(test_mass);
    return mt2_calc.get_mt2();
  }
}

double EventHandler::GetMT() const{
  double max_pt(-std::numeric_limits<double>::max());
  double lep_px(0.0), lep_py(0.0);
  bool found_lepton(false);
  for(unsigned ele(0); ele<pf_els_pt->size(); ++ele){
    if(isElectron(ele, 1)){
      const double this_pt(pf_els_pt->at(ele));
      if(this_pt>max_pt){
        max_pt=this_pt;
        lep_px=pf_els_px->at(ele);
        lep_py=pf_els_py->at(ele);
        found_lepton=true;
      }
    }
  }
  for(unsigned mu(0); mu<pf_mus_pt->size(); ++mu){
    if(isMuon(mu, 1)){
      const double this_pt(pf_mus_pt->at(mu));
      if(this_pt>max_pt){
        max_pt=this_pt;
        lep_px=pf_mus_px->at(mu);
        lep_py=pf_mus_py->at(mu);
        found_lepton=true;
      }
    }
  }

  if(found_lepton){
    return Math::CalcMT(lep_px, lep_py, pfTypeImets_ex->at(0), pfTypeImets_ey->at(0));
  }else{
    return 0.0;
  }
}


double EventHandler::GetDeltaPhiMETLepton() const{
  double max_pt(-std::numeric_limits<double>::max());
  double lep_phi(0.0);
  bool found_lepton(false);
  for(unsigned ele(0); ele<pf_els_pt->size(); ++ele){
    if(isElectron(ele, 1)){
      const double this_pt(pf_els_pt->at(ele));
      if(this_pt>max_pt){
        max_pt=this_pt;
        lep_phi=pf_els_phi->at(ele);
        found_lepton=true;
      }
    }
  }
  for(unsigned mu(0); mu<pf_mus_pt->size(); ++mu){
    if(isMuon(mu, 1)){
      const double this_pt(pf_mus_pt->at(mu));
      if(this_pt>max_pt){
        max_pt=this_pt;
        lep_phi=pf_mus_phi->at(mu);
        found_lepton=true;
      }
    }
  }

  if(found_lepton){
    return Math::GetAbsDeltaPhi(lep_phi, pfTypeImets_phi->at(0));
  }else{
    return std::numeric_limits<double>::max();
  }
}

double EventHandler::GetDeltaPhiWLepton() const{
  double max_pt(-std::numeric_limits<double>::max());
  double lep_px(0.0), lep_py(0.0);
  bool found_lepton(false);
  for(unsigned ele(0); ele<pf_els_pt->size(); ++ele){
    if(isElectron(ele, 1)){
      const double this_pt(pf_els_pt->at(ele));
      if(this_pt>max_pt){
        max_pt=this_pt;
        lep_px=pf_els_px->at(ele);
        lep_py=pf_els_py->at(ele);
        found_lepton=true;
      }
    }
  }
  for(unsigned mu(0); mu<pf_mus_pt->size(); ++mu){
    if(isMuon(mu, 1)){
      const double this_pt(pf_mus_pt->at(mu));
      if(this_pt>max_pt){
        max_pt=this_pt;
        lep_px=pf_mus_px->at(mu);
        lep_py=pf_mus_py->at(mu);
        found_lepton=true;
      }
    }
  }

  const double met_px(pfTypeImets_ex->at(0)), met_py(pfTypeImets_ey->at(0));
  if(found_lepton){
    return Math::GetAbsDeltaPhi(atan2(lep_py, lep_px), atan2(lep_py+met_py, lep_px+met_px));
  }else{
    return std::numeric_limits<double>::max();
  }
}

unsigned EventHandler::GetNumberOfGeneratedEMu(const bool check_W, const bool check_top) const{
  unsigned count(0);
  for(unsigned particle(0); particle<mc_doc_id->size(); ++particle){
    if((fabs(mc_doc_id->at(particle))==11.0 || fabs(mc_doc_id->at(particle))==13)
       && (fabs(mc_doc_mother_id->at(particle))==24.0 || !check_W)
       && (fabs(mc_doc_grandmother_id->at(particle))==6.0 || !check_top)){
      ++count;
    }
  }
  return count;
}

std::vector<double> EventHandler::GetBLInvariantMasses(const unsigned num_bs, const double csv_cut){
  //Returns all possible b-l invariant masses for the highest pt electron or muon and the num_bs highest
  //csv-valued jets with a csv of at_least csv_cut. If num_bs==0 (the default), it uses all jets.
  std::vector<double> vals(0);
  double max_pt(-std::numeric_limits<double>::max());
  unsigned lep_index(0);
  bool found_electron(false), found_muon(false);
  for(unsigned ele(0); ele<pf_els_pt->size(); ++ele){
    if(isElectron(ele, 1)){
      const double this_pt(pf_els_pt->at(ele));
      if(this_pt>max_pt){
        max_pt=this_pt;
        lep_index=ele;
        found_electron=true;
      }
    }
  }
  for(unsigned mu(0); mu<pf_mus_pt->size(); ++mu){
    if(isMuon(mu, 1)){
      const double this_pt(pf_mus_pt->at(mu));
      if(this_pt>max_pt){
        max_pt=this_pt;
        lep_index=mu;
        found_muon=true;
      }
    }
  }
  double lep_px(0.0), lep_py(0.0), lep_pz(0.0), lep_e(0.0);
  if(found_muon){
    lep_px=pf_mus_px->at(lep_index);
    lep_py=pf_mus_py->at(lep_index);
    lep_pz=pf_mus_pz->at(lep_index);
    lep_e=pf_mus_energy->at(lep_index);
  }else if(found_electron){
    lep_px=pf_els_px->at(lep_index);
    lep_py=pf_els_py->at(lep_index);
    lep_pz=pf_els_pz->at(lep_index);
    lep_e=pf_els_energy->at(lep_index);
  }
  
  std::vector<std::pair<double, unsigned> > tags(0);
  for(unsigned jet(0); jet<jets_AK5PF_btag_secVertexCombined->size(); ++jet){
    if(isGoodJet(jet)) tags.push_back(std::make_pair(jets_AK5PF_btag_secVertexCombined->at(jet), jet));
  }
  std::sort(tags.begin(), tags.end(), std::greater<std::pair<double, unsigned> >());
  for(unsigned jet(0); jet<tags.size() && (num_bs==0 || jet<num_bs) && tags.at(jet).first>=csv_cut; ++jet){
    const unsigned i(tags.at(jet).second);
    TLorentzVector v(jets_AK5PF_px->at(i)+lep_px, jets_AK5PF_py->at(i)+lep_py,
                     jets_AK5PF_pz->at(i)+lep_pz, jets_AK5PF_energy->at(i)+lep_e);
    vals.push_back(v.M());
  }
  return vals;
}
