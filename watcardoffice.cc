#include "watcardoffice.h"
#include "printer.h"
#include "bank.h"
#include "watcard.h"
#include "MPRNG.h"
#include <iostream>
using namespace std;

extern MPRNG rng;

WATCardOffice::WATCardOffice( Printer &prt, Bank &bank, unsigned int numCouriers ) : prt(&prt), bank(&bank), numCouriers(numCouriers) {
    for (unsigned int i = 0; i < numCouriers; i++) {
        courierPool.push_back(new Courier(*this, prt, bank, i));
    }
}

WATCardOffice::~WATCardOffice() {
    for (unsigned int i = 0; i < numCouriers; i++) {
        delete courierPool[i];
    }
}


WATCard::FWATCard WATCardOffice::create( unsigned int sid, unsigned int amount ) {
    Args args;     
    args.sid = sid;
    args.amount = amount;
    Job *job = new Job(args);
    jobs.push(job);
    return job->result;
}

WATCard::FWATCard WATCardOffice::transfer( unsigned int sid, unsigned int amount, WATCard *card ) {
    Args args;     
    args.sid = sid;
    args.amount = amount;
    args.watcard = card;
    Job *job = new Job(args);
    jobs.push(job);
    return job->result;
}

WATCardOffice::Job* WATCardOffice::requestWork() {
    Job *job = jobs.front();
    jobs.pop();
    return job;
}

void WATCardOffice::main() {
    prt->print(Printer::WATCardOffice, TableCell::Start);
    for (;;) {
        _Accept(~WATCardOffice) {
            // empty the job queue
            while (jobs.size()) {
                Job *job = jobs.front();
                delete job;
                jobs.pop();
            }
        
            // replace the jobs with NULL jobs, signifying termination
            for (unsigned int i = 0; i < numCouriers; i++) {
                jobs.push(NULL);            // null job signals to couriers to stop
            }
            _Accept(requestWork);       // let couriers receive NULL job
            break;
        } or _Accept( create ) {
            Job *job = jobs.back();
            prt->print(Printer::WATCardOffice, TableCell::CreateDone, job->args.sid, job->args.amount);
        } or _Accept( transfer ) {
            Job *job = jobs.back();
            prt->print(Printer::WATCardOffice, TableCell::TransferDone, job->args.sid, job->args.amount);

        } or _When(jobs.size()) _Accept (requestWork) {
            prt->print(Printer::WATCardOffice, TableCell::RequestWorkDone);
        }     
    }
    prt->print(Printer::WATCardOffice, TableCell::Finish);
}

void WATCardOffice::Courier::main() {
    prt->print(Printer::Courier, id, TableCell::Start);
    for (;;) {
        WATCardOffice::Job *job = office->requestWork();
        if (job) {
            prt->print(Printer::Courier, id, TableCell::TransferBegin, job->args.sid, job->args.amount);
            bank->withdraw(job->args.sid, job->args.amount);            
            prt->print(Printer::Courier, id, TableCell::TransferDone, job->args.sid, job->args.amount);
            WATCard *card;
            if (job->args.watcard) {
                card = job->args.watcard;
            } else {
                card = new WATCard();
            }
            card->deposit(job->args.amount);
            if ( rng(5) == 0 ) { // 1 in 6 chance
                delete card;
                job->result.exception(new Lost);
            } else {
                job->result.delivery(card);
            }
            delete job;
        } else {           // null job is the signal to stop
            break;
        }
    }
    prt->print(Printer::Courier, id, TableCell::Finish);
}
WATCardOffice::Courier::Courier(WATCardOffice &office, Printer &prt, Bank &bank, unsigned int id) : office(&office), prt(&prt), bank(&bank), id(id) {}

// vim: set filetype=ucpp :
