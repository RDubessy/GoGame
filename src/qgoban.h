#ifndef QGOBAN_H
#define QGOBAN_H
#include <QMainWindow>
#include "goban.h"
class QGobanView;
class QStringList;
namespace Ui {
    class QGoban;
}
class QGoban : public QMainWindow, public Goban {
    Q_OBJECT
public:
    QGoban(QWidget *parent = 0);
    ~QGoban();
    void init(int size);
    void replay(const QStringList &game);
public slots:
    void selectNode(int i, int j);
    void about();
    void newGame();
    void saveGame();
    void openGame();
    void undoMove();
    void passTurn();
protected:
    void changeEvent(QEvent *e);

private:
    Ui::QGoban *ui;
    Goban *_goban;
    QGobanView *_gobanView;
    QStringList *_game;
    char _turn;
};
#endif // QGOBAN_H
/* goban.h */