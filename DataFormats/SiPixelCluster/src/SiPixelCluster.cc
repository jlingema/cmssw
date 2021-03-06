#include "DataFormats/SiPixelCluster/interface/SiPixelCluster.h"

//---------------------------------------------------------------------------
//!  \class SiPixelCluster
//!  \brief Pixel cluster -- collection of pixels with ADC counts
//!
//!  Class to contain and store all the topological information of pixel clusters:
//!  charge, global size, size and the barycenter in x and y
//!  local directions. It builds a vector of SiPixel (which is
//!  an inner class) and a container of channels. 
//!
//!  March 2007: Edge pixel methods moved to RectangularPixelTopology (V.Chiochia)
//!  May   2008: Offset based packing (D.Fehling / A. Rizzi)  
//!  \author Petar Maksimovic, JHU
//---------------------------------------------------------------------------


SiPixelCluster::SiPixelCluster( const SiPixelCluster::PixelPos& pix, int adc) :
  detId_(0),     // &&& To be fixed ?? 
  // The center of pixel with row # N is at N+0.5 in the meas. frame!
	//  theSumX( (pix.row()+0.5) * float(adc)), 
	// theSumY( (pix.col()+0.5) * float(adc)),
	//  theCharge( float(adc)),
  theMinPixelRow( pix.row()),
	//  theMaxPixelRow( pix.row()),
  theMinPixelCol( pix.col())
	//  theMaxPixelCol( pix.col())
{
  // First pixel in this cluster.
  thePixelADC.push_back( adc );
  thePixelOffset.push_back( pix.row() - theMinPixelRow );
  thePixelOffset.push_back( pix.col() - theMinPixelCol );
}

void SiPixelCluster::add( const SiPixelCluster::PixelPos& pix, int adc) {

  // The center of pixel with row # N is at N+0.5 in the meas. frame!
  //theSumX += (pix.row()+0.5) * float(adc); 
  //theSumY += (pix.col()+0.5) * float(adc); 
  //theCharge += float(adc);
	//  thePixels.push_back( Pixel( pix.row(), pix.col(), adc ) );
	
	int minRow = theMinPixelRow;
	int minCol = theMinPixelCol;
	bool recalculate = false;
	
	if (pix.row() < theMinPixelRow) {
		theMinPixelRow = pix.row();
		recalculate = true;
	}
	if (pix.col() < theMinPixelCol) {
		theMinPixelCol = pix.col();
		recalculate = true;
	}
	if (recalculate) {
		int isize = thePixelADC.size();
		for (int i=0; i<isize; ++i) {
			int xoffset = (thePixelOffset[i*2] ) + minRow - theMinPixelRow;
			int yoffset = (thePixelOffset[i*2+1] ) + minCol - theMinPixelCol;
			thePixelOffset[i*2] = xoffset;
                        thePixelOffset[i*2+1] = yoffset;
		}
	}
	
	thePixelADC.push_back( adc );
	thePixelOffset.push_back( (pix.row() - theMinPixelRow) );
        thePixelOffset.push_back( (pix.col() - theMinPixelCol) );
}

