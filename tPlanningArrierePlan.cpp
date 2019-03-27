#include "tPlanningArrierePlan.h"
#include "tParametreGlobal.h"
#include "tAddBackground.h"
#include "tBackground.h"

#include <stdio.h>

#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QListWidget>
#include <QSettings>
#include <QTime>

extern tParametreGlobal  paramGlobal;

tPlanningArrierePlan::tPlanningArrierePlan()
    : QWidget(), nameTv()
{
    this->init();
    this->updatePlanningFromParamGlobal();
}

tPlanningArrierePlan::~tPlanningArrierePlan()
{
    //dtor
}

void tPlanningArrierePlan::init()
{
    QSettings setting;

    /** Crée un Tableau pour le planning */
    int const step = setting.value("StepBackground", 15).toInt();  // pas de 15 min

    QList<QTime> timeOpen, timeClose;
    for (int i=0 ; i<7 ; ++i) {
        timeOpen.append(QTime().fromString(setting.value(QString("OpeningTime%1").arg(i), "09:00").toString(), "hh:mm"));
        timeClose.append(QTime().fromString(setting.value(QString("ClosingTime%1").arg(i), "21:00").toString(), "hh:mm"));
    }

    timeMin=24*60;
    timeMax=0;
    for (int i=0 ; i<7 ; ++i) {
        int currentTime = timeOpen.at(i).hour()*60 + timeOpen.at(i).minute();
        if ((currentTime < timeMin) && (currentTime > 0)) {
            timeMin = currentTime;
        }
        currentTime = timeClose.at(i).hour()*60 + timeClose.at(i).minute();
        if (currentTime > timeMax) {
            timeMax = currentTime;
        }
    }

    int const nbRaw = ((timeMax - timeMin) / step);
    this->tableWidgetPlanning = new QTableWidget(nbRaw, 7);

    /** initialise le tableau */
    this->initTableWidgetPlanning();

    /** Options: Read Only and Single Selection */
    this->tableWidgetPlanning->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->tableWidgetPlanning->setSelectionMode(QAbstractItemView::SingleSelection);

    /** Boutons */
    this->boutonAdd = new QPushButton(tr("Ajouter"));
    connect(this->boutonAdd, SIGNAL(clicked()), this, SLOT(boutonAddClick()));
    this->boutonModify = new QPushButton(tr("Modifier"));
    connect(this->boutonModify, SIGNAL(clicked()), this, SLOT(boutonModifyClick()));
    this->boutonDel = new QPushButton(tr("Supprimer"));
    connect(this->boutonDel, SIGNAL(clicked()), this, SLOT(boutonDelClick()));

    /** Layout pour la liste est les boutons */
    QVBoxLayout *layoutList = new QVBoxLayout();
    layoutList->addWidget(this->boutonAdd);
    layoutList->addWidget(this->boutonModify);
    layoutList->addWidget(this->boutonDel);
    layoutList->addStretch();

    /* Layout principale */
    QHBoxLayout *layoutPrincipale = new QHBoxLayout();
    layoutPrincipale->addWidget(this->tableWidgetPlanning);
    layoutPrincipale->addLayout(layoutList);

    this->setLayout(layoutPrincipale);
}

void tPlanningArrierePlan::initTableWidgetPlanning()
{
    QSettings setting;

    /** Fabrique le Header Verticale (hh:mm) */
    QStringList listVerticalHeader;
    int const step = setting.value("StepBackground", 15).toInt();

    if (!paramGlobal.listTv.isEmpty()) {
        for (QList<tTv>::iterator i=paramGlobal.listTv.begin() ; i!=paramGlobal.listTv.end() ; ++i) {
            if ((*i).getName() == this->nameTv) {
                timeMin=24*60;
                timeMax=0;
                for (QList<tScheduler>::iterator j=(*i).listScheduler.begin() ; j!=(*i).listScheduler.end() ; ++j) {
                    int currentTime = (*j).getOpeningTime().hour()*60 + (*j).getOpeningTime().minute();
                    if ((currentTime < timeMin) && (currentTime > 0)) {
                        timeMin = currentTime;
                    }
                    currentTime = (*j).getClosingTime().hour()*60 + (*j).getClosingTime().minute();
                    if (currentTime > timeMax) {
                        timeMax = currentTime;
                    }
                }
            }
        }
    }

    int const nbRaw = ((timeMax - timeMin) / step);
    if (this->tableWidgetPlanning->rowCount() != nbRaw) {
        this->tableWidgetPlanning->setRowCount(nbRaw);
    }

    QTime timeMin_(0,0,0);
    timeMin_ = timeMin_.addSecs(timeMin*60);
    QTime timeMax_(0,0,0);
    timeMax_ = timeMax_.addSecs(timeMax*60);
    for (QTime time(timeMin_) ; time <= timeMax_ ; time = time.addSecs(step*60)) {
        listVerticalHeader << time.toString("hh:mm");
    }
    this->tableWidgetPlanning->setVerticalHeaderLabels(listVerticalHeader);

    /** Fabrique le Header Horizontale (Jour) */
    QStringList listHorizontalHeader;
    listHorizontalHeader << "Lundi" << "Mardi" << "Mercredi" << "Jeudi" << "Vendredi" << "Samedi" << "Dimanche";
    this->tableWidgetPlanning->setHorizontalHeaderLabels(listHorizontalHeader);

    /** Redimensionne la taille des lignes */
    for (int i=0 ; i<this->tableWidgetPlanning->rowCount() ; ++i) {
        this->tableWidgetPlanning->setRowHeight(i, 20);
    }
}

