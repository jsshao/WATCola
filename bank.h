#ifndef BANK_H
#define BANK_H
#include <map>

_Monitor Bank {
    unsigned int numStudents;
    std::map<unsigned int, unsigned int> accounts;
  public:
    Bank( unsigned int numStudents );
    void deposit( unsigned int id, unsigned int amount );
    void withdraw( unsigned int id, unsigned int amount );
};
#endif 

// vim: set filetype=ucpp :
