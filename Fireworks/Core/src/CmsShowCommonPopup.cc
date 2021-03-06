#include "TGFont.h"
#include "TGButton.h"
#include "TGLabel.h"
#include "TGSlider.h"
#include "TGComboBox.h"
#include "TGClient.h"
#include "TG3DLine.h"
#include "TGResourcePool.h"

#include "Fireworks/Core/interface/CmsShowCommonPopup.h"
#include "Fireworks/Core/interface/CmsShowCommon.h"
#include "Fireworks/Core/interface/FWColorManager.h"
#include "Fireworks/Core/src/FWDialogBuilder.h"
#include "Fireworks/Core/src/FWColorSelect.h"
#include "Fireworks/Core/interface/FWViewEnergyScaleEditor.h"

#include "Fireworks/Core/interface/FWParameterSetterEditorBase.h"

#include "Fireworks/Core/interface/FWParameterSetterBase.h"

CmsShowCommonPopup::CmsShowCommonPopup(CmsShowCommon* model, const TGWindow* p, UInt_t w, UInt_t h) :
   TGTransientFrame(gClient->GetDefaultRoot(),p,w,h),
   m_common(model),
   m_backgroundButton(0),
   m_gammaSlider(0),
   m_gammaButton(0)
{
   SetCleanup(kDeepCleanup);

   TGGC *fTextGC;
   const TGFont *font = gClient->GetFont("-adobe-helvetica-bold-r-*-*-14-*-*-*-*-*-iso8859-1");
   if (!font)
      font = gClient->GetResourcePool()->GetDefaultFont();
   GCValues_t   gval;
   gval.fMask = kGCBackground | kGCFont | kGCForeground;
   gval.fFont = font->GetFontHandle();
   fTextGC = gClient->GetGC(&gval, kTRUE);

   TGCompositeFrame* vf2 = new TGVerticalFrame(this);
   AddFrame(vf2, new TGLayoutHints(kLHintsExpandX, 2, 2, 2, 2));
   //==============================================================================
   // scales
   //
   {
      TGLabel* xx = new TGLabel(vf2, "GlobalScales     ", fTextGC->GetGC());
      vf2->AddFrame(xx, new TGLayoutHints(kLHintsLeft ,2,2,4,4));
   }
   FWViewEnergyScaleEditor* scaleEditor = new FWViewEnergyScaleEditor(m_common->m_energyScale.get(), vf2);
   vf2->AddFrame(scaleEditor);
   //==============================================================================
   // Projections
   //
   vf2->AddFrame(new TGHorizontal3DLine(vf2),  new TGLayoutHints(kLHintsExpandX,4 ,8, 3, 3));
   {
      TGLabel* xx = new TGLabel(vf2, "Projections     ", fTextGC->GetGC());
      vf2->AddFrame(xx, new TGLayoutHints(kLHintsLeft ,2,2,4,4));
   }
   vf2->AddFrame(new TGLabel(vf2, "Track behaviour when crossing y=0 in RhoZ-view:"), new TGLayoutHints(kLHintsLeft ,2,2,0,0));
   makeSetter(vf2, &m_common->m_trackBreak);
   makeSetter(vf2, &m_common->m_drawBreakPoints);

   //==============================================================================
   // brigtness
   //

   vf2->AddFrame(new TGHorizontal3DLine(vf2),  new TGLayoutHints(kLHintsExpandX ,4 ,8, 8, 2));
   {
      TGLabel* xx = new TGLabel(vf2, "General Colors        ", fTextGC->GetGC());
      vf2->AddFrame(xx, new TGLayoutHints(kLHintsLeft,2,2,4,4));
   }
   m_backgroundButton = new TGTextButton(vf2, "Black/White Background");
   vf2->AddFrame(m_backgroundButton);
   makeSetter(vf2, &m_common->m_gamma);


   //==============================================================================
   // geom colors
   //

   {
      TGLabel* xx = new TGLabel(vf2, "DetectorColors       ", fTextGC->GetGC());
      vf2->AddFrame(xx, new TGLayoutHints(kLHintsLeft,2,2,8,0));
   }

   TGFont* smallFont = 0;
   FontStruct_t defaultFontStruct = m_backgroundButton->GetDefaultFontStruct();
   try
   { 
      TGFontPool *pool = gClient->GetFontPool();
      TGFont* defaultFont = pool->GetFont(defaultFontStruct);
      FontAttributes_t attributes = defaultFont->GetFontAttributes();
      smallFont = pool->GetFont(attributes.fFamily, 8,  attributes.fWeight, attributes.fSlant);                                      
   } 
   catch(...)
   {
      // Ignore exceptions.
   }

  
   TGHSlider* transpWidget2D = 0;
   TGHSlider* transpWidget3D = 0;
   TGCompositeFrame* top  = new TGVerticalFrame(vf2);
   vf2->AddFrame(top, new TGLayoutHints(kLHintsNormal, 2, 2, 2, 0));

   {
      TGHorizontalFrame* hf = new TGHorizontalFrame(top); 
      hf->AddFrame(new TGLabel(hf, "Tansparency 2D:"), new TGLayoutHints(kLHintsBottom, 2,  2, 3, 3));
      transpWidget2D = new TGHSlider(hf, 100, kSlider1);
      hf->AddFrame( transpWidget2D);
      top->AddFrame(hf,new TGLayoutHints( kLHintsNormal, 0, 0, 2, 2));
   }
   {
      TGHorizontalFrame* hf = new TGHorizontalFrame(top); 
      hf->AddFrame(new TGLabel(hf, "Tansparency 3D:") , new TGLayoutHints(kLHintsBottom,2, 2, 2, 2 ));
      transpWidget3D = new TGHSlider(hf, 100, kSlider1);
      hf->AddFrame( transpWidget3D);
      top->AddFrame(hf, new TGLayoutHints(kLHintsExpandX, 0, 0, 0, 10));
   }
   std::string names[kFWGeomColorSize];
   names[kFWPixelBarrelColorIndex   ] = "Pixel Barrel";
   names[kFWPixelEndcapColorIndex   ] = "Pixel Endcap" ;
   names[kFWTrackerBarrelColorIndex ] = "Tracker Barrel";
   names[kFWTrackerEndcapColorIndex ] = "Tracker Endcap";
   names[kFWMuonBarrelLineColorIndex] = "Muon Barrel";
   names[kFWMuonEndcapLineColorIndex] = "Muon Endcap";
   int i = 0;
   for (int k = 0; k < 3; ++k)
   {
      TGHorizontalFrame* hf = new TGHorizontalFrame(top); 
      top->AddFrame(hf);

      for (int j = 0 ; j < 2; ++j)
      {
         m_colorSelectWidget[i] = new FWColorSelect(hf, names[i].c_str(), 0, m_common->colorManager(), i);
         hf->AddFrame(m_colorSelectWidget[i]); 
         m_colorSelectWidget[i]->SetColorByIndex(m_common->colorManager()->geomColor(FWGeomColorIndex(i)) ,kFALSE);
         m_colorSelectWidget[i]->Connect("ColorChosen(Color_t)", "CmsShowCommonPopup", this, "changeGeomColor(Color_t)");

         TGFrame* lf = new TGHorizontalFrame(hf, 100, 16, kFixedSize);
         TGLabel* label = new TGLabel(lf, m_colorSelectWidget[i]->label().c_str());
         label->SetTextFont(smallFont);
         hf->AddFrame(lf, new TGLayoutHints(kLHintsLeft |kLHintsCenterY , 0, 0, 0,0)); 

         ++i;
      }
   }
   //==============================================================================

   m_backgroundButton->Connect("Clicked()", "CmsShowCommonPopup", this, "switchBackground()");

   transpWidget2D->SetRange(0, 100);
   transpWidget2D->SetPosition(m_common->colorManager()->geomTransparency(true));
   transpWidget2D->Connect("PositionChanged(Int_t)", "CmsShowCommonPopup", this, "changeGeomTransparency2D(Int_t)");

   transpWidget3D->SetRange(0, 100);
   transpWidget3D->SetPosition(m_common->colorManager()->geomTransparency(false));
   transpWidget3D->Connect("PositionChanged(Int_t)", "CmsShowCommonPopup", this, "changeGeomTransparency3D(Int_t)");

   SetWindowName("Common Preferences");
   MapSubwindows();
   Resize(GetDefaultSize());
   Layout();
   CenterOnParent(kTRUE, TGTransientFrame::kTopRight);
}

