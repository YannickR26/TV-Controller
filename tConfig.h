#ifndef TCONFIG_H
#define TCONFIG_H

#include <QWidget>
#include <QSpinBox>
#include <QTimeEdit>
#include <QLineEdit>
#include <QCheckBox>

class tConfig : public QWidget
{
  Q_OBJECT

  public:
    tConfig();
    virtual ~tConfig();

    void updateByName(QString name);
    void setFileName(QString filename_);

  signals:
    void updateConfig();

  protected:
    void init();

  private slots:
    void EnregistrerClick();
    void PowerOnClick();
    void EnvoieFichierClick();
    void ResetClick();
    void PowerOffClick();

  private:
    QString     filename;
    QLineEdit   *ligneNom;
    QLineEdit   *ligneUrl;
    QString     name;
    QSpinBox    *spinBoxNumHDMI;
    QCheckBox   *checkBoxUseCEC;
    QList<QTimeEdit *>    OpeningTime, ClosingTime;
};

#endif // TCONFIG_H
