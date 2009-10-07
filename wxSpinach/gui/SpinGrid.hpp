
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
  void OnRightClick(wxGridEvent& e);

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

  enum MENU_EVENT {
    MENU_SPIN_DIALOG,
    MENU_DELETE_SPINS,
    MENU_NEW_SPIN
  };

protected:
  DECLARE_EVENT_TABLE();

  void ShowPopup(SpinXML::Spin* spin);
  void HidePopup();
  void RefreshFromSpinSystem();
  void UpdateRow(long rowNumber);
  void SetupRow(long rowNumber);

private:
  struct SpinGridColum {
    long type;
    const char* name;
    long width;
  };
  const static SpinGridColum columns[];

  const SpinSysPtr* mHead;

  InterPopup* mInterPopup;
  bool mPopupLock;
};

#endif
