#include <QtCore/QCoreApplication>

#include <math.h>
#include <stdlib.h>
#include <algorithm>

#include "gamearea.h"
#include "vgaTextView.h"
#include "VGAMinesRules.h"
#include "vgamodel.h"

using namespace std;


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

//ввод параметров

    cout<<"input height,width,mines:"<<endl;

    int h, w, m;

    cin>>h>>w>>m;

// установка правил, двумерной модели, отображеня в консоль,создание игры

    VGameArea* vga=new VGameArea();

    vga->setRules(new VGAMinesRules());

    vga->setModel(new VGAModel(h,w));

    vga->addView(new VGATextView(w));

    vga->model->generate(w,h);

    vga->rules->newgame(m);

//процесс игры и вывод времени
    vga->cycle();

    return a.exec();
}

