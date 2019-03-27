#include "tPreferences.h"

#include <QLabel>
#include <QSettings>
#include <QGroupBox>
#include <QFileDialog>
#include <QPushButton>
#include <QMessageBox>
#include <QHBoxLayout>

tPreferences::tPreferences()
    : QDialog()
{
    this->init();
}

tPreferences::~tPreferences()
{
    //dtor
}

void tPreferences::init()
{
    /** Step pour le planning */
    spinBoxStep = new QSpinBox();
    spinBoxStep->setSuffix(" Min");
    spinBoxStep->setMaximum(60);

    /* Layout pour le spinBox */
    QHBoxLayout *layoutSpinBoxStep = new QHBoxLayout();
    layoutSpinBoxStep->addWidget(new QLabel(tr("Granularité planning :")));
    layoutSpinBoxStep->addWidget(spinBoxStep);

    /** Step pour l'arriere plan */
    spinBoxStepBackground = new QSpinBox();
    spinBoxStepBackground->setSuffix(" Min");
    spinBoxStepBackground->setMaximum(60);

    /* Layout pour le spinBox */
    QHBoxLayout *layoutSpinBoxStepBackground = new QHBoxLayout();
    layoutSpinBoxStepBackground->addWidget(new QLabel(tr("Granularité arriére plan :")));
    layoutSpinBoxStepBackground->addWidget(spinBoxStepBackground);

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
    QGroupBox *groupBoxTime = new QGroupBox(tr("Horaires par défaut"));
    groupBoxTime->setLayout(layoutSpinBoxTime);

    /** Parametre du Logo */
    lineEditLogo = new QLineEdit();
    QPushButton *bpLogo = new QPushButton(tr("..."));
    bpLogo->setFixedWidth(50);
    connect(bpLogo, SIGNAL(clicked()), this, SLOT(boutonLogoClick()));
    spinBoxPosXLogo = new QSpinBox();
    spinBoxPosXLogo->setMaximum(10000);
    spinBoxPosXLogo->setSuffix(" Pixels");
    spinBoxPosYLogo = new QSpinBox();
    spinBoxPosYLogo->setMaximum(10000);
    spinBoxPosYLogo->setSuffix(" Pixels");

    /* GroupBox pour le logo */
    QGridLayout *layoutLogo = new QGridLayout();
    layoutLogo->addWidget(new QLabel(tr("Logo :")), 0, 0);
    layoutLogo->addWidget(lineEditLogo, 0, 1);
    layoutLogo->addWidget(bpLogo, 0, 2);
    layoutLogo->addWidget(new QLabel(tr("Position X :")), 1, 0);
    layoutLogo->addWidget(spinBoxPosXLogo, 1, 1, 1, 2);
    layoutLogo->addWidget(new QLabel(tr("Position Y :")), 2, 0);
    layoutLogo->addWidget(spinBoxPosYLogo, 2, 1, 1, 2);
    groupBoxLogo = new QGroupBox(tr("Incrustation du logo"));
    groupBoxLogo->setCheckable(true);
    groupBoxLogo->setLayout(layoutLogo);

    /** Fichier de l'arriere plan */
    lineEditScreen = new QLineEdit();
    QPushButton *bpScreen = new QPushButton(tr("..."));
    bpScreen->setFixedWidth(50);
    connect(bpScreen, SIGNAL(clicked()), this, SLOT(boutonScreenClick()));

    /* Layout pour le lineEdit */
    QHBoxLayout *layoutLineEditScreen = new QHBoxLayout();
    layoutLineEditScreen->addWidget(new QLabel(tr("Arrière plan fixe :")));
    layoutLineEditScreen->addWidget(lineEditScreen);
    layoutLineEditScreen->addWidget(bpScreen);

    /** Fichier ouvert par defaut */
    lineEditPath = new QLineEdit();
    QPushButton *buttonOpen = new QPushButton(tr("..."));
    buttonOpen->setMaximumWidth(40);
    connect(buttonOpen, SIGNAL(clicked()), this, SLOT(boutonOpenClick()));

    /* Layout pour le lineEdit */
    QHBoxLayout *layoutLineEditPath = new QHBoxLayout();
    layoutLineEditPath->addWidget(new QLabel(tr("Lancement au démarrage :")));
    layoutLineEditPath->addWidget(lineEditPath);
    layoutLineEditPath->addWidget(buttonOpen);

    /** Boutons Ok et Annuler */
    QPushButton *boutonOk = new QPushButton(tr("Ok"));
    connect(boutonOk, SIGNAL(clicked()), this, SLOT(boutonOkClick()));
    QPushButton *boutonCancel = new QPushButton(tr("Annuler"));
    connect(boutonCancel, SIGNAL(clicked()), this, SLOT(boutonCancelClick()));

    /* Layout pour les boutons */
    QHBoxLayout *layoutBouton = new QHBoxLayout();
    layoutBouton->addWidget(boutonOk);
    layoutBouton->addStretch(1);
    layoutBouton->addWidget(boutonCancel);

    /* Layout principale */
    QVBoxLayout *layoutPrincipale = new QVBoxLayout();
    layoutPrincipale->addLayout(layoutSpinBoxStep);
    layoutPrincipale->addLayout(layoutSpinBoxStepBackground);
    layoutPrincipale->addWidget(groupBoxTime);
    layoutPrincipale->addWidget(groupBoxLogo);
    layoutPrincipale->addLayout(layoutLineEditScreen);
    layoutPrincipale->addLayout(layoutLineEditPath);
    layoutPrincipale->addStretch(1);
    layoutPrincipale->addLayout(layoutBouton);

    /** Generale */
    this->updateFromSettings();
    this->setWindowTitle(tr("Préférences"));
    this->setWindowIcon(QIcon(":/icon/pixmap/tools.png"));
    this->setLayout(layoutPrincipale);
    this->resize(350, 500);
}

