#include "bank.h"
#include <iostream>
using namespace std;

Bank::Bank( unsigned int numStudents ) : numStudents(numStudents) {
    for (unsigned int i = 0; i < numStudents; i++ ) {
        accounts[i] = 0;
    }
}

void Bank::deposit( unsigned int id, unsigned int amount ) {
    accounts[id] += amount; 
    /* cout << amount << " added to " << id << ", now " << accounts[id] << endl; */
}

void Bank::withdraw( unsigned int id, unsigned int amount ) {
    while (accounts[id] < amount) {
        _Accept(deposit);
    }
    assert(accounts[id] >= amount);
    accounts[id] -= amount;
}

// vim: set filetype=ucpp :
