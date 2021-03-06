

/*
 *  See header file for a description of this class.
 *
 *  $Date: 2010/01/05 10:15:46 $
 *  $Revision: 1.20 $
 *  \author G. Mila - INFN Torino
 */


#include <DQM/DTMonitorClient/src/DTResolutionAnalysisTest.h>

// Framework
#include <FWCore/Framework/interface/Event.h>
#include <FWCore/Framework/interface/EventSetup.h>
#include <FWCore/ParameterSet/interface/ParameterSet.h>


// Geometry
#include "Geometry/Records/interface/MuonGeometryRecord.h"
#include "Geometry/DTGeometry/interface/DTGeometry.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "DQMServices/Core/interface/DQMStore.h"
#include "DQMServices/Core/interface/MonitorElement.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include <string>
#include <sstream>
#include <math.h>


using namespace edm;
using namespace std;


DTResolutionAnalysisTest::DTResolutionAnalysisTest(const ParameterSet& ps){

  LogTrace ("DTDQM|DTMonitorClient|DTResolutionAnalysisTest") << "[DTResolutionAnalysisTest]: Constructor";

  dbe = Service<DQMStore>().operator->();

  prescaleFactor = ps.getUntrackedParameter<int>("diagnosticPrescale", 1);
  // permitted test range
  permittedMeanRange = ps.getUntrackedParameter<double>("permittedMeanRange",0.005); 
  permittedSigmaRange = ps.getUntrackedParameter<double>("permittedSigmaRange",0.01); 
  // top folder for the histograms in DQMStore
  topHistoFolder = ps.getUntrackedParameter<string>("topHistoFolder","DT/02-Segments");

  doCalibAnalysis = ps.getUntrackedParameter<bool>("doCalibAnalysis",false);
}


DTResolutionAnalysisTest::~DTResolutionAnalysisTest(){
  LogTrace ("DTDQM|DTMonitorClient|DTResolutionAnalysisTest") << "DTResolutionAnalysisTest: analyzed " << nevents << " events";

}


void DTResolutionAnalysisTest::beginJob(){

  LogTrace ("DTDQM|DTMonitorClient|DTResolutionAnalysisTest") <<"[DTResolutionAnalysisTest]: BeginJob"; 

  nevents = 0;

  // global residual summary
  dbe->setCurrentFolder(topHistoFolder);
  globalResSummary = dbe->book2D("ResidualsGlbSummary", "Summary residuals",12,1,13,5,-2,3);



  // book summaries for mean and sigma
  dbe->setCurrentFolder(topHistoFolder + "/00-MeanRes");
  meanDistr[-2] = dbe->book1D("MeanDistr","Mean value of the residuals all (cm)",
			      100,-0.1,0.1);
  meanDistr[-1] = dbe->book1D("MeanDistr_Phi","Mean value of the residuals #phi SL (cm)",
			      100,-0.1,0.1);
  meanDistr[0] = dbe->book1D("MeanDistr_ThetaWh0","Mean values of the residuals #theta SL Wh 0 (cm)",
			      100,-0.1,0.1);
  meanDistr[1] = dbe->book1D("MeanDistr_ThetaWh1","Mean value of the residuals #theta SL Wh +/-1 (cm)",
			      100,-0.1,0.1);
  meanDistr[2] = dbe->book1D("MeanDistr_ThetaWh2","Mean value of the residuals #theta SL Wh +/-2 (cm)",
			      100,-0.1,0.1);


  stringstream meanRange; meanRange << (permittedMeanRange*10000);
  string histoTitle = "Fraction of SLs with |mean of res.| > " + meanRange.str() + "#mum";
  wheelMeanHistos[3] = dbe->book2D("MeanResGlbSummary",histoTitle.c_str(),12,1,13,5,-2,3);
  wheelMeanHistos[3]->setAxisTitle("Sector",1);
  wheelMeanHistos[3]->setAxisTitle("Wheel",2);


  dbe->setCurrentFolder(topHistoFolder + "/01-SigmaRes");
  sigmaDistr[-2] = dbe->book1D("SigmaDistr","Sigma value of the residuals all (cm)",
			      50,0.0,0.2);
  sigmaDistr[-1] = dbe->book1D("SigmaDistr_Phi","Sigma value of the residuals #phi SL (cm)",
			      50,0.0,0.2);
  sigmaDistr[0] = dbe->book1D("SigmaDistr_ThetaWh0","Sigma value of the residuals #theta SL Wh 0 (cm)",
			      50,0.0,0.2);
  sigmaDistr[1] = dbe->book1D("SigmaDistr_ThetaWh1","Sigma value of the residuals #theta SL Wh +/-1 (cm)",
			      50,0.0,0.2);
  sigmaDistr[2] = dbe->book1D("SigmaDistr_ThetaWh2","Sigma value of the residuals #theta SL Wh +/-2 (cm)",
			      50,0.0,0.2);

  stringstream sigmaRange; sigmaRange << (permittedSigmaRange*10000);
  histoTitle = "Fraction of SLs with #sigma res. > " + sigmaRange.str() + "#mum";
  wheelSigmaHistos[3] = dbe->book2D("SigmaResGlbSummary",histoTitle.c_str(),12,1,13,5,-2,3);
  wheelSigmaHistos[3]->setAxisTitle("Sector",1);
  wheelSigmaHistos[3]->setAxisTitle("Wheel",2);


  // loop over all the CMS wheels, sectors & book the summary histos
  for (int wheel=-2; wheel<=2; wheel++){
    bookHistos(wheel);
    for (int sector=1; sector<=12; sector++){
       bookHistos(wheel, sector);
    }
  }

}

