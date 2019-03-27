#ifndef TGESMEDIA_H
#define TGESMEDIA_H

#include "tShowColor.h"

#include <QListWidget>
#include <QDomElement>
#include <QLineEdit>
#include <QGroupBox>
#include <QCheckBox>
#include <QComboBox>
#include <QTimeEdit>
#include <QDialog>
#include <QList>

class tGesMedia : public QDialog
{
  Q_OBJECT

  public:
    tGesMedia();
    virtual ~tGesMedia();

  protected:
    void init();
    void updateFromParamGlobal();

  private slots:
    void listMediaChange(QListWidgetItem *item);
    void ajouterMediaClick();
    void supprimerMediaClick();
    void UrlClick();
    void ColorClick();
    void EnregistrerClick();
    void QuitterClick();

  private:
    QListWidget     *listWidgetMedia;
    QComboBox       *comboBoxType;
    QLineEdit       *ligneNom;
    QLineEdit       *ligneUrl;
    QPushButton     *boutonUrl;
    QGroupBox       *groupBoxOptions;
    QCheckBox       *option_fadeIn;
    QCheckBox       *option_fadeOut;
    QCheckBox       *option_repeat;
    QLineEdit       *option_login;
    QLineEdit       *option_password;
    tShowColor      *showColor;
    QPushButton     *boutonColor;
    QTimeEdit       *timeEditDuree;
};

#endif // TGESMEDIA_H
