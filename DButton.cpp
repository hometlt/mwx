#ifndef DBUTTON_H
#define DBUTTON_H
#include <QtGui>

/***автор - Пономарёв Денис**/





class DIndicator : public QGraphicsWidget
{
    Q_OBJECT

    Q_PROPERTY(QIcon pIcon READ icon WRITE setIcon);
    Q_PROPERTY(QString text READ text WRITE setText);
    Q_PROPERTY(bool pEnabled READ enabled WRITE setEnabled);
    Q_PROPERTY(int size READ size WRITE setSize);
    Q_PROPERTY(DButtonDrawMode drawMode READ drawMode WRITE setDrawMode);
public:
    enum DButtonDrawMode {dmButton,dmIcon };
    DIndicator(const QIcon &icon, int newsize=130, QGraphicsItem *parent = 0)
        : QGraphicsWidget(parent), pIcon(icon),m_size(newsize)
    {
        setAcceptHoverEvents(true);
        setCacheMode(DeviceCoordinateCache);
        setEnabled(true);
        setDrawMode(dmButton);
    }
    DIndicator(int newsize=130, QGraphicsItem *parent = 0)
        : QGraphicsWidget(parent),m_size(newsize)
    {
        setCacheMode(DeviceCoordinateCache);
        setEnabled(true);
        setDrawMode(dmButton);
    }

    QRectF boundingRect() const
    {
        return QRectF(-size(), -size(),size()*2, size()*2).adjusted(-5,-5,5,5);
    }

    QPainterPath shape() const
    {
        QPainterPath path;
        path.addEllipse(QRectF(-size()/2, -size()/2,size(), size()));
        return path;
    }

    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
    {
        painter->setRenderHint(QPainter::Antialiasing, true);

        bool down = option->state & QStyle::State_Sunken;

        QRectF r =  QRectF(-size()/2, -size()/2,size(), size());
        switch(m_drawMode)
        {

            case dmButton:
            {

                QPixmap pixmap = pIcon.pixmap(size()/2,size()/2,enabled()?(QIcon::Normal):(QIcon::Disabled));
                QRadialGradient radialGrad(QPointF(0, 0),r.width()/3*2);

                radialGrad.setColorAt(0.6, Qt::white);
                radialGrad.setColorAt(1, Qt::transparent);
                painter->setBrush(radialGrad);
                painter->setPen(Qt::NoPen);
                painter->drawEllipse(r.center(),r.width()/3*2,r.height()/3*2);

                QLinearGradient grad(r.topLeft(), r.bottomRight());
                grad.setColorAt(down ? 1 : 0, option->state & QStyle::State_MouseOver ? Qt::white : Qt::lightGray);
                grad.setColorAt(down ? 0 : 1, Qt::darkGray);
                painter->setPen(Qt::darkGray);

                painter->setBrush(grad);
                painter->drawEllipse(r);
                QLinearGradient grad2(r.topLeft(), r.bottomRight());
                grad.setColorAt(down ? 1 : 0, Qt::darkGray);
                grad.setColorAt(down ? 0 : 1, Qt::lightGray);
                painter->setPen(Qt::NoPen);
                painter->setBrush(grad);
                if (down)     painter->translate(0, 1);
                painter->drawEllipse(r.adjusted(m_size/10,m_size/10, -m_size/10, -m_size/10));

                painter->drawPixmap(-m_size/4, -m_size/4,m_size/2,m_size/2, pixmap);

                painter->setPen(Qt::black);
                painter->setFont(QFont("Arial", m_size/3));
                painter->drawText(QRectF(-m_size/2,-m_size/2,m_size,m_size),m_text,Qt::AlignHCenter|Qt::AlignCenter);
                break;
            }
        case dmIcon:
            {
                QPixmap pixmap = pIcon.pixmap(size(),size(),enabled()?(QIcon::Normal):(QIcon::Disabled));
                painter->drawPixmap(r, pixmap,pixmap.rect());

                painter->setPen(Qt::black);
                painter->setFont(QFont("Arial", size()/3));
                painter->drawText(r, text(),Qt::AlignHCenter|Qt::AlignCenter);
                break;
            }
        }
    }

    int size() const{return m_size;}

