#include "qgoban.h"
#include "ui_qgoban.h"
QGoban::QGoban(QWidget *parent) : QMainWindow(parent), Goban(),
    ui(new Ui::QGoban) {
    ui->setupUi(this);
    addStone(1,1,'w');
    addStone(5,5,'b');
    print();
}
QGoban::~QGoban() {
    delete ui;
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
/* qgoban.cpp */