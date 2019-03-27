#include "tFenPrincipale.h"
#include "tGesMedia.h"
#include "tGesScript.h"
#include "tPreferences.h"
#include "tPlanning.h"

#include <stdio.h>

#include <QFile>
#include <QMenu>
#include <QAction>
#include <QMenuBar>
#include <QSettings>
#include <QStatusBar>
#include <QCloseEvent>
#include <QTextStream>
#include <QMessageBox>
#include <QFileDialog>
#include <QListWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include <QApplication>

tParametreGlobal  paramGlobal;

tFenPrincipale::tFenPrincipale(QApplication *app)
    : QMainWindow()
{
    QSettings setting;

    /** Menu Fichier */
    QMenu *menuFichier = this->menuBar()->addMenu(tr("&Fichier"));

    QAction *actionNouveau = menuFichier->addAction(tr("&Nouveau"));
    actionNouveau->setShortcut(QKeySequence("Ctrl+N"));
    actionNouveau->setIcon(QIcon(":/icon/pixmap/blog_add.png"));
    connect(actionNouveau, SIGNAL(triggered()), this, SLOT(actionNouveauClick()));

    QAction *actionOuvrir = menuFichier->addAction(tr("&Ouvrir"));
    actionOuvrir->setShortcut(QKeySequence("Ctrl+O"));
    actionOuvrir->setIcon(QIcon(":/icon/pixmap/folder_open.png"));
    connect(actionOuvrir, SIGNAL(triggered()), this, SLOT(actionOuvrirClick()));

    this->actionEnregistrer = menuFichier->addAction(tr("&Enregistrer"));
    this->actionEnregistrer->setShortcut(QKeySequence("Ctrl+S"));
    this->actionEnregistrer->setIcon(QIcon(":/icon/pixmap/save.png"));
    this->actionEnregistrer->setEnabled(false);
    connect(this->actionEnregistrer, SIGNAL(triggered()), this, SLOT(actionEnregistrerClick()));

    QAction *actionEnregistrerSous = menuFichier->addAction(tr("Enregistrer &sous..."));
    actionEnregistrerSous->setShortcut(QKeySequence("Ctrl+Alt+S"));
    connect(actionEnregistrerSous, SIGNAL(triggered()), this, SLOT(actionEnregistrerSousClick()));

    menuFichier->addSeparator();

    QAction *actionQuitter = menuFichier->addAction(tr("&Quitter"));
    actionQuitter->setShortcut(QKeySequence("Ctrl+Q"));
    actionQuitter->setIcon(QIcon(":/icon/pixmap/power_blue.png"));
    connect(actionQuitter, SIGNAL(triggered()), app, SLOT(quit()));

    /** Menu Options */
    QMenu *menuOptions = this->menuBar()->addMenu(tr("&Options"));

    QAction *actionMedia = menuOptions->addAction(tr("&Media"));
    actionMedia->setShortcut(QKeySequence("Ctrl+M"));
    actionMedia->setToolTip(tr("Lance le gestionnaire de Media"));
    actionMedia->setIcon(QIcon(":/icon/pixmap/camera.png"));
    connect(actionMedia, SIGNAL(triggered()), this, SLOT(actionMediaClick()));

    QAction *actionScipt = menuOptions->addAction(tr("&Upload"));
    actionScipt->setShortcut(QKeySequence("Ctrl+P"));
    actionScipt->setToolTip(tr("Lance le gestionnaire de Téléchargement"));
    actionScipt->setIcon(QIcon(":/icon/pixmap/blog.png"));
    connect(actionScipt, SIGNAL(triggered()), this, SLOT(actionScriptClick()));

    menuOptions->addSeparator();

    QAction *actionPreferences = menuOptions->addAction(tr("&Préférences"));
    actionPreferences->setToolTip(tr("Lance le gestionnaire de Script"));
    actionPreferences->setIcon(QIcon(":/icon/pixmap/tools.png"));
    connect(actionPreferences, SIGNAL(triggered()), this, SLOT(actionPreferencesClick()));

    /** Menu Infos */
    QMenu *menuInfo = this->menuBar()->addMenu(tr("?"));
    QAction *actionAPropos = menuInfo->addAction(tr("A propos"));
    actionAPropos->setIcon(QIcon(":/icon/pixmap/info_blue.png"));
    connect(actionAPropos, SIGNAL(triggered()), this, SLOT(aProposClick()));


    /** Bar Etat */
    barreEtat = this->statusBar();
    barreEtat->showMessage(tr("Prêt"));

    /** Liste TV */
    listTv = new QListWidget();
    listTv->setMaximumWidth(150);
    connect(listTv, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(listTvChange(QListWidgetItem *)));

    /** Bouton Ajouter TV */
    QPushButton *boutonAjouterTv = new QPushButton(tr("Ajouter"));
    connect(boutonAjouterTv, SIGNAL(clicked()), this, SLOT(ajouterTvClick()));

    /** Bouton Supprimer TV */
    QPushButton *boutonSupprimerTv = new QPushButton(tr("Supprimer"));
    connect(boutonSupprimerTv, SIGNAL(clicked()), this, SLOT(supprimerTvClick()));

    /** Layout pour la liste et les 2 boutons */
    QHBoxLayout *layoutBouton = new QHBoxLayout();
    layoutBouton->addWidget(boutonAjouterTv);
    layoutBouton->addWidget(boutonSupprimerTv);
    QVBoxLayout *layoutListeBouton = new QVBoxLayout();
    layoutListeBouton->addWidget(listTv, 100);
    layoutListeBouton->addLayout(layoutBouton);

    this->config = new tConfig();
    connect(this->config, SIGNAL(updateConfig()), this, SLOT(updateListTvFromConfig()));
    this->planning = new tPlanning();
    this->planningArrierePlan = new tPlanningArrierePlan();
    this->live = new tLive();

    /** Onglet */
    this->tabFenetre = new QTabWidget();
    this->tabFenetre->addTab(this->planning, tr("Planning"));
    this->tabFenetre->addTab(this->planningArrierePlan, tr("Arriére Plan"));
    this->tabFenetre->addTab(this->config, tr("Options"));
    this->tabFenetre->addTab(this->live, tr("Direct"));
    this->tabFenetre->setEnabled(false);

    /** Layout pour la liste et les onglets */
    QHBoxLayout *layoutTab = new QHBoxLayout();
    layoutTab->addLayout(layoutListeBouton);
    layoutTab->addWidget(tabFenetre);

    /** Set Layout Principal */
    this->widgetCentral = new QWidget();
    this->widgetCentral->setLayout(layoutTab);
    this->widgetCentral->setEnabled(false);

    /** Read Filename and apply setting */
    this->fileName = setting.value("DefaultFile").toString();
    if (!this->fileName.isEmpty())
        this->writeParamGlobalFromFile();

    /** General */
    this->setWindowTitle(tr("TV Controleur"));
    this->setWindowIcon(QIcon(":/icon/pixmap/tv-icon.ico"));
    this->resize(1120, 600);
    this->setCentralWidget(widgetCentral);
    this->show();
}