    int enabled() const{return pEnabled;}
    QString text() const{return m_text;}
    DButtonDrawMode drawMode() const{return m_drawMode;}

    QIcon icon() const{return pIcon;}

private:
    bool pEnabled;
signals:
    void pressed();
public slots:
    void setSize(int size){m_size=size;update();}
    void setIcon(QIcon icon){pIcon=icon;update();}
    void setEnabled(bool enabled){pEnabled=enabled;update();}
    void setText(int number){m_text=QString::number(number);update();}
    void setText(QString text){m_text=text;update();}
    void refresh(){update();}
    void setDrawMode(DButtonDrawMode drawMode){m_drawMode=drawMode;update();}

protected:
    QString m_text;
    QIcon pIcon;
    DButtonDrawMode m_drawMode;
    int m_size;

    void mousePressEvent(QGraphicsSceneMouseEvent *)
    {
        if(enabled())
        {
            emit pressed();
        }
        update();
    }

    void mouseReleaseEvent(QGraphicsSceneMouseEvent *)
    {
        update();
    }
};

class DButton : public DIndicator
{
    Q_OBJECT

    Q_PROPERTY(QAction * pAction READ action WRITE setAction);
public:
    DButton(const QIcon &icon, int newsize=130, QGraphicsItem *parent = 0)
        : DIndicator(icon,newsize,parent)
    {
        pAction=NULL;
    }

    DButton(QAction* action, int newsize=130, QGraphicsItem *parent = 0)
        : DIndicator(newsize,parent)
    {
        pAction=NULL;
        setAction(action);
    }


    QAction* action() const{return pAction;}


public slots:
    void setAction(QAction* action)
    {
        if(pAction!=NULL)
        {
            disconnect(this,SIGNAL(pressed()),pAction,SLOT(trigger()));
            disconnect(pAction,SIGNAL(changed()),this,SLOT(refresh()));
        }
        setToolTip(action->text());
       pAction=action;refresh();
       connect(this,SIGNAL(pressed()),pAction,SLOT(trigger()));
       connect(pAction,SIGNAL(changed()),this,SLOT(refresh()));
    }
    void refresh()
    {
        if(pAction)
        {
            QPixmap pixmap;
            pixmap = pAction->icon().pixmap(m_size/2,m_size/2,
                                                pAction->isCheckable()?
                                                pAction->isChecked () ? QIcon::Normal: QIcon::Disabled:
                                                pAction->isEnabled() ? QIcon::Normal: QIcon::Disabled);
            pIcon=QIcon(pixmap);
        }
        update();
    }
protected:
    QAction *pAction;

};





class DRectButton : public DButton
{
public:

    DRectButton(const QIcon &icon, int newsize=130, QGraphicsItem *parent = 0)
        : DButton(icon, newsize, parent)
    {
    }

    DRectButton(QAction* action ,int newsize=130, QGraphicsItem *parent = 0)
       : DButton(action,newsize,parent)
    {
    }

    QPainterPath shape() const
    {
        QPainterPath path;
        path.addRect(-size()/2, -size()/2,size(), size());
        return path;
    }

    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
    {
        painter->setRenderHint(QPainter::Antialiasing, true);
        bool down = option->state & QStyle::State_Sunken;
        QRectF r =  QRectF(-size()/2, -size()/2,size(), size());
        switch(m_drawMode)
        {

            case dmButton:
            {

                QPixmap pixmap = pIcon.pixmap(size()/2,size()/2,enabled()?(QIcon::Normal):(QIcon::Disabled));
                QLinearGradient lGrad;


                if (down)     painter->translate(0, 1);
                lGrad.setColorAt(0.6, Qt::white);
                lGrad.setColorAt(1, Qt::gray);
                painter->setBrush(lGrad);
                painter->setPen(Qt::NoPen);
                painter->drawRect(-size()/2, -size()/2,size(), size());


                painter->drawPixmap(-m_size/4, -m_size/4,m_size/2,m_size/2, pixmap);

                painter->setPen(Qt::black);
                painter->setFont(QFont("Arial", m_size/3));
                painter->drawText(QRectF(-m_size/2,-m_size/2,m_size,m_size),m_text,Qt::AlignHCenter|Qt::AlignCenter);
                break;
            }
        case dmIcon:
            {
                QPixmap pixmap = pIcon.pixmap(size(),size(),enabled()?(QIcon::Normal):(QIcon::Disabled));
                painter->drawPixmap(r, pixmap,pixmap.rect());

                painter->setPen(Qt::black);
                painter->setFont(QFont("Arial", size()/3));
                painter->drawText(r, text(),Qt::AlignHCenter|Qt::AlignCenter);
                break;
            }
        }
    }

};







