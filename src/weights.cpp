#include "weights.hpp"
#include <map>

std::map<std::string, double> WeightCalculator::crossSectionTable;
std::map<std::string, int> WeightCalculator::totalEventsTable;

WeightCalculator::WeightCalculator(const double lumiIn):
  lumi(lumiIn){
  SetCrossSections();
  SetTotalEvents();
}

void WeightCalculator::SetLuminosity(const double lumiIn){
  lumi=lumiIn;
}

double WeightCalculator::GetLuminosity() const{
  return lumi;
}

double WeightCalculator::GetCrossSection(const std::string &process) const{
  for(std::map<std::string, double>::iterator it(crossSectionTable.begin());
      it!=crossSectionTable.end(); ++it){
    if(process.find(it->first)!=std::string::npos){
      return it->second;
    }
  }
  return -1.0;
}

double WeightCalculator::GetCrossSection(const std::string &process, const int m1,
                                         const int m2) const{
  if(m1>=0 && m2>=0){
    if(process.find("SMS-T1tttt_2J_mGo-845to3000_mLSP-1to1355_TuneZ2star_14TeV-madgraph-tauola_Summer12-START53_V7C_FSIM_PU_S12-v1_AODSIM_UCSB1845reshuf_v68")!=std::string::npos){
      return GetT1tttt14TeVCrossSection(m1, m2);
    }else{
      //Other models go here eventually
      return -1.0;
    }
  }else{
    return GetCrossSection(process);
  }
}

int WeightCalculator::GetTotalEvents(const std::string &process) const{
  for(std::map<std::string, int>::iterator it(totalEventsTable.begin());
      it!=totalEventsTable.end(); ++it){
    if(process.find(it->first)!=std::string::npos){
      return it->second;
    }
  }
  return -1;
}

int WeightCalculator::GetTotalEvents(const std::string &process, const int m1,
                                     const int m2) const{
  if(m1>=0 && m2>=0){
    if(process.find("SMS-T1tttt_2J_mGo-845to3000_mLSP-1to1355_TuneZ2star_14TeV-madgraph-tauola_Summer12-START53_V7C_FSIM_PU_S12-v1_AODSIM_UCSB1845reshuf_v68")!=std::string::npos){
      return GetT1tttt14TeVTotalEvents(m1, m2);
    }else{
      //Other models go here eventually
      return -1;
    }
  }else{
    return GetTotalEvents(process);
  }
}

double WeightCalculator::GetWeight(const std::string &process) const{
  const double xsec(GetCrossSection(process)), events(GetTotalEvents(process));
  if(events>=0 && xsec>=0.0){
    return lumi*xsec/static_cast<double>(events);
  }else{
    return 1.0;
  }
}

double WeightCalculator::GetWeight(const std::string &process, const int m1,
                                   const int m2) const{
  if(m1>=0 && m2>=0){
    return GetWeight(process);
  }else{
    return GetWeight(process);
  }
}

