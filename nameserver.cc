#include "nameserver.h"
#include "vendingmachine.h"

NameServer::NameServer( Printer &prt, unsigned int numVendingMachines, 
        unsigned int numStudents ): prt(prt), numVM(numVendingMachines), 
        numStud(numStudents), registeredVM(0)
{
    // Distribute the students across vending machines in round robin order
    vendingMachines = new VendingMachine* [numVM];
    assignedVM = new unsigned int[numStud];
    for (unsigned int studentID = 0; studentID < numStudents; studentID++)
        assignedVM[studentID] = studentID % numVM;
}

NameServer::~NameServer() {
    delete [] assignedVM;
    delete [] vendingMachines;
}

void NameServer::VMregister( VendingMachine *vendingmachine ) {
    prt.print(Printer::NameServer, TableCell::RegisterVM, vendingmachine->getId());
    vendingMachines[registeredVM] = vendingmachine;
}

VendingMachine *NameServer::getMachine( unsigned int id ) {
    unsigned int assigned = assignedVM[id];

    prt.print(Printer::NameServer, TableCell::NewVM, id, vendingMachines[assigned]->getId());
    assignedVM[id] = (assignedVM[id] + 1) % numVM;
    return vendingMachines[assigned];
}

VendingMachine **NameServer::getMachineList() {
    return vendingMachines;
}

void NameServer::main() {
    prt.print(Printer::NameServer, TableCell::Start);

    for (;;) {
        _Accept(~NameServer)
            break;
        or _When (registeredVM < numVM) _Accept(VMregister)
            registeredVM += 1;
        or _When (registeredVM == numVM) _Accept(getMachine, getMachineList);
    }

    prt.print(Printer::NameServer, TableCell::Finish);
}
