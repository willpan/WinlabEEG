#include <QtGui>
#include <cmath>

#include "gyro_widget.h"

gyro_widget::gyro_widget(QWidget *parent)
    : QLabel(parent)
{
  setBackgroundRole(QPalette::Dark);
  setAutoFillBackground(true);
  setSizePolicy(QSizePolicy::Expanding, 
		QSizePolicy::Expanding);
  gyro_x = 0;
  gyro_y = 0;
  pos_x = 300;
  pos_y = 280;
  multiplier_x = 2;
  multiplier_y = 2;
}

QSize gyro_widget::sizeHint() const
{
  return QSize(600, 560);
}

void gyro_widget::paintEvent(QPaintEvent * /* event */)
{
  QStylePainter painter(this);
  painter.drawPixmap(0, 0, pixmap);
}

void gyro_widget::refreshPixmap()
{
  pixmap = QPixmap(size());
  pixmap.fill(this, 0, 0);
 
  
  if(abs(gyro_x) < 2)
    gyro_x = 0;
  else
    gyro_x = multiplier_x * gyro_x;
  if(abs(gyro_y) < 2)
    gyro_y = 0;
  else 
    gyro_y = multiplier_y * gyro_y;
  
  pos_x = (pos_x - gyro_x < 5) ? 5 : (pos_x - gyro_x);
  if(pos_x > width() - 5 )
    pos_x = width() - 5;
  pos_y = (pos_y - gyro_y < 5) ? 5 : (pos_y - gyro_y);
  if(pos_y > height() - 5 ) 
    pos_y = height() - 5;

  QPainter painter(&pixmap);
  painter.initFrom(this);
  drawGrid(&painter);
  drawSquare(&painter);
  update();
}

void gyro_widget::read_gyro(int new_x, int new_y)
{
  gyro_x = new_x;
  gyro_y = new_y;
}

void gyro_widget::drawGrid(QPainter *painter)
{
  painter->setPen(Qt::black);
  painter->drawRect( 0, MARGIN, MARGIN, height() - 2*MARGIN);   //left
  painter->drawRect( MARGIN, 0, width() - 2*MARGIN, MARGIN);   //top
  painter->drawRect( width() - MARGIN, MARGIN, MARGIN, height() - 2*MARGIN); //right
  painter->drawRect( MARGIN, height() - MARGIN, width() - 2*MARGIN, MARGIN); //bottom
}

void gyro_widget::drawSquare(QPainter *painter)
{
  painter->setPen(Qt::red);
  painter->fillRect(pos_x - 5, pos_y - 5, 10, 10 ,Qt::red);
}

void gyro_widget::reset_position()
{
  pos_x = width()/2;
  pos_y = height()/2;
  refreshPixmap();
}


int gyro_widget::action()
{
  
  //  1 = Look left
  //  2 = Look right
  //  3 = look up
  //  4 = look down
  if( pos_x < MARGIN )
    return 1;
  else if(pos_x > width() - MARGIN )
    return 2;
  else if(pos_y < MARGIN)
    return 3;
  else if(pos_y > height() - MARGIN )
    return 4;
  else 
    return 0;

}

void gyro_widget::set_x(int x)
{
  multiplier_x = (double)x/5;
}

void gyro_widget::set_y(int y)
{
  multiplier_y = (double)y/5;
}
