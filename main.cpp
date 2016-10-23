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

//���� ����������

    cout<<"input height,width,mines:"<<endl;

    int h, w, m;

    cin>>h>>w>>m;

// ��������� ������, ��������� ������, ���������� � �������,�������� ����

    VGameArea* vga=new VGameArea();

    vga->setRules(new VGAMinesRules());

    vga->setModel(new VGAModel(h,w));

    vga->addView(new VGATextView(w));

    vga->model->generate(w,h);

    vga->rules->newgame(m);

//������� ���� � ����� �������
    vga->cycle();

    return a.exec();
}

