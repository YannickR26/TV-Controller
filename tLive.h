#ifndef TLIVE_H
#define TLIVE_H

#include "tTcpSocket.h"

#include <QWidget>
#include <QComboBox>
#include <QLineEdit>
#include <QTextEdit>
#include <QListWidget>
#include <QPushButton>

class tLive : public QWidget
{
  Q_OBJECT

  public:
    tLive();
    virtual ~tLive();
    void updateListMediaFromParamGlobal();
    void updateByName(QString name_);

  protected:
    void init();
    bool connectToHost();

  private slots:
    void typeChange(QString const &text);
    void mediaClick(QListWidgetItem *item);
    void dataReceivedFromTcp(QByteArray data);
    void changeMode();
    void readStatus();
    void readMode();
    void startLive();
    void stopLive();

  private:
    QString         nameTv;
    tTcpSocket      *myTcpSocket;
    QComboBox       *comboBoxType;
    QListWidget     *listWidgetMedia;
    QPushButton     *boutonMode, *getStatus, *boutonStart, *boutonStop;
    QLineEdit       *lineEnCours;
    QTextEdit       *textEditStatus;
};

#endif // TLIVE_H
