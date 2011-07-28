#include "qgoban.h"
#include "ui_qgoban.h"
#include "qgobanview.h"
QGoban::QGoban(QWidget *parent) : QMainWindow(parent), ui(new Ui::QGoban) {
    ui->setupUi(this);
    _gobanView=new QGobanView(13,this);
    _goban=new Goban(13);
    _turn='b';
    setCentralWidget(_gobanView);
    connect(_gobanView,SIGNAL(nodeSelected(int,int)),this,SLOT(selectNode(int,int)));
}
QGoban::~QGoban() {
    delete ui;
    delete _goban;
    delete _gobanView;
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
        if(_turn=='b') _turn='w';
        else _turn='b';
    }
}
/* qgoban.cpp */