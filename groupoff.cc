#include "groupoff.h"
#include "watcard.h"
#include "MPRNG.h"
#include <algorithm>

using namespace std;
extern MPRNG rng;

Groupoff::Groupoff( Printer &prt, unsigned int numStudents, unsigned int sodaCost, unsigned int groupoffDelay ) : prt(&prt), numStudents(numStudents), sodaCost(sodaCost), groupoffDelay(groupoffDelay) {}

Groupoff::~Groupoff() {
    for (unsigned int i = 0; i < issued_cards.size(); i++) {
        delete issued_cards[i];
    }
}

WATCard::FWATCard Groupoff::giftCard() {
    futures.push_back(WATCard::FWATCard());
    return futures.back();
}

void Groupoff::main() {
    prt->print(Printer::Groupoff, TableCell::Start);
    for (unsigned int i = 0; i < numStudents; i++) {
        _Accept(giftCard);
    }
    for (unsigned int i = 0; i < numStudents; i++) {
        _Accept(~Groupoff) {
            break;
        } _Else {
            yield(groupoffDelay);
            WATCard *card = new WATCard();      // create the real WATCard and load it with sodaCost
            card->deposit(sodaCost);
            WATCard::FWATCard fcard;
            unsigned int random = rng(futures.size() - 1);       // pick a future at random to fulfill
            if (random != futures.size() - 1) {     
                swap(futures[random], futures.back());      // swap the future to the back for removal
            }
            fcard = futures.back();
            futures.pop_back();                         
            issued_cards.push_back(card);
            fcard.delivery(card);
            prt->print(Printer::Groupoff, TableCell::Deposit, sodaCost);
        } 
    }
    prt->print(Printer::Groupoff, TableCell::Finish);
}

// vim: set filetype=ucpp :
