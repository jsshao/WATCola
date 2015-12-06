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

    WATCard::FWATCard watCard = office.create(id, INITIAL_VALUE);
    WATCard::FWATCard giftCard = group.giftCard();
    VendingMachine *loc = nameServer.getMachine(id);
    prt.print(Printer::Student, id, TableCell::SelectVM, loc->getId());

    
    for (unsigned int i = 0; i < numOfPurch; i++) {
        yield(rng(1, 10));
        for (;;) {
            _Select (watCard) {
                try {
                    watCard();
                } catch (WATCardOffice::Lost &e) {
                    prt.print(Printer::Student, id, TableCell::LostCard);

                    // Create new watcard
                    watCard = office.create(id, INITIAL_VALUE);
                    continue;
                }

                try {
                    _Enable {
                        loc->buy(favFlavour, *watCard);
                        prt.print(Printer::Student, id, TableCell::BoughtSoda, (*watCard).getBalance());
                        break;
                    }
                } catch (VendingMachine::Funds &e) {
                    watCard = office.transfer(id, INITIAL_VALUE + loc->cost(), watCard);
                } catch (VendingMachine::Stock &e) {
                    loc = nameServer.getMachine(id);
                    prt.print(Printer::Student, id, TableCell::SelectVM, loc->getId());
                }
            } or _Select(giftCard) {
                try {
                    _Enable {
                        loc->buy(favFlavour, *giftCard);
                        prt.print(Printer::Student, id, TableCell::GiftedSoda, (*giftCard).getBalance());
                        giftCard.reset();
                        break;
                    }
                } catch (VendingMachine::Stock &e) {
                    loc = nameServer.getMachine(id);
                    prt.print(Printer::Student, id, TableCell::SelectVM, loc->getId());
                }
            }
        }
    }

    prt.print(Printer::Student, id, TableCell::Finish);
}
