#ifndef WATCARDOFFICE_H
#define WATCARDOFFICE_H
#include "printer.h"
#include "bank.h"
#include "watcard.h"
#include <vector>
#include <queue>

_Task WATCardOffice {
    struct Args {
        unsigned int sid, amount;
        WATCard *watcard = NULL;
    };

    struct Job {                           // marshalled arguments and return future
        Args args;                         // call arguments (YOU DEFINE "Args")
        WATCard::FWATCard result;          // return future
        Job( Args args ) : args( args ) {}
    };
    _Task Courier { 
        WATCardOffice *office;
        Printer *prt;
        Bank *bank;
        unsigned int id;
        void main(); 
      public:
        Courier(WATCardOffice &office, Printer &prt, Bank &bank, unsigned int id);

    };                 // communicates with bank
    Printer *prt;
    Bank *bank;
    unsigned int numCouriers;
    std::vector<Courier*> courierPool;
    std::queue<Job*> jobs;

    void main();
  public:
    _Event Lost {};                        // lost WATCard
    ~WATCardOffice();
    WATCardOffice( Printer &prt, Bank &bank, unsigned int numCouriers );
    WATCard::FWATCard create( unsigned int sid, unsigned int amount );
    WATCard::FWATCard transfer( unsigned int sid, unsigned int amount, WATCard *card );
    Job *requestWork();
};

#endif
// vim: set filetype=ucpp :
