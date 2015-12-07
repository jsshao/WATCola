#include "parent.h"
#include "printer.h"
#include "MPRNG.h"

extern MPRNG rng;
Parent::Parent( Printer& prt, Bank &bank, unsigned int numStudents, unsigned int parentalDelay) : prt(&prt), bank(&bank), numStudents(numStudents), parentalDelay(parentalDelay) {
    
}

void Parent::main() {
    prt->print(Printer::Parent, TableCell::Start);
    for (;;) {
        _Accept(~Parent) {
            break;
        } _Else {
            yield(parentalDelay); 
            unsigned int student = rng(numStudents - 1);
            unsigned int money = rng(1, 3);
            prt->print(Printer::Parent, TableCell::Deposit, student, money);
            bank->deposit(student, money);
        }
    }
    prt->print(Printer::Parent, TableCell::Finish);
}

// vim: set filetype=ucpp :
