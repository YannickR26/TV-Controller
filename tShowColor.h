#ifndef TSHOWCOLOR_H
#define TSHOWCOLOR_H

#include <QWidget>

class tShowColor : public QWidget
{
  Q_OBJECT

  public:
    tShowColor();

    void    setNewRandomColor();
    void    setBackgroundColor(QColor _color)   { backGroundColor = _color; }
    QColor  getBackgroundColor()                { return backGroundColor;   }

  protected:
    void paintEvent(QPaintEvent *event);

  private:
    QColor backGroundColor;
};

#endif // TSHOWCOLOR_H
