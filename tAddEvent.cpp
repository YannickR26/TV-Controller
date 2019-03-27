#include "tAddEvent.h"
#include "tEvent.h"
#include "tParametreGlobal.h"

#include <stdio.h>

#include <QHBoxLayout>
#include <QPushButton>
#include <QGroupBox>
#include <QSettings>
#include <QLabel>
#include <QTime>
#include <QIcon>

extern tParametreGlobal  paramGlobal;

tAddEvent::tAddEvent(QString &_tvName)
    : QDialog(), tvName(_tvName), mediaName()
{
    this->event = NULL;
    this->init();
}

tAddEvent::tAddEvent(QString &_tvName, QString _mediaName)
    : QDialog(), tvName(_tvName), mediaName(_mediaName)
{
    this->event = NULL;
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

tAddEvent::tAddEvent(QString &_tvName, tEvent &_event)
    : QDialog(), tvName(_tvName), mediaName()
{
    this->init();

    this->event = new tEvent(_event.getName(), _event.getDate(), _event.getHour());

    for (QList<tMedia>::iterator i=paramGlobal.listMedia.begin() ; i!=paramGlobal.listMedia.end() ; ++i) {
        if ((*i).getName() == this->event->getName()) {
            /** Met a jour la liste de type */
            this->comboBoxType->setCurrentIndex(this->comboBoxType->findText((*i).getType()));
        }
    }

    /** Met a jour la liste des media */
    this->comboBoxMedia->setCurrentIndex(this->comboBoxMedia->findText(this->event->getName()));

    /** Met a jour les jours */
    switch (this->event->getDateIndex()) {
    case 1:
        this->checkBox_lundi->setChecked(true);
        break;

    case 2:
        this->checkBox_mardi->setChecked(true);
        break;

    case 3:
        this->checkBox_mercredi->setChecked(true);
        break;

    case 4:
        this->checkBox_jeudi->setChecked(true);
        break;

    case 5:
        this->checkBox_vendredi->setChecked(true);
        break;

    case 6:
        this->checkBox_samedi->setChecked(true);
        break;

    case 7:
        this->checkBox_dimanche->setChecked(true);
        break;
    }

    /** Met a jour l'horaire de debut */
    this->timeEditDebut->setTime(QTime::fromString(this->event->getHour(), "hh:mm"));

//    this->groupBoxRepeat->setEnabled(false);
}

tAddEvent::~tAddEvent()
{
    //dtor
}

void tAddEvent::init()
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

    /** Ajoute les heures et les minutes */
    this->timeEditDebut = new QTimeEdit();
    this->timeEditDebut->setDisplayFormat("hh'h'mm");
    this->timeEditDebut->setTime(QTime(9,0,0));

    /* Layout pour les heures et les minutes */
    QHBoxLayout *layoutHourMin = new QHBoxLayout;
    layoutHourMin->addWidget(new QLabel(tr("Lancé à : ")));
    layoutHourMin->addWidget(this->timeEditDebut);

    /** Group Box Repeté */
    this->groupBoxRepeat = new QGroupBox(tr("Répéter"));
    this->groupBoxRepeat->setCheckable(true);
    this->groupBoxRepeat->setChecked(false);

    /* Ajoute les heures et les minutes pour la repetition*/
    this->spinBoxHourRepeat = new QSpinBox();
    this->spinBoxHourRepeat->setSuffix(tr(" h"));
    this->spinBoxHourRepeat->setRange(0, 23);
    this->spinBoxMinRepeat = new QSpinBox();
    this->spinBoxMinRepeat->setSuffix(tr(" min"));
    this->spinBoxMinRepeat->setRange(setting.value("Step", 15).toInt(), 60);
    this->spinBoxMinRepeat->setSingleStep(setting.value("Step", 15).toInt());
    this->spinBoxMinRepeat->setValue(setting.value("Step", 15).toInt());

    /* Ajoute les heures et les minutes pour la fin */
    this->spinBoxHourEnd = new QSpinBox();
    this->spinBoxHourEnd->setSuffix(tr(" h"));
    this->spinBoxHourEnd->setRange(0, 23);
    this->spinBoxHourEnd->setValue(9);
    this->spinBoxMinEnd = new QSpinBox();
    this->spinBoxMinEnd->setSuffix(tr(" min"));
    this->spinBoxMinEnd->setRange(0, 60);
    this->spinBoxMinEnd->setSingleStep(setting.value("Step", 15).toInt());

    /* Layout pour la groupBox */
    QHBoxLayout *layoutHourMinRepeat = new QHBoxLayout;
    layoutHourMinRepeat->addWidget(new QLabel(tr("Toutes les ")));
    layoutHourMinRepeat->addWidget(this->spinBoxHourRepeat);
    layoutHourMinRepeat->addWidget(this->spinBoxMinRepeat);
    QHBoxLayout *layoutHourMinEnd = new QHBoxLayout;
    layoutHourMinEnd->addWidget(new QLabel(tr("Jusqu'à ")));
    layoutHourMinEnd->addWidget(this->spinBoxHourEnd);
    layoutHourMinEnd->addWidget(this->spinBoxMinEnd);
    QVBoxLayout *layoutGroupBoxRepeat = new QVBoxLayout;
    layoutGroupBoxRepeat->addLayout(layoutHourMinRepeat);
    layoutGroupBoxRepeat->addLayout(layoutHourMinEnd);
    this->groupBoxRepeat->setLayout(layoutGroupBoxRepeat);

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
    layoutPrincipale->addLayout(layoutHourMin);
    layoutPrincipale->addWidget(this->groupBoxRepeat);
    layoutPrincipale->addStretch(1);
    layoutPrincipale->addLayout(layoutBouton);

    /** Generale */
    this->comboBoxTypeChanged(this->comboBoxType->currentText());
    this->setWindowTitle(tr("Ajouter un événement"));
    this->setWindowIcon(QIcon(":/icon/pixmap/plus.png"));
    this->setLayout(layoutPrincipale);
    this->resize(250, 230);
}