class DSpinButton : public DIndicator
{
    Q_OBJECT
    Q_PROPERTY(int value READ value WRITE setValue);
public:
    int value(){return pValue;}


    DSpinButton(const QIcon &pixmap,int size=130, QGraphicsItem *parent = 0,int value=0)
        : DIndicator(pixmap,size,parent),pValue(value)
    {
    state=0;
}

    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget * wqt)
    {
        DIndicator::paint(painter,option,wqt);

        QRectF r =  QRectF(-size()/2, -size()/2,size(), size());

        if (state){



                    QLinearGradient grad(QPoint(0,r.top()),QPoint(0,r.bottom()));
                    grad.setColorAt(state==2 ? 1 : 0.5, QColor(Qt::darkGray).dark(120));
                    grad.setColorAt(state==1 ? 1 : 0.5, Qt::transparent);
                    painter->setPen(Qt::NoPen);
                    painter->setBrush(grad);
                    painter->drawEllipse(r.adjusted(5, 5, -5, -5));


                   // painter->translate(2, 2);



        }



        painter->setPen(Qt::darkGray);
        painter->setFont(QFont("Arial", r.height()/3));
        painter->drawText(QRect(r.left(),r.top(),r.width(),r.height()/3), "+",Qt::AlignHCenter|Qt::AlignCenter);
        painter->drawText(QRect(r.left(),r.top()+r.height()*2/3,r.width(), r.height()/3), "-",Qt::AlignHCenter|Qt::AlignCenter);


        QRadialGradient radialGrad(QPointF(0, 0), size()/3);
        radialGrad.setColorAt(0, QColor(Qt::white));
        radialGrad.setColorAt(1, Qt::transparent);
        painter->setBrush(radialGrad);
        painter->setPen(Qt::NoPen);
        painter->drawEllipse(r.center(),r.width()/2,r.height()/2);


        painter->setPen(QPen(radialGrad,11));
        painter->drawLine(r.left(),0,r.right(),0);


        painter->setPen(Qt::black);


        painter->drawText(QRect(r.left(),r.top(),r.width(), r.height()), QString::number(pValue),Qt::AlignHCenter|Qt::AlignCenter);

    }

    void mousePressEvent(QGraphicsSceneMouseEvent *event)
    {
        timer=new QTimer();
        if(event->pos().y()<0)
        {state=1;  stepUp();
        connect(timer ,SIGNAL(timeout()),SLOT(stepUp()));
        }else
        {state=2;stepDown();
            connect(timer ,SIGNAL(timeout()),SLOT(stepDown()));
        }
        emit pressed();
        update();
        timer->start(100);
    }

    void mouseReleaseEvent(QGraphicsSceneMouseEvent *)
    {
        state=0;
        update();
        disconnect(timer,0,0,0);
        delete timer;
    }

signals:
    void valueChanged(int);

public slots:
    void stepDown()
    {setValue(value()-1);emit valueChanged(value());}

    void stepUp()
    {setValue(value()+1);emit valueChanged(value());}

    void setValue(int value)
    {
        pValue=value;
        update();
    }

private:
    int pValue;
    int state;
    QTimer* timer;

};



