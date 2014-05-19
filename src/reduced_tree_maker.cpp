#include "reduced_tree_maker.hpp"
#include <ctime>
#include <vector>
#include <string>
#include <set>
#include <stdint.h>
#include "timer.hpp"
#include "event_handler.hpp"
#include "event_number.hpp"
#include "weights.hpp"

const uint16_t ReducedTreeMaker::reduced_tree_version(1);

ReducedTreeMaker::ReducedTreeMaker(const std::string& in_file_name,
                                   const bool is_list,
                                   const double weight_in):
  EventHandler(in_file_name, is_list, weight_in, false){
}

void ReducedTreeMaker::MakeReducedTree(const std::string& out_file_name){
  TFile file(out_file_name.c_str(), "recreate");
  std::set<EventNumber> eventList;
  file.cd();

  const bool isRealData(sampleName.find("Run2012")!=std::string::npos);
  std::vector<float> dataDist(pu::RunsThrough203002, pu::RunsThrough203002+60);
  std::vector<float> MCDist(pu::Summer2012_S10, pu::Summer2012_S10+60);//QQQ this needs to change later for general pileup scenario
  reweight::LumiReWeighting lumiWeights(MCDist, dataDist);

  TTree reduced_tree("reduced_tree","reduced_tree");
  bool passes_JSON_cut(false), passes_PV_cut(false), passes_MET_cleaning_cut(false);
  bool passes_lepton_cut(false), passes_HT_cut(false), passes_MET_cut(false);
  bool passes_num_jets_cut(false), passes_b_tagging_cut(false);
  bool passes_baseline_cuts(false);

  float pu_true_num_interactions(0.0);
  uint8_t num_primary_vertices(0);

  float highest_jet_pt(0.0), second_highest_jet_pt(0.0), third_highest_jet_pt(0.0),
    fourth_highest_jet_pt(0.0), fifth_highest_jet_pt(0.0);
  float highest_csv(0.0), second_highest_csv(0.0),
    third_highest_csv(0.0), fourth_highest_csv(0.0), fifth_highest_csv(0.0);
  float met_sig(0.0), met(0.0);
  uint8_t num_jets(0), num_csvl_jets(0), num_csvm_jets(0), num_csvt_jets(0);

  uint8_t num_iso_tracks(0);
  uint8_t num_veto_electrons(0), num_veto_muons(0), num_veto_taus(0), num_veto_leptons(0);
  uint8_t num_loose_electrons(0), num_loose_muons(0), num_loose_taus(0), num_loose_leptons(0);
  uint8_t num_medium_electrons(0), num_medium_muons(0), num_medium_taus(0), num_medium_leptons(0);
  uint8_t num_tight_electrons(0), num_tight_muons(0), num_tight_taus(0), num_tight_leptons(0);

  float mt2_best_csv_high_pt_loose_emu_Wmass(0.0);
  float mt2_best_csv_high_pt_loose_emu_massless(0.0);
  float mt_high_pt_loose_emu(0.0);
  float delta_phi_met_high_pt_loose_emu(0.0);
  float delta_phi_W_high_pt_loose_emu(0.0);

  float ht_jets(0.0), ht_jets_met(0.0), ht_jets_leps(0.0), ht_jets_met_leps(0.0);
  float full_weight(0.0), lumi_weight(0.0), pu_weight(0.0);

  int16_t mass1(0), mass2(0);

  uint32_t run_here(0), event_here(0), lumiblock_here(0);

  reduced_tree.Branch("passes_JSON_cut", &passes_JSON_cut);
  reduced_tree.Branch("passes_PV_cut",&passes_PV_cut);
  reduced_tree.Branch("passes_MET_cleaning_cut",&passes_MET_cleaning_cut);
  reduced_tree.Branch("passes_lepton_cut",&passes_lepton_cut);
  reduced_tree.Branch("passes_HT_cut",&passes_HT_cut);
  reduced_tree.Branch("passes_MET_cut",&passes_MET_cut);
  reduced_tree.Branch("passes_num_jets_cut",&passes_num_jets_cut);
  reduced_tree.Branch("passes_b_tagging_cut",&passes_b_tagging_cut);
  reduced_tree.Branch("passes_baseline_cuts",&passes_baseline_cuts);

  reduced_tree.Branch("highest_jet_pt", &highest_jet_pt);
  reduced_tree.Branch("second_highest_jet_pt", &second_highest_jet_pt);
  reduced_tree.Branch("third_highest_jet_pt", &third_highest_jet_pt);
  reduced_tree.Branch("fourth_highest_jet_pt", &fourth_highest_jet_pt);
  reduced_tree.Branch("fifth_highest_jet_pt", &fifth_highest_jet_pt);

  reduced_tree.Branch("highest_csv", &highest_csv);
  reduced_tree.Branch("second_highest_csv", &second_highest_csv);
  reduced_tree.Branch("third_highest_csv", &third_highest_csv);
  reduced_tree.Branch("fourth_highest_csv", &fourth_highest_csv);
  reduced_tree.Branch("fifth_highest_csv", &fifth_highest_csv);

  reduced_tree.Branch("pu_true_num_interactions", &pu_true_num_interactions);
  reduced_tree.Branch("num_primary_vertices", &num_primary_vertices);

  reduced_tree.Branch("met_sig", &met_sig);
  reduced_tree.Branch("met", &met);

  reduced_tree.Branch("num_jets", &num_jets);
  reduced_tree.Branch("num_csvl_jets", &num_csvl_jets);
  reduced_tree.Branch("num_csvm_jets", &num_csvm_jets);
  reduced_tree.Branch("num_csvt_jets", &num_csvt_jets);

  reduced_tree.Branch("num_veto_electrons", &num_veto_electrons);
  reduced_tree.Branch("num_veto_muons", &num_veto_muons);
  reduced_tree.Branch("num_veto_taus", &num_veto_taus);
  reduced_tree.Branch("num_veto_leptons", &num_veto_leptons);

  reduced_tree.Branch("num_loose_electrons", &num_loose_electrons);
  reduced_tree.Branch("num_loose_muons", &num_loose_muons);
  reduced_tree.Branch("num_loose_taus", &num_loose_taus);
  reduced_tree.Branch("num_loose_leptons", &num_loose_leptons);

  reduced_tree.Branch("num_medium_electrons", &num_medium_electrons);
  reduced_tree.Branch("num_medium_muons", &num_medium_muons);
  reduced_tree.Branch("num_medium_taus", &num_medium_taus);
  reduced_tree.Branch("num_medium_leptons", &num_medium_leptons);

  reduced_tree.Branch("num_tight_electrons", &num_tight_electrons);
  reduced_tree.Branch("num_tight_muons", &num_tight_muons);
  reduced_tree.Branch("num_tight_taus", &num_tight_taus);
  reduced_tree.Branch("num_tight_leptons", &num_tight_leptons);

  reduced_tree.Branch("num_iso_tracks", &num_iso_tracks);

  reduced_tree.Branch("ht_jets", &ht_jets);
  reduced_tree.Branch("ht_jets_met", &ht_jets_met);
  reduced_tree.Branch("ht_jets_leps", &ht_jets_leps);
  reduced_tree.Branch("ht_jets_met_leps", &ht_jets_met_leps);

  reduced_tree.Branch("mt2_best_csv_high_pt_loose_emu_Wmass", &mt2_best_csv_high_pt_loose_emu_Wmass);
  reduced_tree.Branch("mt2_best_csv_high_pt_loose_emu_massless", &mt2_best_csv_high_pt_loose_emu_massless);
  reduced_tree.Branch("mt_high_pt_loose_emu", &mt_high_pt_loose_emu);
  reduced_tree.Branch("delta_phi_met_high_pt_loose_emu", &delta_phi_met_high_pt_loose_emu);
  reduced_tree.Branch("delta_phi_W_high_pt_loose_emu", &delta_phi_W_high_pt_loose_emu);

  reduced_tree.Branch("full_weight", &full_weight);
  reduced_tree.Branch("lumi_weight", &lumi_weight);
  reduced_tree.Branch("pu_weight", &pu_weight);
 
  reduced_tree.Branch("mass1", &mass1);
  reduced_tree.Branch("mass2", &mass2);

  reduced_tree.Branch("run", &run_here);
  reduced_tree.Branch("event", &event_here);
  reduced_tree.Branch("lumiblock", &lumiblock_here);

  WeightCalculator wc(19399.0);

  Timer timer(GetTotalEntries());
  timer.Start();
  for(int i(0); i<GetTotalEntries(); ++i){
    if(i%1000==0 && i!=0){
      timer.PrintRemainingTime();
    }
    timer.Iterate();
    GetEntry(i);

    std::pair<std::set<EventNumber>::iterator, bool> returnVal(eventList.insert(EventNumber(run, event, lumiblock)));
    if(!returnVal.second) continue;
    
    // Saving our cuts for the reduced tree
    passes_JSON_cut=PassesJSONCut();
    passes_PV_cut=PassesPVCut();
    passes_MET_cleaning_cut=PassesMETCleaningCut();
    passes_lepton_cut=PassesLeptonCut();
    passes_HT_cut=PassesHTCut();
    passes_MET_cut=PassesMETCut();
    passes_num_jets_cut=PassesNumJetsCut();
    passes_b_tagging_cut=PassesBTaggingCut();

    highest_jet_pt=GetHighestJetPt(1);
    second_highest_jet_pt=GetHighestJetPt(2);
    third_highest_jet_pt=GetHighestJetPt(3);
    fourth_highest_jet_pt=GetHighestJetPt(4);
    fifth_highest_jet_pt=GetHighestJetPt(5);

    highest_csv=GetHighestJetCSV(1);
    second_highest_csv=GetHighestJetCSV(2);
    third_highest_csv=GetHighestJetCSV(3);
    fourth_highest_csv=GetHighestJetCSV(4);
    fifth_highest_csv=GetHighestJetCSV(5);

    pu_true_num_interactions=GetNumInteractions();
    num_primary_vertices=GetNumVertices();

    met_sig=pfmets_fullSignif;
    met=pfTypeImets_et->at(0);

    num_jets=GetNumGoodJets();
    num_csvl_jets=GetNumCSVLJets();
    num_csvm_jets=GetNumCSVMJets();
    num_csvt_jets=GetNumCSVTJets();

    num_veto_electrons=GetNumElectrons(0);
    num_veto_muons=GetNumMuons(0);
    num_veto_taus=GetNumTaus(0);
    num_veto_leptons=num_veto_electrons+num_veto_muons+num_veto_taus;
    num_loose_electrons=GetNumElectrons(1);
    num_loose_muons=GetNumMuons(1);
    num_loose_taus=GetNumTaus(1);
    num_loose_leptons=num_loose_electrons+num_loose_muons+num_loose_taus;
    num_medium_electrons=GetNumElectrons(2);
    num_medium_muons=GetNumMuons(2);
    num_medium_taus=GetNumTaus(2);
    num_medium_leptons=num_medium_electrons+num_medium_muons+num_medium_taus;
    num_tight_electrons=GetNumElectrons(3);
    num_tight_muons=GetNumMuons(3);
    num_tight_taus=GetNumTaus(3);
    num_tight_leptons=num_tight_electrons+num_tight_muons+num_tight_taus;
    num_iso_tracks=NewGetNumIsoTracks();

    mt2_best_csv_high_pt_loose_emu_Wmass=GetMT2(80.399);
    mt2_best_csv_high_pt_loose_emu_massless=GetMT2(0.0);
    mt_high_pt_loose_emu=GetMT();
    delta_phi_met_high_pt_loose_emu=GetDeltaPhiMETLepton();
    delta_phi_W_high_pt_loose_emu=GetDeltaPhiWLepton();

    ht_jets=GetHT(false, false);
    ht_jets_met=GetHT(true, false);
    ht_jets_leps=GetHT(false, true);
    ht_jets_met_leps=GetHT(true, true);

    mass1=GetMass1();
    mass2=GetMass2();

    double this_scale_factor(scaleFactor);
    if(sampleName.substr(0, 3)=="SMS"){
      this_scale_factor=wc.GetWeight(sampleName, mass1, mass2);
    }

    pu_weight=isRealData?1.0:GetPUWeight(lumiWeights);
    lumi_weight=this_scale_factor;
    full_weight=pu_weight*lumi_weight;

    run_here=run;
    event_here=event;
    lumiblock_here=lumiblock;

    reduced_tree.Fill(); 
  }

  reduced_tree.Write();

  time_t raw_time;
  time(&raw_time);
  struct tm * utc_creation_time(gmtime(&raw_time));
  uint16_t utc_creation_year(utc_creation_time->tm_year+1900);
  uint8_t utc_creation_month(utc_creation_time->tm_mon+1);
  uint8_t utc_creation_day(utc_creation_time->tm_mday);
  uint8_t utc_creation_hour(utc_creation_time->tm_hour);
  uint8_t utc_creation_minute(utc_creation_time->tm_min);
  uint8_t utc_creation_second(utc_creation_time->tm_sec);
  int32_t utc_creation_isdst(utc_creation_time->tm_isdst);

  uint32_t original_file_entries(GetTotalEntries());

  TTree meta_info("meta_info", "meta_info");
  meta_info.Branch("original_file_name", &sampleName);
  meta_info.Branch("original_file_entries", &original_file_entries);
  meta_info.Branch("reduced_tree_version", const_cast<uint16_t*>(&reduced_tree_version));
  meta_info.Branch("utc_creation_year", &utc_creation_year);
  meta_info.Branch("utc_creation_month", &utc_creation_month);
  meta_info.Branch("utc_creation_day", &utc_creation_day);
  meta_info.Branch("utc_creation_hour", &utc_creation_hour);
  meta_info.Branch("utc_creation_minute", &utc_creation_minute);
  meta_info.Branch("utc_creation_second", &utc_creation_second);
  meta_info.Branch("utc_creation_isdst", &utc_creation_isdst);
  meta_info.Fill();
  meta_info.Write();

  file.Close();
}
