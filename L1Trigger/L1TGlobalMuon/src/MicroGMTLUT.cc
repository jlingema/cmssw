
#include "../interface/MicroGMTLUT.h"

int 
MicroGMTLUT::hashInput(const std::vector<std::pair<input_t, int> >& inputset) const 
{
  std::vector<std::pair<input_t, int> >::const_iterator inputIt;
  int shift = 0;
  int result = 0;
  for (inputIt = inputset.begin(); inputIt != inputset.end(); ++inputIt) {
    result += inputIt->second << shift;
    shift += MicroGMTConfiguration::getInputWidth(inputIt->first);
  }
  return result;
}


// I/O functions
void 
MicroGMTLUT::save(std::ofstream& output) 
{
  std::stringstream out;
  headerToStream(out);
  contentsToStream(out);
  output << out.str();
}

void
MicroGMTLUT::load(const std::string& inFileName) {
  std::ifstream fstream;
  try {
    fstream.open(inFileName.c_str());
    if (!fstream.good()) throw -1;
  } catch (int i) {
    // cms::MessageLogger(std::string("Failed to open file")+m_fname);
    return;
  }

  std::string lineID = "";
  
  while (!fstream.eof()) {
    // read until either end of file is reached or the CONTENT line is found
    lineID = "";
    fstream >> lineID;
    if (lineID.find("CONTENT_VECTOR") != std::string::npos) break;
  }

  if (fstream.eof()) {
    std::cout << "end of file before contents!" << std::endl;
    return;
  }

  for (int cntr = 0; cntr < (1 << m_totalInWidth); ++cntr) {
    fstream >> m_contents[cntr];
  }
  m_initialized = true;
}

void 
MicroGMTLUT::print() 
{
  std::stringstream out;
  contentsToStream(out);
  std::cout << out.str() << std::endl;
}


int 
MicroGMTLUT::lookup(const std::vector<PortType>& inputset) const 
{
  // sort to make sure the hash is correct?
  int hashedInput = hashInput(inputset);
  return lookupPacked(hashedInput);
}


int 
MicroGMTLUT::lookupPacked(const int input) const 
{
  // sort to make sure the hash is correct?
  if (m_initialized) {
    if ((unsigned)input > m_contents.size()) {
      std::cout << input << std::endl;
    }

    return m_contents.at(input);
  }
  std::cout << "Error, trying to lookup without initialization!" << std::endl;
  return 0;
}

void 
MicroGMTLUT::lookup(const std::vector<std::vector<PortType> >& inputsets, std::vector<int>&outputs) const 
{
  outputs.clear();
  for (std::vector<std::vector<PortType> >::const_iterator it = inputsets.begin(); it != inputsets.end(); ++it) {
    outputs.push_back(lookup(*it));
  }
}

void 
MicroGMTLUT::initialize() 
{
  for (int in = 0; in < (1 << m_totalInWidth); ++in) {
    int out = lookupPacked(in);
    m_contents[in] = out;
  }
  m_initialized = true;
}

void 
MicroGMTLUT::contentsToStream(std::stringstream& stream) 
{
  stream << "CONTENT_";
  for (std::vector<input_t>::iterator it = m_inputs.begin(); it != m_inputs.end(); ++it) {
    stream << MicroGMTConfiguration::getInputName(*it);
    stream << "_";
  }
  stream << " : ";
  for (int in = 0; in < (1 << m_totalInWidth); ++in) {
    stream << m_contents[in] << " ";
  }
  stream << std::endl;
}

void
MicroGMTLUT::headerToStream(std::stringstream& stream) const 
{
  stream << "NAME      : Name of the LUT";
  stream << "INSTNACES : List (space separated) of instances of this LUT (differing contents but same in/output)" << std::endl;
  stream << "INPUTS    : List (space separated) of inputs in format <input_name>(<input_width>)" << std::endl;
  stream << "OUTPUTS   : List (space separated) of outputs in format <output_name>(<output_width>)" << std::endl;
  stream << "IPBUS_ADD : Address for access via IPBus" << std::endl;
  stream << "CONTENT_X : List (space separated) of outputs from packed int for zero-indexed instance X" << std::endl;
}
