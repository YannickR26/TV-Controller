#include "tLive.h"
#include "tParametreGlobal.h"

#include <QFile>
#include <QVBoxLayout>
#include <QHostAddress>

#include <windows.h> // for Sleep

extern tParametreGlobal  paramGlobal;

tLive::tLive()
    : QWidget()
{
    this->init();
}

tLive::~tLive()
{
    //dtor
}

void tLive::updateByName(QString name_)
{
    this->nameTv = name_;
    this->boutonStart->setEnabled(false);
    this->boutonStop->setEnabled(false);
    this->textEditStatus->clear();
}

void tLive::init()
{
    /** Liste des types */
    this->comboBoxType = new QComboBox();
    this->comboBoxType->addItem(tr("Arriere Plan"));
    this->comboBoxType->addItem(tr("Photo"));
    this->comboBoxType->addItem(tr("Video"));
    connect(this->comboBoxType, SIGNAL(currentIndexChanged(QString const &)), this, SLOT(typeChange(QString const &)));

    /** Liste des Medias */
    this->listWidgetMedia = new QListWidget();
    this->listWidgetMedia->setMaximumWidth(150);
    connect(this->listWidgetMedia, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(mediaClick(QListWidgetItem *)));

    /** Boutons */
    this->boutonMode = new QPushButton(tr("Mode Auto"));
    connect(this->boutonMode, SIGNAL(clicked()), this, SLOT(changeMode()));
    this->boutonMode->setStyleSheet("QPushButton { background-color: green; font-size: 30px; }");
    this->boutonMode->setMinimumHeight(50);
    this->getStatus = new QPushButton(tr("Lecture du Status"));
    connect(this->getStatus, SIGNAL(clicked()), this, SLOT(readStatus()));
    this->boutonStart = new QPushButton(tr("Start"));
    this->boutonStart->setEnabled(false);
    connect(this->boutonStart, SIGNAL(clicked()), this, SLOT(startLive()));
    this->boutonStop = new QPushButton(tr("Stop"));
    this->boutonStop->setEnabled(false);
    connect(this->boutonStop, SIGNAL(clicked()), this, SLOT(stopLive()));

    /* Layout pour les boutons */
    QHBoxLayout *layoutBotton = new QHBoxLayout();
    layoutBotton->addWidget(this->boutonStart);
    layoutBotton->addWidget(this->boutonStop);

    /** En cours */
    this->lineEnCours = new QLineEdit();
    this->lineEnCours->setReadOnly(true);

    /** Console */
    this->textEditStatus = new QTextEdit();
    this->textEditStatus->setReadOnly(true);
    this->textEditStatus->setStyleSheet("* { background-color: rgb(50, 50, 50); }");
    this->textEditStatus->setTextColor(QColor("white"));

    /* Layout pour les boutons et la ligne en cours */
    QVBoxLayout *layoutBottonLine = new QVBoxLayout();
    layoutBottonLine->addWidget(boutonMode);
    layoutBottonLine->addSpacing(20);
    layoutBottonLine->addWidget(getStatus);
    layoutBottonLine->addLayout(layoutBotton);
    layoutBottonLine->addWidget(lineEnCours);
    layoutBottonLine->addWidget(textEditStatus);
//    layoutBottonLine->addStretch();

    /* Layout pour la liste */
    QVBoxLayout *layoutList = new QVBoxLayout();
    layoutList->addWidget(this->comboBoxType);
    layoutList->addWidget(this->listWidgetMedia);

    /* Layout principale */
    QHBoxLayout *layoutPrincipale = new QHBoxLayout();
    layoutPrincipale->addLayout(layoutList);
    layoutPrincipale->addLayout(layoutBottonLine);
    //layoutPrincipale->addStretch();

    this->myTcpSocket = new tTcpSocket();
    connect(this->myTcpSocket, SIGNAL(dataReceived(QByteArray)), this, SLOT(dataReceivedFromTcp(QByteArray)));

    this->setLayout(layoutPrincipale);
}

void tLive::typeChange(QString const &text)
{
    Q_UNUSED(text);
    this->updateListMediaFromParamGlobal();
    this->boutonStart->setEnabled(false);
}

void tLive::mediaClick(QListWidgetItem *item)
{
    if (item->isSelected())
        this->boutonStart->setEnabled(true);
    else
        this->boutonStart->setEnabled(false);
}

void tLive::updateListMediaFromParamGlobal()
{
    this->listWidgetMedia->clear();

    /** On ajoute les media correspondant au type */
    for (QList<tMedia>::iterator i=paramGlobal.listMedia.begin() ; i!=paramGlobal.listMedia.end() ; ++i) {
        if ((*i).getType() == this->comboBoxType->currentText()) {
            this->listWidgetMedia->addItem((*i).getName());
        }
    }
}

