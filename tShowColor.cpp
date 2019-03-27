#include "tShowColor.h"

#include <QtGlobal>
#include <QPainter>
#include <QTime>

#define randInt() (qrand() % (256))

tShowColor::tShowColor()
{
  backGroundColor = QColor(255,255,255);
  this->setMinimumWidth(50);
}

void tShowColor::setNewRandomColor()
{
  QTime time = QTime::currentTime();
  qsrand((uint)time.msec());
  backGroundColor = QColor(randInt(),randInt(),randInt());
}

void tShowColor::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
  //create a QPainter and pass a pointer to the device.
  //A paint device can be a QWidget, a QPixmap or a QImage
  QPainter painter(this);

  QRectF rectangle(0.0, 0.0, this->width()-1, this->height()-1);

  // Définit de la couleur de fond
  painter.setBrush(this->backGroundColor);

  // Desine un rectangle arrondi
  painter.drawRoundedRect(rectangle, 10.0, 10.0);

  painter.setRenderHint(QPainter::Antialiasing, true);
}