void DTResolutionAnalysisTest::beginRun(const Run& run, const EventSetup& context){

  LogTrace ("DTDQM|DTMonitorClient|DTResolutionAnalysisTest") <<"[DTResolutionAnalysisTest]: BeginRun"; 

  // Get the geometry
  context.get<MuonGeometryRecord>().get(muonGeom);

}


void DTResolutionAnalysisTest::beginLuminosityBlock(LuminosityBlock const& lumiSeg, EventSetup const& context) {

  LogTrace ("DTDQM|DTMonitorClient|DTResolutionAnalysisTest") <<"[DTResolutionAnalysisTest]: Begin of LS transition";

  // Get the run number
  run = lumiSeg.run();


}



void DTResolutionAnalysisTest::analyze(const Event& e, const EventSetup& context){

  nevents++;

}



void DTResolutionAnalysisTest::endLuminosityBlock(LuminosityBlock const& lumiSeg, EventSetup const& context) {
}


void DTResolutionAnalysisTest::endRun(Run const& run, EventSetup const& context) {
  

  LogTrace ("DTDQM|DTMonitorClient|DTResolutionAnalysisTest") <<"[DTResolutionAnalysisTest]: End of Run transition, performing the DQM client operation";

//   // counts number of lumiSegs 
//   nLumiSegs = lumiSeg.id().luminosityBlock();
  
//   // prescale factor
//   if ( nLumiSegs%prescaleFactor != 0 ) return;

  // reset the ME with fixed scale
  resetMEs();

  for (vector<DTChamber*>::const_iterator ch_it = muonGeom->chambers().begin();
       ch_it != muonGeom->chambers().end(); ++ch_it) {  // loop over the chambers

    DTChamberId chID = (*ch_it)->id();

    // Fill the test histos
    for(vector<const DTSuperLayer*>::const_iterator sl_it = (*ch_it)->superLayers().begin();
	sl_it != (*ch_it)->superLayers().end(); ++sl_it) {    // loop over SLs


      DTSuperLayerId slID = (*sl_it)->id();
      MonitorElement * res_histo = dbe->get(getMEName(slID));

      if(res_histo) { // Gaussian Fit
	float statMean = res_histo->getMean(1);
	float statSigma = res_histo->getRMS(1);
	double mean = -1;
	double sigma = -1;
	TH1F * histo_root = res_histo->getTH1F();

	// fill the summaries
	int entry= (chID.station() - 1) * 3;
	int binSect = slID.sector();
	if(slID.sector() == 13) binSect = 4;
	else if(slID.sector() == 14) binSect = 10;
	int binSL = entry+slID.superLayer();
	if(chID.station() == 4 && slID.superLayer() == 3) binSL--;
	if((slID.sector()==13 || slID.sector()==14)  && slID.superLayer()==1) binSL=12;
	if((slID.sector()==13 || slID.sector()==14) && slID.superLayer()==3) binSL=13;

	if(histo_root->GetEntries()>20) {
	  TF1 *gfit = new TF1("Gaussian","gaus",(statMean-(2*statSigma)),(statMean+(2*statSigma)));
	  try {
	    histo_root->Fit(gfit, "Q0", "", -0.1, 0.1);
	  } catch (...) {
	    LogWarning ("DTDQM|DTMonitorModule|DTResolutionAnalysisTask")
	      << "[DTResolutionAnalysisTask]: Exception when fitting SL : " << slID;
	    // FIXME: the SL is set as OK in the summary
	    double weight = 1/11.;
	    if((binSect == 4 || binSect == 10) && slID.station() == 4)  weight = 1/22.;
	    globalResSummary->Fill(binSect, slID.wheel(), weight);
	    continue;
	  }
	  
	  if(gfit){
	    // get the mean and the sigma of the distribution
	    mean = gfit->GetParameter(1); 
	    sigma = gfit->GetParameter(2);
	    
	    // fill the distributions
	    meanDistr[-2]->Fill(mean);
	    sigmaDistr[-2]->Fill(sigma);
	    if(slID.superlayer() == 2) {
	      meanDistr[abs(slID.wheel())]->Fill(mean);
	      sigmaDistr[abs(slID.wheel())]->Fill(sigma);
	    } else {
	      meanDistr[-1]->Fill(mean);
	      sigmaDistr[-1]->Fill(sigma);
	    }


	    
	    // sector summaries
	    MeanHistos[make_pair(slID.wheel(),binSect)]->setBinContent(binSL, mean);	
	    SigmaHistos[make_pair(slID.wheel(),binSect)]->setBinContent(binSL, sigma);
	    
	    // set the weight
	    double weight = 1/11.;
	    if((binSect == 4 || binSect == 10) && slID.station() == 4)  weight = 1/22.;
	    
	    // test the values of mean and sigma
	    if(meanInRange(mean) && sigmaInRange(sigma)) { // sigma and mean ok
	      globalResSummary->Fill(binSect, slID.wheel(), weight);
	      wheelMeanHistos[3]->Fill(binSect,slID.wheel(),weight);
	      wheelSigmaHistos[3]->Fill(binSect,slID.wheel(),weight);
	    } else {
	      if(!meanInRange(mean)) { // only sigma ok
		wheelMeanHistos[slID.wheel()]->Fill(binSect,binSL);
	      } else {
		wheelMeanHistos[3]->Fill(binSect,slID.wheel(),weight);
	      }
	      if(!sigmaInRange(sigma)) { // only mean ok
		wheelSigmaHistos[slID.wheel()]->Fill(binSect,binSL);
	      } else {
		wheelSigmaHistos[3]->Fill(binSect,slID.wheel(),weight);
	      }
	    }
	  }
	  delete gfit;
	}
	else{
	  LogVerbatim ("DTDQM|DTMonitorModule|DTResolutionAnalysisTask")
	    << "[DTResolutionAnalysisTask] Fit of " << slID
	    << " not performed because # entries < 20 ";
	  // FIXME: the SL is set as OK in the summary
	  double weight = 1/11.;
	  if((binSect == 4 || binSect == 10) && slID.station() == 4)  weight = 1/22.;
	  globalResSummary->Fill(binSect, slID.wheel(), weight);
	  wheelMeanHistos[3]->Fill(binSect,slID.wheel(),weight);
	  wheelSigmaHistos[3]->Fill(binSect,slID.wheel(),weight);
	}
      } else {
	LogWarning ("DTDQM|DTMonitorModule|DTResolutionAnalysisTask")
	  << "[DTResolutionAnalysisTask] Histo: " << getMEName(slID) << " not found" << endl;
      }
    } // loop on SLs
  } // Loop on Stations

}



