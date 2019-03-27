#ifndef TPLANNINGARRIEREPLAN_H
#define TPLANNINGARRIEREPLAN_H

#include "tEvent.h"

#include <QWidget>
#include <QComboBox>
#include <QPushButton>
#include <QListWidget>
#include <QTableWidget>

class tPlanningArrierePlan : public QWidget
{
  Q_OBJECT

  public:
    tPlanningArrierePlan();
    virtual ~tPlanningArrierePlan();

    void updateByName(QString name);
    void updatePlanningFromParamGlobal();

  protected:
    void init();
    void initTableWidgetPlanning();

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

#endif // TPLANNINGARRIEREPLAN_H
