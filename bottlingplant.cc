#include "bottlingplant.h"
#include "vendingmachine.h" // Flavours::No_Of_Flavours

BottlingPlant::BottlingPlant( Printer &prt, NameServer &nameServer, 
        unsigned int numVendingMachines, unsigned int maxShippedPerFlavour, 
        unsigned int maxStockPerFlavour, unsigned int timeBetweenShipments ) :
    prt(prt), nameServer(nameServer), numVM(numVendingMachines), 
    maxShip(maxShippedPerFlavour), maxStock(maxStockPerFlavour), 
    timeDelay(timeBetweenShipments), shuttingDown(false)
{
    truck = new Truck(prt, nameServer, *this, numVM, maxStock);
}

BottlingPlant::~BottlingPlant() {
    delete truck;
}

void BottlingPlant::getShipment( unsigned int cargo[] ) {
    if (shuttingDown)
        return;

    // Deliver shipment to truck
    for (int i = 0; i < VendingMachine::Flavours::No_Of_Flavours; i++)
        cargo[i] = inventory[i];
}

void BottlingPlant::stockUp() {
    // Production run
    unsigned int generated = 0;
    for (int i = 0; i < VendingMachine::Flavours::No_Of_Flavours; i++) {
        inventory[i] = rng(0, maxShip); 
        generated += inventory[i];
    }

    prt.print(Printer::BottlingPlant, TableCell::GenerateSoda, generated);
    yield(timeDelay);
}

void BottlingPlant::main() {
    prt.print(Printer::BottlingPlant, TableCell::Start);
    stockUp();

    for (;;) {
        _Accept(~BottlingPlant)
            shuttingDown = true;
        or _Accept(getShipment) {
            if (shuttingDown) {
                _Resume Shutdown() _At truck;
                break;
            }

            prt.print(Printer::BottlingPlant, TableCell::PickUp);
            stockUp();
        }
    } 

    prt.print(Printer::BottlingPlant, TableCell::Finish);
}
