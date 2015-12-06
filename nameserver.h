#ifndef NAMESERVER_H
#define NAMESERVER_H

#include "printer.h"

// Forward declaration to avoid circular dependency
_Task VendingMachine;

_Task NameServer {
    void main();
    Printer &prt;
    unsigned int numVM;
    unsigned int numStud;
    unsigned int *assignedVM;
    VendingMachine **vendingMachines;
    unsigned int registeredVM;
    unsigned int lastMachine;
  public:
    NameServer( Printer &prt, unsigned int numVendingMachines, unsigned int numStudents );
    ~NameServer();
    void VMregister( VendingMachine *vendingmachine );
    VendingMachine *getMachine( unsigned int id );
    VendingMachine **getMachineList();
};

#endif
// vim: set filetype=ucpp :