tFenPrincipale::~tFenPrincipale()
{
    delete this->listTv;
    delete this->config;
    delete this->planning;
    delete this->tabFenetre;
}

void tFenPrincipale::updateListTvFromParamGlobal()
{
    this->listTv->clear();

    /** On ajoute le nom de la TV dans la liste des TV */
    for (QList<tTv>::iterator i=paramGlobal.listTv.begin() ; i!=paramGlobal.listTv.end() ; ++i) {
        this->listTv->addItem((*i).getName());
    }
}

void tFenPrincipale::writeParamGlobalFromFile()
{
    /** Ouvre le fichier Xml */
    QDomDocument dom("XML");
    QFile xml_doc(this->fileName);
    if(!xml_doc.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(this, "Erreur à l'ouverture du document XML", "Impossible d'ouvrir le fichier !");
        return;
    }
    if (!dom.setContent(&xml_doc))
    {
        xml_doc.close();
        QMessageBox::warning(this, "Erreur à l'ouverture du document XML", "Le document XML n'a pas pu être attribué à l'objet QDomDocument.");
        return;
    }
    xml_doc.close();
    QDomElement dom_element = dom.documentElement();

    /** Efface les anciens parametres */
    paramGlobal.clearAll();

    /** Parse le Xml et rempli les listes */
    paramGlobal.fromXml(dom_element);

    this->updateListTvFromParamGlobal();
    this->widgetCentral->setEnabled(true);
    this->actionEnregistrer->setEnabled(true);
    this->live->updateListMediaFromParamGlobal();

    barreEtat->showMessage(tr("Lecture du fichier terminer"));
}

void tFenPrincipale::closeEvent(QCloseEvent *event)
{
    if (!paramGlobal.maybeSave()) {
        QMainWindow::closeEvent(event);
    }
    else {
        int ret = QMessageBox::information(this, tr("Quittez"), tr("Le fichier a été modifié.\nVoulez vous enregistrez vos changements ?"), QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel, QMessageBox::Save);

        switch (ret) {
        case QMessageBox::Save :
            this->actionEnregistrerClick();
            QMainWindow::closeEvent(event);
            break;

        case QMessageBox::Discard :
            QMainWindow::closeEvent(event);
            break;

        case QMessageBox::Cancel :
            event->ignore();
            break;
        }
    }

}