void tAddEvent::comboBoxTypeChanged(const QString &text)
{
    this->comboBoxMedia->clear();
    for (QList<tMedia>::iterator i=paramGlobal.listMedia.begin() ; i!=paramGlobal.listMedia.end() ; ++i) {
        if ((*i).getType() == text) {
            this->comboBoxMedia->addItem((*i).getName());
        }
    }
}

void tAddEvent::boutonOkClick()
{
    qDebug()  <<    "tAddEvent::boutonOkClick() => 1";

    /** On supprime l'évenement si nous l'avons passé en paramétre (Modification) */
    if (this->event != NULL) {
        /** Cherche la bonne TV à mettre a jour */
        for (QList<tTv>::iterator i=paramGlobal.listTv.begin() ; i!=paramGlobal.listTv.end() ; ++i) {
            if ((*i).getName() == this->tvName) {
                /** On cherche l'evenement parmis la liste*/
                for (QList<tEvent>::iterator j=(*i).listEvent.begin() ; j!=(*i).listEvent.end() ; ++j) {
                    if (this->event->operator==((*j))) {
                        (*i).listEvent.erase(j);
                        break;
                    }
                }
            }
        }
    }

    qDebug()  <<    "tAddEvent::boutonOkClick() => 2";

    /** Crée l'événement */
    tEvent *newEvent = new tEvent();
    newEvent->setName(this->comboBoxMedia->currentText());
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
            newEvent->setDateIndex(i);
            QTime time = this->timeEditDebut->time();
            if (this->groupBoxRepeat->isChecked()) {
                for ( ; time <= QTime(this->spinBoxHourEnd->value(),this->spinBoxMinEnd->value(),0) ; time = time.addSecs(this->spinBoxHourRepeat->value()*3600 + this->spinBoxMinRepeat->value()*60)) {
                    newEvent->setHour(time.toString("hh:mm"));

                    /** Cherche la bonne TV à mettre a jour */
                    for (QList<tTv>::iterator i=paramGlobal.listTv.begin() ; i!=paramGlobal.listTv.end() ; ++i) {
                        if ((*i).getName() == this->tvName) {
                            /** Ajoute l'événement à la TV */
                            (*i).listEvent.append(*newEvent);
                        }
                    }
                }
            }
            else {
                newEvent->setHour(time.toString("hh:mm"));

                /** Cherche la bonne TV à mettre a jour */
                for (QList<tTv>::iterator i=paramGlobal.listTv.begin() ; i!=paramGlobal.listTv.end() ; ++i) {
                    if ((*i).getName() == this->tvName) {
                        /** Ajoute l'événement à la TV */
                        (*i).listEvent.append(*newEvent);
                    }
                }
            }
        }
    }

    this->accept();
}

void tAddEvent::boutonCancelClick()
{
    this->reject();
}
