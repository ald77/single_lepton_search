#ifndef H_PU_CONSTANTS
#define H_PU_CONSTANTS

#include "TObject.h"

namespace pu {

  //See this twiki for more info:
  //  https://twiki.cern.ch/twiki/bin/viewauth/CMS/PileupMCReweightingUtilities

  /*
    this data distribution is an hadd of the following histograms:  
    (all in /afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions11/7TeV/PileUp/)

    Cert_160404-163869_7TeV_May10ReReco_Collisions11_JSON_v3.pileupTruth_v2.root
    Cert_165088-167913_7TeV_PromptReco_JSON.pileupTruth_v2.root
    Cert_170249-172619_7TeV_ReReco5Aug_Collisions11_JSON_v2.pileupTruth_v2.root
    Cert_172620-173692_PromptReco_JSON.pileupTruth_v2.root
    Cert_175832-177515_PromptReco_JSON.pileupTruth_v2.root
    Cert_177718_178078_7TeV_PromptReco_Collisons11_JSON.pileupTruth_v2.root
    Cert_178098-180252_7TeV_PromptReco_Collisions11_JSON.pileupTruth.root
  */

  const extern float TrueDist2011_f[35];

  /*
    this data distribution is an hadd of the following histograms:  
    (all in /afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions11/7TeV/PileUp/)

    Cert_160404-163869_7TeV_May10ReReco_Collisions11_JSON_v3.pileup_v2.root
    Cert_165088-167913_7TeV_PromptReco_JSON.pileup_v2.root
    Cert_170249-172619_7TeV_ReReco5Aug_Collisions11_JSON_v2.pileup_v2.root
    Cert_172620-173692_PromptReco_JSON.pileup_v2.root
    Cert_175832-177515_PromptReco_JSON.pileup_v2.root
    Cert_177718_178078_7TeV_PromptReco_Collisons11_JSON.pileup_v2.root
    Cert_178098-180252_7TeV_PromptReco_Collisions11_JSON.pileup_v2.root
  */

  const extern float ObsDist2011_f[35];


  // Flat10+Tail distribution taken directly from MixingModule input:  
  //(Can be used for Spring11 and Summer11 if you don't worry about small shifts in the mean) 
  //SHOULD be used for 3-D Reweighting, as this is the "true" input for all Summer11 samples.

  const extern Double_t probdistFlat10_f[35];


  // Summer11 PU_S4, distribution obtained by only looking at the in-time crossing.  This is the "spike+smear" distribution
  // RECOMMENDED FOR REWEIGHTING (if ignoring out-of-time PU)
  const extern float PoissonOneXDist_f[35];

  // from https://twiki.cern.ch/twiki/bin/view/CMS/Pileup_MC_Gen_Scenarios
  // allegedly correct for PU_S6 ?? https://twiki.cern.ch/twiki/bin/view/CMS/PdmVPileUpDescription#Profiles_in_Simulation
  const extern Double_t Fall2011[60];

  // Distribution used for Summer2012 MC.
  //from https://twiki.cern.ch/twiki/bin/viewauth/CMS/PileupMCReweightingUtilities
  const extern float Summer2012[60];

  //for S10 scenario, see here: https://twiki.cern.ch/twiki/bin/view/CMS/Pileup_MC_Gen_Scenarios
  const extern float Summer2012_S10[60];

  // $ pileupCalc.py -i /afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions12/8TeV/Prompt/Cert_190456-207469_8TeV_PromptReco_Collisions12_JSON.txt --inputLumiJSON  /afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions12/8TeV/PileUp/pileup_JSON_DCSONLY_190389-207372_corr.txt --calcMode true --minBiasXsec 73500 --maxPileupBin 60 --numPileupBins 60 MyDataPileupHistogramNov23.root
  const extern float RunsThrough207469[60];

  //systematic variation
  // $ pileupCalc.py -i /afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions12/8TeV/Prompt/Cert_190456-207469_8TeV_PromptReco_Collisions12_JSON.txt --inputLumiJSON /afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions12/8TeV/PileUp/pileup_JSON_DCSONLY_190389-207372_corr.txt --calcMode true --minBiasXsec 69400 --maxPileupBin 60 --numPileupBins 60 MyDataPileupHistogramNov23syst.root
  const extern float RunsThrough207469systVar[60];

  //https://twiki.cern.ch/twiki/bin/view/CMS/PileupJSONFileforData
  // command run for all pre technical stop data (ku):
  // pileupCalc.py -i /afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions12/8TeV/Prompt/Cert_190456-203002_8TeV_PromptReco_Collisions12_JSON.txt --inputLumiJSON /afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions12/8TeV/PileUp/pileup_JSON_DCSONLY_190389-203002_corr.txt --calcMode true --minBiasXsec 73500 --maxPileupBin 60 --numPileupBins 60 MyDataPileupHistogram.root

  const extern float RunsThrough203002[60];                   

  // consider lower min bias cross section for systematic variation: https://twiki.cern.ch/twiki/bin/view/CMS/PileupJSONFileforData
  // pileupCalc.py -i /afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions12/8TeV/Prompt/Cert_190456-203002_8TeV_PromptReco_Collisions12_JSON.txt --inputLumiJSON /afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions12/8TeV/PileUp/pileup_JSON_DCSONLY_190389-203002_corr.txt --calcMode true --minBiasXsec 69400 --maxPileupBin 60 --numPileupBins 60 MyDataPileupHistogram.root

  const extern float RunsThrough203002systVar[60];

  // Note that this systematic variation version of the pileup distribution was created later than the nominal version above.
  // When I try recreating the nominal now, I get different results, which I commented out below. (10-30-12, ku)
  /*
    const extern float RunsThrough203002[60];
  */


  //very crude first attempt...just trying to get something
  //at command line:
  /*
    pileupCalc.py -i GoldenJSON.txt --inputLumiJSON /afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions12/8TeV/PileUp/pileup_JSON_DCSONLY_190389-202478_corr.txt --calcMode true --minBiasXsec 69400 --maxPileupBin 60 --numPileupBins 60 MyDataPileupHistogram.root

    note that the json files don't exactly correspond
    Then in ROOT:
    for (int i=1; i<=60; i++) cout<<pileup->GetBinContent(i)<<","<<endl;
    Then copy/paste here


  */

  const extern float RunsThrough202016[60];

  const extern float RunsThrough199703[60];

  // /afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions12/8TeV/PileUp/

  ///////////////////////////////////////////////////
  //The below PU distributions were used for the Summer 2011 result
  ///////////////////////////////////////////////////

  ////the data histogram obtained from: 
  //// /afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions11/7TeV/PileUp/Pileup_2011_EPS_8_jul.root
  //const extern float TrueDist2011_f[25];

  ////Summer11 PU_S4 distribution
  ////obtained from https://twiki.cern.ch/twiki/bin/viewauth/CMS/PileupMCReweightingUtilities
  //const extern Double_t PoissonIntDist_f[25];

}
#endif
