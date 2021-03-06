#ifndef ERRORSANALYZER_HH
#define ERRORSANALYZER_HH

// -*- C++ -*-
//
// Package:    ErrorsPropagationAnalyzer
// Class:      ErrorsPropagationAnalyzer
// 
/**\class ErrorsPropagationAnalyzer ErrorsPropagationAnalyzer.cc MuonAnalysis/MomentumScaleCalibration/plugins/ErrorsPropagationAnalyzer.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Marco De Mattia
//         Created:  Thu Sep 11 12:16:00 CEST 2008
// $Id: ErrorsPropagationAnalyzer.h,v 1.1 2010/07/13 11:02:16 demattia Exp $
//
//

// system include files
#include <memory>
#include <string>
#include <vector>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include <TH1D.h>
#include <TProfile.h>
#include <TString.h>
#include <TCanvas.h>
#include <TGraphAsymmErrors.h>
#include <TROOT.h>

#include "MuonAnalysis/MomentumScaleCalibration/interface/Functions.h"
#include "MuonAnalysis/MomentumScaleCalibration/interface/RootTreeHandler.h"
#include "MuonAnalysis/MomentumScaleCalibration/interface/MuScleFitUtils.h"
#include "MuonAnalysis/MomentumScaleCalibration/interface/SigmaPtDiff.h"

//
// class declaration
//

class ErrorsPropagationAnalyzer : public edm::EDAnalyzer
{
public:
  explicit ErrorsPropagationAnalyzer(const edm::ParameterSet&);
  ~ErrorsPropagationAnalyzer();

private:
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  void fillHistograms();
  void drawHistograms(const TProfile * histo, const TProfile * histoPlusErr, const TProfile * histoMinusErr, const TString & type);
  void fillValueError();
  virtual void endJob() {};

  TString treeFileName_;
  int resolFitType_;
  uint32_t maxEvents_;
  TString outputFileName_;
  int ptBins_;
  double ptMin_;
  double ptMax_;
  int etaBins_;
  double etaMin_;
  double etaMax_;
  bool debug_;

  double ptMinCut_, ptMaxCut_, etaMinCut_, etaMaxCut_;

  std::vector<double> parameters_;
  std::vector<double> errors_;
  std::vector<int> errorFactors_;

  std::vector<double> valuePlusError_;
  std::vector<double> valueMinusError_;

  TProfile * sigmaPtVsEta_;
  TProfile * sigmaPtVsEtaPlusErr_;
  TProfile * sigmaPtVsEtaMinusErr_;

  TProfile * sigmaPtVsPt_;
  TProfile * sigmaPtVsPtPlusErr_;
  TProfile * sigmaPtVsPtMinusErr_;

  TProfile * sigmaPtVsEtaDiff_;
  TProfile * sigmaPtVsPtDiff_;

  // Mass resolution
  TProfile * sigmaMassVsEta_;
  TProfile * sigmaMassVsEtaPlusErr_;
  TProfile * sigmaMassVsEtaMinusErr_;

  TProfile * sigmaMassVsPt_;
  TProfile * sigmaMassVsPtPlusErr_;
  TProfile * sigmaMassVsPtMinusErr_;
};

#endif // RESOLUTIONANALYZER_HH
