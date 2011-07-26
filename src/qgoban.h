#ifndef QGOBAN_H
#define QGOBAN_H
#include <QMainWindow>
#include "goban.h"
namespace Ui {
    class QGoban;
}
class QGoban : public QMainWindow, public Goban {
    Q_OBJECT
public:
    QGoban(QWidget *parent = 0);
    ~QGoban();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::QGoban *ui;
};
#endif // QGOBAN_H
/* goban.h */