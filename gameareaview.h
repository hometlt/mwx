#pragma once

#include <vector>
#include <algorithm>
#include "gamearea.h"

using namespace std;

class VGAView;
class VGameArea ;
	
class VGAView{
public:
    //virtual VCellView*  getItem(int index){return NULL;}
    virtual int getItemCount(){return 0;}

    virtual void output(){};
    virtual int input(){};

    void cycle();

    virtual void event(int type,int parameter){}

    VGameArea * gamearea;

    VGAView(){}

};
