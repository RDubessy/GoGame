#ifndef QGOBAN_H
#define QGOBAN_H
#include <QMainWindow>
#include "goban.h"
class QGobanView;
namespace Ui {
    class QGoban;
}
class QGoban : public QMainWindow, public Goban {
    Q_OBJECT
public:
    QGoban(QWidget *parent = 0);
    ~QGoban();
public slots:
    void selectNode(int i, int j);
protected:
    void changeEvent(QEvent *e);

private:
    Ui::QGoban *ui;
    Goban *_goban;
    QGobanView *_gobanView;
    char _turn;
};
#endif // QGOBAN_H
/* goban.h */