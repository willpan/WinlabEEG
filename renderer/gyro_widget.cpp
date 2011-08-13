#include "gyro_widget.h"
#include <QtGui>

gyro_widget::gyro_widget(QWidget *parent)
  : QWidget(parent)
{
  setAutoFillBackground(TRUE);
  pos_x = 298;
  pos_y = 248;
  gyro_x = 0;
  gyro_x = 0;
   
}

QSize gyro_widget::sizeHint() const
{
  return QSize(600, 540);
}

void gyro_widget::timerEvent(QTimerEvent *event)
{
  cout << "gyro widget 1\n";
  if(event->timerId() == (*gyro_timer_id))
    {
      QPainter painter(this);
      painter.setRenderHint(QPainter::Antialiasing);
      pos_x = pos_x + (*gyro_x - 121);
      pos_y = pos_y + (*gyro_y - 120);
      pos_x = ( (pos_x > 2 ) ? pos_x : 2 );
      pos_x = ( (pos_x < 598) ? pos_x : 598 );
      pos_y = ( (pos_y < 2 ) ? pos_y : 2 );
      pos_y = ( (pos_y < 538) ? pos_y : 538 );
      cout << "x = " << pos_x << "\t";
      cout << "y = " << pos_y << endl;
      painter.fillRect(pos_x, pos_y, 4, 4, Qt::red);
    }
  else 
    {
      QWidget::timerEvent(event);
    }
}
