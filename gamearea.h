
#ifndef GAMEAREA_H_
#define GAMEAREA_H_

#include <vector>
#include <time.h> 

#include "gameareaview.h"
#include "VGARules.h"
#include "vgamodel.h"

#include "vcell.h"

using namespace std;


enum MESSAGETYPE{MT_SPECIAL,MT_RBUTTON,MT_LBUTTON,MT_MBUTTON};
enum STDMESSAGES{MSG_START,MSG_WIN,MSG_OVER,MSG_NEWGAME,MSG_EXIT};
enum CELLSTATE{CS_MORDER,CS_MINE,CS_CLOSED,CS_OPENED, CS_FLAG ,CS_MISTAKE};
enum CELLVALUE{CV_MINED=-1,CV_EMPTY=0};
enum GAMESTATE{
    GS_NEW      = 0x00000001,
    GS_NONE     = 0x00000002,
    GS_ACTIVE   = 0x00000004,
    GS_OVER     = 0x00000008,
    GS_WIN      = 0x00000010,
    GS_PAUSE    = 0x00000020};
	
	
class VGAView;
class VGameArea;
class VGARules;
class VGAModel;
//class VCellView ;
//class VGameArea3DView ;



//основной класс
class VGameArea {
	
public:

    VGameArea();

    int gameTime();

    //обработка событий
    void event(int type,int parameter);


    vector < VCell *> cells;

    VGARules* rules;

    VGAModel* model;

    vector <VGAView*> view;

    void setRules(VGARules* r);

    void setModel(VGAModel* m);

    void addView(VGAView* v);

    inline int getStatus(){return _gamestatus;}

    void updateTime();

    //не очень пон€тно. но тут происходит и отрисовка и ввод данных.
    //(незавивимо от выбранного класса, VSoundView , VGLView или VTextView)
    // в opengl функци€ draw - основной цикл opengl. в textview обычный ввод с консоли. в soundview - только звук.

    void cycle();



    void startGame();

    int _gamestatus;
    int _closedCells;
    int _minesCount;

private:
    clock_t timer;

    clock_t _gametime;
};

#endif
