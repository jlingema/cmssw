#ifndef PLOTMILLEPEDE_H
#define PLOTMILLEPEDE_H
// Original Author: Gero Flucke
// last change    : $Date: 2010/08/12 09:17:10 $
// by             : $Author: flucke $
//
// PlotMillePede is a class to interprete the content of the ROOT
// TTrees that are stored as output of the millepede alignment algorithm.
// The histograms are created by the methods inherited from the
// MillePedeTrees class. Drawn quantities and selections are handled
// as strings as they can be understood by TTree::Draw(..) commands.
//
// The main methods all start with 'Draw', so you can use tab completion
// on what is available:
//     PlotMillePede p("treeFile.root"); // file form pede step
//     p.Draw<tab>
//
// Management of the created histograms is done using the ('ancient')
// class GFHistManager. A pointer to that class can be accessed by
//     GFHistManager *man = p.GetHistManager();
// and then be used manipulate the drawings, e.g. by  
//     man->SetLogY();
// or
//     man->SetNumHistsXY(3,1);
//     man->Draw();
//
// By calling new p.Draw..(..) commands, usually previously drawn canvases are
// deleted. But most methods provide either a boolean flag 'addPlots' or
// understand the option 'add'. If the flag is true or the optione string
// contains 'add', canvases from previous Draw-commands are kept (in fact they
// are re-drawn).
//
// A title can be set by 'void SetTitle(const char *title)':
// The title will appear as part of all histogram titles.
//
// Several cuts can be set and are active until reset for all drawing routines
// called after setting them. Active selections are mentioned in histogram titles.
// The possibilites to set cuts are:
//
// 1) Select subdetector(s):
// void SetSubDetId(Int_t subDetId);
//  *  1-6 are TPB, TPE, TIB, TID, TOB, TEC, 
//  *  -1 means: reset, i.e. take all
//  * using (repeated) calls of
//    void AddSubDetId(Int_t subDetId);
//    leads to an OR selection of subdetectors
//
// 2) Special alignable types:
// Int_t SetAlignableTypeId(Int_t alignableTypeId);
//  * DetUnit=1,
//  * Det=2,
//  * TIBString=15,
//  * etc. from CommonAlignment/interface/StructureType.h 
//  * -1 resets
// The methods returns the previous setting.
//
// 3) Select hierarchy level:
// In case of hierarchical alignment, one can look at the parameters etc. of each 
// (active!) hierarchy level separately:
// Int_t SetHieraLevel(Int_t hieraLevel);
//  * 0 (default!): lowest level only
//  * 1,2,...: first, second,... larger structures only
//  * -1: resets, shows all levels
// The methods returns the previous setting.
//
// 4) General selection:
// void AddAdditionalSel(const char *selection);
//  * whatever selection string that is valid on the TTree's...
//  * ... therefore for experts 
//  * current setting cleared by 'ClearAdditionalSel()'
//
// 4a) Strip stereo, rphi, 1Dlayer/double sided selection:
//  * Known keywords for AddAdditionalSel(..)
//     * StripStereo:     only Id's of stereo modules in 2D layers/rings 
//     * StripRphi:       only Id's of rphi modules in 2D layers/rings 
//     * StripDoubleOr1D: only Id's from 1D layers or composed Dets in 2D layers/rings
//
// 4b) Geometrical and N(hit) selection:
// void AddAdditionalSel(const TString &xyzrPhiNhit, Float_t min, Float_t max);
//  This method makes internal use of the above "AddAdditionalSel(const char *)".
//  Selects quantity 'xyzrPhiNhit' between 'min' and 'max' where
//  'xyzrPhiNhit' can be any of x, y, z, r, phi, Nhit.
//  Also cleared by 'ClearAdditionalSel()'.

#include "MillePedeTrees.h"
#include <TArrayI.h>

class GFHistManager;

class PlotMillePede : public MillePedeTrees
{
 public:
  explicit PlotMillePede(const char *fileName, Int_t iter = 2, Int_t hieraLevel = 0,
			 bool useDiff = false);// iter=1/2: singlerun/merged; heiraLev: -1 ignore, 0 lowest level, etc.; useDiff = true only for before2007_02_26
  PlotMillePede(const char *fileName, Int_t iter, Int_t hieraLevel, const char *treeNameAdd);
  virtual ~PlotMillePede();

  void SetTitle(const char *title) {fTitle = title;}
  const TString& GetTitle() const { return fTitle;}
  GFHistManager* GetHistManager() { return fHistManager;}

  void DrawAll(Option_t *opt = "rogpmeh"); // r=ParamResult,o=OrigParam,g=GlobCorr,p=Pull,m=MisVsLocation,e=ErrorVsHit,h=HitMaps

