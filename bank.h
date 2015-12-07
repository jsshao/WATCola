#ifndef BANK_H
#define BANK_H
#include <vector>

_Monitor Bank {
    unsigned int numStudents;
    std::vector<uCondition*> conds;
    std::vector<unsigned int> accounts;
    std::vector<unsigned int> desired;
  public:
    Bank( unsigned int numStudents );
    ~Bank();
    void deposit( unsigned int id, unsigned int amount );
    void withdraw( unsigned int id, unsigned int amount );
};
#endif 

// vim: set filetype=ucpp :
