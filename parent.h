#ifndef PARENT_H
#define PARENT_H
#include "printer.h"
#include "bank.h"

_Task Parent {
    void main();
    Printer *prt;
    Bank *bank;
    unsigned int numStudents, parentalDelay;
  public:
    Parent( Printer &prt, Bank &bank, unsigned int numStudents, unsigned int parentalDelay );
};

#endif
// vim: set filetype=ucpp :
