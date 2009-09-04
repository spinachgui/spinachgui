
#ifndef SPINGRID_H
#define SPINGRID_H

#include <wx/grid.h>

class SpinGrid : public wxGrid {
public:
  SpinGrid(wxWindow* parent,wxWindowID id= -1) : wxGrid(parent,id) {};
};

#endif