void DTResolutionAnalysisTest::bookHistos(int wh) { 

  stringstream wheel; wheel <<wh;

  dbe->setCurrentFolder(topHistoFolder + "/00-MeanRes");
  string histoName =  "MeanSummaryRes_W" + wheel.str();
  stringstream meanRange; meanRange << (permittedMeanRange*10000);
  string histoTitle = "# of SL with |mean of res.| > " + meanRange.str() + "#mum (Wheel " + wheel.str() + ")";
  wheelMeanHistos[wh] = dbe->book2D(histoName.c_str(),histoTitle.c_str(),12,1,13,11,1,12);
  wheelMeanHistos[wh]->setAxisTitle("Sector",1);
  wheelMeanHistos[wh]->setBinLabel(1,"MB1_SL1",2);
  wheelMeanHistos[wh]->setBinLabel(2,"MB1_SL2",2);
  wheelMeanHistos[wh]->setBinLabel(3,"MB1_SL3",2);
  wheelMeanHistos[wh]->setBinLabel(4,"MB2_SL1",2);
  wheelMeanHistos[wh]->setBinLabel(5,"MB2_SL2",2);
  wheelMeanHistos[wh]->setBinLabel(6,"MB2_SL3",2);
  wheelMeanHistos[wh]->setBinLabel(7,"MB3_SL1",2);
  wheelMeanHistos[wh]->setBinLabel(8,"MB3_SL2",2);
  wheelMeanHistos[wh]->setBinLabel(9,"MB3_SL3",2);
  wheelMeanHistos[wh]->setBinLabel(10,"MB4_SL1",2);
  wheelMeanHistos[wh]->setBinLabel(11,"MB4_SL3",2); 
//   wheelMeanHistos[wh]->setBinLabel(12,"MB4_SL1",2);
//   wheelMeanHistos[wh]->setBinLabel(13,"MB4_SL3",2); 
  


  dbe->setCurrentFolder(topHistoFolder + "/01-SigmaRes");
  histoName =  "SigmaSummaryRes_W" + wheel.str();
  stringstream sigmaRange; sigmaRange << (permittedSigmaRange*10000);
  histoTitle = "# of SL with #sigma res. > " + sigmaRange.str() + "#mum (Wheel " + wheel.str() + ")";
  wheelSigmaHistos[wh] = dbe->book2D(histoName.c_str(),histoTitle.c_str(),12,1,13,11,1,12);
  wheelSigmaHistos[wh]->setAxisTitle("Sector",1);
  wheelSigmaHistos[wh]->setBinLabel(1,"MB1_SL1",2);
  wheelSigmaHistos[wh]->setBinLabel(2,"MB1_SL2",2);
  wheelSigmaHistos[wh]->setBinLabel(3,"MB1_SL3",2);
  wheelSigmaHistos[wh]->setBinLabel(4,"MB2_SL1",2);
  wheelSigmaHistos[wh]->setBinLabel(5,"MB2_SL2",2);
  wheelSigmaHistos[wh]->setBinLabel(6,"MB2_SL3",2);
  wheelSigmaHistos[wh]->setBinLabel(7,"MB3_SL1",2);
  wheelSigmaHistos[wh]->setBinLabel(8,"MB3_SL2",2);
  wheelSigmaHistos[wh]->setBinLabel(9,"MB3_SL3",2);
  wheelSigmaHistos[wh]->setBinLabel(10,"MB4_SL1",2);
  wheelSigmaHistos[wh]->setBinLabel(11,"MB4_SL3",2);
//   wheelSigmaHistos[wh]->setBinLabel(12,"MB4_SL1",2);
//   wheelSigmaHistos[wh]->setBinLabel(13,"MB4_SL3",2);

}


