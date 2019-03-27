#ifndef TPLANNING_H
#define TPLANNING_H

#include "tEvent.h"

#include <QWidget>
#include <QDropEvent>
#include <QPushButton>
#include <QTableWidget>

class tPlanning : public QWidget
{
    Q_OBJECT

public:
    tPlanning();
    virtual ~tPlanning();

    void updateByName(QString name);
    void updateListMediaFromParamGlobal();
    void updatePlanningFromParamGlobal();


protected:
    void init();
    void initTableWidgetPlanning();
    virtual void dropEvent(QDropEvent *event);

private slots:
    void boutonAddClick();
    void boutonModifyClick();
    void boutonDelClick();

private:
    QTableWidget    *tableWidgetPlanning;
    QPushButton     *boutonAdd;
    QPushButton     *boutonModify;
    QPushButton     *boutonDel;
    QString         nameTv;
    int             timeMin, timeMax;
};

#endif // TPLANNING_H
