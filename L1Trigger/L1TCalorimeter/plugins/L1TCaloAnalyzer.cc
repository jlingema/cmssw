// -*- C++ -*-
//
// Package:    L1Trigger/L1TCalorimeter
// Class:      L1TCaloAnalyzer
// 
/**\class L1TCaloAnalyzer L1TCaloAnalyzer.cc L1Trigger/L1TCalorimeter/plugins/L1TCaloAnalyzer.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  James Brooke
//         Created:  Tue, 11 Mar 2014 14:55:45 GMT
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "CondFormats/L1TObjects/interface/CaloParams.h"
#include "CondFormats/DataRecord/interface/L1TCaloParamsRcd.h"

#include "DataFormats/L1TCalorimeter/interface/CaloTower.h"
#include "DataFormats/L1TCalorimeter/interface/CaloCluster.h"
#include "DataFormats/L1Trigger/interface/EGamma.h"
#include "DataFormats/L1Trigger/interface/Tau.h"
#include "DataFormats/L1Trigger/interface/Jet.h"
#include "DataFormats/L1Trigger/interface/EtSum.h"
#include "DataFormats/L1Trigger/interface/L1EtMissParticleFwd.h"
#include "DataFormats/L1Trigger/interface/L1EtMissParticle.h"
#include "DataFormats/L1Trigger/interface/L1JetParticleFwd.h"
#include "DataFormats/L1Trigger/interface/L1JetParticle.h"
#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/METReco/interface/GenMET.h"

#include "TString.h"
#include "TH2F.h"
#include "TGraphAsymmErrors.h"

//
// class declaration
//

class L1TCaloAnalyzer : public edm::EDAnalyzer {
public:
  explicit L1TCaloAnalyzer(const edm::ParameterSet&);
  ~L1TCaloAnalyzer();
  
  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);
  
  
private:
  virtual void beginJob() override;
  virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
  virtual void endJob() override;
  
  //virtual void beginRun(edm::Run const&, edm::EventSetup const&) override;
  //virtual void endRun(edm::Run const&, edm::EventSetup const&) override;
  //virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;
  //virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;
  
  // ----------member data ---------------------------
  edm::EDGetToken m_towerToken;
  edm::EDGetToken m_clusterToken;
  edm::EDGetToken m_egToken;
  edm::EDGetToken m_tauToken;
  edm::EDGetToken m_jetToken;
  edm::EDGetToken m_sumToken;
  
  enum ObjectType{Tower=0x1,
		  Cluster=0x2,
		  EG=0x3,
		  Tau=0x4,
		  Jet=0x5,
		  Sum=0x6};
  
  std::vector< ObjectType > types_;
  std::vector< std::string > typeStr_;
  
  std::map< ObjectType, TFileDirectory > dirs_;
  std::map< ObjectType, TH1F* > het_;
  std::map< ObjectType, TH1F* > het2_;
  std::map< ObjectType, TH1F* > heta_;
  std::map< ObjectType, TH1F* > hphi_;
  std::map< ObjectType, TH1F* > hem_;
  std::map< ObjectType, TH1F* > hhad_;
  std::map< ObjectType, TH1F* > hratio_;

  //My own histograms
  std::map< TString, std::vector<double> > bins_;
  std::map< TString, std::vector<double> > binsESum_;
  std::map< TString, TFileDirectory > jetDirs_;
  std::map< TString, TH1F* > hjets_;//Histograms
  std::vector< TString > vars_;//Variables to plot
  std::vector< TString > categories_;//Categories of jets
  std::vector< TString > eSumCategories_;//Categories of e sums 

  //2D Comparison histograms
  std::map< TString, TH2F* > h2d_;

  //Turn on curves
  std::map< TString, TGraphAsymmErrors > gTurnon_;

  //Resolution histograms
  std::map< TString, TH1F* > hResolution_;

  std::vector< TString > varLevel_;//The type of variable, eg gen or l1
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//My own function
//
const double PI = acos(-1.0);
int PhitoiPhi(double phiMissingEt){
  int intPhiMissingEt;
  if(phiMissingEt > 0)
  {
    intPhiMissingEt = int32_t((36.*(phiMissingEt)+0.5)/PI);
  }
  else
  {
    intPhiMissingEt = int32_t((36.*(phiMissingEt+2.*PI)+0.5)/PI);
  }
  return intPhiMissingEt;
}

double iPhitoPhi(int iPhi){
  if(iPhi<37) return (5.0*iPhi-2.5)*PI/180.;
  else return (5.0*(iPhi-72)-2.5)*PI/180.; 
}

double iEtatoEta(int iEta){
  double etaMapping[57]={-3.0,-2.65,-2.5,-2.322,-2.172,-2.043,-1.93,-1.83,-1.74,-1.653,
                        -1.566,-1.4790,-1.3920,-1.3050,-1.2180,-1.1310,-1.0440,-0.9570,
                        -0.8700,-0.7830,-0.6950,-0.6090,-0.5220,-0.4350,-0.3480,-0.2610,
                        -0.1740,-0.0870,0.0,0.0870,0.1740,0.2610,0.3480,0.4350,0.5220,0.6090,
                        0.6950,0.7830,0.8700,0.9570,1.0440,1.1310,1.2180,1.3050,1.3920,
                        1.4790,1.566,1.653,1.74,1.83,1.93,2.043,2.172,2.322,2.5,2.65,3.0};
  return etaMapping[iEta+28];
}


//
// constructors and destructor
//
L1TCaloAnalyzer::L1TCaloAnalyzer(const edm::ParameterSet& iConfig)
{
  //now do what ever initialization is needed

  // register what you consume and keep token for later access:
  m_towerToken   = consumes<l1t::CaloTowerBxCollection>  (iConfig.getParameter<edm::InputTag>("towerToken"));
  m_clusterToken = consumes<l1t::CaloClusterBxCollection>(iConfig.getParameter<edm::InputTag>("clusterToken"));
  m_egToken      = consumes<l1t::EGammaBxCollection>     (iConfig.getParameter<edm::InputTag>("egToken"));
  m_tauToken     = consumes<l1t::TauBxCollection>        (iConfig.getParameter<edm::InputTag>("tauToken"));
  m_jetToken     = consumes<l1t::JetBxCollection>        (iConfig.getParameter<edm::InputTag>("jetToken"));
  m_sumToken     = consumes<l1t::EtSumBxCollection>      (iConfig.getParameter<edm::InputTag>("etSumToken"));

  types_.push_back( Tower );
  types_.push_back( Cluster );
  types_.push_back( EG );
  types_.push_back( Tau );
  types_.push_back( Jet );
  types_.push_back( Sum );

  typeStr_.push_back( "tower" );
  typeStr_.push_back( "cluster" );
  typeStr_.push_back( "eg" );
  typeStr_.push_back( "tau" );
  typeStr_.push_back( "jet" );
  typeStr_.push_back( "sum" );

  vars_.push_back( "et" );
  bins_["et"].push_back(2000.);
  bins_["et"].push_back(-0.5);
  bins_["et"].push_back(1999.5);
  vars_.push_back( "eta" );
  bins_["eta"].push_back(70.);
  bins_["eta"].push_back(-35.);
  bins_["eta"].push_back(35.);
  vars_.push_back( "phi" );
  bins_["phi"].push_back(72.);
  bins_["phi"].push_back(0.);
  bins_["phi"].push_back(72.);
  bins_["real_phi"].push_back(72.);
  bins_["real_phi"].push_back(-3.15);
  bins_["real_phi"].push_back(3.15);
  bins_["real_eta"].push_back(70.);
  bins_["real_eta"].push_back(-3.3);
  bins_["real_eta"].push_back(3.3);
  bins_["real_et"].push_back(2000.);
  bins_["real_et"].push_back(-0.5);
  bins_["real_et"].push_back(1999.5);

  categories_.push_back( "lead_jet");
  categories_.push_back( "second_jet");
  categories_.push_back( "third_jet");
  categories_.push_back( "remaining_jets");

  varLevel_.push_back( "gen" );
  varLevel_.push_back( "l1_stage1" );

  eSumCategories_.push_back( "et" );
  binsESum_["et"].push_back(2000.);
  binsESum_["et"].push_back(-0.5);
  binsESum_["et"].push_back(1999.5);
  eSumCategories_.push_back( "met" );
  binsESum_["met"].push_back(2000.);
  binsESum_["met"].push_back(-0.5);
  binsESum_["met"].push_back(1999.5);
  eSumCategories_.push_back( "met_phi" );
  binsESum_["met_phi"].push_back(72.);
  binsESum_["met_phi"].push_back(-3.15);
  binsESum_["met_phi"].push_back(3.15);
  
  eSumCategories_.push_back( "ht" );
  binsESum_["ht"].push_back(2000.);
  binsESum_["ht"].push_back(-0.5);
  binsESum_["ht"].push_back(1999.5);
  eSumCategories_.push_back( "mht" );
  binsESum_["mht"].push_back(2000.);
  binsESum_["mht"].push_back(-0.5);
  binsESum_["mht"].push_back(1999.5);
  eSumCategories_.push_back( "mht_phi" );
  binsESum_["mht_phi"].push_back(72.);
  binsESum_["mht_phi"].push_back(-3.15);
  binsESum_["mht_phi"].push_back(3.15);


}


L1TCaloAnalyzer::~L1TCaloAnalyzer()
{

  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
  void
L1TCaloAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace edm;

  // only does anything for BX=0 right now

  // get TPs ?
  // get regions ?
  // get RCT clusters ?


  // get towers
  Handle< BXVector<l1t::CaloTower> > towers;
  iEvent.getByToken(m_towerToken,towers);

  for ( auto itr = towers->begin(0); itr != towers->end(0); ++itr ) {
    het_.at(Tower)->Fill( itr->hwPt() );
    heta_.at(Tower)->Fill( itr->hwEta() );
    hphi_.at(Tower)->Fill( itr->hwPhi() );
    hem_.at(Tower)->Fill( itr->hwEtEm() );
    hhad_.at(Tower)->Fill( itr->hwEtHad() );
    hratio_.at(Tower)->Fill( itr->hwEtRatio() );
  }

  // get cluster
  Handle< BXVector<l1t::CaloCluster> > clusters;
  iEvent.getByToken(m_clusterToken,clusters);

  for ( auto itr = clusters->begin(0); itr !=clusters->end(0); ++itr ) {
    het_.at(Cluster)->Fill( itr->hwPt() );
    heta_.at(Cluster)->Fill( itr->hwEta() );
    hphi_.at(Cluster)->Fill( itr->hwPhi() );
  }

  // get EG
  Handle< BXVector<l1t::EGamma> > egs;
  iEvent.getByToken(m_egToken,egs);

  for ( auto itr = egs->begin(0); itr != egs->end(0); ++itr ) {
    het_.at(EG)->Fill( itr->hwPt() );
    heta_.at(EG)->Fill( itr->hwEta() );
    hphi_.at(EG)->Fill( itr->hwPhi() );
  }

  // get tau
  Handle< BXVector<l1t::Tau> > taus;
  iEvent.getByToken(m_tauToken,taus);

  for ( auto itr = taus->begin(0); itr != taus->end(0); ++itr ) {
    het_.at(Tau)->Fill( itr->hwPt() );
    heta_.at(Tau)->Fill( itr->hwEta() );
    hphi_.at(Tau)->Fill( itr->hwPhi() );
  }

  // get jet
  Handle< BXVector<l1t::Jet> > jets;
  iEvent.getByToken(m_jetToken,jets);

  //l1t::Jet* leadJet=NULL;
  //l1t::Jet* secondJet=NULL;
  //l1t::Jet* thirdJet=NULL;

  //Get the l1 extra collection

  Handle< std::vector<l1extra::L1EtMissParticle> > l1S1EtMiss;
  iEvent.getByLabel("l1extraParticles","MET",l1S1EtMiss);

  Handle< std::vector<l1extra::L1EtMissParticle> > l1S1HtMiss;
  iEvent.getByLabel("l1extraParticles","MHT",l1S1HtMiss);

  //Get the l1 jets in the central region
  Handle< std::vector<l1extra::L1JetParticle> > l1GctJet;
  iEvent.getByLabel("l1extraParticles","Central", l1GctJet);

  //Get the gen jets
  Handle< std::vector<reco::GenJet> > genJet;
  iEvent.getByLabel("ak4GenJets", genJet);

  Handle< std::vector<reco::GenMET> > genMet;
  iEvent.getByLabel("genMetCalo", genMet);

  // get l1 sums
  Handle< BXVector<l1t::EtSum> > sums;
  iEvent.getByToken(m_sumToken,sums);


  //Make the histograms
  //
  //For the energy sums
  for(auto itr = sums->begin(0); itr != sums->end(0); itr++){

    if(itr->getType() == l1t::EtSum::EtSumType::kTotalEt && l1S1EtMiss->size()>0){
      het_.at(Sum)->Fill( itr->hwPt() );
      h2d_["et_l1_stage1"]->Fill( l1S1EtMiss->at(0).etTotal(), 0.5*itr->hwPt() );
      h2d_["et_gen"]->Fill( genMet->at(0).sumEt(), 0.5*itr->hwPt() );
    }
    if(itr->getType() == l1t::EtSum::EtSumType::kMissingEt && l1S1EtMiss->size()>0 ){
      het2_.at(Sum)->Fill( itr->hwPt() );
      h2d_["met_l1_stage1"]->Fill( l1S1EtMiss->at(0).etMiss(), (1.0/511.0)*itr->hwPt() );
      h2d_["met_gen"]->Fill( genMet->at(0).et(), (1.0/511.0)*itr->hwPt() );
      heta_.at(Sum)->Fill( itr->hwEta() );
      hphi_.at(Sum)->Fill( itr->hwPhi() );
      h2d_["met_phi_l1_stage1"]->Fill( l1S1EtMiss->at(0).phi(), iPhitoPhi(itr->hwPhi()) );
      h2d_["met_phi_gen"]->Fill( genMet->at(0).phi(), iPhitoPhi(itr->hwPhi()) );
    }
    if(itr->getType() == l1t::EtSum::EtSumType::kTotalHt && l1S1HtMiss->size()>0 ){
      het_.at(Sum)->Fill( itr->hwPt() );
      h2d_["ht_l1_stage1"]->Fill( l1S1HtMiss->at(0).etTotal(), 0.5*itr->hwPt() );
      //h2d_["ht_gen"]->Fill( genMHt->at(0).sumEt(), 0.5*itr->hwPt() );
    }
    if(itr->getType() == l1t::EtSum::EtSumType::kMissingHt && l1S1HtMiss->size()>0 ){
      het2_.at(Sum)->Fill( itr->hwPt() );
      h2d_["mht_l1_stage1"]->Fill( l1S1HtMiss->at(0).etMiss(), (1.0/511.0)*itr->hwPt() );
      //h2d_["mht_gen"]->Fill( genMht->at(0).et(), (1.0/511.0)*itr->hwPt() );
      heta_.at(Sum)->Fill( itr->hwEta() );
      hphi_.at(Sum)->Fill( itr->hwPhi() );
      h2d_["mht_phi_l1_stage1"]->Fill( l1S1HtMiss->at(0).phi(), iPhitoPhi(itr->hwPhi()) );
      //h2d_["mht_phi_gen"]->Fill( genMht->at(0).phi(), iPhitoPhi(itr->hwPhi()) );
    }


  }

  // Categorise the jets and make histograms
  double leadEt=0.0, secondEt=0.0, thirdEt=0.0;
  double leadEta=0.0, secondEta=0.0, thirdEta=0.0;
  double leadPhi=0.0, secondPhi=0.0, thirdPhi=0.0;

  for ( auto itr = jets->begin(0); itr != jets->end(0); ++itr ) {
    het_.at(Jet)->Fill( itr->hwPt() );
    heta_.at(Jet)->Fill( itr->hwEta() );
    hphi_.at(Jet)->Fill( itr->hwPhi() );

    //Make sure just use central jets for comparison
    if(abs(itr->hwEta()) > 28) continue;

    if(itr->hwPt() > leadEt){
      //If there was a third jet, there are remaining jets
      if(thirdEt > 0.01){
        hjets_["remaining_jets_et"]->Fill( thirdEt );
        hjets_["remaining_jets_eta"]->Fill( thirdEta );
        hjets_["remaining_jets_phi"]->Fill( thirdPhi );
      }

      //Find the leading energy and prop through
      thirdEt=secondEt;
      thirdEta=secondEta;
      thirdPhi=secondPhi;
      secondEt=leadEt;
      secondEta=leadEta;
      secondPhi=leadPhi;
      leadEt=itr->hwPt();
      leadEta=itr->hwEta();
      leadPhi=itr->hwPhi();
    }
    else if(itr->hwPt() > secondEt){
      //If there was a third jet, there are remaining jets
      if(thirdEt > 0.01){
        hjets_["remaining_jets_et"]->Fill( thirdEt );
        hjets_["remaining_jets_eta"]->Fill( thirdEta );
        hjets_["remaining_jets_phi"]->Fill( thirdPhi );
      }
      thirdEt=secondEt;
      thirdEta=secondEta;
      thirdPhi=secondPhi;
      secondEt=itr->hwPt();
      secondEta=itr->hwEta();
      secondPhi=itr->hwPhi();
    }
    else if(itr->hwPt() > thirdEt){
      //If there was a third jet, there are remaining jets
      if(thirdEt>0.01){
        hjets_["remaining_jets_et"]->Fill( thirdEt );
        hjets_["remaining_jets_eta"]->Fill( thirdEta );
        hjets_["remaining_jets_phi"]->Fill( thirdPhi );
      }
      thirdEt=itr->hwPt();
      thirdEta=itr->hwEta();
      thirdPhi=itr->hwPhi();
    }

  }

  //Fill the jet histograms
  if(leadEt>0.01){
    hjets_["lead_jet_et"]->Fill( leadEt );
    hjets_["lead_jet_eta"]->Fill( leadEta );
    hjets_["lead_jet_phi"]->Fill( leadPhi );
    if(l1GctJet->size()>0){
      h2d_["lead_jet_l1_stage1_et"]->Fill( l1GctJet->at(0).pt(),0.5*leadEt );
      h2d_["lead_jet_l1_stage1_eta"]->Fill( l1GctJet->at(0).eta(), iEtatoEta(leadEta) );
      h2d_["lead_jet_l1_stage1_phi"]->Fill( l1GctJet->at(0).phi(), iPhitoPhi(leadPhi) );
    }
    if(genJet->size()>0){
      h2d_["lead_jet_gen_et"]->Fill( genJet->at(0).pt(),0.5*leadEt );
      h2d_["lead_jet_gen_eta"]->Fill( genJet->at(0).eta(), iEtatoEta(leadEta) );
      h2d_["lead_jet_gen_phi"]->Fill( genJet->at(0).phi(), iPhitoPhi(leadPhi) );
    }
  }
  if(secondEt>0.01){
    hjets_["second_jet_et"]->Fill( secondEt );
    hjets_["second_jet_eta"]->Fill( secondEta );
    hjets_["second_jet_phi"]->Fill( secondPhi );
    if(l1GctJet->size()>1){
      h2d_["second_jet_l1_stage1_et"]->Fill( l1GctJet->at(1).pt(),0.5*secondEt );
      h2d_["second_jet_l1_stage1_eta"]->Fill( l1GctJet->at(1).eta(), iEtatoEta(secondEta) );
      h2d_["second_jet_l1_stage1_phi"]->Fill( l1GctJet->at(1).phi(), iPhitoPhi(secondPhi) );
    }
    if(genJet->size()>1){
      h2d_["second_jet_gen_eta"]->Fill( genJet->at(1).eta(), iEtatoEta(secondEta) );
      h2d_["second_jet_gen_et"]->Fill( genJet->at(1).pt(),0.5*secondEt );
      h2d_["second_jet_gen_phi"]->Fill( genJet->at(1).phi(), iPhitoPhi(secondPhi) );
    }
  }
  if(thirdEt>0.01){
    hjets_["third_jet_et"]->Fill( thirdEt );
    hjets_["third_jet_eta"]->Fill( thirdEta );
    hjets_["third_jet_phi"]->Fill( thirdPhi );
    if(l1GctJet->size()>2){
      h2d_["third_jet_l1_stage1_et"]->Fill( l1GctJet->at(2).pt(),0.5*thirdEt );
      h2d_["third_jet_l1_stage1_eta"]->Fill( l1GctJet->at(2).eta(), iEtatoEta(thirdEta) );
      h2d_["third_jet_l1_stage1_phi"]->Fill( l1GctJet->at(2).phi(), iPhitoPhi(thirdPhi) );
    }
    if(genJet->size()>2){
      h2d_["third_jet_gen_eta"]->Fill( genJet->at(2).eta(), iEtatoEta(thirdEta) );
      h2d_["third_jet_gen_et"]->Fill( genJet->at(2).pt(),0.5*thirdEt );
      h2d_["third_jet_gen_phi"]->Fill( genJet->at(2).phi(), iPhitoPhi(thirdPhi) );
    }
  }

}


// ------------ method called once each job just before starting event loop  ------------
  void 
L1TCaloAnalyzer::beginJob()
{

  edm::Service<TFileService> fs;

  auto itr = types_.cbegin();
  auto str = typeStr_.cbegin();

  for (; itr!=types_.end(); ++itr, ++str ) {

    dirs_.insert( std::pair< ObjectType, TFileDirectory >(*itr, fs->mkdir(*str) ) );

    het_.insert( std::pair< ObjectType, TH1F* >(*itr, dirs_.at(*itr).make<TH1F>("et", "", 1000, 0., 2000.) ));
    het2_.insert( std::pair< ObjectType, TH1F* >(*itr, dirs_.at(*itr).make<TH1F>("et2", "", 5000, -10., 99990.) ));
    heta_.insert( std::pair< ObjectType, TH1F* >(*itr, dirs_.at(*itr).make<TH1F>("eta", "", 70, -35., 35.) ));
    hphi_.insert( std::pair< ObjectType, TH1F* >(*itr, dirs_.at(*itr).make<TH1F>("phi", "", 72, 0., 72.) ));

    if (*itr==Tower) {
      hem_.insert( std::pair< ObjectType, TH1F* >(*itr, dirs_.at(*itr).make<TH1F>("em", "", 50, 0., 100.) ));
      hhad_.insert( std::pair< ObjectType, TH1F* >(*itr, dirs_.at(*itr).make<TH1F>("had", "", 50, 0., 100.) ));
      hratio_.insert( std::pair< ObjectType, TH1F* >(*itr, dirs_.at(*itr).make<TH1F>("ratio", "", 10, 0., 10.) ));
    }

  }

  //Add histograms exclusively for jets, based on category and vars

  for(auto catIt = categories_.cbegin(); catIt!= categories_.end(); ++catIt){

    jetDirs_[*catIt] = fs->mkdir(catIt->Data());

    for(auto varIt = vars_.cbegin(); varIt!=vars_.end(); ++varIt){
      hjets_[*catIt+"_"+*varIt] = jetDirs_.at(*catIt).make<TH1F>(*varIt, *catIt+"_"+*varIt, 
          (int)bins_.at(*varIt)[0],bins_.at(*varIt)[1],bins_.at(*varIt)[2]);
    }

  }

  std::cout << "1\n";
  //Add 2d histograms for comparison for the jets, based on var and var level


  for(auto catIt = categories_.cbegin(); catIt!= categories_.end(); ++catIt){
    for(auto lvlIt = varLevel_.cbegin(); lvlIt!= varLevel_.end(); ++lvlIt){
      for(auto varIt = vars_.cbegin(); varIt!=vars_.end(); ++varIt){

        h2d_[*catIt+"_"+*lvlIt+"_"+*varIt] = jetDirs_.at(*catIt).make<TH2F>(*varIt+"_"+*lvlIt, *catIt+"_"+*lvlIt+"_"+*varIt, 
            (int)bins_.at("real_"+*varIt)[0],bins_.at("real_"+*varIt)[1],bins_.at("real_"+*varIt)[2],
            (int)bins_.at("real_"+*varIt)[0],bins_.at("real_"+*varIt)[1],bins_.at("real_"+*varIt)[2]);

      }
    }
  }

  std::cout << "2\n";
  //Add 2d histograms for the energy sums

  jetDirs_["eSums"] = fs->mkdir("eSums");
  for(auto lvlIt = eSumCategories_.cbegin(); lvlIt!= eSumCategories_.end(); ++lvlIt){

    for(auto varIt = varLevel_.cbegin(); varIt!=varLevel_.end(); ++varIt){
      h2d_[*lvlIt+"_"+*varIt] = jetDirs_["eSums"].make<TH2F>(*lvlIt+"_"+*varIt, *lvlIt+"_"+*varIt, 
          (int)binsESum_.at(*lvlIt)[0],binsESum_.at(*lvlIt)[1],binsESum_.at(*lvlIt)[2],
          (int)binsESum_.at(*lvlIt)[0],binsESum_.at(*lvlIt)[1],binsESum_.at(*lvlIt)[2]);
    }

  }

  std::cout << "3\n";

  //Add turnon curves for the et of the jets


  //Add turnon curves for the met and et

}

// ------------ method called once each job just after ending the event loop  ------------
  void 
L1TCaloAnalyzer::endJob() 
{
}

// ------------ method called when starting to processes a run  ------------
/*
   void 
   L1TCaloAnalyzer::beginRun(edm::Run const&, edm::EventSetup const&)
   {
   }
   */

// ------------ method called when ending the processing of a run  ------------
/*
   void 
   L1TCaloAnalyzer::endRun(edm::Run const&, edm::EventSetup const&)
   {
   }
   */

// ------------ method called when starting to processes a luminosity block  ------------
/*
   void 
   L1TCaloAnalyzer::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
   {
   }
   */

// ------------ method called when ending the processing of a luminosity block  ------------
/*
   void 
   L1TCaloAnalyzer::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
   {
   }
   */

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
L1TCaloAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(L1TCaloAnalyzer);
