#include "tGesMedia.h"
#include "tParametreGlobal.h"

#include <stdio.h>

#include <QDomDocument>
#include <QColorDialog>
#include <QMessageBox>
#include <QPushButton>
#include <QTextStream>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QCheckBox>
#include <QGroupBox>
#include <QPainter>
#include <QLabel>
#include <QFile>

extern tParametreGlobal  paramGlobal;

tGesMedia::tGesMedia()
    : QDialog()
{
    this->init();
    this->updateFromParamGlobal();
}

tGesMedia::~tGesMedia()
{
    delete this->listWidgetMedia;
    delete this->comboBoxType;
    delete this->ligneNom;
    delete this->ligneUrl;
    delete this->groupBoxOptions;
    delete this->option_fadeIn;
    delete this->option_fadeOut;
    delete this->option_repeat;
}

void tGesMedia::init()
{
    /** Liste des Medias */
    listWidgetMedia = new QListWidget();
    listWidgetMedia->setMaximumWidth(150);
    connect(listWidgetMedia, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(listMediaChange(QListWidgetItem *)));

    /** Bouton Ajouter TV */
    QPushButton *boutonAjouterMedia = new QPushButton(tr("Ajouter"));
    connect(boutonAjouterMedia, SIGNAL(clicked()), this, SLOT(ajouterMediaClick()));

    /** Bouton Supprimer TV */
    QPushButton *boutonSupprimerMedia = new QPushButton(tr("Supprimer"));
    connect(boutonSupprimerMedia, SIGNAL(clicked()), this, SLOT(supprimerMediaClick()));

    /* Layout pour la liste et les 2 boutons */
    QHBoxLayout *layoutBouton = new QHBoxLayout();
    layoutBouton->addWidget(boutonAjouterMedia);
    layoutBouton->addWidget(boutonSupprimerMedia);
    QVBoxLayout *layoutListeBouton = new QVBoxLayout();
    layoutListeBouton->addWidget(listWidgetMedia, 100);
    layoutListeBouton->addLayout(layoutBouton);

    /** Combo Box Type */
    this->comboBoxType = new QComboBox();
    this->comboBoxType->addItem(tr("Arriere Plan"));
    this->comboBoxType->addItem(tr("Photo"));
    this->comboBoxType->addItem(tr("Video"));
    this->comboBoxType->setEnabled(false);

    /* Layout pour le Type */
    QHBoxLayout *layoutType = new QHBoxLayout();
    layoutType->addWidget(new QLabel(tr("Type : ")));
    layoutType->addWidget(comboBoxType);

    /** Ligne Nom */
    ligneNom = new QLineEdit();
    this->ligneNom->setEnabled(false);

    /* Layout pour le Nom */
    QHBoxLayout *layoutNom = new QHBoxLayout();
    layoutNom->addWidget(new QLabel(tr("Nom : ")));
    layoutNom->addWidget(ligneNom);

    /** Ligne URL */
    ligneUrl = new QLineEdit();
    this->ligneUrl->setEnabled(false);
    this->boutonUrl = new QPushButton(tr("..."));
    this->boutonUrl->setEnabled(false);
    this->boutonUrl->setMaximumWidth(35);
    connect(this->boutonUrl, SIGNAL(clicked()), this, SLOT(UrlClick()));

    /* Layout pour l'URL */
    QHBoxLayout *layoutUrl = new QHBoxLayout();
    layoutUrl->addWidget(new QLabel(tr("Url : ")));
    layoutUrl->addWidget(ligneUrl);
    layoutUrl->addWidget(boutonUrl);

    /** Group Box Options */
    this->groupBoxOptions = new QGroupBox(tr("Options"));
    this->groupBoxOptions->setEnabled(false);

    /* Ajoute 5 options */
    option_fadeIn = new QCheckBox(tr("Fondu entrant"));
    option_fadeOut = new QCheckBox(tr("Fondu sortant"));
    option_repeat = new QCheckBox(tr("Repeter"));
    option_login  = new QLineEdit();
    option_password  = new QLineEdit();

    /* Layout pour la groupBox */
    QVBoxLayout *layoutGroupBox = new QVBoxLayout;
    layoutGroupBox->addWidget(option_fadeIn);
    layoutGroupBox->addWidget(option_fadeOut);
    layoutGroupBox->addWidget(option_repeat);
    QHBoxLayout *layoutGroupBoxLogin = new QHBoxLayout;
    layoutGroupBoxLogin->addWidget(new QLabel(tr("Login : ")));
    layoutGroupBoxLogin->addWidget(option_login);
    QHBoxLayout *layoutGroupBoxPassword = new QHBoxLayout;
    layoutGroupBoxPassword->addWidget(new QLabel(tr("Password : ")));
    layoutGroupBoxPassword->addWidget(option_password);
    layoutGroupBox->addLayout(layoutGroupBoxLogin);
    layoutGroupBox->addLayout(layoutGroupBoxPassword);
    this->groupBoxOptions->setLayout(layoutGroupBox);

    /** Couleur du media */
    this->showColor = new tShowColor();
    this->showColor->setEnabled(false);

    this->boutonColor = new QPushButton(tr("Modifier Couleur"));
    this->boutonColor->setMinimumWidth(100);
    this->boutonColor->setEnabled(false);
    connect(this->boutonColor, SIGNAL(clicked()), this, SLOT(ColorClick()));

    /* Layout pour la couleur */
    QHBoxLayout *layoutcolor = new QHBoxLayout;
    layoutcolor->addWidget(new QLabel(tr("Couleur : ")));
    layoutcolor->addStretch(1);
    layoutcolor->addWidget(this->showColor);
    layoutcolor->addStretch(1);
    layoutcolor->addWidget(this->boutonColor);

    /** Durée du media */
    this->timeEditDuree = new QTimeEdit();
    this->timeEditDuree->setDisplayFormat("hh'h'mm'm'ss's'");
    this->timeEditDuree->setEnabled(false);

    /* Layout pour la durée */
    QHBoxLayout *layoutDuree = new QHBoxLayout;
    layoutDuree->addWidget(new QLabel(tr("Pendant : ")));
    layoutDuree->addStretch(1);
    layoutDuree->addWidget(this->timeEditDuree);

    /** Bouton Enregistrer */
    QPushButton *boutonEnregistrer = new QPushButton(tr("Enregistrer"));
    connect(boutonEnregistrer, SIGNAL(clicked()), this, SLOT(EnregistrerClick()));

    /** Bouton Quitter */
    QPushButton *boutonQuitter = new QPushButton(tr("Quitter"));
    connect(boutonQuitter, SIGNAL(clicked()), this, SLOT(QuitterClick()));

    /* Layout pour le bouton enregistrer et quitter */
    QHBoxLayout *layoutBouton2 = new QHBoxLayout();
    layoutBouton2->addWidget(boutonEnregistrer);
    layoutBouton2->addStretch(1);
    layoutBouton2->addWidget(boutonQuitter);

    /* Layout pour la ligne Nom, la ligne Url et la groupBox Options */
    QVBoxLayout *layoutDroite = new QVBoxLayout();
    layoutDroite->addLayout(layoutType);
    layoutDroite->addLayout(layoutNom);
    layoutDroite->addLayout(layoutUrl);
    layoutDroite->addWidget(groupBoxOptions);
    layoutDroite->addLayout(layoutcolor);
    layoutDroite->addLayout(layoutDuree);
    layoutDroite->addStretch(1);
    layoutDroite->addLayout(layoutBouton2);

    /* Layout principale */
    QHBoxLayout *layoutPrincipale = new QHBoxLayout();
    layoutPrincipale->addLayout(layoutListeBouton);
    layoutPrincipale->addLayout(layoutDroite);

    /** Generale */
    this->setWindowTitle(tr("Gestionnaire de Media"));
    this->setWindowIcon(QIcon(":/icon/pixmap/camera.png"));
    this->setLayout(layoutPrincipale);
    this->resize(450, 250);
}

