
#ifndef SPINGRID_H
#define SPINGRID_H

#include <sigc++/sigc++.h>
#include <shared/spinsys.hpp>

#include <wx/grid.h>
#include <set>


class SpinGrid : public wxGrid, public sigc::trackable {
public:
    SpinGrid(wxWindow* parent);

    //wx Event Handlers
    void OnEdit(wxGridEvent& e);
    void OnEndEdit(wxGridEvent& e);
    void OnCellChange(wxGridEvent& e);
    void OnCellSelect(wxGridEvent& e);
    void OnRightClick(wxGridEvent& e);
    void OnDeleteSpinHover(wxCommandEvent& e);
    void OnMouseMove(wxMouseEvent& e);

    //sigc slots
    void OnNewSpin(SpinXML::Spin* newSpin,long number);
    void SlotHover(SpinXML::Spin* spin);
    void SlotSelectChange(std::set<SpinXML::Spin*> spins);
    void SlotAnySpinDie(SpinXML::Spin* spin);

    //Overridden in order to emit a signal
    bool DeleteRows(int pos=0,int numRows=1,bool updateLables=true);

    enum COL_TYPE {
        COL_SELECTED,
        COL_LABEL,
        COL_ELEMENT,
        COL_ISOTOPES,
        COL_X,
        COL_Y,
        COL_Z
    };

    enum MENU_EVT {
        MENU_SPIN_DIALOG,
        MENU_DELETE_SPINS,
        MENU_NEW_SPIN
    };

    sigc::signal<void,SpinXML::Spin*> sigSelect;
protected:
    DECLARE_EVENT_TABLE();

    void RefreshFromSpinSystem();
    void UpdateRowIsotopes(long row);
    void SetupRow(long rowNumber);
    void UpdateRow(long rowNumber);
    void ColourRow(long rowNumber,const wxColor& c);

private:
    ~SpinGrid() {}
    struct SpinGridColum {
        COL_TYPE type;
        const char* name;
        long width;
    };
    const static SpinGridColum columns[];
    bool mUpdating;
    long mLastHover;
};


#endif