void WeightCalculator::SetCrossSections(){
  const double ttbar_xsec(245.8);
  const double ttbar_norm(ttbar_xsec/(13.43+53.4+53.2));
  const double mysterious_k_factor(1.19);
  crossSectionTable["BJets_HT-1000ToInf_8TeV-madgraph"]=4.712;
  crossSectionTable["BJets_HT-250To500_8TeV-madgraph"]=5828.0;
  crossSectionTable["BJets_HT-500To1000_8TeV-madgraph"]=217.6;
  crossSectionTable["QCD_Pt-1000to1400_TuneZ2star_8TeV_pythia6"]=0.737844;
  crossSectionTable["QCD_Pt-120to170_TuneZ2star_8TeV_pythia6"]=156293.3;
  crossSectionTable["QCD_Pt-1400to1800_TuneZ2star_8TeV_pythia6"]=0.03352235;
  crossSectionTable["QCD_Pt-170to300_TuneZ2star_8TeV_pythia6_v2"]=34138.15;
  crossSectionTable["QCD_Pt-1800_TuneZ2star_8TeV_pythia6"]=0.001829005;
  crossSectionTable["QCD_Pt-300to470_TuneZ2star_8TeV_pythia6_v3"]=1759.549;
  crossSectionTable["QCD_Pt-470to600_TuneZ2star_8TeV_pythia6"]=113.8791;
  crossSectionTable["QCD_Pt-600to800_TuneZ2star_8TeV_pythia6"]=26.9921;
  crossSectionTable["QCD_Pt-800to1000_TuneZ2star_8TeV_pythia6"]=3.550036;
  crossSectionTable["SMS-HbbHbb_mHiggsino-200_mLSP-1_8TeV-Pythia6Z_jgsmith-SMS-HbbHbb_mHiggsino-200_mLSP-1_8TeV-Pythia6Z-26439e701cfb9736f297615863e915f9_USER_UCSB1807_v69"]=0.608*0.561*0.561;
  crossSectionTable["SMS-HbbHbb_mHiggsino-250_mLSP-1_8TeV-Pythia6Z_jgsmith-SMS-HbbHbb_mHiggsino-250_mLSP-1_8TeV-Pythia6Z-26439e701cfb9736f297615863e915f9_USER_UCSB1808_v69"]=0.244*0.561*0.561;
  crossSectionTable["SMS-HbbHbb_mHiggsino-300_mLSP-1_8TeV-Pythia6Z_jgsmith-SMS-HbbHbb_mHiggsino-300_mLSP-1_8TeV-Pythia6Z-26439e701cfb9736f297615863e915f9_USER_UCSB1810_v69"]=0.111*0.561*0.561;
  crossSectionTable["SMS-HbbHbb_mHiggsino-350_mLSP-1_8TeV-Pythia6Z_jgsmith-SMS-HbbHbb_mHiggsino-350_mLSP-1_8TeV-Pythia6Z-26439e701cfb9736f297615863e915f9_USER_UCSB1811_v69"]=0.0552*0.561*0.561;
  crossSectionTable["SMS-HbbHbb_mHiggsino-400_mLSP-1_8TeV-Pythia6Z_jgsmith-SMS-HbbHbb_mHiggsino-400_mLSP-1_8TeV-Pythia6Z-26439e701cfb9736f297615863e915f9_USER_UCSB1812_v69"]=0.0294*0.561*0.561;
  crossSectionTable["SMS-HbbHbb_mHiggsino-450_mLSP-1_8TeV-Pythia6Z_jgsmith-SMS-HbbHbb_mHiggsino-450_mLSP-1_8TeV-Pythia6Z-26439e701cfb9736f297615863e915f9_USER_UCSB1809_v69"]=0.0163*0.561*0.561;
  crossSectionTable["SMS-TChiHH_2b2b_2J_mChargino-200_mLSP-1_TuneZ2star_8TeV-madgraph-tauola"]=0.608*0.561*0.561;
  crossSectionTable["SMS-TChiHH_2b2b_2J_mChargino-250_mLSP-1_TuneZ2star_8TeV-madgraph-tauola"]=0.244*0.561*0.561;
  crossSectionTable["SMS-TChiHH_2b2b_2J_mChargino-300_mLSP-1_TuneZ2star_8TeV-madgraph-tauola"]=0.111*0.561*0.561;
  crossSectionTable["SMS-TChiHH_2b2b_2J_mChargino-350_mLSP-1_TuneZ2star_8TeV-madgraph-tauola"]=0.0552*0.561*0.561;
  crossSectionTable["SMS-TChiHH_2b2b_2J_mChargino-400_mLSP-1_TuneZ2star_8TeV-madgraph-tauola"]=0.0294*0.561*0.561;
  crossSectionTable["SMS-TChiHH_2b2b_2J_mChargino-450_mLSP-1_TuneZ2star_8TeV-madgraph-tauola"]=0.0163*0.561*0.561;
  crossSectionTable["SMS-TChiZH_ZccbbHbb_2J_mChargino-130to500_mLSP-1to370_TuneZ2star_8TeV-madgraph-tauola"]=0.111*0.561*0.1512;
  crossSectionTable["TChihh_200"]=0.6975*0.561*0.561;
  crossSectionTable["TChihh_250"]=0.271*0.561*0.561;
  crossSectionTable["TChihh_400"]=0.03*0.561*0.561;
  crossSectionTable["TTH_HToBB_M-125_8TeV-pythia6"]=0.1293*0.577;
  crossSectionTable["TTH_Inclusive_M-125_8TeV_pythia6"]=0.1293*0.577;
  crossSectionTable["TTJets_FullLeptMGDecays_8TeV-madgraph"]=13.43*ttbar_norm;
  crossSectionTable["TTJets_HadronicMGDecays_8TeV-madgraph"]=53.4*ttbar_norm;
  crossSectionTable["TTJets_MassiveBinDECAY_TuneZ2star_8TeV-madgraph-tauola"]=ttbar_xsec;
  crossSectionTable["TTJets_SemiLeptMGDecays_8TeV-madgraph"]=53.2*ttbar_norm;
  crossSectionTable["TTJets_WToBC_8TeV-madgraph-tauola"]=ttbar_xsec*0.00084*0.676;
  crossSectionTable["TTWJets_8TeV-madgraph"]=0.2149;
  crossSectionTable["TTZJets_8TeV-madgraph_v2"]=0.172;
  crossSectionTable["TTbar_TuneZ2star_13TeV"]=818.8;//Need to find out where Manuel got this from
  crossSectionTable["T_s-channel_TuneZ2star_8TeV-powheg-tauola"]=3.79;
  crossSectionTable["T_t-channel_TuneZ2star_8TeV-powheg"]=56.4;
  crossSectionTable["T_tW-channel-DR_TuneZ2star_8TeV-powheg"]=11.1;
  crossSectionTable["Tbar_s-channel_TuneZ2star_8TeV-powheg-tauola"]=1.76;
  crossSectionTable["Tbar_t-channel_TuneZ2star_8TeV-powheg"]=30.7;
  crossSectionTable["Tbar_tW-channel-DR_TuneZ2star_8TeV-powheg"]=11.1;
  crossSectionTable["W2JetsToLNu_TuneZ2Star_8TeV-madgraph"]=1750.0*mysterious_k_factor;
  crossSectionTable["W3JetsToLNu_TuneZ2Star_8TeV-madgraph"]=519.0*mysterious_k_factor;
  crossSectionTable["W4JetsToLNu_TuneZ2Star_8TeV-madgraph"]=214.0*mysterious_k_factor;
  crossSectionTable["WH_WToLNu_HToBB_M-125_8TeV-powheg-herwigpp"]=0.7046 *(0.1075+0.1057+0.1125) *0.577;
  crossSectionTable["WW_TuneZ2star_8TeV_pythia6_tauola"]=55.0;
  crossSectionTable["WZ_TuneZ2star_8TeV_pythia6_tauola"]=32.3;
  crossSectionTable["WbbJetsToLNu_Massive_TuneZ2star_8TeV-madgraph-pythia6_tauola"]=211.3;
  crossSectionTable["ZH_ZToBB_HToBB_M-125_8TeV-powheg-herwigpp"]=0.4153*0.1512*0.577;
  crossSectionTable["ZJetsToNuNu_100_HT_200_TuneZ2Star_8TeV_madgraph"]=160.3*mysterious_k_factor;
  crossSectionTable["ZJetsToNuNu_200_HT_400_TuneZ2Star_8TeV_madgraph"]=41.49*mysterious_k_factor;
  crossSectionTable["ZJetsToNuNu_400_HT_inf_TuneZ2Star_8TeV_madgraph"]=5.272*mysterious_k_factor;
  crossSectionTable["ZJetsToNuNu_50_HT_100_TuneZ2Star_8TeV_madgraph"]=381.2*mysterious_k_factor;
  crossSectionTable["ZZ_TuneZ2star_8TeV_pythia6_tauola"]=17.654;
}

