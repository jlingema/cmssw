#ifndef DQM_L1TMONITORCLIENT_L1TDEECALCLIENT_H
#define DQM_L1TMONITORCLIENT_L1TDEECALCLIENT_H

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include <FWCore/Framework/interface/EDAnalyzer.h>
#include "DQMServices/Core/interface/DQMStore.h"
#include "DQMServices/Core/interface/MonitorElement.h"

#include <memory>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <TH1F.h>
#include <TH2F.h>
#include <TProfile2D.h>

class L1TdeECALClient: public edm::EDAnalyzer {

public:

  /// Constructor
  L1TdeECALClient(const edm::ParameterSet& ps);
  
  /// Destructor
  virtual ~L1TdeECALClient();
 
protected:

  /// BeginJob
  void beginJob(void);

  /// BeginRun
  void beginRun(const edm::Run& r, const edm::EventSetup& c);

  /// Fake Analyze
  void analyze(const edm::Event& e, const edm::EventSetup& c) ;

  void beginLuminosityBlock(const edm::LuminosityBlock& lumiSeg, 
                            const edm::EventSetup& context) ;

  /// DQM Client Diagnostic
  void endLuminosityBlock(const edm::LuminosityBlock& lumiSeg, 
                          const edm::EventSetup& c);

  /// EndRun
  void endRun(const edm::Run& r, const edm::EventSetup& c);

  /// Endjob
  void endJob();

private:

  void initialize();
  TH1F * get1DHisto(std::string meName, DQMStore * dbi);
  TH2F * get2DHisto(std::string meName, DQMStore * dbi);
  TProfile2D * get2DProfile(std::string meName, DQMStore * dbi);
  TProfile * get1DProfile(std::string meName, DQMStore * dbi);
  edm::ParameterSet parameters_;

  DQMStore* dbe_;  
  std::string monitorDir_;
  bool verbose_;
  int counterLS_;      ///counter
  int counterEvt_;     ///counter
  int prescaleLS_;     ///units of lumi sections
  int prescaleEvt_;    ///prescale on number of events

  // -------- member data --------
  MonitorElement * ecalEtMapDiff1D_proj;
  MonitorElement * ecalEtMapDiff1D_proj_badChs;
  MonitorElement * ecalEtMapDiff_badChs;


};

#endif
