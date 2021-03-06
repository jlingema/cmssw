/** \class HLTConfigData
 *
 * See header file for documentation
 *
 *  $Date: 2011/03/09 13:58:06 $
 *  $Revision: 1.11 $
 *
 *  \author Martin Grunewald
 *
 */

#include "HLTrigger/HLTcore/interface/HLTConfigData.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include <iostream>

static const edm::ParameterSet* s_dummyPSet()
{
  static edm::ParameterSet dummyPSet;
  dummyPSet.registerIt();
  return &dummyPSet;
}

HLTConfigData::HLTConfigData():
  processPSet_(s_dummyPSet()),
  processName_(""),
  tableName_(), triggerNames_(), moduleLabels_(),
  triggerIndex_(), moduleIndex_(),
  hltL1GTSeeds_(),
  streamNames_(), streamIndex_(), streamContents_(),
  datasetNames_(), datasetIndex_(), datasetContents_(),
  hltPrescaleTable_()
{
  if (processPSet_->id().isValid()) {
    extract();
  }
}

HLTConfigData::HLTConfigData(const edm::ParameterSet* iPSet):
  processPSet_(iPSet),
  processName_(""),
  tableName_(), triggerNames_(), moduleLabels_(),
  triggerIndex_(), moduleIndex_(),
  hltL1GTSeeds_(),
  streamNames_(), streamIndex_(), streamContents_(),
  datasetNames_(), datasetIndex_(), datasetContents_(),
  hltPrescaleTable_()
{
  if (processPSet_->id().isValid()) {
    extract();
  }
}

void HLTConfigData::extract()
{
   using namespace std;
   using namespace edm;
   using namespace trigger;

   // Extract process name
   if (processPSet_->existsAs<string>("@process_name",true)) {
     processName_= processPSet_->getParameter<string>("@process_name");
   }

   // Obtain PSet containing table name (available only in 2_1_10++ files)
   if (processPSet_->existsAs<ParameterSet>("HLTConfigVersion",true)) {
     const ParameterSet& HLTPSet(processPSet_->getParameterSet("HLTConfigVersion"));
     if (HLTPSet.existsAs<string>("tableName",true)) {
       tableName_=HLTPSet.getParameter<string>("tableName");
     }
   }
   LogVerbatim("HLTConfigData") << "ProcessPSet with: "
				<< processName_ << " "
				<< tableName();

   // Extract trigger paths (= paths - end_paths)
   if (processPSet_->existsAs<ParameterSet>("@trigger_paths",true)) {
     const ParameterSet& HLTPSet(processPSet_->getParameterSet("@trigger_paths"));
     if (HLTPSet.existsAs<vector<string> >("@trigger_paths",true)) {
       triggerNames_= HLTPSet.getParameter<vector<string> >("@trigger_paths");
     }
   }

   // Obtain module labels of all modules on all trigger paths
   const unsigned int n(size());
   moduleLabels_.reserve(n);
   for (unsigned int i=0;i!=n; ++i) {
     if (processPSet_->existsAs<vector<string> >(triggerNames_[i],true)) {
       moduleLabels_.push_back(processPSet_->getParameter<vector<string> >(triggerNames_[i]));
     }
   }

   // Fill index maps for fast lookup
   moduleIndex_.resize(n);
   for (unsigned int i=0; i!=n; ++i) {
     triggerIndex_[triggerNames_[i]]=i;
     moduleIndex_[i].clear();
     const unsigned int m(size(i));
     for (unsigned int j=0; j!=m; ++j) {
       moduleIndex_[i][moduleLabels_[i][j]]=j;
     }
   }

   // Extract and fill HLTLevel1GTSeed information for each trigger path
   hltL1GTSeeds_.resize(n);
   for (unsigned int i=0; i!=n; ++i) {
     hltL1GTSeeds_[i].clear();
     const unsigned int m(size(i));
     for (unsigned int j=0; j!=m; ++j) {
       const string& label(moduleLabels_[i][j]);
       if (moduleType(label) == "HLTLevel1GTSeed") {
	 const ParameterSet& pset(modulePSet(label));
	 if (pset!=ParameterSet()) {
	   const bool   l1Tech(pset.getParameter<bool>("L1TechTriggerSeeding"));
	   const string l1Seed(pset.getParameter<string>("L1SeedsLogicalExpression"));
	   hltL1GTSeeds_[i].push_back(pair<bool,string>(l1Tech,l1Seed));
	 }
       }
     }
   }

   // Extract and fill streams information
   if (processPSet_->existsAs<ParameterSet>("streams",true)) {
     const ParameterSet& streams(processPSet_->getParameterSet("streams"));
     streamNames_=streams.getParameterNamesForType<vector<string> >();
     sort(streamNames_.begin(),streamNames_.end());
     const unsigned int n(streamNames_.size());
     streamContents_.resize(n);
     for (unsigned int i=0; i!=n; ++i) {
       streamIndex_[streamNames_[i]]=i;
       streamContents_[i]=streams.getParameter<vector<string> >(streamNames_[i]);
       sort(streamContents_[i].begin(),streamContents_[i].end());
     }
     
   }

   // Extract and fill datasets information
   if (processPSet_->existsAs<ParameterSet>("datasets",true)) {
     const ParameterSet& datasets(processPSet_->getParameterSet("datasets"));
     datasetNames_=datasets.getParameterNamesForType<vector<string> >();
     sort(datasetNames_.begin(),datasetNames_.end());
     const unsigned int n(datasetNames_.size());
     datasetContents_.resize(n);
     for (unsigned int i=0; i!=n; ++i) {
       datasetIndex_[datasetNames_[i]]=i;
       datasetContents_[i]=datasets.getParameter< vector<string> >(datasetNames_[i]);
       sort(datasetContents_[i].begin(),datasetContents_[i].end());
     }
   }

   // Extract and fill Prescale information

   // Check various possibilities to get the HLT prescale sets:
   string prescaleName("");
   const string preS("PrescaleService");
   const string preT("PrescaleTable");
   if (processPSet_->existsAs<ParameterSet>(preS,true)) {
     prescaleName=preS;
   } else if ( processPSet_->existsAs<ParameterSet>(preT,true)) {
     prescaleName=preT;
   }
   if (prescaleName=="") {
     hltPrescaleTable_=HLTPrescaleTable();
   } else {
     const ParameterSet& iPS(processPSet_->getParameterSet(prescaleName));
     string defaultLabel(iPS.getUntrackedParameter<string>("lvl1DefaultLabel",""));
     vector<string> labels;
     if (iPS.existsAs<vector<string> >("lvl1Labels",true)) {
       labels = iPS.getParameter<vector<string> >("lvl1Labels");
     }
     unsigned int set(0);
     const unsigned int n(labels.size());
     for (unsigned int i=0; i!=n; ++i) {
       if (labels[i]==defaultLabel) set=i;
     }
     map<string,vector<unsigned int> > table;
     if (iPS.existsAs< vector<ParameterSet> >("prescaleTable",true)) {
       const vector<ParameterSet>& vpTable(iPS.getParameterSetVector("prescaleTable"));
       const unsigned int m (vpTable.size());
       for (unsigned int i=0; i!=m; ++i) {
	 table[vpTable[i].getParameter<std::string>("pathName")] = 
	   vpTable[i].getParameter<std::vector<unsigned int> >("prescales");
       }
     }
     if (n>0) {
       hltPrescaleTable_=HLTPrescaleTable(set,labels,table);
     } else {
       hltPrescaleTable_=HLTPrescaleTable();
     }

   }

   return;
}

