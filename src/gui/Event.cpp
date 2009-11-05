
#include <gui/Event.hpp>


long GetUID() {
  long static counter=0;
  return counter++;
}

