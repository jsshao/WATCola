#ifndef STUDENT_H
#define STUDENT_H

#include "printer.h"
#include "nameserver.h"
#include "watcardoffice.h"
#include "groupoff.h"
#include "vendingmachine.h"
#include "MPRNG.h"

#define INITIAL_VALUE 5

extern MPRNG rng;

_Task Student {
    Printer &prt;
    NameServer &nameServer;
    WATCardOffice &office;
    Groupoff &group;
    unsigned int id;
    unsigned int maxPurch;
    void main();
    void buy(WATCard::FWATCard&, VendingMachine*, VendingMachine::Flavours);
  public:
    Student( Printer &prt, NameServer &nameServer, WATCardOffice &cardOffice, Groupoff &groupoff,
             unsigned int id, unsigned int maxPurchases );
};

#endif
// vim: set filetype=ucpp :
