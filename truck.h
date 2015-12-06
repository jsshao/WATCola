#ifndef TRUCK_H
#define TRUCK_H

#include "printer.h"
#include "nameserver.h"
#include "MPRNG.h"
#include "vendingmachine.h"

extern MPRNG rng;
_Task BottlingPlant;

_Task Truck {
    void main();
    Printer &prt;
    NameServer &nameServer;
    BottlingPlant &plant;
    unsigned int numVM;
    unsigned int maxStock;
    unsigned int nextVM;
    unsigned int cargo[VendingMachine::Flavours::No_Of_Flavours];
  public:
    Truck( Printer &prt, NameServer &nameServer, BottlingPlant &plant,
           unsigned int numVendingMachines, unsigned int maxStockPerFlavour );
};

#endif
// vim: set filetype=ucpp :
