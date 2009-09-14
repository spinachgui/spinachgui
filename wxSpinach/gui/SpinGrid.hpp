
#ifndef SPINGRID_H
#define SPINGRID_H

#include <gui/InteractionEdit.hpp>
#include <shared/spinsys.hpp>
#include <boost/shared_ptr.hpp>
#include <wx/grid.h>

class SpinGrid : public wxGrid {
public:
  SpinGrid(wxWindow* parent,wxWindowID id= -1);


  //Event Handlers
  void OnEdit(wxGridEvent& e);
  void OnEndEdit(wxGridEvent& e);
  void OnCellChange(wxGridEvent& e);
  void OnCellSelect(wxGridEvent& e);

  enum COL_TYPE {
    COL_SELECTED,   
    COL_LABEL,       
    COL_ELEMENT,
    COL_ISOTOPES,
    COL_X,
    COL_Y,          
    COL_Z,
    COL_LINEAR,
    COL_BILINEAR,
    COL_QUADRAPOLAR
  };


protected:
  DECLARE_EVENT_TABLE();

  void ShowPopup(SpinXML::Spin* spin);
  void HidePopup();
  void RefreshFromSpinSystem();

private:
  struct SpinGridColum {
    long type;
    const char* name;
    long width;
  };
  const static SpinGridColum columns[];

  boost::shared_ptr<SpinXML::SpinSystem> mSS;

  InterPopup* mInterPopup;
  bool mPopupLock;
};

#endif
