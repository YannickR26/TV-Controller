#include "tConfig.h"
#include "tParametreGlobal.h"

#include "tTcpSocket.h"

#include <QPushButton>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QTime>
#include <QDate>

extern tParametreGlobal  paramGlobal;

tConfig::tConfig()
    : QWidget(), name()
{
    this->init();
}

tConfig::~tConfig()
{
    //dtor
}

void tConfig::init()
{
    /** Ligne Nom */
    ligneNom = new QLineEdit();

    /* Layout pour le Nom */
    QHBoxLayout *layoutNom = new QHBoxLayout();
    layoutNom->addWidget(new QLabel(tr("Nom : ")));
    layoutNom->addWidget(ligneNom);

    /** Ligne URL */
    ligneUrl = new QLineEdit();
    ligneUrl->setInputMask("000.000.000.000;_");

    /* Layout pour l'URL */
    QHBoxLayout *layoutUrl = new QHBoxLayout();
    layoutUrl->addWidget(new QLabel(tr("Adresse Ip : ")));
    layoutUrl->addWidget(ligneUrl);

    /** Ligne Numero HDMI */
    spinBoxNumHDMI = new QSpinBox();
    spinBoxNumHDMI->setMinimumWidth(50);
    checkBoxUseCEC = new QCheckBox(tr("Utiliser le CEC"));

    /* Layout pour le numero d'HDMI */
    QHBoxLayout *layoutHDMI = new QHBoxLayout();
    layoutHDMI->addWidget(new QLabel(tr("Numero HDMI : ")));
    layoutHDMI->addWidget(spinBoxNumHDMI);
    layoutHDMI->addSpacing(50);
    layoutHDMI->addWidget(checkBoxUseCEC);
    layoutHDMI->addStretch(1);

    /** SpinBox pour les horaires d'ouverture et de fermeture */
    QGridLayout *layoutSpinBoxTime = new QGridLayout();
    layoutSpinBoxTime->addWidget(new QLabel(tr("Ouverture")), 0, 1, Qt::AlignHCenter);
    layoutSpinBoxTime->addWidget(new QLabel(tr("Fermeture")), 0, 2, Qt::AlignHCenter);
    for (int i=1 ; i<8 ; ++i) {
        QTimeEdit *timeOpen = new QTimeEdit();
        timeOpen->setDisplayFormat("hh'h'mm");
        OpeningTime.append(timeOpen);
        QTimeEdit *timeClose = new QTimeEdit();
        timeClose->setDisplayFormat("hh'h'mm");
        ClosingTime.append(timeClose);
        /* Layout pour les spinBox d'horaire */
        layoutSpinBoxTime->addWidget(new QLabel(QDate().longDayName(i)+" : "), i, 0);
        layoutSpinBoxTime->addWidget(OpeningTime.last(), i, 1);
        layoutSpinBoxTime->addWidget(ClosingTime.last(), i, 2);
    }

    /* GroupBox pour les horaires */
    QGroupBox *groupBoxTime = new QGroupBox(tr("Horaires"));
    groupBoxTime->setLayout(layoutSpinBoxTime);
    groupBoxTime->setMinimumWidth(300);
    QHBoxLayout *layoutGBTime = new QHBoxLayout();
    layoutGBTime->addWidget(groupBoxTime);
    layoutGBTime->addStretch(1);

    /** Bouton pour allumer l'écran */
    QPushButton *boutonPowerON = new QPushButton(tr("Allumer TV"));
    connect(boutonPowerON, SIGNAL(clicked()), this, SLOT(PowerOnClick()));
    /** Bouton pour l'envoie du fichier */
    QPushButton *boutonEnvoieFichier = new QPushButton(tr("Envoie du fichier"));
    connect(boutonEnvoieFichier, SIGNAL(clicked()), this, SLOT(EnvoieFichierClick()));
    /** Bouton pour l'envoie du fichier */
    QPushButton *boutonReset = new QPushButton(tr("Redemarrage"));
    connect(boutonReset, SIGNAL(clicked()), this, SLOT(ResetClick()));
    /** Bouton pour allumer l'écran */
    QPushButton *boutonPowerOFF = new QPushButton(tr("Eteindre TV"));
    connect(boutonPowerOFF, SIGNAL(clicked()), this, SLOT(PowerOffClick()));

    /* Layout pour les boutons */
    QHBoxLayout *layoutBoutons = new QHBoxLayout();
    layoutBoutons->addWidget(boutonPowerON);
    layoutBoutons->addWidget(boutonEnvoieFichier);
    layoutBoutons->addWidget(boutonReset);
    layoutBoutons->addWidget(boutonPowerOFF);
    layoutBoutons->addStretch(100);

    /** Bouton Enregistrer */
    QPushButton *boutonEnregistrer = new QPushButton(tr("Enregistrer"));
    connect(boutonEnregistrer, SIGNAL(clicked()), this, SLOT(EnregistrerClick()));

    /* Layout principale */
    QVBoxLayout *layoutPrincipale = new QVBoxLayout();
    layoutPrincipale->addLayout(layoutNom);
    layoutPrincipale->addLayout(layoutUrl);
    layoutPrincipale->addLayout(layoutHDMI);
    layoutPrincipale->addLayout(layoutGBTime);
    layoutPrincipale->addLayout(layoutBoutons);
    layoutPrincipale->addStretch(1);
    layoutPrincipale->addWidget(boutonEnregistrer);

    /** Generale */
    this->setLayout(layoutPrincipale);
}