void DTResolutionAnalysisTest::bookHistos(int wh, int sect) {

  stringstream wheel; wheel << wh;		
  stringstream sector; sector << sect;	


  string MeanHistoName =  "MeanTest_W" + wheel.str() + "_Sec" + sector.str(); 
  string SigmaHistoName =  "SigmaTest_W" + wheel.str() + "_Sec" + sector.str(); 
 
  string folder = topHistoFolder + "/Wheel" + wheel.str() + "/Sector" + sector.str();
  dbe->setCurrentFolder(folder);

  if(sect!=4 && sect!=10) {
    MeanHistos[make_pair(wh,sect)] =
      dbe->book1D(MeanHistoName.c_str(),"Mean (from gaussian fit) of the residuals distribution",11,1,12);
  } else {
    MeanHistos[make_pair(wh,sect)] =
      dbe->book1D(MeanHistoName.c_str(),"Mean (from gaussian fit) of the residuals distribution",13,1,14);
  }
  (MeanHistos[make_pair(wh,sect)])->setBinLabel(1,"MB1_SL1",1);
  (MeanHistos[make_pair(wh,sect)])->setBinLabel(2,"MB1_SL2",1);
  (MeanHistos[make_pair(wh,sect)])->setBinLabel(3,"MB1_SL3",1);
  (MeanHistos[make_pair(wh,sect)])->setBinLabel(4,"MB2_SL1",1);
  (MeanHistos[make_pair(wh,sect)])->setBinLabel(5,"MB2_SL2",1);
  (MeanHistos[make_pair(wh,sect)])->setBinLabel(6,"MB2_SL3",1);
  (MeanHistos[make_pair(wh,sect)])->setBinLabel(7,"MB3_SL1",1);
  (MeanHistos[make_pair(wh,sect)])->setBinLabel(8,"MB3_SL2",1);
  (MeanHistos[make_pair(wh,sect)])->setBinLabel(9,"MB3_SL3",1);
  (MeanHistos[make_pair(wh,sect)])->setBinLabel(10,"MB4_SL1",1);
  (MeanHistos[make_pair(wh,sect)])->setBinLabel(11,"MB4_SL3",1);
  if(sect==4){
    (MeanHistos[make_pair(wh,sect)])->setBinLabel(12,"MB4S4_SL1",1);
    (MeanHistos[make_pair(wh,sect)])->setBinLabel(13,"MB4S4_SL3",1);
  }
  if(sect==10){
    (MeanHistos[make_pair(wh,sect)])->setBinLabel(12,"MB4S10_SL1",1);
    (MeanHistos[make_pair(wh,sect)])->setBinLabel(13,"MB4S10_SL3",1);
  }

  if(sect!=4 && sect!=10) {
    SigmaHistos[make_pair(wh,sect)] =
      dbe->book1D(SigmaHistoName.c_str(),"Sigma (from gaussian fit) of the residuals distribution",11,1,12);
  } else {
    SigmaHistos[make_pair(wh,sect)] =
      dbe->book1D(SigmaHistoName.c_str(),"Sigma (from gaussian fit) of the residuals distribution",13,1,14);
  }
  (SigmaHistos[make_pair(wh,sect)])->setBinLabel(1,"MB1_SL1",1);  
  (SigmaHistos[make_pair(wh,sect)])->setBinLabel(2,"MB1_SL2",1);
  (SigmaHistos[make_pair(wh,sect)])->setBinLabel(3,"MB1_SL3",1);
  (SigmaHistos[make_pair(wh,sect)])->setBinLabel(4,"MB2_SL1",1);
  (SigmaHistos[make_pair(wh,sect)])->setBinLabel(5,"MB2_SL2",1);
  (SigmaHistos[make_pair(wh,sect)])->setBinLabel(6,"MB2_SL3",1);
  (SigmaHistos[make_pair(wh,sect)])->setBinLabel(7,"MB3_SL1",1);
  (SigmaHistos[make_pair(wh,sect)])->setBinLabel(8,"MB3_SL2",1);
  (SigmaHistos[make_pair(wh,sect)])->setBinLabel(9,"MB3_SL3",1);
  (SigmaHistos[make_pair(wh,sect)])->setBinLabel(10,"MB4_SL1",1);
  (SigmaHistos[make_pair(wh,sect)])->setBinLabel(11,"MB4_SL3",1);
  if(sect==4){
    (SigmaHistos[make_pair(wh,sect)])->setBinLabel(12,"MB4S13_SL1",1);
    (SigmaHistos[make_pair(wh,sect)])->setBinLabel(13,"MB4S13_SL3",1);
  }
  if(sect==10){
    (SigmaHistos[make_pair(wh,sect)])->setBinLabel(12,"MB4S14_SL1",1);
    (SigmaHistos[make_pair(wh,sect)])->setBinLabel(13,"MB4S14_SL3",1);
  }


}


