#include <QMessageBox>
#include <QInputDialog>
#include <QStringList>
#include <QFileDialog>
#include <QTextStream>
#include "qgoban.h"
#include "ui_qgoban.h"
#include "goban.h"
#include "qgobanview.h"
QGoban::QGoban(QWidget *parent) : QMainWindow(parent), ui(new Ui::QGoban) {
    ui->setupUi(this);
    _gobanView=new QGobanView(this);
    _game=new QStringList();
    _goban=0;
    _endgame=false;
    init(13);
    _menu=new QMenu(this);
    _menu->addAction(tr(""));
    _menu->addAction(tr(""));
    connect(_gobanView,SIGNAL(nodeSelected(int,int)),this,SLOT(selectNode(int,int)));
    connect(_gobanView,SIGNAL(stoneSelected(int,int,QPoint)),this,SLOT(selectStone(int,int,QPoint)));
    connect(ui->action_About,SIGNAL(triggered()),this,SLOT(about()));
    connect(ui->action_Quit,SIGNAL(triggered()),this,SLOT(close()));
    connect(ui->action_New,SIGNAL(triggered()),this,SLOT(newGame()));
    connect(ui->action_Save,SIGNAL(triggered()),this,SLOT(saveGame()));
    connect(ui->action_Open,SIGNAL(triggered()),this,SLOT(openGame()));
    connect(ui->pushButtonUndo,SIGNAL(clicked()),this,SLOT(undoMove()));
    connect(ui->pushButtonPass,SIGNAL(clicked()),this,SLOT(passTurn()));
    setCentralWidget(_gobanView);
}
QGoban::~QGoban() {
    delete ui;
    delete _goban;
    delete _gobanView;
    delete _game;
    delete _menu;
}
void QGoban::init(int size) {
    if(_goban!=0)
        delete _goban;
    _goban=new Goban(size);
    _turn='b';
    _gobanView->init(size);
    _gobanView->redraw(_goban->stones());
    //Update scores
    ui->lcdNumberBlack->display(_goban->score('b'));
    ui->lcdNumberWhite->display(_goban->score('w'));
    _game->clear();
    _game->append(tr("Goban %1x%1").arg(size));
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
        _endgame=false;
        _gobanView->redraw(_goban->stones());
        //Update scores
        ui->lcdNumberBlack->display(_goban->score('b'));
        ui->lcdNumberWhite->display(_goban->score('w'));
        //Save player's move
        if(_turn=='b') _game->append(tr("Black %1 %2").arg(i).arg(j));
        else _game->append(tr("White %1 %2").arg(i).arg(j));
        //Select next player
        if(_turn=='b') _turn='w';
        else _turn='b';
    }
}
void QGoban::selectStone(int i, int j, const QPoint &pos) {
    Stone *stone=&_goban->stones()[i][j];
    if(stone->colour()!='.') {
        _menu->actions().at(0)->setText(tr("# of freedom : %1").arg(stone->group()->freedom()->size()));
        _menu->actions().at(1)->setText(tr("# of stones : %1").arg(stone->group()->stones()->size()));
        _menu->exec(pos);
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
void QGoban::saveGame() {
    QString filename=QFileDialog::getSaveFileName(this,tr("Save Game"),"./untilted.gogame",tr("Go Game Files (*.gogame)"));
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;
    QTextStream out(&file);
    out << _game->join("\n");
    file.close();
}
void QGoban::openGame() {
    QString filename=QFileDialog::getOpenFileName(this,tr("Open Game"),".",tr("Go Game Files (*.gogame)"));
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;
    QTextStream in(&file);
    if(in.atEnd()) {
        QMessageBox::warning(this,tr("Opening game : error occured"),tr("Unable to read a game from ")+filename);
        return;
    }
    QStringList newGame;
    while (!in.atEnd()) {
        newGame.append(in.readLine());
    }
    replay(newGame);
}
void QGoban::replay(const QStringList &game) {
    init(game.at(0).split(" ").at(1).split("x").at(0).toInt());
    for(int i=1;i<game.size();i++) {
        QStringList line=game.at(i).split(" ");
        if(line.at(0)==tr("White")) _turn='w';
        else _turn='b';
        selectNode(line.at(1).toInt(),line.at(2).toInt());
    }
}
void QGoban::undoMove() {
    if(_game->size()==1)
        return;
    QStringList game(*_game);
    game.removeLast();
    replay(game);
}
void QGoban::passTurn() {
    if(_endgame) {
        int white,black,dame;
        _goban->endGame(white,black,dame);
        _gobanView->redraw(_goban->stones());
        //Update scores
        ui->lcdNumberBlack->display(_goban->score('b'));
        ui->lcdNumberWhite->display(_goban->score('w'));
        QMessageBox::information(this,tr("End Game"),tr("White : %1\nBlack : %2\nDame : %3\n%4 wins !").arg(white).arg(black).arg(dame).arg(white>black?tr("White"):tr("Black")));
    }
    if(_turn=='b') _turn='w';
    else _turn='b';
    _endgame=true;
}
/* qgoban.cpp */