class DMenuButton : public DIndicator
{
    Q_OBJECT
    Q_PROPERTY(QAction * pAction READ action WRITE setAction);
    Q_PROPERTY(int pChildSize READ childSize WRITE setChildSize);
    Q_PROPERTY(int pMenuRadius READ menuRadius WRITE setMenuRadius);
    Q_PROPERTY(int pActiveSize READ activeSize WRITE setActiveSize);
public:
    DMenuButton(QAction* action, int newsize=130, QGraphicsItem *parent = 0)
        : DIndicator(newsize,parent)
    {
        machine=NULL;
        pAction=NULL;

        pChildSize=newsize/2;
        pActiveSize=newsize*2;
        menuIsOpened=false;
        pMenuRadius=size()-5;



        machine=new QStateMachine;
        state1 = new QState(machine);
        state2 = new QState(machine);

        t1 = state1->addTransition(this, SIGNAL(openMenu()), state2);

        t2 = state2->addTransition(this, SIGNAL(closeMenu()), state1);

        state1->assignProperty(this ,"rotation", qreal(0));
        state2->assignProperty(this ,"rotation", qreal(360));
        state1->assignProperty(this ,"size", size() );
        state2->assignProperty(this ,"size", pActiveSize  );

        t1->addAnimation(new QPropertyAnimation(this, "rotation"));
        t2->addAnimation(new QPropertyAnimation(this, "rotation"));
        t1->addAnimation(new QPropertyAnimation(this, "size"));
        t2->addAnimation(new QPropertyAnimation(this, "size"));

        machine->setInitialState(state1);
        machine->start();

        setAction(action);

        connect(this,SIGNAL(pressed()),this,SLOT(openCloseMenuSlot()));
    }



    QAction* action() const{return pAction;}


    void addAction(QAction* pNewAction)
    {
        action()->menu()->addAction(pNewAction);

        DMenuButton* it = new DMenuButton(pNewAction, size()*3/4,this);
        connect(this,SIGNAL(closeMenu()),it,SLOT(closeMenuSlot()));

        state1->assignProperty(it ,"opacity", qreal(0));
        state2->assignProperty(it ,"opacity", qreal(0.9));
        state1->assignProperty(it ,"size", size()/2);
        state2->assignProperty(it ,"size", pChildSize);
        state1->assignProperty(it ,"pos", QPointF(0,0));

        t1->addAnimation(new QPropertyAnimation(it, "opacity"));
        t1->addAnimation(new QPropertyAnimation(it, "size"));
        t1->addAnimation(new QPropertyAnimation(it, "pos"));
        t2->addAnimation(new QPropertyAnimation(it, "opacity"));
        t2->addAnimation(new QPropertyAnimation(it, "size"));
        t2->addAnimation(new QPropertyAnimation(it, "pos"));

        int i=0;
        foreach(QGraphicsItem* child,childItems())
        {
            QPointF pos(menuRadius()*sin  (2 * M_PI *i/childItems().count()),menuRadius()*cos(2 * M_PI *i/childItems().count()));
            DMenuButton* it=(DMenuButton*)child;
            state2->assignProperty(it ,"pos",  pos);
            if(!it->menuIsOpened)it->setProperty("pos",pos);
            i++;
        }


    }


    void updateMenu()
    {
        foreach(QGraphicsItem* child,childItems())
        {
            delete (QGraphicsWidget*)child;
        }


        if(pAction->menu())
        {
            foreach(QAction* action,pAction->menu()->actions())
            {
                DMenuButton* button = new DMenuButton(action, size()*3/4,this);
                connect(this,SIGNAL(closeMenu()),button,SLOT(closeMenuSlot()));

            }

            int i=0;
            foreach(QGraphicsItem* child,childItems())
            {
                QGraphicsWidget* it=(QGraphicsWidget*)child;
                state1->assignProperty(it ,"opacity", qreal(0));
                state2->assignProperty(it ,"opacity", qreal(0.9));
                state1->assignProperty(it ,"size", size()/2);
                state2->assignProperty(it ,"size", pChildSize);
                state1->assignProperty(it ,"pos", QPointF(0,0));
                state2->assignProperty(it ,"pos",  QPointF(menuRadius()*sin  (2 * M_PI *i/childItems().count()),menuRadius()*cos(2 * M_PI *i/childItems().count())));

                t1->addAnimation(new QPropertyAnimation(it, "opacity"));
                t1->addAnimation(new QPropertyAnimation(it, "size"));
                t1->addAnimation(new QPropertyAnimation(it, "pos"));
                t2->addAnimation(new QPropertyAnimation(it, "opacity"));
                t2->addAnimation(new QPropertyAnimation(it, "size"));
                t2->addAnimation(new QPropertyAnimation(it, "pos"));
                i++;
            }

        }
    }





