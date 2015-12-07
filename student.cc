#include "student.h"

Student::Student( Printer &prt, NameServer &nameServer, WATCardOffice &cardOffice, 
        Groupoff &groupoff, unsigned int id, unsigned int maxPurchases ): 
    prt(prt), nameServer(nameServer), office(cardOffice), group(groupoff), id(id),
    maxPurch(maxPurchases) {}

void Student::main() {
    unsigned int numOfPurch = rng(1, maxPurch);
    VendingMachine::Flavours favFlavour = static_cast<VendingMachine::Flavours>
        (rng(0, VendingMachine::Flavours::No_Of_Flavours - 1));
    prt.print(Printer::Student, id, TableCell::Start, favFlavour, numOfPurch);

    WATCard::FWATCard futureWatCard = office.create(id, INITIAL_VALUE);
    WATCard::FWATCard futureGiftCard = group.giftCard();
    WATCard *card = NULL;
    WATCard *giftcard = NULL;
    VendingMachine *loc = nameServer.getMachine(id);
    prt.print(Printer::Student, id, TableCell::SelectVM, loc->getId());

    
    for (unsigned int i = 0; i < numOfPurch; i++) {
        yield(rng(1, 10));
        for (;;) {
            _Select (futureWatCard) {
                try {
                    futureWatCard();
                } catch (WATCardOffice::Lost &e) {
                    prt.print(Printer::Student, id, TableCell::LostCard);

                    // Create new watcard
                    futureWatCard = office.create(id, INITIAL_VALUE);
                    continue;
                }

                try {
                    _Enable {
                        card = futureWatCard();
                        loc->buy(favFlavour, *card);
                        prt.print(Printer::Student, id, TableCell::BoughtSoda, card->getBalance());
                        break;
                    }
                } catch (VendingMachine::Funds &e) {
                    futureWatCard = office.transfer(id, INITIAL_VALUE + loc->cost(), card);
                } catch (VendingMachine::Stock &e) {
                    loc = nameServer.getMachine(id);
                    prt.print(Printer::Student, id, TableCell::SelectVM, loc->getId());
                }
            } or _Select(futureGiftCard) {
                try {
                    _Enable {
                        giftcard = futureGiftCard();
                        loc->buy(favFlavour, *giftcard);
                        prt.print(Printer::Student, id, TableCell::GiftedSoda, giftcard->getBalance());
                        futureGiftCard.reset();
                        break;
                    }
                } catch (VendingMachine::Stock &e) {
                    loc = nameServer.getMachine(id);
                    prt.print(Printer::Student, id, TableCell::SelectVM, loc->getId());
                }
            }
        }
    }
    // delete WATCard at the end
    if (!card) {  // if we never used the WATCard, then we never got it from the courier
        try {     // we still have to clean it up though, so we will just block until we get it
            card = futureWatCard();
            delete card;
        } catch (WATCardOffice::Lost &e) {}     // if we end up losing it, that works too
    } else {
        delete card;
    }

    prt.print(Printer::Student, id, TableCell::Finish);
}
