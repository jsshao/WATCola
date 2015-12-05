#ifndef PRINTER_H
#define PRINTER_H
#include <vector>

class TableCell {
  public:
    static const char Start = 'S', Deposit = 'D', Finish = 'F', RequestWorkDone = 'W', CreateDone = 'C', TransferDone = 'T',
        RegisterVM = 'R', NewVM = 'N', PickUp = 'P', DeliverBegin = 'd', Unsuccessful = 'U', DeliverDone = 'D',
        GenerateSoda = 'G', SelectVM = 'V', GiftedSoda = 'G' /*same as GenerateSoda*/, BoughtSoda = 'B', LostCard = 'L',
        ReloadBegin = 'r', ReloadDone = 'R', TransferBegin = 't', None = '\0';
  private:
    // this class handles the output of each cell in the output table
    char state;
    std::vector<int> params;
  public:
    TableCell();
    TableCell( char state );
    TableCell( char state, int param1 );
    TableCell( char state, int param1, int param2 );
    bool empty() const;
    bool finish() const;
    friend std::ostream& operator<<( std::ostream& o, const TableCell& tc );
};

_Monitor Printer {
  public:
    enum Kind { Parent, Groupoff, WATCardOffice, NameServer, Truck, BottlingPlant, Student, Vending, Courier };
  private:
    std::vector<TableCell> buffer;
    unsigned int numStudents, numVendingMachines, numCouriers;
    unsigned int getIndex( Kind kind, unsigned int lid = 0);
    void bufferedPrint(const TableCell& cell, unsigned int index);
    void flushBuffer();
  public:
    Printer( unsigned int numStudents, unsigned int numVendingMachines, unsigned int numCouriers );
    void print( Kind kind, char state );
    void print( Kind kind, char state, int value1 );
    void print( Kind kind, char state, int value1, int value2 );
    void print( Kind kind, unsigned int lid, char state );
    void print( Kind kind, unsigned int lid, char state, int value1 );
    void print( Kind kind, unsigned int lid, char state, int value1, int value2 );
};
#endif
// vim: set filetype=ucpp :
