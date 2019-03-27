#include "tAddBackground.h"
#include "tBackground.h"
#include "tParametreGlobal.h"

#include <stdio.h>

#include <QHBoxLayout>
#include <QPushButton>
#include <QGroupBox>
#include <QSettings>
#include <QLabel>
#include <QDate>
#include <QTime>
#include <QIcon>

extern tParametreGlobal  paramGlobal;

tAddBackground::tAddBackground(QString &_tvName)
    : QDialog(), tvName(_tvName), mediaName()
{
    this->init();
}

tAddBackground::tAddBackground(QString &_tvName, QString _mediaName)
    : QDialog(), tvName(_tvName), mediaName(_mediaName)
{
    this->init();

    for (QList<tMedia>::iterator i=paramGlobal.listMedia.begin() ; i!=paramGlobal.listMedia.end() ; ++i) {
        if ((*i).getName() == mediaName) {
            /** Met a jour la liste de type */
            this->comboBoxType->setCurrentIndex(this->comboBoxType->findText((*i).getType()));
        }
    }

    /** Met a jour la liste des media */
    this->comboBoxMedia->setCurrentIndex(this->comboBoxMedia->findText(mediaName));
}

tAddBackground::~tAddBackground()
{
    //dtor
}

void tAddBackground::init()
{
    QSettings setting;

    /** Combo Box Type */
    this->comboBoxType = new QComboBox();
    this->comboBoxType->addItem(tr("Arriere Plan"));
    this->comboBoxType->addItem(tr("Photo"));
    this->comboBoxType->addItem(tr("Video"));
    connect(this->comboBoxType, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(comboBoxTypeChanged(const QString &)));

    /* Layout pour le Type */
    QHBoxLayout *layoutType = new QHBoxLayout();
    layoutType->addWidget(new QLabel(tr("Type : ")));
    layoutType->addWidget(this->comboBoxType);


    /** Combo Box Media */
    this->comboBoxMedia = new QComboBox();

    /* Layout pour le Type */
    QHBoxLayout *layoutMedia = new QHBoxLayout();
    layoutMedia->addWidget(new QLabel(tr("Media : ")));
    layoutMedia->addWidget(this->comboBoxMedia);

    /** Group Box Jour */
    QGroupBox *groupBoxJour = new QGroupBox(tr("Jour"));

    /* Ajoute les jours */
    this->checkBox_lundi = new QCheckBox(tr("Lundi"));
    this->checkBox_mardi = new QCheckBox(tr("Mardi"));
    this->checkBox_mercredi = new QCheckBox(tr("Mercredi"));
    this->checkBox_jeudi = new QCheckBox(tr("Jeudi"));
    this->checkBox_vendredi = new QCheckBox(tr("Vendredi"));
    this->checkBox_samedi = new QCheckBox(tr("Samedi"));
    this->checkBox_dimanche = new QCheckBox(tr("Dimanche"));

    /* Layout pour la groupBox */
    QHBoxLayout *layoutLunMarMer = new QHBoxLayout;
    layoutLunMarMer->addWidget(this->checkBox_lundi);
    layoutLunMarMer->addWidget(this->checkBox_mardi);
    layoutLunMarMer->addWidget(this->checkBox_mercredi);
    QHBoxLayout *layoutJeuVenSam = new QHBoxLayout;
    layoutJeuVenSam->addWidget(this->checkBox_jeudi);
    layoutJeuVenSam->addWidget(this->checkBox_vendredi);
    layoutJeuVenSam->addWidget(this->checkBox_samedi);
    QVBoxLayout *layoutGroupBox = new QVBoxLayout;
    layoutGroupBox->addLayout(layoutLunMarMer);
    layoutGroupBox->addLayout(layoutJeuVenSam);
    layoutGroupBox->addWidget(checkBox_dimanche);
    groupBoxJour->setLayout(layoutGroupBox);

    /** Ajoute les heures et les minutes de debut*/
    this->timeEditDebut = new QTimeEdit();
    this->timeEditDebut->setDisplayFormat("hh'h'mm");
    this->timeEditDebut->setTime(QTime(9,0,0));

    /* Layout pour les heures et les minutes de debut*/
    QHBoxLayout *layoutHourMinDebut = new QHBoxLayout;
    layoutHourMinDebut->addWidget(new QLabel(tr("Lancé à : ")));
    layoutHourMinDebut->addWidget(this->timeEditDebut);


    /** Ajoute les heures et les minutes de fin*/
    this->timeEditFin = new QTimeEdit();
    this->timeEditFin->setDisplayFormat("hh'h'mm");
    this->timeEditFin->setTime(QTime(22,0,0));

    /* Layout pour les heures et les minutes de fin*/
    QHBoxLayout *layoutHourMinFin = new QHBoxLayout;
    layoutHourMinFin->addWidget(new QLabel(tr("Términer à : ")));
    layoutHourMinFin->addWidget(this->timeEditFin);


    /** Boutons */
    QPushButton *boutonOk = new QPushButton(tr("Ok"));
    connect(boutonOk, SIGNAL(clicked()), this, SLOT(boutonOkClick()));
    QPushButton *boutonCancel = new QPushButton(tr("Annuler"));
    connect(boutonCancel, SIGNAL(clicked()), this, SLOT(boutonCancelClick()));

    /* Layout pour les boutons */
    QHBoxLayout *layoutBouton = new QHBoxLayout();
    layoutBouton->addWidget(boutonOk);
    layoutBouton->addWidget(boutonCancel);

    /* Layout principale */
    QVBoxLayout *layoutPrincipale = new QVBoxLayout();
    layoutPrincipale->addLayout(layoutType);
    layoutPrincipale->addLayout(layoutMedia);
    layoutPrincipale->addWidget(groupBoxJour);
    layoutPrincipale->addLayout(layoutHourMinDebut);
    layoutPrincipale->addLayout(layoutHourMinFin);
    layoutPrincipale->addStretch(1);
    layoutPrincipale->addLayout(layoutBouton);

    /** Generale */
    this->comboBoxTypeChanged(this->comboBoxType->currentText());
    this->setWindowTitle(tr("Ajouter un Arriére plan"));
    this->setWindowIcon(QIcon(":/icon/pixmap/plus.png"));
    this->setLayout(layoutPrincipale);
    this->resize(250, 230);
}

