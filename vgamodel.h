#ifndef VGAMODEL_H
#define VGAMODEL_H
#include "gamearea.h"
#include "VGAMinesRules.h"

class VGAModel
{
public:
    VGAModel(int h, int w){height=h; width=w;}

    //��������� ����(�������� ����� � ������������ ������)
    void generate(int w, int h);

    VGameArea * gamearea;

private:
    int height;
    int width;

};

#endif // VGAMODEL_H