CmsShowCommonPopup::~CmsShowCommonPopup()
{
}

void
CmsShowCommonPopup::switchBackground()
{
   m_common->switchBackground();
}
 

void
CmsShowCommonPopup::changeGeomColor(Color_t iColor)
{
   TGColorSelect *cs = (TGColorSelect *) gTQSender;
   FWGeomColorIndex cidx = FWGeomColorIndex(cs->WidgetId());
   m_common->setGeomColor(cidx, iColor);
}

void
CmsShowCommonPopup::changeGeomTransparency2D(int iTransp)
{
   m_common->setGeomTransparency(iTransp, true);
}

void
CmsShowCommonPopup::changeGeomTransparency3D(int iTransp)
{
   m_common->setGeomTransparency(iTransp, false);
}


/* Called by FWGUIManager when change background. */
void 
CmsShowCommonPopup::colorSetChanged()
{
   for (int i = 0 ; i < kFWGeomColorSize; ++i)
      m_colorSelectWidget[i]->SetColorByIndex(m_common->colorManager()->geomColor(FWGeomColorIndex(i)), kFALSE);
   
}


void
CmsShowCommonPopup::makeSetter(TGCompositeFrame* frame, FWParameterBase* param) 
{
   boost::shared_ptr<FWParameterSetterBase> ptr( FWParameterSetterBase::makeSetterFor(param) );
   ptr->attach(param, this);
 
   TGFrame* pframe = ptr->build(frame);
   frame->AddFrame(pframe, new TGLayoutHints(kLHintsExpandX, 0, 0, 2,2));

   m_setters.push_back(ptr);
}
