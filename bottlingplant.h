#ifndef BOTTLINGPLANT_H
#define BOTTLINGPLANT_H

#include "nameserver.h"
#include "printer.h"
#include "MPRNG.h"
#include "truck.h"

extern MPRNG rng;

_Task BottlingPlant {
    void main();
    void stockUp();
    Printer &prt;
    NameServer &nameServer;
    unsigned int numVM;
    unsigned int maxShip;
    unsigned int maxStock;
    unsigned int timeDelay;
    Truck *truck;
    bool shuttingDown;
    unsigned int inventory[VendingMachine::Flavours::No_Of_Flavours];
  public:
    _Event Shutdown {};                    // shutdown plant
    BottlingPlant( Printer &prt, NameServer &nameServer, unsigned int numVendingMachines,
                 unsigned int maxShippedPerFlavour, unsigned int maxStockPerFlavour,
                 unsigned int timeBetweenShipments );
    ~BottlingPlant();
    void getShipment( unsigned int cargo[] );
};
#endif
// vim: set filetype=ucpp :