void tConfig::setFileName(QString filename_)
{
    this->filename = filename_;
}

void tConfig::updateByName(QString name)
{
    QSettings setting;
    for (QList<tTv>::iterator i=paramGlobal.listTv.begin() ; i!=paramGlobal.listTv.end() ; ++i) {
        if ((*i).getName() == name) {
            ligneNom->setText((*i).getName());
            ligneUrl->setText((*i).getUrl());
            spinBoxNumHDMI->setValue((*i).getNumHDMI());
            checkBoxUseCEC->setChecked((*i).getUseCEC());
            // Update Scheduler
            if ((*i).listScheduler.count() != 7) {
                (*i).listScheduler.clear();
                for (int j=0 ; j<7 ; ++j) {
                    tScheduler sch;
                    sch.setOpeningTime(QTime().fromString(setting.value(QString("OpeningTime%1").arg(j), "09:00").toString(), "hh:mm"));
                    sch.setClosingTime(QTime().fromString(setting.value(QString("ClosingTime%1").arg(j), "21:00").toString(), "hh:mm"));
                    sch.setDate(QDate().fromString(QDate().longDayName(j+1), "dddd"));
                    (*i).listScheduler.append(sch);
                }
            }
            for (QList<tScheduler>::iterator j=(*i).listScheduler.begin() ; j!=(*i).listScheduler.end() ; ++j) {
                int k = (*j).getDate().dayOfWeek() - 1;
                OpeningTime.at(k)->setTime((*j).getOpeningTime());
                ClosingTime.at(k)->setTime((*j).getClosingTime());
            }
        }
    }
    this->name = name;
}

void tConfig::EnregistrerClick()
{
    for (QList<tTv>::iterator i=paramGlobal.listTv.begin() ; i!=paramGlobal.listTv.end() ; ++i) {
        if ((*i).getName() == this->name) {
            (*i).setName(ligneNom->text());
            (*i).setUrl(ligneUrl->text());
            (*i).setNumHDMI(spinBoxNumHDMI->value());
            (*i).setUseCEC(checkBoxUseCEC->isChecked());
            int k=0;
            // Update Scheduler
            for (QList<tScheduler>::iterator j=(*i).listScheduler.begin() ; j!=(*i).listScheduler.end() ; ++j, ++k) {
                (*j).setOpeningTime(OpeningTime.at(k)->time());
                (*j).setClosingTime(ClosingTime.at(k)->time());
            }
        }
    }
    emit this->updateConfig();
}

void tConfig::PowerOnClick()
{
    // Create Socket
    tTcpSocket *myTcpSocket = new tTcpSocket();

    // Get IP Address
    QString addr = ligneUrl->text();

    // Delete all 0 after .
    while (addr.indexOf(".0") >= 0) {
        addr = addr.replace(".0", ".");
    }

    // Connect to Host
    myTcpSocket->connectToHost(QHostAddress(addr), 10000);

    // Send Data
    myTcpSocket->writeData("Set: PowerOnHDMI\n");

    // Close connection
    myTcpSocket->disconnect();
}

void tConfig::EnvoieFichierClick()
{
    // Create Socket
    tTcpSocket *myTcpSocket = new tTcpSocket();

    // Get IP Address
    QString addr = ligneUrl->text();

    // Delete all 0 after .
    while (addr.indexOf(".0") >= 0) {
        addr = addr.replace(".0", ".");
    }

    // Connect to Host
    myTcpSocket->connectToHost(QHostAddress(addr), 10000);

    /** Send File */

    QString _filename = this->filename.mid(this->filename.lastIndexOf("/")+1);
    QFile inputFile(this->filename);
    inputFile.open(QIODevice::ReadOnly);
    QByteArray data = "File: " + _filename.toUtf8() + "\n";
    data += inputFile.readAll();
    myTcpSocket->writeData(data);
    inputFile.close();

    // Close connection
    myTcpSocket->disconnect();
}

void tConfig::ResetClick()
{
    // Create Socket
    tTcpSocket *myTcpSocket = new tTcpSocket();

    // Get IP Address
    QString addr = ligneUrl->text();

    // Delete all 0 after .
    while (addr.indexOf(".0") >= 0) {
        addr = addr.replace(".0", ".");
    }

    // Connect to Host
    myTcpSocket->connectToHost(QHostAddress(addr), 10000);

    // Send Data
    myTcpSocket->writeData("Set: Restart\n");

    // Close connection
    myTcpSocket->disconnect();
}

void tConfig::PowerOffClick()
{
    // Create Socket
    tTcpSocket *myTcpSocket = new tTcpSocket();

    // Get IP Address
    QString addr = ligneUrl->text();

    // Delete all 0 after .
    while (addr.indexOf(".0") >= 0) {
        addr = addr.replace(".0", ".");
    }

    // Connect to Host
    myTcpSocket->connectToHost(QHostAddress(addr), 10000);

    // Send Data
    myTcpSocket->writeData("Set: PowerOffHDMI\n");

    // Close connection
    myTcpSocket->disconnect();
}