void tGesMedia::updateFromParamGlobal()
{
    for (QList<tMedia>::iterator i=paramGlobal.listMedia.begin() ; i!=paramGlobal.listMedia.end() ; ++i) {
        listWidgetMedia->addItem((*i).getName());
    }
}

void tGesMedia::listMediaChange(QListWidgetItem *item)
{
    if (item == NULL) return;

    /** Active les Widgets */
    this->comboBoxType->setEnabled(true);
    this->ligneNom->setEnabled(true);
    this->ligneUrl->setEnabled(true);
    this->boutonUrl->setEnabled(true);
    this->groupBoxOptions->setEnabled(true);
    this->boutonColor->setEnabled(true);
    this->timeEditDuree->setEnabled(true);
    this->showColor->setEnabled(true);

    /** MAJ des valeurs */
    for (QList<tMedia>::iterator i=paramGlobal.listMedia.begin() ; i!=paramGlobal.listMedia.end() ; ++i) {
        if ((*i).getName() == item->text()) {
            this->ligneNom->setText((*i).getName());
            this->ligneUrl->setText((*i).getUrl());
            this->comboBoxType->setCurrentIndex(this->comboBoxType->findText((*i).getType()));
            this->option_fadeIn->setChecked((*i).getFadeIn());
            this->option_fadeOut->setChecked((*i).getFadeOut());
            this->option_repeat->setChecked((*i).getRepeat());
            this->option_login->setText((*i).getLogin());
            this->option_password->setText((*i).getPassword());
            this->showColor->setBackgroundColor((*i).getColor());
            this->showColor->repaint();
            this->timeEditDuree->setTime((*i).getTime());
            break;
        }
    }
}