void tPreferences::updateFromSettings()
{
    QSettings setting;
    this->spinBoxStep->setValue(setting.value("Step", 15).toInt());
    this->spinBoxStepBackground->setValue(setting.value("StepBackground", 15).toInt());
    this->lineEditPath->setText(setting.value("DefaultFile").toString());
    // Planning
    for (int i=0 ; i<7 ; ++i) {
        this->OpeningTime.at(i)->setTime(QTime().fromString(setting.value(QString("OpeningTime%1").arg(i), "09:00").toString(), "hh:mm"));
        this->ClosingTime.at(i)->setTime(QTime().fromString(setting.value(QString("ClosingTime%1").arg(i), "21:00").toString(), "hh:mm"));
    }
    // Logo
    this->lineEditLogo->setText(setting.value("Logo/Url").toString());
    this->spinBoxPosXLogo->setValue(setting.value("Logo/PosX", 0).toInt());
    this->spinBoxPosYLogo->setValue(setting.value("Logo/PosY", 0).toInt());
    this->groupBoxLogo->setChecked(setting.value("Logo/Enable", false).toBool());
    // Background
    this->lineEditScreen->setText(setting.value("Background").toString());
}

void tPreferences::boutonLogoClick()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Ouvrir"), "", tr("Images (*.png)"));
    if (filename.isEmpty()) return;
    this->lineEditLogo->setText(filename);
}

void tPreferences::boutonScreenClick()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Ouvrir"), "", tr("Images (*.png)"));
    if (filename.isEmpty()) return;
    this->lineEditScreen->setText(filename);
}

void tPreferences::boutonOpenClick()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Ouvrir"), "", tr("Xml Files (*.xml)"));
    if (filename.isEmpty()) return;
    this->lineEditPath->setText(filename);
}

void tPreferences::boutonOkClick()
{
    QSettings setting;
    setting.setValue("Step", this->spinBoxStep->value());
    setting.setValue("StepBackground", this->spinBoxStepBackground->value());
    setting.setValue("DefaultFile", this->lineEditPath->text());
    // Planning
    for (int i=0 ; i<7 ; ++i) {
        setting.setValue(QString("OpeningTime%1").arg(i), this->OpeningTime.at(i)->time().toString("hh:mm"));
        setting.setValue(QString("ClosingTime%1").arg(i), this->ClosingTime.at(i)->time().toString("hh:mm"));
    }
    //Logo
    setting.setValue("Logo/Url", this->lineEditLogo->text());
    setting.setValue("Logo/Enable", this->groupBoxLogo->isChecked());
    setting.setValue("Logo/PosX", this->spinBoxPosXLogo->value());
    setting.setValue("Logo/PosY", this->spinBoxPosYLogo->value());
    // Background
    setting.setValue("Background", this->lineEditScreen->text());

    /** Avertissement de redemarrage nécésaire */
    this->accept();
}

void tPreferences::boutonCancelClick()
{
    this->reject();
}