void tPlanningArrierePlan::updateByName(QString name)
{
    this->nameTv = name;
    this->updatePlanningFromParamGlobal();
}

void tPlanningArrierePlan::updatePlanningFromParamGlobal()
{
    /** Efface le tableau et l'initialise */
    this->tableWidgetPlanning->clear();
    this->initTableWidgetPlanning();

    /** On cherche la bonne TV */
    for (QList<tTv>::iterator i=paramGlobal.listTv.begin() ; i!=paramGlobal.listTv.end() ; ++i) {
        if ((*i).getName() == this->nameTv) {
            /** On ajoute tout les arriéres plans */
            for (QList<tBackground>::iterator j=(*i).listBackground.begin() ; j!=(*i).listBackground.end() ; ++j) {
                int idxStart, idxEnd;
                /** cherche le debut */
                for (idxStart=0 ; idxStart<this->tableWidgetPlanning->rowCount() ; ++idxStart) {
                    if (this->tableWidgetPlanning->verticalHeaderItem(idxStart)->text() == (*j).getHourStart()) {
                        break;
                    }
                }
                /** cherche la fin */
                for (idxEnd=0 ; idxEnd<this->tableWidgetPlanning->rowCount() ; ++idxEnd) {
                    if (this->tableWidgetPlanning->verticalHeaderItem(idxEnd)->text() == (*j).getHourEnd()) {
                        break;
                    }
                }

                for (int idx=idxStart ; idx<idxEnd ; ++idx) {
                    QTableWidgetItem *item = new QTableWidgetItem((*j).getName());
                    /** On cherche la couleur du media */
                    for (QList<tMedia>::iterator k=paramGlobal.listMedia.begin() ; k!=paramGlobal.listMedia.end() ; ++k) {
                        if ((*k).getName() == (*j).getName()) {
                            item->setBackgroundColor((*k).getColor());
                        }
                    }

                    this->tableWidgetPlanning->setItem(idx, (*j).getDateIndex()-1, item);
                }
            }

            /** Désactive les cellules suivant les horaires */
            for (int k=0 ; k<7 ; ++k) {
                QTime startTime = (*i).listScheduler[k].getOpeningTime();
                for (int j=0 ; j<this->tableWidgetPlanning->rowCount() ; ++j) {
                    if (QTime().fromString(this->tableWidgetPlanning->verticalHeaderItem(j)->text(), "hh:mm") < startTime) {
                        QTableWidgetItem *item = new QTableWidgetItem();
                        item->setBackgroundColor(QColor(Qt::lightGray));
                        this->tableWidgetPlanning->setItem(j,k,item);
                    }
                }
                QTime endTime = (*i).listScheduler[k].getClosingTime();
                for (int j=0 ; j<this->tableWidgetPlanning->rowCount() ; ++j) {
                    if (QTime().fromString(this->tableWidgetPlanning->verticalHeaderItem(j)->text(), "hh:mm") >= endTime) {
                        QTableWidgetItem *item = new QTableWidgetItem();
                        item->setBackgroundColor(QColor(Qt::lightGray));
                        this->tableWidgetPlanning->setItem(j,k,item);
                    }
                }
            }
        }
    }
}

void tPlanningArrierePlan::boutonAddClick()
{
    tAddBackground *event = new tAddBackground(this->nameTv);
    event->exec();
    this->updatePlanningFromParamGlobal();
}

void tPlanningArrierePlan::boutonModifyClick()
{
    /** On récupere l'item qui est selectionné */
    QTableWidgetItem *item = this->tableWidgetPlanning->currentItem();

    if (item == NULL) return;

    /** On crée le tEvent correspondant */
    tEvent *currentEvent = new tEvent(item->text());
    currentEvent->setHour(this->tableWidgetPlanning->verticalHeaderItem(item->row())->text());
    currentEvent->setDateIndex(item->column());

    //  tAddBackground *event = new tAddBackground(this->nameTv, *currentEvent);
    //  event->exec();
    this->updatePlanningFromParamGlobal();
}

void tPlanningArrierePlan::boutonDelClick()
{
    /** On récupere l'item qui est selectionné */
    QTableWidgetItem *item = this->tableWidgetPlanning->currentItem();

    if (item == NULL) return;

    /** On crée un tEvent pour pouvoir le comparé à la liste */
    tBackground *eventToDel = new tBackground(item->text());
    eventToDel->setHourStart(this->tableWidgetPlanning->verticalHeaderItem(item->row())->text());
    eventToDel->setDateIndex(item->column()+1);

    /** On cherche la bonne TV */
    for (QList<tTv>::iterator i=paramGlobal.listTv.begin() ; i!=paramGlobal.listTv.end() ; ++i) {
        if ((*i).getName() == this->nameTv) {
            /** On cherche l'evenement parmis la liste*/
            for (QList<tBackground>::iterator j=(*i).listBackground.begin() ; j!=(*i).listBackground.end() ; ++j) {
                if (eventToDel->operator==((*j))) {
                    /** Demande de confirmation de suppression */
                    int ret = QMessageBox::question(this, tr("Confirmation"), QString(tr("Etes vous sur de vouloir supprimer l'arriére plan :\n %1, %2, %3, %4?")).arg((*j).getName()).arg((*j).getDateName()).arg((*j).getHourStart()).arg((*j).getHourEnd()), QMessageBox::Yes | QMessageBox::No);
                    if (ret != QMessageBox::Yes) return;
                    (*i).listBackground.erase(j);
                    break;
                }
            }
        }
    }

    this->updatePlanningFromParamGlobal();
}
