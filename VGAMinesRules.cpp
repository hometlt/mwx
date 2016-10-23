#include "VGAMinesRules.h"


void VGAMinesRules::newgame(int mines){

    gamearea->_gamestatus=GS_NEW;
    for(unsigned int i=0;i<gamearea->cells.size();i++)
    {
        gamearea->cells[i]->value=CV_EMPTY;
        gamearea->cells[i]->state=CS_CLOSED;
    }

    if(mines<5)mines=5;

    gamearea->updateTime();

    if(mines>gamearea->cells.size()*3/4-1)mines=gamearea->cells.size()*3/4-1;
    gamearea->_minesCount=mines;

    int i=0;
    while(i<gamearea->_minesCount)
    {
        int cellIndex=rand()%gamearea->cells.size();
        if(gamearea->cells[cellIndex]->value!=CV_MINED)
        {
            gamearea->cells[cellIndex]->value=CV_MINED;
            isnear(gamearea->cells[cellIndex],ACT_INCCOUNTERS);
            i++;
        }
    }

    gamearea->event(MT_SPECIAL,MSG_NEWGAME);
}


void VGAMinesRules::event(int type,int p)
{
    switch(type)
    {
    case MT_RBUTTON:
        point(gamearea->cells[p]);break;
    case MT_LBUTTON:
        execute(gamearea->cells[p]);break;
    case MT_MBUTTON:
        spread(gamearea->cells[p]);break;


    case MT_SPECIAL:
        if(p==MSG_NEWGAME)
        {

        }
        break;
    }
}


void VGAMinesRules::execute(VCell* cell)
{
    if(gamearea->getStatus()==GS_NEW)
    {
        gamearea->startGame();

        if(cell->value==CV_MINED)
        while(true)
        {
            int index=rand()%gamearea->cells.size();
            if(gamearea->cells[index]->value!=CV_MINED)
            {
                gamearea->cells[index]->value=CV_MINED;
                isnear(gamearea->cells[index],ACT_INCCOUNTERS);
                isnear(cell,ACT_DECCOUNTERS);
                cell->value=isnear(cell,ACT_NEARMINES);
                break;
            }

        }

    }
    if(cell->state==CS_CLOSED&&gamearea->getStatus()==GS_ACTIVE)
    {
       // gamearea->event(MT_LBUTTON,cell->index);
        if(open(cell)>1)
            gamearea->event(MT_SPECIAL,MSG_SPREAD);
    }
}




int VGAMinesRules::open(VCell* cell)
{
    if(cell->state!=CS_OPENED)
    {
        if(cell->value==CV_MINED) //Если была мина , то Поражение
        {
            cell->state=CS_MORDER;
            /*if( cellValues[cellIndex]==CV_MINED&&cellStates[cellIndex]!=CS_FLAG)
                    cellStates[cellIndex]=CS_OPENED;
            else if(cellStates[cellIndex]!=CS_MINE&&cellStates[cellIndex]==CS_FLAG)
                    cellStates[cellIndex]=CS_MISTAKE;*/

            gamearea->updateTime();

            gamearea->_gamestatus= GS_OVER;

            gamearea->event(MT_SPECIAL,MSG_OVER);

        }
        else    //Открытие кликнутой ячейки
        {
            cell->state=CS_OPENED;
            gamearea->event(MT_OPENED,cell->index);
            if(--gamearea->_closedCells==gamearea->_minesCount)
            {
                gamearea->updateTime();
                gamearea->_gamestatus= GS_WIN;
                gamearea->event(MT_SPECIAL,MSG_WIN);
            }
            if(cell->value==0)
            {
                return isnear(cell,ACT_OPEN);
            }
        }
    }
    return 1;
}



void VGAMinesRules::spread(VCell* cell)
{
    if(cell->state==CS_OPENED)
    {
        //автоматическое открытие соседних клеток
        if(isnear(cell,ACT_NEARFLAGS)==cell->value)isnear(cell,ACT_OPEN);
        //автоматическая утановка флажков
        if(isnear(cell,ACT_NEARCLOSED)==cell->value)isnear(cell,ACT_FLAGSET);
    }
}

void VGAMinesRules::point(VCell* cell)
{
    if(gamearea->_gamestatus==GS_ACTIVE)
    {
        gamearea->event(MT_RBUTTON,cell->index);
        if(cell->state==CS_CLOSED)
            cell->state=CS_FLAG;
        else if(cell->state==CS_FLAG)
            cell->state=CS_CLOSED;
    }
}



int VGAMinesRules::isnear(VCell* cell,ACTIONTYPE act)
{ 
    int counter=0;

    for(unsigned int i=0;i<cell->neighbors.size();i++)
    {
        VCell* n=cell->neighbors[i];

        //количество закрытых соседей
        if(act==ACT_FLAGSET&&n->state==CS_CLOSED  )
            {counter++;n->state=CS_FLAG;}
        //количество закрытых соседей
        if(act==ACT_NEARCLOSED&&(n->state==CS_CLOSED||n->state==CS_FLAG ) )
            {counter++;}
        //количество флагов в ближайших клетках
        if(act==ACT_NEARMINES&&n->value==CV_MINED  )
            {counter++;}
        //открытие всех соседних ячеек
        if(act==ACT_OPEN&&n->state==CS_CLOSED)
            {counter+=open(n);}
        //инкрементирование счетчиков соседних клеток при установке мины
        if(act==ACT_INCCOUNTERS&&n->value!=CV_MINED )
            {counter++;n->value+=1;}
        //количество флагов в ближайших клетках
        if(act==ACT_NEARFLAGS&&n->state==CS_FLAG  )
            {counter++;}
        if(act==ACT_DECCOUNTERS&&n->value!=CV_MINED )
            {counter++;n->value--;}

    }

    return counter;
}