void HLTConfigData::dump(const std::string& what) const {
   using namespace std;
   using namespace edm;

   if (what=="ProcessPSet") {
     cout << "HLTConfigData::dump: ProcessPSet = " << endl << *processPSet_ << endl;
   } else if (what=="ProcessName") {
     cout << "HLTConfigData::dump: ProcessName = " << processName_ << endl;

   } else if (what=="TableName") {
     cout << "HLTConfigData::dump: TableName = " << tableName_ << endl;
   } else if (what=="Triggers") {
     const unsigned int n(size());
     cout << "HLTConfigData::dump: Triggers: " << n << endl;
     for (unsigned int i=0; i!=n; ++i) {
       cout << "  " << i << " " << triggerNames_[i] << endl;
     }
   } else if (what=="TriggerSeeds") {
     const unsigned int n(size());
     cout << "HLTConfigData::dump: TriggerSeeds: " << n << endl;
     for (unsigned int i=0; i!=n; ++i) {
       const unsigned int m(hltL1GTSeeds_[i].size());
       cout << "  " << i << " " << triggerNames_[i] << " " << m << endl;
       for (unsigned int j=0; j!=m; ++j) {
	 cout << "    " << j
	      << " " << hltL1GTSeeds_[i][j].first
	      << "/" << hltL1GTSeeds_[i][j].second << endl;
       }
     }
   } else if (what=="Modules") {
     const unsigned int n(size());
     cout << "HLTConfigData::dump Triggers and Modules: " << n << endl;
     for (unsigned int i=0; i!=n; ++i) {
       const unsigned int m(size(i));
       cout << i << " " << triggerNames_[i] << " " << m << endl;
       cout << " - Modules: ";
       unsigned int nHLTPrescalers(0);
       unsigned int nHLTLevel1GTSeed(0);
       for (unsigned int j=0; j!=m; ++j) {
	 const string& label(moduleLabels_[i][j]);
	 const string  type(moduleType(label));
	 const string  edmtype(moduleEDMType(label));
	 cout << " " << j << ":" << label << "/" << type << "/" << edmtype;
	 if (type=="HLTPrescaler") nHLTPrescalers++;
	 if (type=="HLTLevel1GTSeed") nHLTLevel1GTSeed++;
       }
       cout << endl;
       cout << " - Number of HLTPrescaler/HLTLevel1GTSeed modules: " 
	    << nHLTPrescalers << "/" << nHLTLevel1GTSeed << endl;
     }
   } else if (what=="StreamNames") {
     const unsigned int n(streamNames_.size());
     cout << "HLTConfigData::dump: StreamNames: " << n << endl;
     for (unsigned int i=0; i!=n; ++i) {
       cout << "  " << i << " " << streamNames_[i] << endl;
     }
   } else if (what=="Streams") {
     const unsigned int n(streamNames_.size());
     cout << "HLTConfigData::dump: Streams: " << n << endl;
     for (unsigned int i=0; i!=n; ++i) {
       const unsigned int m(streamContents_[i].size());
       cout << "  " << i << " " << streamNames_[i] << " " << m << endl;
       for (unsigned int j=0; j!=m; ++j) {
	 cout << "    " << j << " " << streamContents_[i][j] << endl;
       }
     }
   } else if (what=="DatasetNames") {
     const unsigned int n(datasetNames_.size());
     cout << "HLTConfigData::dump: DatasetNames: " << n << endl;
     for (unsigned int i=0; i!=n; ++i) {
       cout << "  " << i << " " << datasetNames_[i] << endl;
     }
   } else if (what=="Datasets") {
     const unsigned int n(datasetNames_.size());
     cout << "HLTConfigData::dump: Datasets: " << n << endl;
     for (unsigned int i=0; i!=n; ++i) {
       const unsigned int m(datasetContents_[i].size());
       cout << "  " << i << " " << datasetNames_[i] << " " << m << endl;
       for (unsigned int j=0; j!=m; ++j) {
	 cout << "    " << j << " " << datasetContents_[i][j] << endl;
       }
     }
   } else if (what=="PrescaleTable") {
     const unsigned int n (hltPrescaleTable_.size());
     cout << "HLTConfigData::dump: PrescaleTable: # of sets : " << n << endl;
     const vector<string>& labels(hltPrescaleTable_.labels());
     for (unsigned int i=0; i!=n; ++i) {
       cout << " " << i << "/'" << labels.at(i) << "'";
     }
     if (n>0) cout << endl;
     const map<string,vector<unsigned int> >& table(hltPrescaleTable_.table());
     cout << "HLTConfigData::dump: PrescaleTable: # of paths: " << table.size() << endl;
     const map<string,vector<unsigned int> >::const_iterator tb(table.begin());
     const map<string,vector<unsigned int> >::const_iterator te(table.end());
     for (map<string,vector<unsigned int> >::const_iterator ti=tb; ti!=te; ++ti) {
       for (unsigned int i=0; i!=n; ++i) {
	 cout << " " << ti->second.at(i);
       }
       cout << " " << ti->first << endl;
     }
   } else {
     cout << "HLTConfigData::dump: Unkown dump request: " << what << endl;
   }
   return;
}

