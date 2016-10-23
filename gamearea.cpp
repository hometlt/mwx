
#include "gamearea.h"

VGameArea::VGameArea()
{
    _minesCount=0;
    srand((unsigned int)time(NULL));
    _gamestatus=GS_NONE;
    _gametime=0;
}

void VGameArea::event(int t, int p)
{
    for(unsigned int i=0;i<view.size();i++)
        view.at(i)->event(t,p);
    rules->event(t,p);
}


void VGameArea::startGame(){
    _gamestatus=GS_ACTIVE;
    timer=clock()-CLOCKS_PER_SEC;
    event(MT_SPECIAL,MSG_START);
}

void VGameArea::cycle(){
    for(unsigned int i=0;i<view.size();i++){
        view.at(i)->cycle();
    }
}

int VGameArea::gameTime()
{
    if(_gamestatus==GS_NEW)return 0;
    if(_gamestatus==GS_ACTIVE) return (clock()-timer)/CLOCKS_PER_SEC;
    else return _gametime;
}


void VGameArea::updateTime(){
    _gametime=gameTime();
}




 void VGameArea::setRules(VGARules* r){
     rules=r;r->gamearea=this;
 }

 void VGameArea::setModel(VGAModel* m){
     model=m;m->gamearea=this;
 }

 void VGameArea::addView(VGAView* v){
     view.push_back(v);v->gamearea=this;
 }

