#include <iostream>
#include <string>
#include "config.h"
#include "printer.h"
#include "parent.h"
#include "bank.h"
#include "watcardoffice.h"
#include "MPRNG.h"
#include "nameserver.h"
#include "vendingmachine.h"
#include "bottlingplant.h"
#include "student.h"
#include "groupoff.h"
using namespace std;

MPRNG rng;

// This function prints a usage error to stderr and terminates program with exit code 1
void usage(char *exec) {
    cerr << "Usage: " << exec << " [ config-file [ random-seed (> 0) ] ]";
    exit( EXIT_FAILURE );
}

// Converts a string input to its integer. Throws an exception for
// Invalid input or if its value is less than min
uint32_t convertStringToInt(char *str, int min) {
    int result;
    size_t processed_char;

    result = stoi(str, &processed_char);
    if (processed_char != strlen(str) || result < min)
        throw invalid_argument("Not an integer or value too small");

    return result;
}

void uMain::main() {
    string config_file = "soda.config";
    unsigned int seed = getpid();

    switch(argc) {
        case 3:
            try {
                seed = convertStringToInt(argv[2], 1);
            } catch (invalid_argument &e) {
                usage(argv[0]);
            }
        case 2:
            config_file = string(argv[1]);
        case 1:
            break;
        default:
            usage(argv[0]);
    }

    rng.seed(seed);
    ConfigParms cparms;

    // Read in the config file
    processConfigFile(config_file.c_str(), cparms);
    cout << "DEBUG INFO" << endl;
    cout << "sodaCost: " << cparms.sodaCost << endl;				
    cout << "numStudents: " << cparms.numStudents << endl;		
    cout << "maxPurchases: " << cparms.maxPurchases << endl;			
    cout << "numVendingMachines: " << cparms.numVendingMachines << endl;	
    cout << "maxStockPerFlavour: " << cparms.maxStockPerFlavour << endl;	
    cout << "maxShippedPerFlavour: " << cparms.maxShippedPerFlavour << endl;	
    cout << "timeBetweenShipments: " << cparms.timeBetweenShipments << endl;	
    cout << "groupoffDelay: " << cparms.groupoffDelay << endl;			
    cout << "parentalDelay: " << cparms.parentalDelay << endl;			
    cout << "numCouriers: " << cparms.numCouriers << endl;			
    Printer printer(cparms.numStudents, cparms.numVendingMachines, cparms.numCouriers);
    NameServer nameServer(printer, cparms.numVendingMachines, cparms.numStudents);

    VendingMachine *vendingmachine[cparms.numVendingMachines];
    for (unsigned int id = 0; id < cparms.numVendingMachines; id++)
        vendingmachine[id] = new VendingMachine(printer, nameServer, id, cparms.sodaCost, 
                                               cparms.maxStockPerFlavour);

    BottlingPlant plant(printer, nameServer, cparms.numVendingMachines,
                        cparms.maxShippedPerFlavour, cparms.maxStockPerFlavour,
                        cparms.timeBetweenShipments);
    Bank bank(cparms.numStudents);
    Parent parent(printer, bank, cparms.numStudents, cparms.parentalDelay);
    WATCardOffice office(printer, bank, cparms.numCouriers);
    Groupoff group(printer, cparms.numStudents, cparms.sodaCost, cparms.groupoffDelay);

    Student *students[cparms.numStudents];
    for (unsigned int id = 0; id < cparms.numStudents; id++)
        students[id] = new Student(printer, nameServer, office, group, id, cparms.maxPurchases);

    yield(150);
    cout << "FINISHED SLEEPING" << endl;

    for (unsigned int id = 0; id < cparms.numStudents; id++)
        delete students[id];

    /* MOCKED STUDENT BEHAVIOUR */
    /* printer.print(Printer::Student, 0, TableCell::Start); */ 
    /* WATCard::FWATCard fcard = office.create(0, 5); */
    /* WATCard *card = fcard(); */
    /* printer.print(Printer::Student, 0, TableCell::CreateDone); */ 
    /* cout << "Card Received with balance: " << card->getBalance() << endl; */
    /* fcard = office.transfer(0, 4, card); */
    /* card = fcard(); */
    /* cout << "Card updated with new balance: " << card->getBalance() << endl; */
    /* delete card; */
    
    /* fcard = group.giftCard(); */
    /* card = fcard(); */
    /* printer.print(Printer::Student, 0, TableCell::CreateDone); */ 
    /* cout << "Giftcard Received with balance: " << card->getBalance() << endl; */

        

    for (unsigned int id = 0; id < cparms.numVendingMachines; id++)
        delete vendingmachine[id];
}
// vim: set filetype=ucpp :
