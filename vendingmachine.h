#ifndef VENDINGMACHINE_H
#define VENDINGMACHINE_H

#include "watcard.h"
#include "printer.h"
#include "nameserver.h"

_Task VendingMachine {
  public:
    enum Flavours { Black_Cherry, Cream_Soda, Root_Beer, Jazz_Lime, No_Of_Flavours };
    _Event Funds {};                       // insufficient funds
    _Event Stock {};                       // out of stock for particular flavour
    VendingMachine( Printer &prt, NameServer &nameServer, unsigned int id, unsigned int sodaCost,
                    unsigned int maxStockPerFlavour );
    void buy( Flavours flavour, WATCard &card );
    unsigned int *inventory();
    void restocked();
    _Nomutex unsigned int cost();
    _Nomutex unsigned int getId();
  private:
    Printer &prt;
    NameServer &nameServer;
    unsigned int id;
    unsigned int sodaCost;
    unsigned int maxStockPerFlavour;
    uBaseCoroutine *student;
    unsigned int sodaInventory[No_Of_Flavours];
    uCondition blockBuyer;
    bool throwFunds;
    bool throwStock;
    void main();
};

#endif
// vim: set filetype=ucpp :
