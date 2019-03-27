#ifndef TGESSCRIPT_H
#define TGESSCRIPT_H

#include "tTcpSocket.h"

#include <QDialog>
#include <QProcess>
#include <QTextEdit>
#include <QListWidget>

class tGesScript : public QDialog
{
  Q_OBJECT

  public:
    tGesScript(QString _filename);
    virtual ~tGesScript();

  protected:
    void init();

  private slots:
    void majTvClick();
    void annulerClick();
    void QuitterClick();

  private:
    QTextEdit       *textEditStatus;
    tTcpSocket      *myTcpSocket;
    QString         filename;

    QListWidget     *listTV;
    QPushButton     *bouttonConnect;
    QPushButton     *bouttonDeconnect;
};

#endif // TGESSCRIPT_H
