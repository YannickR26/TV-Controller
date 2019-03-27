#ifndef TADDEVENT_H
#define TADDEVENT_H

#include "tEvent.h"

#include <QDialog>
#include <QSpinBox>
#include <QTimeEdit>
#include <QCheckBox>
#include <QGroupBox>
#include <QComboBox>

class tAddBackground : public QDialog
{
  Q_OBJECT

  public:
    tAddBackground(QString &_tvName);
    tAddBackground(QString &_tvName, QString _mediaName);
    virtual ~tAddBackground();

  protected:
    void init();

  private slots:
    void comboBoxTypeChanged(const QString &text);
    void boutonOkClick();
    void boutonCancelClick();

  private:
    QString     tvName;
    QString     mediaName;
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
    QTimeEdit   *timeEditFin;
};

#endif // TADDEVENT_H
