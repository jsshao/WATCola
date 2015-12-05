#include <iostream>
#include <string>
#include "config.h"
#include "printer.h"
#include "MPRNG.h"
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
    processConfigFile(config_file.c_str(), cparms);
    Printer printer(cparms.numStudents, cparms.numVendingMachines, cparms.numCouriers);
    printer.print(Printer::Parent, TableCell::Start); 
    printer.print(Printer::Groupoff, TableCell::TransferDone, 69); 
    printer.print(Printer::Student, 1, TableCell::LostCard); 
    printer.print(Printer::Courier, 0, TableCell::LostCard, 25, 99); 
    printer.print(Printer::Courier, 0, TableCell::BoughtSoda, 25); 
    printer.print(Printer::Parent, TableCell::Finish); 


    
}
// vim: set filetype=ucpp :