  void DrawParam(bool addPlots = false, const TString &sel = ""); // default: not fixed params
  void DrawPedeParam(Option_t *option = "", unsigned int nNonRigidParam = 12);//"add": add plots, "vs": p_i vs p_j; params beyond rigid body 
  void DrawPedeParamVsLocation(Option_t *option = "", unsigned int nNonRigidParam = 12);//"add" to add plots; params beyond rigid body 
  void DrawOrigParam(bool addPlots = false, const TString &sel = ""); //this->AnyFreePar()
  void DrawOrigPos(bool addPlots = false, const TString &sel = "");

/*   void DrawParamDiff(bool addPlots = false); */
  void DrawParamResult(bool addPlots = false);
  void DrawPosResult(bool addPlots = false, const TString &selection = "");// sel=SelIs2D()/SelIs1D()
  void DrawPosMisVsLocation(bool addPlots=false, const TString &selection="", Option_t *option="");// sel=e.g. SelIs2D()/SelIs1D(), opt="start" starting misalignment profile
  void DrawGlobCorr(bool addPlots = false, const TString &sel = "", Option_t *opt = "",
		    Float_t min = 0., Float_t max = 1.);// opt "nal" (no axis limit), otherwise take min/max; opt 'valid' for pede rad OK

  void DrawPull(Option_t *opt = "");// 'add' to add plots, 'valid' to select only valid parameter, 'nolimit' for pulls without prior axis limits

  void DrawMisVsLocation(bool addPlots = false, const TString &sel = "", Option_t *opt = "");//opt: vse0,vse1,mis,withfixed
  void DrawLabelDiffAbove(UInt_t iPar, float minDiff, bool addPlots = false);
  void DrawErrorVsHit(bool addPlots = false, const TString &sel = "");
  void DrawHitMaps(bool addPlots = false, bool inclFullFixed = true);
  void DrawBigPullLabel(float minPull, bool addPlots = false);
  void DrawBigPullPos(float minPull, bool addPlots = false);
  void DrawSubDetId(bool addPlots = false);
  void DrawXyArrow(Double_t factor, Option_t *option = ""); // opt. "add", "zcirc"
  void ScanSelection(const char *sel = 0, const char *addColumns = 0); // scan tree with selection, if !sel, take basic selection, addColumns like ":ObjId:HitsY" to add columns ObjId and HitsY
  void ScanPedeParAbove(UInt_t iPar, float absMin = 1.e5); // scan tree, selecting large parameters

  void DrawCheck();
  TString Unique(const char *name) const;

  Float_t SetMaxDev(Float_t maxDev); // set x-axis range for result plots
  const TArrayI* GetSubDetIds() const { return &fSubDetIds;} // selected subdets
  void SetSubDetId(Int_t subDetId); // 1-6 are TPB, TPE, TIB, TID, TOB, TEC, -1 means: take all
  void AddSubDetId(Int_t subDetId); // 1-6 are TPB, TPE, TIB, TID, TOB, TEC
  Int_t SetAlignableTypeId(Int_t alignableTypeId);//detunit=1,det=2,...,TIBString=15,etc. from StructureType.h (-1: all)
  Int_t SetHieraLevel(Int_t hieraLevel); // select hierarchical level (-1: all)
  void AddAdditionalSel(const char *selection);// special select; StripDoubleOr1D,StripRphi,StripStereo
  void AddAdditionalSel(const TString &xyzrPhiNhit, Float_t min, Float_t max); // x,y,z,r,phi,Nhit

  const TString GetAdditionalSel () const { return fAdditionalSel;}
  void ClearAdditionalSel () { fAdditionalSel = ""; fAdditionalSelTitle = "";}
  void AddBasicSelection(TString &sel) const;

  TString FinalMisAlignment(UInt_t iPar) const;
  TString TitleAdd() const; // for subdet settings etc.

  void CopyAddBinning(TString &name, const TH1 *hist) const;// extend 'name' taking binning from hist
 private: 
  Int_t PrepareAdd(bool addPlots);

  TString        fTitle; // added to hist titles
  GFHistManager *fHistManager;
  Int_t          fHieraLevel; // which hierarchical level to require (-1: all)
  bool           fUseDiff; // true: result is diff between MisParT() and ParT()
                           // false: result is ParT() (from 2007-02-27th on...)
  TArrayI       fSubDetIds; // select subdets (empty: take all)
  Int_t          fAlignableTypeId; // select ony alignables of type det,detunit,rod,etc.(-1: take all)
  TString       fAdditionalSel; // selection to be set by user, used in AddBasicSelection
  TString       fAdditionalSelTitle; // title add-on for fAdditionalSel
  Float_t        fMaxDev;   // max deviation in result plot - so far only there...
};

#endif
