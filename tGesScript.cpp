#include "tGesScript.h"
#include "tParametreGlobal.h"

#include <stdio.h>

#include <QIcon>
#include <QFile>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QPushButton>
#include <QHostAddress>

extern tParametreGlobal  paramGlobal;

tGesScript::tGesScript(QString _filename)
  : QDialog()
{
  this->filename = _filename;
  this->init();
  this->myTcpSocket = new tTcpSocket();
}

tGesScript::~tGesScript()
{
}

void tGesScript::init()
{
  /** List TV */
  this->listTV = new QListWidget();
  this->listTV->setMaximumWidth(150);

  /** On ajoute le nom de la TV dans la liste des TV */
  for (QList<tTv>::iterator i=paramGlobal.listTv.begin() ; i!=paramGlobal.listTv.end() ; ++i) {
    QListWidgetItem* item = new QListWidgetItem((*i).getName());
    item->setFlags(item->flags() | Qt::ItemIsUserCheckable); // set checkable flag
    item->setCheckState(Qt::Checked); // AND initialize check state
    this->listTV->addItem(item);
  }

  /** Bouton Connect */
  this->bouttonConnect = new QPushButton(tr("MAJ TV"));
  connect(this->bouttonConnect, SIGNAL(clicked()), this, SLOT(majTvClick()));
  /** Bouton Deconnect */
  this->bouttonDeconnect = new QPushButton(tr("Annuler"));
  this->bouttonDeconnect->setEnabled(false);
  connect(this->bouttonDeconnect, SIGNAL(clicked()), this, SLOT(annulerClick()));
  /* Layout Bouton connect et deconect */
  QHBoxLayout *layout_bouttonConnectDisconnect = new QHBoxLayout();
  layout_bouttonConnectDisconnect->addWidget(this->bouttonConnect);
  layout_bouttonConnectDisconnect->addWidget(this->bouttonDeconnect);

  this->textEditStatus = new QTextEdit();
  this->textEditStatus->setReadOnly(true);

  /** Bouton Quitter */
  QPushButton *boutonQuitter = new QPushButton(tr("Quitter"));
  connect(boutonQuitter, SIGNAL(clicked()), this, SLOT(QuitterClick()));

  /* Layout principale */
  QVBoxLayout *layoutPrincipale = new QVBoxLayout();
  layoutPrincipale->addLayout(layout_bouttonConnectDisconnect);
  layoutPrincipale->addWidget(textEditStatus);
  layoutPrincipale->addWidget(boutonQuitter);


  QHBoxLayout *layout_Main = new QHBoxLayout();
  layout_Main->addWidget(this->listTV);
  layout_Main->addLayout(layoutPrincipale);

  /** Generale */
  this->setWindowTitle(tr("Gestionnaire de Téléchargement"));
  this->setWindowIcon(QIcon(":/icon/pixmap/blog.png"));
  this->setLayout(layout_Main);
  this->resize(450, 250);
}

void tGesScript::majTvClick()
{
  this->bouttonConnect->setEnabled(false);
  this->bouttonDeconnect->setEnabled(true);

  for (int i=0 ; i!=this->listTV->count() ; ++i) {
    QListWidgetItem *item = this->listTV->item(i);

    /** Si nous avons selectionné la TV, nous la mettons a jour */
    if (item->checkState()) {

      QString addr;
      for (QList<tTv>::iterator i=paramGlobal.listTv.begin() ; i!=paramGlobal.listTv.end() ; ++i) {
        if ((*i).getName() == item->text()) {
          addr = (*i).getUrl();
          break;
        }
      }

      // Delete all 0 after .
      while (addr.indexOf(".0") >= 0) {
          addr = addr.replace(".0", ".");
      }

      this->textEditStatus->append(QString("Connection en cours à : %1").arg(item->text()));
      bool res = this->myTcpSocket->connectToHost(QHostAddress(addr), 10000);

      if (!res) {
        this->textEditStatus->append(QString("Impossible de ce connecter !!!\n  Error: %1").arg(this->myTcpSocket->getError()));
      }
      else {
        this->textEditStatus->append("Connecter !!!");

        QString _filename = filename.mid(filename.lastIndexOf("/")+1);
        this->textEditStatus->append(QString("Transfer du fichier : %1").arg(_filename));

        /** Send File */
        QFile inputFile(filename);
        inputFile.open(QIODevice::ReadOnly);
        QByteArray data = "File: " + _filename.toUtf8() + "\n";
        data += inputFile.readAll();
        this->myTcpSocket->writeData(data);
        inputFile.close();

        this->myTcpSocket->writeData("Set: Restart\n");

        this->myTcpSocket->disconnect();

        this->textEditStatus->append(QString("Fichier transferé avec success !!!"));
      }
      this->textEditStatus->append(QString(""));
    }
  }
  this->bouttonConnect->setEnabled(true);
  this->bouttonDeconnect->setEnabled(false);
}

void tGesScript::annulerClick()
{
  this->myTcpSocket->disconnect();
  this->bouttonConnect->setEnabled(true);
  this->bouttonDeconnect->setEnabled(false);
  this->textEditStatus->append("Transfer annulé !!!");
}

void tGesScript::QuitterClick()
{
  this->accept();
}
