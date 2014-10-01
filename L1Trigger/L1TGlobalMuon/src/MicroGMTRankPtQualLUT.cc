#include "../interface/MicroGMTRankPtQualLUT.h"

l1t::MicroGMTRankPtQualLUT::MicroGMTRankPtQualLUT (const edm::ParameterSet& iConfig) {
  edm::ParameterSet config = iConfig.getParameter<edm::ParameterSet>("SortRankLUTSettings");
  m_ptInWidth = config.getParameter<int>("pT_in_width");
  m_qualInWidth = config.getParameter<int>("qual_in_width");
  
  m_totalInWidth = m_ptInWidth + m_qualInWidth;

  m_ptMask = (1 << m_ptInWidth) - 1;
  m_qualMask = (1 << (m_totalInWidth - 1)) - m_ptMask - 1;
  
  m_inputs.push_back(PT);
  std::string m_fname = config.getParameter<std::string>("filename");
  if (m_fname != std::string("")) {
    load(m_fname);
  } 
}

l1t::MicroGMTRankPtQualLUT::~MicroGMTRankPtQualLUT ()
{

}

int 
l1t::MicroGMTRankPtQualLUT::lookup(int pt, int qual) const 
{
  // normalize these two to the same scale and then calculate?
  if (m_initialized) {
    return lookupPacked(hashInput(pt, qual));
  }

  int result = 0;
  result = pt + qual*2; 
  // normalize to out width
  return result;  
}

int 
l1t::MicroGMTRankPtQualLUT::hashInput(int pt, int qual) const
{

  int result = 0;
  result += pt;
  result += qual << m_ptInWidth;
  return result;
}

void 
l1t::MicroGMTRankPtQualLUT::unHashInput(int input, int& pt, int& qual) const 
{
  pt = input & m_ptMask;
  qual = (input & m_qualMask) >> m_ptInWidth;
} 