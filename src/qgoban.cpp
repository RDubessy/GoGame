#include <QMessageBox>
#include <QInputDialog>
#include "qgoban.h"
#include "ui_qgoban.h"
#include "qgobanview.h"
QGoban::QGoban(QWidget *parent) : QMainWindow(parent), ui(new Ui::QGoban) {
    ui->setupUi(this);
    _gobanView=new QGobanView(this);
    _goban=0;
    init(13);
    connect(_gobanView,SIGNAL(nodeSelected(int,int)),this,SLOT(selectNode(int,int)));
    connect(ui->action_About,SIGNAL(triggered()),this,SLOT(about()));
    connect(ui->action_Quit,SIGNAL(triggered()),this,SLOT(close()));
    connect(ui->action_New,SIGNAL(triggered()),this,SLOT(newGame()));
    setCentralWidget(_gobanView);
}
QGoban::~QGoban() {
    delete ui;
    delete _goban;
    delete _gobanView;
}
void QGoban::init(int size) {
    if(_goban!=0)
        delete _goban;
    _goban=new Goban(size);
    _turn='b';
    _gobanView->init(size);
    _gobanView->redraw(_goban->stones());
}
void QGoban::changeEvent(QEvent *e) {
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
void QGoban::selectNode(int i,int j) {
    if(_goban->addStone(i,j,_turn)) {
        _gobanView->redraw(_goban->stones());
        //Update scores
        ui->lcdNumberBlack->display(_goban->score('b'));
        ui->lcdNumberWhite->display(_goban->score('w'));
        //Select next player
        if(_turn=='b') _turn='w';
        else _turn='b';
    }
}
void QGoban::about() {
    QMessageBox::about(this,tr("About GoGame"),tr("Simple Go game\nR. Dubessy 2011\nromain.dubessy@m4x.org"));
}
void QGoban::newGame() {
    QStringList sizes;
    sizes << tr("9x9") << tr("13x13") << tr("19x19");
    QString size=QInputDialog::getItem(this,"Goban size","Choose the goban size",sizes,1);
    init(size.split("x").at(0).toInt());
}
/* qgoban.cpp */