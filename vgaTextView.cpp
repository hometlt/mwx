#include "vgaTextView.h"


void VGATextView::output(){
    //отображение игрового поля в консоль
    system("cls");
    int i=0;
    foreach(VCell *a, gamearea->cells){
        if(a->state==CS_CLOSED)
        {
           cout<<char(178);
        }
        else
        {
            if(a->value==CV_EMPTY)
               cout<<char(176);
            else if(a->value==CV_MINED)
               cout<<0;
            else
                cout<<a->value;
        }
        i++;
        if(i==w){
            i=0;
            cout<<endl;
        }
    }
    if(gamearea->getStatus()==GS_OVER)
        cout<<"time"<<gamearea->gameTime();

}

int VGATextView::input(){
    if(gamearea->getStatus()&(GS_OVER)){
        system("pause");
        return MSG_EXIT;
    }
    else{
    //ввод данных с консоли
    int x,y;
        cout<<"input x,y: ";
        cin>>x>>y;
        gamearea->event(MT_LBUTTON,--x + --y * w);
    }
}

void VGATextView::event(int type,int parameter)
{
    switch(type)
    {
    case MT_SPECIAL:
        switch(parameter)
        {
            case MSG_OVER:
              cout<<"game over";break;
            case MSG_WIN:
              cout<<"game win";break;
            case MSG_START:
              cout<<"game start";break;
        }
        break;
    case MT_LBUTTON:
        cout<<"clicked "<<parameter<<endl;break;
    }
}