void WeightCalculator::SetTotalEvents(){
  totalEventsTable["BJets_HT-1000ToInf_8TeV-madgraph_Summer12_DR53X-PU_S10_START53_V7A-v1_AODSIM_UCSB1895_v71"]=3137949;
  totalEventsTable["BJets_HT-250To500_8TeV-madgraph_Summer12_DR53X-PU_S10_START53_V7A-v1_AODSIM_UCSB1893_v71"]=13183812;
  totalEventsTable["BJets_HT-500To1000_8TeV-madgraph_Summer12_DR53X-PU_S10_START53_V7A-v1_AODSIM_UCSB1894_v71"]=6650243;
  totalEventsTable["QCD_Pt-1000to1400_TuneZ2star_8TeV_pythia6_Summer12_DR53X-PU_S10_START53_V7A-v1_AODSIM_UCSB1664_v67"]=1964088;
  totalEventsTable["QCD_Pt-1000to1400_TuneZ2star_8TeV_pythia6_Summer12_DR53X-PU_S10_START53_V7A-v1_AODSIM_UCSB1664_v67"]=1964088;
  totalEventsTable["QCD_Pt-1000to1400_TuneZ2star_8TeV_pythia6_Summer12_DR53X-PU_S10_START53_V7A-v1_AODSIM_UCSB1903_v71"]=1964088;
  totalEventsTable["QCD_Pt-120to170_TuneZ2star_8TeV_pythia6_Summer12_DR53X-PU_S10_START53_V7A-v3_AODSIM_UCSB1513_v66"]=5985732;
  totalEventsTable["QCD_Pt-120to170_TuneZ2star_8TeV_pythia6_Summer12_DR53X-PU_S10_START53_V7A-v3_AODSIM_UCSB1654_v67"]=5985732;
  totalEventsTable["QCD_Pt-120to170_TuneZ2star_8TeV_pythia6_Summer12_DR53X-PU_S10_START53_V7A-v3_AODSIM_UCSB1897_v71"]=5985732;
  totalEventsTable["QCD_Pt-1400to1800_TuneZ2star_8TeV_pythia6_Summer12_DR53X-PU_S10_START53_V7A-v1_AODSIM_UCSB1665_v67"]=2000062;
  totalEventsTable["QCD_Pt-1400to1800_TuneZ2star_8TeV_pythia6_Summer12_DR53X-PU_S10_START53_V7A-v1_AODSIM_UCSB1665_v67"]=2000062;//
  totalEventsTable["QCD_Pt-1400to1800_TuneZ2star_8TeV_pythia6_Summer12_DR53X-PU_S10_START53_V7A-v1_AODSIM_UCSB1904_v71"]=2000062;
  totalEventsTable["QCD_Pt-170to300_TuneZ2star_8TeV_pythia6_Summer12_DR53X-PU_S10_START53_V7A-v2_AODSIM_UCSB1898_v71"]=5814398;
  totalEventsTable["QCD_Pt-170to300_TuneZ2star_8TeV_pythia6_v2_Summer12_DR53X-PU_S10_START53_V7A-v1_AODSIM_UCSB1603_v66"]=19970232;
  totalEventsTable["QCD_Pt-170to300_TuneZ2star_8TeV_pythia6_v2_Summer12_DR53X-PU_S10_START53_V7A-v1_AODSIM_UCSB1657_v67"]=19970232;
  totalEventsTable["QCD_Pt-1800_TuneZ2star_8TeV_pythia6_Summer12_DR53X-PU_S10_START53_V7A-v1_AODSIM_UCSB1585_v66"]=977586;
  totalEventsTable["QCD_Pt-1800_TuneZ2star_8TeV_pythia6_Summer12_DR53X-PU_S10_START53_V7A-v1_AODSIM_UCSB1666_v67"]=977586;
  totalEventsTable["QCD_Pt-1800_TuneZ2star_8TeV_pythia6_Summer12_DR53X-PU_S10_START53_V7A-v1_AODSIM_UCSB1905_v71"]=977586;
  totalEventsTable["QCD_Pt-300to470_TuneZ2star_8TeV_pythia6_Summer12_DR53X-PU_S10_START53_V7A-v2_AODSIM_UCSB1899_v71"]=5978500;
  totalEventsTable["QCD_Pt-300to470_TuneZ2star_8TeV_pythia6_v3_Summer12_DR53X-PU_S10_START53_V7A-v1_AODSIM_UCSB1609_v66"]=19894000;
  totalEventsTable["QCD_Pt-300to470_TuneZ2star_8TeV_pythia6_v3_Summer12_DR53X-PU_S10_START53_V7A-v1_AODSIM_UCSB1658_v67"]=19894000;
  totalEventsTable["QCD_Pt-470to600_TuneZ2star_8TeV_pythia6_Summer12_DR53X-PU_S10_START53_V7A-v2_AODSIM_UCSB1659_v67"]=3994848;
  totalEventsTable["QCD_Pt-470to600_TuneZ2star_8TeV_pythia6_Summer12_DR53X-PU_S10_START53_V7A-v2_AODSIM_UCSB1659_v67"]=3994848;
  totalEventsTable["QCD_Pt-470to600_TuneZ2star_8TeV_pythia6_Summer12_DR53X-PU_S10_START53_V7A-v2_AODSIM_UCSB1900_v71"]=3994848;
  totalEventsTable["QCD_Pt-600to800_TuneZ2star_8TeV_pythia6_Summer12_DR53X-PU_S10_START53_V7A-v2_AODSIM_UCSB1663_v67"]=3996864;
  totalEventsTable["QCD_Pt-600to800_TuneZ2star_8TeV_pythia6_Summer12_DR53X-PU_S10_START53_V7A-v2_AODSIM_UCSB1663_v67"]=3996864;
  totalEventsTable["QCD_Pt-600to800_TuneZ2star_8TeV_pythia6_Summer12_DR53X-PU_S10_START53_V7A-v2_AODSIM_UCSB1901_v71"]=3996864;
  totalEventsTable["QCD_Pt-800to1000_TuneZ2star_8TeV_pythia6_Summer12_DR53X-PU_S10_START53_V7A-v2_AODSIM_UCSB1559_v66"]=3998563;
  totalEventsTable["QCD_Pt-800to1000_TuneZ2star_8TeV_pythia6_Summer12_DR53X-PU_S10_START53_V7A-v2_AODSIM_UCSB1660_v67"]=3998563;
  totalEventsTable["QCD_Pt-800to1000_TuneZ2star_8TeV_pythia6_Summer12_DR53X-PU_S10_START53_V7A-v2_AODSIM_UCSB1902_v71"]=3998563;
  totalEventsTable["SMS-HbbHbb_mHiggsino-200_mLSP-1_8TeV-Pythia6Z_jgsmith-SMS-HbbHbb_mHiggsino-200_mLSP-1_8TeV-Pythia6Z-26439e701cfb9736f297615863e915f9_USER_UCSB1807_v69"]=99996;
  totalEventsTable["SMS-HbbHbb_mHiggsino-250_mLSP-1_8TeV-Pythia6Z_jgsmith-SMS-HbbHbb_mHiggsino-250_mLSP-1_8TeV-Pythia6Z-26439e701cfb9736f297615863e915f9_USER_UCSB1808_v69"]=99994;
  totalEventsTable["SMS-HbbHbb_mHiggsino-300_mLSP-1_8TeV-Pythia6Z_jgsmith-SMS-HbbHbb_mHiggsino-300_mLSP-1_8TeV-Pythia6Z-26439e701cfb9736f297615863e915f9_USER_UCSB1810_v69"]=99995;
  totalEventsTable["SMS-HbbHbb_mHiggsino-350_mLSP-1_8TeV-Pythia6Z_jgsmith-SMS-HbbHbb_mHiggsino-350_mLSP-1_8TeV-Pythia6Z-26439e701cfb9736f297615863e915f9_USER_UCSB1811_v69"]=99994;
  totalEventsTable["SMS-HbbHbb_mHiggsino-400_mLSP-1_8TeV-Pythia6Z_jgsmith-SMS-HbbHbb_mHiggsino-400_mLSP-1_8TeV-Pythia6Z-26439e701cfb9736f297615863e915f9_USER_UCSB1812_v69"]=99987;
  totalEventsTable["SMS-HbbHbb_mHiggsino-450_mLSP-1_8TeV-Pythia6Z_jgsmith-SMS-HbbHbb_mHiggsino-450_mLSP-1_8TeV-Pythia6Z-26439e701cfb9736f297615863e915f9_USER_UCSB1809_v69"]=99986;
  totalEventsTable["SMS-TChiHH_2b2b_2J_mChargino-200_mLSP-1_TuneZ2star_8TeV-madgraph-tauola_Summer12-START53_V19_FSIM-v1_AODSIM_UCSB1872_v71"]=388371;
  totalEventsTable["SMS-TChiHH_2b2b_2J_mChargino-250_mLSP-1_TuneZ2star_8TeV-madgraph-tauola_Summer12-START53_V19_FSIM-v1_AODSIM_UCSB1872_v71"]=151750;
  totalEventsTable["SMS-TChiHH_2b2b_2J_mChargino-300_mLSP-1_TuneZ2star_8TeV-madgraph-tauola_Summer12-START53_V19_FSIM-v1_AODSIM_UCSB1872_v71"]=147484;
  totalEventsTable["SMS-TChiHH_2b2b_2J_mChargino-350_mLSP-1_TuneZ2star_8TeV-madgraph-tauola_Summer12-START53_V19_FSIM-v1_AODSIM_UCSB1871_v71"]=80478;
  totalEventsTable["SMS-TChiHH_2b2b_2J_mChargino-400_mLSP-1_TuneZ2star_8TeV-madgraph-tauola_Summer12-START53_V19_FSIM-v1_AODSIM_UCSB1871_v71"]=79013;
  totalEventsTable["SMS-TChiHH_2b2b_2J_mChargino-450_mLSP-1_TuneZ2star_8TeV-madgraph-tauola_Summer12-START53_V19_FSIM-v1_AODSIM_UCSB1871_v71"]=76402;
  totalEventsTable["SMS-TChiZH_ZccbbHbb_2J_mChargino-130to500_mLSP-1to370_TuneZ2star_8TeV-madgraph-tauola_Summer12-START53_V19_FSIM-v1_AODSIM_UCSB1873_v71"]=2155;
  totalEventsTable["TChihh_200"]=9999;
  totalEventsTable["TChihh_250"]=9999;
  totalEventsTable["TChihh_400"]=9999;
  totalEventsTable["TTH_HToBB_M-125_8TeV-pythia6_Summer12_DR53X-PU_S10_START53_V7A-v1_AODSIM_UCSB1855_v71"]=1000008;
  totalEventsTable["TTH_Inclusive_M-125_8TeV_pythia6_Summer12_DR53X-PU_S10_START53_V7A-v1_AODSIM+_UCSB1783_v68"]=1000000;
  totalEventsTable["TTJets_FullLeptMGDecays_8TeV-madgraph-tauola_Summer12_DR53X-PU_S10_START53_V7C-v2_AODSIM_UCSB1883_v71"]=12011428;
  totalEventsTable["TTJets_FullLeptMGDecays_8TeV-madgraph_Summer12_DR53X-PU_S10_START53_V7A-v2_AODSIM_UCSB1596_v66"]=12119013;
  totalEventsTable["TTJets_HadronicMGDecays_8TeV-madgraph_Summer12_DR53X-PU_S10_START53_V7A_ext-v1_AODSIM_UCSB1613_v66"]=31223821;
  totalEventsTable["TTJets_HadronicMGDecays_8TeV-madgraph_Summer12_DR53X-PU_S10_START53_V7A_ext-v1_AODSIM_UCSB1880_v71"]=31223821;
  totalEventsTable["TTJets_MassiveBinDECAY_TuneZ2star_8TeV-madgraph-tauola_Summer12-START53_V7C_FSIM-v2_AODSIM_UCSB1976_v71"]=7299862;
  totalEventsTable["TTJets_MassiveBinDECAY_TuneZ2star_8TeV-madgraph-tauola_Summer12_DR53X-PU_S10_START53_V7A-v1_AODSIM_UCSB1850_v71"]=6923750;
  totalEventsTable["TTJets_SemiLeptMGDecays_8TeV-madgraph-tauola_Summer12_DR53X-PU_S10_START53_V19_ext1-v1_AODSIM_UCSB1962_v71"]=24953451+30856876+30999167;
  totalEventsTable["TTJets_SemiLeptMGDecays_8TeV-madgraph-tauola_Summer12_DR53X-PU_S10_START53_V19_ext2-v1_AODSIM_UCSB1959_v71"]=24953451+30856876+30999167;
  totalEventsTable["TTJets_SemiLeptMGDecays_8TeV-madgraph-tauola_Summer12_DR53X-PU_S10_START53_V7C-v1_AODSIM_UCSB1884_v71"]=24953451+30856876+30999167;
  totalEventsTable["TTJets_SemiLeptMGDecays_8TeV-madgraph_Summer12_DR53X-PU_S10_START53_V7A_ext-v1_AODSIM_UCSB1606_v66"]=25413514;
  totalEventsTable["TTJets_WToBC_8TeV-madgraph-tauola_Summer12_DR53X-PU_S10_START53_V19-v1_AODSIM_UCSB1966_v71"]=276156;
  totalEventsTable["TTWJets_8TeV-madgraph_Summer12_DR53X-PU_S10_START53_V7A-v1_AODSIM_UCSB1605_v66"]=196046;
  totalEventsTable["TTWJets_8TeV-madgraph_Summer12_DR53X-PU_S10_START53_V7A-v1_AODSIM_UCSB1857_v71"]=196046;
  totalEventsTable["TTWJets_8TeV-madgraph_Summer12_DR53X-PU_S10_START53_V7A-v1_AODSIM_UCSB1857_v71"]=196046;
  totalEventsTable["TTZJets_8TeV-madgraph_v2_Summer12_DR53X-PU_S10_START53_V7A-v1_AODSIM_UCSB1604_v66"]=210160;
  totalEventsTable["TTZJets_8TeV-madgraph_v2_Summer12_DR53X-PU_S10_START53_V7A-v1_AODSIM_UCSB1856_v71"]=210160;
  totalEventsTable["TTZJets_8TeV-madgraph_v2_Summer12_DR53X-PU_S10_START53_V7A-v1_AODSIM_UCSB1856_v71"]=210160;
  totalEventsTable["TTbar_TuneZ2star_13TeV-powheg-tauola_Summer13dr53X-PU25bx25_START53_V19D-v1_AODSIM_UCSB2027_v71"]=993322;
  totalEventsTable["TTbar_TuneZ2star_13TeV-pythia6-tauola_Summer13dr53X-PU45bx25_START53_V19D-v2_AODSIM_UCSB1933_v71"]=997120;
  totalEventsTable["T_s-channel_TuneZ2star_8TeV-powheg-tauola_Summer12_DR53X-PU_S10_START53_V7A-v1_AODSIM_UCSB1860_v71"]=259961;
  totalEventsTable["T_t-channel_TuneZ2star_8TeV-powheg-tauola_Summer12_DR53X-PU_S10_START53_V7A-v1_AODSIM_UCSB1861_v71"]=3758227;
  totalEventsTable["T_tW-channel-DR_TuneZ2star_8TeV-powheg-tauola_Summer12_DR53X-PU_S10_START53_V7A-v1_AODSIM_UCSB1862_v71"]=497658;
  totalEventsTable["Tbar_s-channel_TuneZ2star_8TeV-powheg-tauola_Summer12_DR53X-PU_S10_START53_V7A-v1_AODSIM_UCSB1864_v71"]=139974;
  totalEventsTable["Tbar_t-channel_TuneZ2star_8TeV-powheg-tauola_Summer12_DR53X-PU_S10_START53_V7A-v1_AODSIM_UCSB1865_v71"]=1935072;
  totalEventsTable["Tbar_tW-channel-DR_TuneZ2star_8TeV-powheg-tauola_Summer12_DR53X-PU_S10_START53_V7A-v1_AODSIM_UCSB1866_v71"]=493460;
  totalEventsTable["W2JetsToLNu_TuneZ2Star_8TeV-madgraph_Summer12_DR53X-PU_S10_START53_V7A-v1_AODSIM_UCSB1877_v71"]=34044921;
  totalEventsTable["W3JetsToLNu_TuneZ2Star_8TeV-madgraph_Summer12_DR53X-PU_S10_START53_V7A-v1_AODSIM_UCSB1878_v71"]=15539503;
  totalEventsTable["W4JetsToLNu_TuneZ2Star_8TeV-madgraph_Summer12_DR53X-PU_S10_START53_V7A-v1_AODSIM_UCSB1879_v71"]=13382803;
  totalEventsTable["WH_WToLNu_HToBB_M-125_8TeV-powheg-herwigpp_Summer12_DR53X-PU_S10_START53_V7A-v1_AODSIM_UCSB1858_v71"]=1000000;
  totalEventsTable["WH_WToLNu_HToBB_M-125_8TeV-powheg-herwigpp_Summer12_DR53X-PU_S10_START53_V7A-v1_AODSIM_UCSB1858_v71"]=1000000;
  totalEventsTable["WW_TuneZ2star_8TeV_pythia6_tauola_Summer12_DR53X-PU_S10_START53_V7A-v1_AODSIM_UCSB1874_v71"]=10000431;
  totalEventsTable["WW_TuneZ2star_8TeV_pythia6_tauola_Summer12_DR53X-PU_S10_START53_V7A-v1_AODSIM_UCSB1874_v71"]=10000431;
  totalEventsTable["WZ_TuneZ2star_8TeV_pythia6_tauola_Summer12_DR53X-PU_S10_START53_V7A-v1_AODSIM_UCSB1875_v71"]=10000283;
  totalEventsTable["WbbJetsToLNu_Massive_TuneZ2star_8TeV-madgraph-pythia6_tauola_Summer12_DR53X-PU_S10_START53_V7A-v1_AODSIM_UCSB1677_v67"]=20646001;
  totalEventsTable["WbbJetsToLNu_Massive_TuneZ2star_8TeV-madgraph-pythia6_tauola_Summer12_DR53X-PU_S10_START53_V7A-v1_AODSIM_UCSB1859_v71"]=20646001;
  totalEventsTable["ZH_ZToBB_HToBB_M-125_8TeV-powheg-herwigpp_Summer12_DR53X-PU_S10_START53_V7A-v1_AODSIM_UCSB1868_v71"]=996299;
  totalEventsTable["ZH_ZToBB_HToBB_M-125_8TeV-powheg-herwigpp_Summer12_DR53X-PU_S10_START53_V7A-v1_AODSIM_UCSB1868_v71"]=996299;
  totalEventsTable["ZJetsToNuNu_100_HT_200_TuneZ2Star_8TeV_madgraph_Summer12_DR53X-PU_S10_START53_V7A-v1_AODSIM_UCSB1525_v66"]=5571413+4416646;
  totalEventsTable["ZJetsToNuNu_100_HT_200_TuneZ2Star_8TeV_madgraph_Summer12_DR53X-PU_S10_START53_V7A-v1_AODSIM_UCSB1887_v71"]=4416646;
  totalEventsTable["ZJetsToNuNu_100_HT_200_TuneZ2Star_8TeV_madgraph_ext_Summer12_DR53X-PU_S10_START53_V7C-v1_AODSIM_UCSB1607_v66"]=5571413+4416646;
  totalEventsTable["ZJetsToNuNu_200_HT_400_TuneZ2Star_8TeV_madgraph_Summer12_DR53X-PU_S10_START53_V7A-v1_AODSIM_UCSB1524_v66"]=4689734+5055885;
  totalEventsTable["ZJetsToNuNu_200_HT_400_TuneZ2Star_8TeV_madgraph_Summer12_DR53X-PU_S10_START53_V7A-v1_AODSIM_UCSB1888_v71"]=5055885+4689734;
  totalEventsTable["ZJetsToNuNu_200_HT_400_TuneZ2Star_8TeV_madgraph_ext_Summer12_DR53X-PU_S10_START53_V7A-v1_AODSIM_UCSB1594_v66"]=4689734+5055885;
  totalEventsTable["ZJetsToNuNu_200_HT_400_TuneZ2Star_8TeV_madgraph_ext_Summer12_DR53X-PU_S10_START53_V7A-v1_AODSIM_UCSB1889_v71"]=4689734+5055885;
  totalEventsTable["ZJetsToNuNu_400_HT_inf_TuneZ2Star_8TeV_madgraph_Summer12_DR53X-PU_S10_START53_V7A-v1_AODSIM_UCSB1523_v66"]=4088782+1006928;
  totalEventsTable["ZJetsToNuNu_400_HT_inf_TuneZ2Star_8TeV_madgraph_Summer12_DR53X-PU_S10_START53_V7A-v1_AODSIM_UCSB1890_v71"]=1006928+4088782;
  totalEventsTable["ZJetsToNuNu_400_HT_inf_TuneZ2Star_8TeV_madgraph_ext_Summer12_DR53X-PU_S10_START53_V7A-v1_AODSIM_UCSB1602_v66"]=4088782+1006928;
  totalEventsTable["ZJetsToNuNu_400_HT_inf_TuneZ2Star_8TeV_madgraph_ext_Summer12_DR53X-PU_S10_START53_V7A-v1_AODSIM_UCSB1891_v71"]=4088782+1006928;
  totalEventsTable["ZZ_TuneZ2star_8TeV_pythia6_tauola_Summer12_DR53X-PU_S10_START53_V7A-v1_AODSIM_UCSB1876_v71"]=9799908;
  totalEventsTable["ZZ_TuneZ2star_8TeV_pythia6_tauola_Summer12_DR53X-PU_S10_START53_V7A-v1_AODSIM_UCSB1876_v71"]=9799908;
}

