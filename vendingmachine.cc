#include "vendingmachine.h"

VendingMachine::VendingMachine( Printer &prt, NameServer &nameServer, unsigned int id, 
        unsigned int sodaCost, unsigned int maxStockPerFlavour ) : prt(prt), 
        nameServer(nameServer), id(id), sodaCost(sodaCost), 
        maxStockPerFlavour(maxStockPerFlavour), throwFunds(false), throwStock(false)
{
    // Register and initialize stock
    nameServer.VMregister(this);
    for (int i = 0; i < No_Of_Flavours; i++) 
        sodaInventory[i] = 0;
}

void VendingMachine::buy( Flavours flavour, WATCard &card ) {
    // Record caller for non-local exception
    student = &uThisCoroutine();
    if (card.getBalance() < sodaCost)
        throwFunds = true;
    else if (sodaInventory[flavour] == 0)
        throwStock = true;
    else {
        card.withdraw(sodaCost);
        sodaInventory[flavour] -= 1;
        prt.print(Printer::Vending, id, TableCell::BoughtSoda, flavour, sodaInventory[flavour]);
    }
    blockBuyer.wait();
}

unsigned int* VendingMachine::inventory() {
    return sodaInventory;
}
void VendingMachine::restocked() {}

_Nomutex unsigned int VendingMachine::cost() {
    return sodaCost;
}

_Nomutex unsigned int VendingMachine::getId() {
    return id;
}

void VendingMachine::main() {
    prt.print(Printer::Vending, id, TableCell::Start, sodaCost);

    for (;;) {
        _Accept(~VendingMachine) {
            break;
        } or _Accept(inventory) {
            // Block until stocking is complete
            prt.print(Printer::Vending, id, TableCell::ReloadBegin);
            _Accept(restocked);
            prt.print(Printer::Vending, id, TableCell::ReloadDone);
        } or _Accept(buy) {
            if (throwFunds) {
                _Resume Funds() _At *student; 
                throwFunds = false;
            } else if (throwStock) {
                _Resume Stock() _At *student;
                throwStock = false;
            }
            
            blockBuyer.signal();
        }
    }
    prt.print(Printer::Vending, id, TableCell::Finish);
}
