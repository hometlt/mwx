#pragma once

#include "gamearea.h"

class VGameArea;

//абстрактный класс правил
class VGARules
{

public:
    VGARules(){}
    ~VGARules(void){}

    virtual void event(int type,int parameter){}

    virtual void generate(){}

    virtual void newgame(int parameter){}

    VGameArea* gamearea;

};
