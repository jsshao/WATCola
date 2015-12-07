#ifndef GROUPOFF_H
#define GROUPOFF_H
#include "watcard.h"
#include "printer.h"
#include <vector>

_Task Groupoff {
    void main();
    Printer *prt;
    unsigned int numStudents, sodaCost, groupoffDelay;
    std::vector<WATCard::FWATCard> futures;
    std::vector<WATCard*> issued_cards;
  public:
    Groupoff( Printer &prt, unsigned int numStudents, unsigned int sodaCost, unsigned int groupoffDelay );
    WATCard::FWATCard giftCard();
    ~Groupoff();
};

#endif
// vim: set filetype=ucpp :