const std::string& HLTConfigData::processName() const {
  return processName_;
}

unsigned int HLTConfigData::size() const {
  return triggerNames_.size();
}
unsigned int HLTConfigData::size(unsigned int trigger) const {
  return moduleLabels_.at(trigger).size();
}
unsigned int HLTConfigData::size(const std::string& trigger) const {
  return size(triggerIndex(trigger));
}

const std::string& HLTConfigData::tableName() const {
  return tableName_;
}
const std::vector<std::string>& HLTConfigData::triggerNames() const {
  return triggerNames_;
}
const std::string& HLTConfigData::triggerName(unsigned int trigger) const {
  return triggerNames_.at(trigger);
}
unsigned int HLTConfigData::triggerIndex(const std::string& trigger) const {
  const std::map<std::string,unsigned int>::const_iterator index(triggerIndex_.find(trigger));
  if (index==triggerIndex_.end()) {
    return size();
  } else {
    return index->second;
  }
}

const std::vector<std::string>& HLTConfigData::moduleLabels(unsigned int trigger) const {
  return moduleLabels_.at(trigger);
}
const std::vector<std::string>& HLTConfigData::moduleLabels(const std::string& trigger) const {
  return moduleLabels_.at(triggerIndex(trigger));
}

const std::string& HLTConfigData::moduleLabel(unsigned int trigger, unsigned int module) const {
  return moduleLabels_.at(trigger).at(module);
}
const std::string& HLTConfigData::moduleLabel(const std::string& trigger, unsigned int module) const {
  return moduleLabels_.at(triggerIndex(trigger)).at(module);
}

