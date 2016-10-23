#include "vgamodel.h"

void addRelation( VCell *a,VCell *b){
    a->neighbors.push_back(b);
    b->neighbors.push_back(a);
}

//создание игрового пол€
void VGAModel::generate(int w, int h)
{
    //удаление старых €чеек
    gamearea->cells.clear();
    gamearea->_closedCells=w*h;

    //создание €чеек
    for(int i=0;i<gamearea->_closedCells ;i++)
    {
    VCell* newcell=new VCell(i);
    gamearea->cells.push_back(newcell);
    gamearea->cells[i]->index=i;
    }

    //создание св€зей между €чейками
    for(int i=0;i<gamearea->_closedCells ;i++)
    {
        if(i%w<w-1){
            addRelation(gamearea->cells[i],gamearea->cells[i+1]);
            if(i/w<h-1) addRelation(gamearea->cells[i],gamearea->cells[i+w+1]);
        }
        if(i/w<h-1){addRelation(gamearea->cells[i],gamearea->cells[i+w]);
            if(i%w>0){addRelation(gamearea->cells[i],gamearea->cells[i+w-1]);}
        }
    }

}


