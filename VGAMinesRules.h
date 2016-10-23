#pragma once
#include "VGARules.h"

enum ACTIONTYPE{ACT_NEARMINES ,ACT_OPEN ,ACT_INCCOUNTERS,ACT_NEARFLAGS, ACT_NEARCLOSED, ACT_FLAGSET,ACT_DECCOUNTERS};
enum MINESMESSAGES{MSG_SPREAD=100};
enum MINEMSGTYPES{ MT_OPENED=100};

class VCell;

//������� ���� "�����"
class VGAMinesRules:public VGARules
{
public:
    VGAMinesRules(){}

    ~VGAMinesRules(void);

    virtual void newgame(int mines);

    virtual void event(int type,int parameter);

private:
        //������� ������(�������� ��� ��� ������
    void addRelation(int a,int b);
     //��������� ������ ������ ����
    void point(VCell* cell);
     //��������� ����� ������ ����
    void execute(VCell* cell);
     //��������� ����������� ������ ����
    void spread(VCell* cell);

    //�������� ������(� �������, ���� �������� ������ - 0)
    int open(VCell* cell);

    //����� ������� � ��������� ���������� ��������, ���������� ��������� ���������� ACTIONTYPE(�����)
    //����������� ������ ���������� ���� ���������� �� ����� ���������.
    int isnear(VCell* cell,ACTIONTYPE act);

};
