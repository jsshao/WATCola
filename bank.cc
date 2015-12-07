#include "bank.h"
#include <iostream>
using namespace std;

Bank::Bank( unsigned int numStudents ) : numStudents(numStudents) {
    for (unsigned int i = 0; i < numStudents; i++ ) {
        accounts.push_back(0);
        desired.push_back(0);
        conds.push_back(new uCondition);
    }
}

Bank::~Bank() {
    for (unsigned int i = 0; i < numStudents; i++) {
        delete conds[i];
    }
}

void Bank::deposit( unsigned int id, unsigned int amount ) {
    accounts[id] += amount; 
    if (!conds[id]->empty() && accounts[id] >= desired[id]) {
        conds[id]->signal();
    }
}

void Bank::withdraw( unsigned int id, unsigned int amount ) {
    if (accounts[id] < amount) {        // no busy waiting
        desired[id] = amount;
        conds[id]->wait();
    }
    assert(accounts[id] >= amount);
    accounts[id] -= amount;
}

// vim: set filetype=ucpp :
