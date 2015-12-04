#ifndef PARENT_H
#define PARENT_H

_Task Parent {
    void main();
  public:
    Parent( Printer &prt, Bank &bank, unsigned int numStudents, unsigned int parentalDelay );
};

#endif
// vim: set filetype=ucpp :
