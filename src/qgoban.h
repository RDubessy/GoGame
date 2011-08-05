#ifndef QGOBAN_H
#define QGOBAN_H
#include <QMainWindow>
class Goban;
class QGobanView;
class QStringList;
class QMenu;
namespace Ui {
    class QGoban;
}
class QGoban : public QMainWindow {
    Q_OBJECT
public:
    QGoban(QWidget *parent = 0);
    ~QGoban();
    void init(int size);
    void replay(const QStringList &game);
public slots:
    void selectNode(int i, int j);
    void selectStone(int i, int j, const QPoint &pos);
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
    QMenu *_menu;
    char _turn;
    bool _endgame;
};
#endif // QGOBAN_H
/* goban.h */