    int menuRadius(){return pMenuRadius;}
    int activeSize(){return pActiveSize;}
    int childSize(){return pChildSize;}
public slots:

    void setSize(int size){
        state1->assignProperty(this ,"size", size);
        if(!menuIsOpened)DIndicator::setSize(size);

    }

    void setChildSize(int childSize){
        pChildSize=childSize;
        foreach(QGraphicsItem* child,childItems())
        {
            DMenuButton* it=(DMenuButton*)child;
            state2->assignProperty(it ,"size", pChildSize);
             if(!it->menuIsOpened)it->setProperty("size",pChildSize);

        }
    }

    void setChildActiveSize(int childActiveSize){
        pChildSize=childActiveSize;
        foreach(QGraphicsItem* child,childItems())
        {
            DMenuButton* it=(DMenuButton*)child;
            it->setProperty("activeSize",childActiveSize);
            if(it->menuIsOpened)it->setProperty("size",childActiveSize);
        }
    }

    void setMenuRadius(int menuRadius){
        pMenuRadius=menuRadius;
        int i=0;
        foreach(QGraphicsItem* child,childItems())
        {
            QGraphicsWidget* it=(QGraphicsWidget*)child;
            QPointF dest(menuRadius*sin  (2 * M_PI *i/childItems().count()),menuRadius*cos(2 * M_PI *i/childItems().count()));
            state2->assignProperty(it ,"pos",dest);
            it->setPos(dest);
            i++;
        }
    }

    void setActiveSize(int activeSize){
        state2->assignProperty(this ,"size", pActiveSize  );
        pActiveSize=activeSize;
        if(menuIsOpened)DIndicator::setSize(activeSize);
    }

    void setAction(QAction* action)
    {
        if(pAction!=NULL)
        {
            disconnect(this,SIGNAL(pressed()),pAction,SLOT(trigger()));
            disconnect(pAction,SIGNAL(changed()),this,SLOT(refresh()));
        }
        setToolTip(action->text());
       pAction=action;
       updateMenu();
       refresh();
       connect(this,SIGNAL(pressed()),pAction,SLOT(trigger()));
       connect(pAction,SIGNAL(changed()),this,SLOT(refresh()));
    }


    void refresh()
    {
        if(pAction)
        {
            QPixmap pixmap;
            pixmap = pAction->icon().pixmap(m_size/2,m_size/2,
                                                pAction->isCheckable()?
                                                pAction->isChecked () ? QIcon::Normal: QIcon::Disabled:
                                                pAction->isEnabled() ? QIcon::Normal: QIcon::Disabled);
            pIcon=QIcon(pixmap);
        }
        update();
    }


        void closeMenuSlot()
        {
            menuIsOpened=false;
            emit closeMenu();
            setZValue(0);
        }

        void openMenuSlot()
        {
            menuIsOpened=true;
            emit openMenu();
            setZValue(1);
        }



        void openCloseMenuSlot()
        {
            if(pAction->menu())
            {
                if(menuIsOpened)
                {
                    closeMenuSlot();
                }
                else
                {
                    openMenuSlot();
                }
            }
        }

signals:
    void closeMenu();
    void openMenu();

protected:

    QAction *pAction;

    void mousePressEvent(QGraphicsSceneMouseEvent *event)
    {
        DIndicator::mousePressEvent(event);
    }

    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
    {
        DIndicator::mouseReleaseEvent(event);
    }

private:
    int pMenuRadius;
    bool menuIsOpened;
    int pChildSize;
    int pActiveSize;
    QState *state1 ;
    QState *state2;

    QStateMachine* machine;
    QAbstractTransition* t1;

    QAbstractTransition * t2;
};
/*
for (int i = 0; i < 4; ++i)
{
    QAction *action = new QAction(names[i], this);
    action->setData(i);
    action->setIcon(QIcon(fileNames[i]));
    connect(action, SIGNAL(triggered()),this, slot);
    difficultLevelMenu->addAction(action);
}
return difficultLevelMenu;
*/



#endif // DBUTTON_H
