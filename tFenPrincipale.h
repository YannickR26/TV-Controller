#ifndef TFENPRINCIPALE_H
#define TFENPRINCIPALE_H

#include "tPlanning.h"
#include "tPlanningArrierePlan.h"
#include "tConfig.h"
#include "tLive.h"
#include "tParametreGlobal.h"

#include <QMainWindow>
#include <QListWidget>
#include <QTabWidget>

class tFenPrincipale : public QMainWindow
{
  Q_OBJECT

  public:
    tFenPrincipale(QApplication *app);
    virtual ~tFenPrincipale();

    QString getFileName() { return this->fileName; }

  protected:
    void updateListTvFromParamGlobal();
    void writeParamGlobalFromFile();
    void closeEvent(QCloseEvent *event);

  private slots:
    void actionNouveauClick();
    void actionOuvrirClick();
    void actionEnregistrerClick();
    void actionEnregistrerSousClick();
    void actionMediaClick();
    void actionScriptClick();
    void actionPreferencesClick();
    void aProposClick();
    void listTvChange(QListWidgetItem *item);
    void ajouterTvClick();
    void supprimerTvClick();
    void updateListTvFromConfig();

  private:
    QAction               *actionEnregistrer;
    QStatusBar            *barreEtat;
    QWidget               *widgetCentral;
    QListWidget           *listTv;
    QTabWidget            *tabFenetre;
    tConfig               *config;
    tPlanning             *planning;
    tPlanningArrierePlan  *planningArrierePlan;
    tLive                 *live;
    QString               fileName;
};

#endif // TFENPRINCIPALE_H
