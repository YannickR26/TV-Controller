#ifndef TPREFERENCES_H
#define TPREFERENCES_H

#include <QLineEdit>
#include <QTimeEdit>
#include <QGroupBox>
#include <QSpinBox>
#include <QDialog>

class tPreferences : public QDialog
{
  Q_OBJECT

  public:
    tPreferences();
    virtual ~tPreferences();

  protected:
    void init();
    void updateFromSettings();

  private slots:
    void boutonLogoClick();
    void boutonScreenClick();
    void boutonOpenClick();
    void boutonOkClick();
    void boutonCancelClick();

  private:
    QSpinBox    *spinBoxStep;
    QSpinBox    *spinBoxStepBackground;
    QList<QTimeEdit *>    OpeningTime, ClosingTime;
    QSpinBox    *spinBoxPosXLogo, *spinBoxPosYLogo;
    QLineEdit   *lineEditLogo;
    QLineEdit   *lineEditScreen;
    QGroupBox   *groupBoxLogo;
    QLineEdit   *lineEditPath;
};

#endif // TPREFERENCES_H