string DTResolutionAnalysisTest::getMEName(const DTSuperLayerId & slID) {
  
  stringstream wheel; wheel << slID.wheel();	
  stringstream station; station << slID.station();	
  stringstream sector; sector << slID.sector();	
  stringstream superLayer; superLayer << slID.superlayer();
  
  string folderName = 
    topHistoFolder + "/Wheel" +  wheel.str() +
    "/Sector" + sector.str() +
    "/Station" + station.str() + "/";

  if(doCalibAnalysis) folderName =
	"DT/DTCalibValidation/Wheel" +  wheel.str() +
	"/Station" + station.str() + "/Sector" + sector.str() + "/";
  
  string histoname = folderName + "hResDist" 
    + "_W" + wheel.str() 
    + "_St" + station.str() 
    + "_Sec" + sector.str() 
    + "_SL" + superLayer.str(); 

  if(doCalibAnalysis) histoname = folderName + "hResDist_STEP3" 
    + "_W" + wheel.str() 
    + "_St" + station.str() 
    + "_Sec" + sector.str() 
    + "_SL" + superLayer.str();
  
  return histoname;
  
}



int DTResolutionAnalysisTest::stationFromBin(int bin) const {
  return (int) (bin /3.1)+1;
}
 

int DTResolutionAnalysisTest::slFromBin(int bin) const {
  int ret = bin%3;
  if(ret == 0 || bin == 11) ret = 3;
  
  return ret;
}


bool DTResolutionAnalysisTest::meanInRange(double mean) const {
  return fabs(mean) < permittedMeanRange;
}


bool DTResolutionAnalysisTest::sigmaInRange(double sigma) const {
  return sigma < permittedSigmaRange;
}


void DTResolutionAnalysisTest::resetMEs() {
  globalResSummary->Reset();
  // Reset the summary histo
  for(map<int, MonitorElement*> ::const_iterator histo = wheelMeanHistos.begin();
      histo != wheelMeanHistos.end();
      histo++) {
    (*histo).second->Reset();
  }
  for(map<int, MonitorElement*> ::const_iterator histo = wheelSigmaHistos.begin();
      histo != wheelSigmaHistos.end();
      histo++) {
    (*histo).second->Reset();
  }

  for(int indx = -2; indx != 3; ++indx) {
    meanDistr[indx]->Reset();
    sigmaDistr[indx]->Reset();
  }
}