int WeightCalculator::GetT1tttt14TeVTotalEvents(const int m1, const int m2){
  switch(m1){
  case 845:
    switch(m2){
    case 500:
      return 40470;
    default:
      return -1;
    }
  case 1145:
    switch(m2){
    case 500:
      return 40457;
    case 800:
      return 40321;
    default:
      return -1;
    }
  case 1500:
    switch(m2){
    case 1:
      return 41575;
    case 500:
      return 42376;
    case 750:
      return 42190;
    case 855:
      return 42825;
    case 1000:
      return 43345;
    case 1155:
      return 42004;
    default:
      return -1;
    }
  case 1750:
    switch(m2){
    case 1:
      return 43300;
    case 500:
      return 41961;
    case 750:
      return 44104;
    case 1000:
      return 45045;
    case 1105:
      return 43990;
    default:
      return -1;
    }
  case 2000:
    switch(m2){
    case 1:
      return 45724;
    case 500:
      return 46587;
    case 750:
      return 45926;
    case 1000:
      return 46032;
    case 1250:
      return 46671;
    case 1355:
      return 46599;
    default:
      return -1;
    }
  case 2250:
    switch(m2){
    case 1:
      return 48768;
    case 500:
      return 46335;
    case 750:
      return 45571;
    case 1000:
      return 48632;
    case 1250:
      return 48748;
    default:
      return -1;
    }
  case 2500:
    switch(m2){
    case 1:
      return 49949;
    case 500:
      return 50001;
    case 750:
      return 50109;
    case 1000:
      return 49997;
    case 1250:
      return 49962;
    default:
      return -1;
    }
  case 2750:
    switch(m2){
    case 1:
      return 51661;
    case 500:
      return 52393;
    case 750:
      return 53379;
    case 1000:
      return 49980;
    case 1250:
      return 52326;
    default:
      return -1;
    }
  case 3000:
    switch(m2){
    case 1:
      return 53056;
    case 500:
      return 51890;
    case 750:
      return 53938;
    case 1000:
      return 51358;
    case 1250:
      return 53387;
    default:
      return -1;
    }
  default:
    return -1;
  }
}

double WeightCalculator::GetT1tttt14TeVCrossSection(const int m1, const int m2){
  if(false && m2){}
  switch(m1){
  case 845:
    return 1.300e+00;
  case 1145:
    return 1.580e-01;
  case 1500:
    return 1.950e-02;
  case 1750:
    return 5.200e-03;
  case 2000:
    return 1.500e-03;
  case 2250:
    return 4.550e-04;
  case 2500:
    return 1.420e-04;
  case 2750:
    return 4.490e-05;
  case 3000:
    return 1.420e-05;
  default:
    return -1;
  }
}
