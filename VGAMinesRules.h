#pragma once
#include "VGARules.h"

enum ACTIONTYPE{ACT_NEARMINES ,ACT_OPEN ,ACT_INCCOUNTERS,ACT_NEARFLAGS, ACT_NEARCLOSED, ACT_FLAGSET,ACT_DECCOUNTERS};
enum MINESMESSAGES{MSG_SPREAD=100};
enum MINEMSGTYPES{ MT_OPENED=100};

class VCell;

//правила игры "сапер"
class VGAMinesRules:public VGARules
{
public:
    VGAMinesRules(){}

    ~VGAMinesRules(void);

    virtual void newgame(int mines);

    virtual void event(int type,int parameter);

private:
        //св€зать €чейки(показать что они соседи
    void addRelation(int a,int b);
     //обработка правой кнопки мыши
    void point(VCell* cell);
     //обработка левой кнопки мыши
    void execute(VCell* cell);
     //обработка центральной кнопки мыши
    void spread(VCell* cell);

    //открытие €чейки(и соседей, если значение клетки - 0)
    int open(VCell* cell);

    //обход соседей и выпонение некоторого действи€, указанного значением переменной ACTIONTYPE(число)
    //результатом выдает количество чеек отобранных по этому параметру.
    int isnear(VCell* cell,ACTIONTYPE act);

};