void tFenPrincipale::actionNouveauClick()
{
    /** Efface les anciens parametres */
    paramGlobal.clearAll();

    this->updateListTvFromParamGlobal();
    this->widgetCentral->setEnabled(true);
    this->fileName.clear();
    this->actionEnregistrer->setEnabled(false);
    this->live->updateListMediaFromParamGlobal();

    barreEtat->showMessage(tr("Nouveau document"));
}

void tFenPrincipale::actionOuvrirClick()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Ouvrir"), "", tr("Xml Files (*.xml)"));

    if (filename.isEmpty()) return;

    this->fileName = filename;
    this->writeParamGlobalFromFile();
}

void tFenPrincipale::actionEnregistrerClick()
{
    if (this->fileName.isEmpty()) { QMessageBox::critical(this, "Erreur à l'enregistrement du document XML", "Impossible d'enregistrer le fichier ! \nLe nom du fichier n'est pas correct !"); return; }

    /** Ouvre le fichier Xml */
    QDomDocument dom("XML");
    QFile xml_doc(this->fileName);
    if(!xml_doc.open(QIODevice::WriteOnly))
    {
        QMessageBox::warning(this, "Erreur à l'ouverture du document XML", "Impossible d'ouvrir le fichier !");
        return;
    }

    /** Crée la 1ere Balise Xml */
    QDomElement child = dom.createElement("TV_Controller");
    dom.appendChild(child);

    /** Crée le string Xml */
    QString xml = paramGlobal.toXml(dom, child).toString();

    //printf("%s\n",xml.toAscii().constData());

    QTextStream ts(&xml_doc);
    ts << xml;

    xml_doc.close();

    paramGlobal.saveDone();
    barreEtat->showMessage(tr("Fichier enregistrer"));
}

void tFenPrincipale::actionEnregistrerSousClick()
{
    QString filename = QFileDialog::getSaveFileName(this, tr("Enregistrer sous..."), "", tr("Xml Files (*.xml)"));

    if (filename.isEmpty()) return;

    this->fileName = filename;
    this->actionEnregistrer->setEnabled(true);
    this->actionEnregistrerClick();
}

void tFenPrincipale::actionPreferencesClick()
{
    tPreferences *pref = new tPreferences();
    pref->exec();

    this->planning->updatePlanningFromParamGlobal();
    this->planningArrierePlan->updatePlanningFromParamGlobal();
}

void tFenPrincipale::actionMediaClick()
{
    tGesMedia *media = new tGesMedia();
    media->exec();

    this->live->updateListMediaFromParamGlobal();
    this->planning->updatePlanningFromParamGlobal();
    this->planningArrierePlan->updatePlanningFromParamGlobal();
}

void tFenPrincipale::actionScriptClick()
{
    tGesScript *script = new tGesScript(this->fileName);
    script->exec();
}

void tFenPrincipale::aProposClick()
{
    QMessageBox::about(this, tr("A propos"), tr("Logiciel de gestion multimedia\n V1.1"));
}

void tFenPrincipale::listTvChange(QListWidgetItem *item)
{
    if (item == NULL) return;

    /** Active les Widgets */
    this->tabFenetre->setEnabled(true);

    /** MAJ des onglets */
    this->config->setFileName(this->fileName);
    this->config->updateByName(item->text());
    this->planning->updateByName(item->text());
    this->planningArrierePlan->updateByName(item->text());
    this->live->updateByName(item->text());
}

void tFenPrincipale::ajouterTvClick()
{
    static int numberOfTv = 1;
    QString nameOfNewTv = QString(tr("TV %1")).arg(numberOfTv);
    this->listTv->addItem(nameOfNewTv);
    paramGlobal.listTv.append(tTv(nameOfNewTv));
    numberOfTv++;

    barreEtat->showMessage(tr("TV Ajouter"));
}

void tFenPrincipale::supprimerTvClick()
{
    QListWidgetItem *item = this->listTv->currentItem();
    if (item == NULL) return;

    /** Demande de confirmation de suppression */
    int ret = QMessageBox::question(this, tr("Confirmation"), QString(tr("Etes vous sur de vouloir supprimer la TV : %1 ?")).arg(item->text()), QMessageBox::Yes | QMessageBox::No);
    if (ret != QMessageBox::Yes) return;

    this->listTv->takeItem(this->listTv->currentRow());

    for (QList<tTv>::iterator i=paramGlobal.listTv.begin() ; i!=paramGlobal.listTv.end() ; ++i) {
        if ((*i).getName() == item->text()) {
            i = paramGlobal.listTv.erase(i);
            break;
        }
    }

    barreEtat->showMessage(tr("TV Supprimer"));
}

void tFenPrincipale::updateListTvFromConfig()
{
    this->updateListTvFromParamGlobal();
    this->planning->updatePlanningFromParamGlobal();
    this->planningArrierePlan->updatePlanningFromParamGlobal();

    barreEtat->showMessage(tr("Configuration sauvegardé"));
}