unsigned int HLTConfigData::moduleIndex(unsigned int trigger, const std::string& module) const {
  const std::map<std::string,unsigned int>::const_iterator index(moduleIndex_.at(trigger).find(module));
  if (index==moduleIndex_.at(trigger).end()) {
    return size(trigger);
  } else {
    return index->second;
  }
}
unsigned int HLTConfigData::moduleIndex(const std::string& trigger, const std::string& module) const {
  return moduleIndex(triggerIndex(trigger),module);
}

const std::string HLTConfigData::moduleType(const std::string& module) const {
  if (modulePSet(module).existsAs<std::string>("@module_type",true)) {
    return modulePSet(module).getParameter<std::string>("@module_type");
  } else {
    return "";
  }
}

const std::string HLTConfigData::moduleEDMType(const std::string& module) const {
  if (modulePSet(module).existsAs<std::string>("@module_edm_type",true)) {
    return modulePSet(module).getParameter<std::string>("@module_edm_type");
  } else {
    return "";
  }
}

const edm::ParameterSet& HLTConfigData::processPSet() const {
  return *processPSet_;
}

const edm::ParameterSet& HLTConfigData::modulePSet(const std::string& module) const {
  if (processPSet_->exists(module)) {
    return processPSet_->getParameterSet(module);
  } else {
    return *s_dummyPSet();
  }
}

const std::vector<std::vector<std::pair<bool,std::string> > >& HLTConfigData::hltL1GTSeeds() const {
  return hltL1GTSeeds_;
}

const std::vector<std::pair<bool,std::string> >& HLTConfigData::hltL1GTSeeds(const std::string& trigger) const {
  return hltL1GTSeeds(triggerIndex(trigger));
}

const std::vector<std::pair<bool,std::string> >& HLTConfigData::hltL1GTSeeds(unsigned int trigger) const {
  return hltL1GTSeeds_.at(trigger);
}

/// Streams                                                                   
const std::vector<std::string>& HLTConfigData::streamNames() const {
  return streamNames_;
}

const std::string& HLTConfigData::streamName(unsigned int stream) const {
  return streamNames_.at(stream);
}

unsigned int HLTConfigData::streamIndex(const std::string& stream) const {
  const std::map<std::string,unsigned int>::const_iterator index(streamIndex_.find(stream));
  if (index==streamIndex_.end()) {
    return streamNames_.size();
  } else {
    return index->second;
  }
}

const std::vector<std::vector<std::string> >& HLTConfigData::streamContents() const {
  return streamContents_;
}

const std::vector<std::string>& HLTConfigData::streamContent(unsigned int stream) const {
  return streamContents_.at(stream);
}

const std::vector<std::string>& HLTConfigData::streamContent(const std::string& stream) const {
  return streamContent(streamIndex(stream));
}

/// Datasets                                                                  
const std::vector<std::string>& HLTConfigData::datasetNames() const {
  return datasetNames_;
}

const std::string& HLTConfigData::datasetName(unsigned int dataset) const {
  return datasetNames_.at(dataset);
}

unsigned int HLTConfigData::datasetIndex(const std::string& dataset) const {
  const std::map<std::string,unsigned int>::const_iterator index(datasetIndex_.find(dataset));
  if (index==datasetIndex_.end()) {
    return datasetNames_.size();
  } else {
    return index->second;
  }
}

const std::vector<std::vector<std::string> >& HLTConfigData::datasetContents() const {
  return datasetContents_;
}

const std::vector<std::string>& HLTConfigData::datasetContent(unsigned int dataset) const {
  return datasetContents_.at(dataset);
}

const std::vector<std::string>& HLTConfigData::datasetContent(const std::string& dataset) const {
  return datasetContent(datasetIndex(dataset));
}


unsigned int HLTConfigData::prescaleSize() const {
  return hltPrescaleTable_.size();
}
unsigned int HLTConfigData::prescaleValue(unsigned int set, const std::string& trigger) const {
  return hltPrescaleTable_.prescale(set,trigger);
}

const std::vector<std::string>& HLTConfigData::prescaleLabels() const {
  return hltPrescaleTable_.labels();
}
const std::map<std::string,std::vector<unsigned int> >& HLTConfigData::prescaleTable() const {
  return hltPrescaleTable_.table();
}

edm::ParameterSetID HLTConfigData::id() const {
  return processPSet_->id();
}
