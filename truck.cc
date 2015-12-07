#include "truck.h"
#include "bottlingplant.h"

Truck::Truck( Printer &prt, NameServer &nameServer, BottlingPlant &plant,
        unsigned int numVendingMachines, unsigned int maxStockPerFlavour ) :
    prt(prt), nameServer(nameServer), plant(plant), numVM(numVendingMachines),
    maxStock(maxStockPerFlavour), nextVM(0) 
{
    for (int i = 0; i < VendingMachine::Flavours::No_Of_Flavours; i++)
        cargo[i] = 0;    
}

void Truck::main() {
    VendingMachine **locations = nameServer.getMachineList();
    prt.print(Printer::Truck, TableCell::Start);

    for (;;) {
        try {
            _Enable {
                // Get coffee from Tim Hortons
                yield(rng(1, 10));

                // Ignore existing cargo by letting the plant overwrite them
                plant.getShipment(cargo);
            }
        } catch (BottlingPlant::Shutdown &e) {
            break;
        }

        // Keep track of total cargo
        unsigned int totalCargo = 0;
        for (int i = 0; i < VendingMachine::Flavours::No_Of_Flavours; i++)
            totalCargo += cargo[i];

        prt.print(Printer::Truck, TableCell::PickUp, totalCargo);

        // Stock up the vending machines
        for (unsigned int offset = 0; offset < numVM; offset++) {
            if (totalCargo == 0)
                break;

            VendingMachine *curVM = locations[(nextVM + offset) % numVM];
            prt.print(Printer::Truck, TableCell::DeliverBegin, curVM->getId(), totalCargo);
            unsigned int *inventory = curVM->inventory();
            unsigned int VMTotal = 0;

            for (int flav = 0; flav < VendingMachine::Flavours::No_Of_Flavours; flav++) {
                // Try to fill it to max
                unsigned int amount = std::min(maxStock - inventory[flav], cargo[flav]);
                totalCargo -= amount;
                cargo[flav] -= amount;
                inventory[flav] += amount;
                VMTotal += inventory[flav];
            }
           
            // Check if vending machine was completely filled
            if (VMTotal != maxStock * VendingMachine::Flavours::No_Of_Flavours)
                prt.print(Printer::Truck, TableCell::Unsuccessful, curVM->getId(), 
                    maxStock * VendingMachine::Flavours::No_Of_Flavours - VMTotal);
            
            curVM->restocked();
            prt.print(Printer::Truck, TableCell::DeliverDone, curVM->getId(), totalCargo);
            nextVM = (nextVM + 1) % numVM;
        }
    }

    prt.print(Printer::Truck, TableCell::Finish);
}
