#ifndef TRUCK_H
#define TRUCK_H

_Task Truck {
    void main();
  public:
    Truck( Printer &prt, NameServer &nameServer, BottlingPlant &plant,
           unsigned int numVendingMachines, unsigned int maxStockPerFlavour );
};

#define TRUCK_H
// vim: set filetype=ucpp :
