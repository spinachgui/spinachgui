
#ifndef SPINGRID_H
#define SPINGRID_H

#include <gui/Event.hpp>
#include <gui/SpinInteractionEdit.hpp>
#include <shared/spinsys.hpp>
#include <boost/shared_ptr.hpp>
#include <wx/grid.h>


class SpinGrid : public wxGrid, public IEventListener {
public:
  SpinGrid(wxWindow* parent,wxWindowID id= -1);


  //wx Event Handlers
  void OnEdit(wxGridEvent& e);
  void OnEndEdit(wxGridEvent& e);
  void OnCellChange(wxGridEvent& e);
  void OnCellSelect(wxGridEvent& e);
  void OnRightClick(wxGridEvent& e);
  void OnDeleteSpinHover(wxCommandEvent& e);

  //Other event handlers
  void OnChange();
  void OnAnnihilation();

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
    COL_QUAD
  };

  enum MENU_EVENT {
    MENU_SPIN_DIALOG,
    MENU_DELETE_SPINS,
    MENU_NEW_SPIN
  };

protected:
  DECLARE_EVENT_TABLE();

  void RefreshFromSpinSystem();
  void UpdateRow(long rowNumber);
  void UpdateRowIsotopes(long row);
  void SetupRow(long rowNumber);

private:
  struct SpinGridColum {
    long type;
    const char* name;
    long width;
  };
  const static SpinGridColum columns[];

  const SpinSysPtr* mHead;
  bool mUpdating;
};

class SpinGridPanel : public wxPanel {
public:
  SpinGridPanel(wxWindow* parent,wxWindowID id=-1);

  void OnInterSelect(wxCommandEvent& e);
  void OnInterUnSelect(wxCommandEvent& e);
protected:
  DECLARE_EVENT_TABLE();

private:
  SpinInterEditPanel* mInterEdit;
  SpinGrid* mGrid;
};

#endif
