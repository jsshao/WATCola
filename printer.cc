#include "printer.h"
#include <iostream>

using namespace std;

int xx = 0;   // hacky way to stop printer from printing infinite lines when I cause a deadlock accidentally

TableCell::TableCell() : state(None) {}
TableCell::TableCell( char state ) : state(state) {}
TableCell::TableCell( char state, int param1 ) : state(state) {
    params.push_back(param1);
}
TableCell::TableCell( char state, int param1, int param2 ) : state(state) {
    params.push_back(param1);
    params.push_back(param2);
}

bool TableCell::empty() const {
    return state == None;
} 

bool TableCell::finish() const {
    return state == Finish;
} 

std::ostream& operator<<( std::ostream& o, const TableCell& tc ) {
    if (tc.state == TableCell::None) {
        return o;
    }
    o << (char)tc.state;
    if (tc.params.size() > 0) {
        o << tc.params[0];
        if (tc.params.size() > 1) {
            o << ',' << tc.params[1];
        }
    }
    return o;
}

Printer::Printer( unsigned int numStudents, unsigned int numVendingMachines, unsigned int numCouriers ) : numStudents(numStudents), numVendingMachines(numVendingMachines), numCouriers(numCouriers) {
    cout << "Parent\tGropoff\tWATOff\tNames\tTruck\tPlant";
    for (unsigned int i = 0; i < numStudents; i++) {
        cout << "\t" << "Stud" << i;
    }
    for (unsigned int i = 0; i < numVendingMachines; i++) {
        cout << "\t" << "Mach" << i;
    }
    for (unsigned int i = 0; i < numCouriers; i++) {
        cout << "\t" << "Cour" << i;
    }
    cout << endl;

    for (unsigned int i = 0; i < 6 + numStudents + numVendingMachines + numCouriers; i++) {
        cout << "*******\t";
    }
    cout << endl;
}

unsigned int Printer::getIndex( Kind kind, unsigned int lid ) {
    if ( kind < Kind::Student ) {
        return kind;
    }
    if ( kind < Kind::Vending ) {
        return Kind::Student + lid;
    }
    int current = Kind::Student + numStudents;
    if ( kind < Kind::Courier ) {
        return current + lid;
    }
    current += numVendingMachines;
    return current + lid;
}

void Printer::flushBuffer() {
        for (unsigned int i = 0; i < buffer.size(); i++) {
            cout << buffer[i];
            if (i != buffer.size() - 1) {
                cout << '\t';
            }
        }
        cout << endl;
        buffer.clear();
}

void Printer::sequentialPrint( const TableCell& cell, unsigned int index) {
    // for debugging purposes only
    for (unsigned int i = 0; i < index; i++) {
        cout << '\t';
    }
    cout << cell << endl;
}
void Printer::bufferedPrint( const TableCell& cell, unsigned int index) {
    // TODO DEBUG
    xx++;
    if (xx > 999) {
        cerr << "HIT MAXIMUM OUTPUT LIMIT, EXITING" << endl;
        exit(0);
    }
    /* sequentialPrint(cell, index); */
    /* return; */
    // TODO DEBUG

    if (cell.finish()) {
        flushBuffer();
        for (unsigned int i = 0; i < Kind::Student + numStudents + numVendingMachines + numCouriers; i++) {
            if (i == index) {
                cout << cell;
            } else {
                cout << "...";
            }
            if (i != Kind::Student + numStudents + numVendingMachines + numCouriers - 1) {
                cout << '\t';
            }
        }
        cout << endl;
    } else {
        if (buffer.size() < index + 1) {
            buffer.resize(index + 1);
        }
        if (buffer[index].empty()) {
            buffer[index] = cell;
        } else {
            flushBuffer();
            buffer.resize(index + 1);
            buffer[index] = cell;
        }
    }
}

void Printer::print( Kind kind, char state ) {
    unsigned int index = getIndex(kind);
    bufferedPrint(TableCell(state), index);
}

void Printer::print( Kind kind, char state, int value1 ) {
    unsigned int index = getIndex(kind);
    bufferedPrint(TableCell(state, value1), index);
}

void Printer::print( Kind kind, char state, int value1, int value2 ) {
    unsigned int index = getIndex(kind);
    bufferedPrint(TableCell(state, value1, value2), index);
}

void Printer::print( Kind kind, unsigned int lid, char state ) {
    unsigned int index = getIndex(kind, lid);
    bufferedPrint(TableCell(state), index);
}

void Printer::print( Kind kind, unsigned int lid, char state, int value1 ) {
    unsigned int index = getIndex(kind, lid);
    bufferedPrint(TableCell(state, value1), index);
}

void Printer::print( Kind kind, unsigned int lid, char state, int value1, int value2 ) {
    unsigned int index = getIndex(kind, lid);
    bufferedPrint(TableCell(state, value1, value2), index);
}

// vim: set filetype=ucpp :
