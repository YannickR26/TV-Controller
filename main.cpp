#include <QApplication>
#include "tFenPrincipale.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    QCoreApplication::setOrganizationName("TVController");
    QCoreApplication::setApplicationName("TV Controleur");
    app.setApplicationName("TV Controlleur");
    app.setWindowIcon(QIcon(":/icon/pixmap/tv-icon.ico"));
    tFenPrincipale fenPrincipale(&app);

    return app.exec();
}
