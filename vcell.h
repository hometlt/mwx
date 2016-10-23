#ifndef VCELL_H
#define VCELL_H


#include <QVector>
using namespace std;

class VCell{
public:

    vector< VCell* > neighbors;

    int value;
    int state;
    int index;

    VCell(int i){index=i;}
};

#endif // VCELL_H
