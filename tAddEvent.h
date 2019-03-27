#ifndef TADDEVENT_H
#define TADDEVENT_H

#include "tEvent.h"

#include <QDialog>
#include <QSpinBox>
#include <QTimeEdit>
#include <QCheckBox>
#include <QGroupBox>
#include <QComboBox>

class tAddEvent : public QDialog
{
  Q_OBJECT

  public:
    tAddEvent(QString &_tvName);
    tAddEvent(QString &_tvName, QString _mediaName);
    tAddEvent(QString &_tvName, tEvent &_event);
    virtual ~tAddEvent();

  protected:
    void init();

  private slots:
    void comboBoxTypeChanged(const QString &text);
    void boutonOkClick();
    void boutonCancelClick();

  private:
    QString     tvName;
    QString     mediaName;
    tEvent      *event;
    QComboBox   *comboBoxType;
    QComboBox   *comboBoxMedia;
    QCheckBox   *checkBox_lundi;
    QCheckBox   *checkBox_mardi;
    QCheckBox   *checkBox_mercredi;
    QCheckBox   *checkBox_jeudi;
    QCheckBox   *checkBox_vendredi;
    QCheckBox   *checkBox_samedi;
    QCheckBox   *checkBox_dimanche;
    QGroupBox   *groupBoxRepeat;
    QTimeEdit   *timeEditDebut;
    QSpinBox    *spinBoxHourRepeat;
    QSpinBox    *spinBoxMinRepeat;
    QSpinBox    *spinBoxHourEnd;
    QSpinBox    *spinBoxMinEnd;
};

#endif // TADDEVENT_H
