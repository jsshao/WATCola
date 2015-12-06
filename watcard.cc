#include "watcard.h"

WATCard::WATCard() : balance(0) {}

void WATCard::deposit( unsigned int amount ) {
    moneyLock.acquire();
    balance += amount;
    moneyLock.release();
}

void WATCard::withdraw( unsigned int amount ) {
    moneyLock.acquire();
    assert(balance >= amount);
    balance -= amount;
    moneyLock.release();
}

unsigned int WATCard::getBalance() {
    // Return most updated balance
    try {
        moneyLock.acquire();
        return balance;
    } _Finally {
        moneyLock.release();
    }
}

// vim: set filetype=ucpp :