void tLive::dataReceivedFromTcp(QByteArray data)
{
    QString firstLine = QString(data).mid(0, QString(data).indexOf("\n"));
    QString cmd = firstLine.mid(0, firstLine.indexOf(":"));
    QString param = firstLine.mid(firstLine.indexOf(":")+2);

    data.remove(0, data.indexOf("\n")+1);

    if (cmd == QString("Get")) {
        if (param == QString("Mode")) {
            if (data == "Auto\n") {
                this->boutonMode->setText("Mode Auto");
                this->boutonMode->setStyleSheet("QPushButton { background-color: green; font-size: 30px; }");
                this->textEditStatus->append(QString("Mode Auto !"));
            }
            else if (data == "Manuel\n") {
                this->boutonMode->setText("Mode Manuel");
                this->boutonMode->setStyleSheet("QPushButton { background-color: red; font-size: 30px; }");
                this->textEditStatus->append(QString("Mode Manuel !"));
            }
        }
        else if (param == QString("Live")) {
            QString cmdData = data.mid(0, data.indexOf(":"));
            QString paramData = data.mid(data.indexOf(":")+1);
            if (cmdData == "1") {
                this->textEditStatus->append(QString("Direct en cours !"));
                this->lineEnCours->setText(paramData);
                this->boutonStart->setEnabled(false);
                this->boutonStop->setEnabled(true);
            }
            else if (cmdData == "0") {
                this->textEditStatus->append(QString("Aucun direct en cours !"));
                this->lineEnCours->clear();
                this->boutonStart->setEnabled(true);
                this->boutonStop->setEnabled(false);
            }
        }
    }

    this->textEditStatus->append("");
    //disconnect(this->myTcpSocket, SIGNAL(dataReceived(QByteArray)), this, SLOT(dataReceivedFromTcp(QByteArray)));
    this->myTcpSocket->disconnect();
}

void tLive::changeMode()
{
    if (!connectToHost()) {
        this->textEditStatus->append(QString("Impossible de ce connecter !!!\n  Error: %1").arg(this->myTcpSocket->getError()));
    }
    else {
//        this->textEditStatus->append("Connecter !!!");

        QString modeAuto = this->boutonMode->text() == "Mode Manuel" ? "Auto" : "Manuel";
//        if (modeAuto == "Auto") {
//            this->boutonMode->setText("Mode Auto");
//            this->boutonMode->setStyleSheet("QPushButton { background-color: green; font-size: 30px; }");
//        }
//        else {
//            this->boutonMode->setText("Mode Manuel");
//            this->boutonMode->setStyleSheet("QPushButton { background-color: red; font-size: 30px; }");
//        }

        this->textEditStatus->append(QString("Set mode: " + modeAuto));
        QByteArray data = "Set: Mode\n";
        data += modeAuto;
        this->myTcpSocket->writeData(data);
    }
}

void tLive::readStatus()
{
    if (!connectToHost()) {
        this->textEditStatus->append(QString("Impossible de ce connecter !!!\n  Error: %1").arg(this->myTcpSocket->getError()));
    }
    else {
        //this->textEditStatus->append("Connecter !!!");

        this->textEditStatus->append(QString("Lecture du status en cours..."));

        this->myTcpSocket->writeData("Get: Live\n");
        this->myTcpSocket->writeData("Get: Mode\n");
    }
}

void tLive::readMode()
{
    if (!connectToHost()) {
        this->textEditStatus->append(QString("Impossible de ce connecter !!!\n  Error: %1").arg(this->myTcpSocket->getError()));
    }
    else {
        //this->textEditStatus->append("Connecter !!!");

        this->myTcpSocket->writeData("Get: Mode\n");
    }
}

void tLive::startLive()
{
    if (!connectToHost()) {
        this->textEditStatus->append(QString("Impossible de ce connecter !!!\n  Error: %1").arg(this->myTcpSocket->getError()));
    }
    else {
        //this->textEditStatus->append("Connecter !!!");

        QByteArray data = "Set: Live_Start\n";
        this->lineEnCours->clear();
        this->lineEnCours->setText(this->listWidgetMedia->currentItem()->text());
        data += this->listWidgetMedia->currentItem()->text();
        this->myTcpSocket->writeData(data);

        this->textEditStatus->append(QString("Direct en cours de diffusion..."));
        this->textEditStatus->append("");

        this->myTcpSocket->disconnect();

        this->boutonStart->setEnabled(false);
        this->boutonStop->setEnabled(true);
    }
}

void tLive::stopLive()
{
    if (!connectToHost()) {
        this->textEditStatus->append(QString("Impossible de ce connecter !!!\n  Error: %1").arg(this->myTcpSocket->getError()));
    }
    else {
        //this->textEditStatus->append("Connecter !!!");

        this->lineEnCours->clear();
        this->myTcpSocket->writeData("Set: Live_Stop\n");

        this->textEditStatus->append(QString("Direct interrompu !"));
        this->textEditStatus->append("");

        this->myTcpSocket->disconnect();

        this->boutonStart->setEnabled(true);
        this->boutonStop->setEnabled(false);
    }
}

bool tLive::connectToHost()
{
    QString addr;
    for (QList<tTv>::iterator i=paramGlobal.listTv.begin() ; i!=paramGlobal.listTv.end() ; ++i) {
        if ((*i).getName() == this->nameTv) {
            addr = (*i).getUrl();
            break;
        }
    }

    // Delete all 0 after .
    while (addr.indexOf(".0") >= 0) {
        addr = addr.replace(".0", ".");
    }

//    this->textEditStatus->append(QString("Connection en cours à : %1 (ip: %2)").arg(this->nameTv).arg(addr));
//    this->update();
    return this->myTcpSocket->connectToHost(QHostAddress(addr), 10000);
}
