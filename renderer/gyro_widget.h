#ifndef GYRO_WIDGET_H
#define GYRO_WIDGET_H

#include <QWidget>
#include <QPen>
#include <iostream>

using namespace std;

class gyro_widget : public QWidget
{
Q_OBJECT

  public:
  gyro_widget(QWidget * parent = 0);
  QSize sizeHint() const;
  int * gyro_x;
  int * gyro_y;
  int * gyro_timer_id;
  int pos_x;
  int pos_y;
  
 protected:
  void timerEvent(QTimerEvent *event);
};

#endif
