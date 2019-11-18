#include "ncurses_display.h"
#include "system.h"

/**
 * 2019-11-16 Meeting with Mohamad
 * 1. ncurses_display.cpp: 3 + n displays 3 additional non exosting processes
 * 2. Code breaks in ncurses_display when strings are converted to c strings
 */

int main() {
  System system;
  NCursesDisplay::Display(system);
}
