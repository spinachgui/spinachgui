
#ifndef SPINGRID_H
#define SPINGRID_H

#include <gui/SpinInteractionEdit.hpp>
#include <shared/spinsys.hpp>
#include <sigc++/sigc++.h>
#include <wx/grid.h>



class SpinGrid : public wxGrid, public sigc::trackable {
public:
  SpinGrid(wxWindow* parent,wxWindowID id= -1);
  ~SpinGrid() {sigDying();}

  //wx Event Handlers
  void OnEdit(wxGridEvent& e);
  void OnEndEdit(wxGridEvent& e);
  void OnCellChange(wxGridEvent& e);
  void OnCellSelect(wxGridEvent& e);
  void OnRightClick(wxGridEvent& e);
  void OnDeleteSpinHover(wxCommandEvent& e);
  
  void OnNewSpin(SpinXML::Spin* newSpin,long number);

  //Overridden in order to emit a signal
  bool DeleteRows(int pos=0,int numRows=1,bool updateLables=true);

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

  sigc::signal<void,COL_TYPE,SpinXML::Spin*> sigSelect;
  sigc::signal<void> sigDying;
  sigc::signal<void> sigClearing;
  sigc::signal<void,int,int> sigRowDelete;
protected:
  DECLARE_EVENT_TABLE();

  void RefreshFromSpinSystem();
  void UpdateRowIsotopes(long row);
  void SetupRow(long rowNumber);

private:
  struct SpinGridColum {
    COL_TYPE type;
    const char* name;
    long width;
  };
  const static SpinGridColum columns[];
  SpinXML::SpinSystem* mSS;
  bool mUpdating;

};

class SpinGridPanel : public wxPanel, public sigc::trackable {
public:
  SpinGridPanel(wxWindow* parent,wxWindowID id=-1);

  void OnGridCellSelect(SpinGrid::COL_TYPE col,SpinXML::Spin* spin);

private:
  SpinInterEditPanel* mInterEdit;
  SpinGrid* mGrid;
};

#endif