void tGesMedia::ajouterMediaClick()
{
    static int numberOfMedia = 1;
    QString nameOfNewMedia;
    bool continu = false;
    do {
        nameOfNewMedia = QString(tr("Media %1")).arg(numberOfMedia);
        continu = false;
        for (QList<tMedia>::iterator i=paramGlobal.listMedia.begin() ; i!=paramGlobal.listMedia.end() ; ++i) {
            if ((*i).getName() == nameOfNewMedia) {
                numberOfMedia++;
                continu = true;
                break;
            }
        }
    } while (continu == true);

    this->listWidgetMedia->addItem(nameOfNewMedia);
    this->showColor->setNewRandomColor();
    paramGlobal.listMedia.append(tMedia(nameOfNewMedia, this->showColor->getBackgroundColor()));
    numberOfMedia++;
}

void tGesMedia::supprimerMediaClick()
{
    QListWidgetItem *item = this->listWidgetMedia->currentItem();
    if (item == NULL) return;

    /** Demande de confirmation de suppression */
    int ret = QMessageBox::question(this, tr("Confirmation"), QString(tr("Etes vous sur de vouloir supprimer le Media: %1 ?")).arg(item->text()), QMessageBox::Yes | QMessageBox::No);
    if (ret != QMessageBox::Yes) return;

    /** Suppression du Media */
    this->listWidgetMedia->takeItem(this->listWidgetMedia->currentRow());
    for (QList<tMedia>::iterator i=paramGlobal.listMedia.begin() ; i!=paramGlobal.listMedia.end() ; ++i) {
        if ((*i).getName() == item->text()) {
            i = paramGlobal.listMedia.erase(i);
            break;
        }
    }

    /** Si la liste est vide, on désactive les widgets */
    if (!this->listWidgetMedia->count()) {
        this->comboBoxType->setEnabled(false);
        this->ligneNom->setEnabled(false);
        this->ligneUrl->setEnabled(false);
        this->groupBoxOptions->setEnabled(false);
    }
}

void tGesMedia::UrlClick()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Ouvrir"), "", tr("All supported (*.avi *.mkv *.png);;Video (*.avi *.mkv);;Images (*.png)"));
    if (filename.isEmpty()) return;
    this->ligneUrl->setText(filename);
}

void tGesMedia::ColorClick()
{
    QColor color = QColorDialog::getColor(this->showColor->getBackgroundColor());
    if (color.isValid()) {
        this->showColor->setBackgroundColor(color);
        this->showColor->repaint();
    }
}

void tGesMedia::EnregistrerClick()
{
    /** Verification des Champs Type, Nom et Url */
    if (this->comboBoxType->currentText().isEmpty()) { QMessageBox::warning(this, tr("Impossible d'enregistrer"), tr("Vous n'avez pas séléctionnée le type !")); return; }
    if (this->ligneNom->text().isEmpty()) { QMessageBox::warning(this, tr("Impossible d'enregistrer"), tr("Vous n'avez pas entrer de nom !")); return; }
    if (this->ligneUrl->text().isEmpty()) { QMessageBox::warning(this, tr("Impossible d'enregistrer"), tr("Vous n'avez pas entrer d'URL !")); return; }

    /** Maj du nom du Media */
    QListWidgetItem *item = this->listWidgetMedia->currentItem();
    if (item == NULL) return;

    /** MAJ List Of Media */
    for (QList<tMedia>::iterator i=paramGlobal.listMedia.begin() ; i!=paramGlobal.listMedia.end() ; ++i) {
        if ((*i).getName() == item->text()) {
            (*i).setName(this->ligneNom->text());
            (*i).setUrl(this->ligneUrl->text());
            (*i).setType(this->comboBoxType->currentText());
            (*i).setFadeIn(this->option_fadeIn->isChecked());
            (*i).setFadeOut(this->option_fadeOut->isChecked());
            (*i).setRepeat(this->option_repeat->isChecked());
            (*i).setLogin(this->option_login->text());
            (*i).setPassword(this->option_password->text());
            (*i).setColor(this->showColor->getBackgroundColor());
            (*i).setTime(this->timeEditDuree->time());
            break;
        }
    }
    item->setText(this->ligneNom->text());
}

void tGesMedia::QuitterClick()
{
    this->accept();
}