void tAddBackground::comboBoxTypeChanged(const QString &text)
{
    this->comboBoxMedia->clear();
    for (QList<tMedia>::iterator i=paramGlobal.listMedia.begin() ; i!=paramGlobal.listMedia.end() ; ++i) {
        if ((*i).getType() == text) {
            this->comboBoxMedia->addItem((*i).getName());
        }
    }
}

void tAddBackground::boutonOkClick()
{

    /** Crée l'événement */
    tBackground *newBackground = new tBackground();
    newBackground->setName(this->comboBoxMedia->currentText());
    int i;
    for (i=1 ; i<8 ; ++i) {
        bool isChecked;
        switch (i) {
        case 1: isChecked = this->checkBox_lundi->isChecked(); break;
        case 2: isChecked = this->checkBox_mardi->isChecked(); break;
        case 3: isChecked = this->checkBox_mercredi->isChecked(); break;
        case 4: isChecked = this->checkBox_jeudi->isChecked(); break;
        case 5: isChecked = this->checkBox_vendredi->isChecked(); break;
        case 6: isChecked = this->checkBox_samedi->isChecked(); break;
        case 7: isChecked = this->checkBox_dimanche->isChecked(); break;
        }
        if (isChecked) {
            newBackground->setDateIndex(i);
            QTime time = this->timeEditDebut->time();
            newBackground->setHourStart(time.toString("hh:mm"));
            time = this->timeEditFin->time();
            newBackground->setHourEnd(time.toString("hh:mm"));

//            printf("background: %s\n", newBackground->toString().constData());

            /** Cherche la bonne TV à mettre a jour */
            for (QList<tTv>::iterator i=paramGlobal.listTv.begin() ; i!=paramGlobal.listTv.end() ; ++i) {
                if ((*i).getName() == this->tvName) {
                    /** Ajoute l'événement à la TV */
                    (*i).listBackground.append(*newBackground);
                }
            }
        }
    }

    this->accept();
}

void tAddBackground::boutonCancelClick()
{
    this->reject();
}
