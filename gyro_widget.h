#ifndef GYRO_WIDGET_H
#define GYRO_WIDGET_H

#include <QPixmap>
#include <QWidget>
#include <iostream>
#include <unistd.h>
#include <QLabel>

#define MARGIN 60

class gyro_widget : public QLabel
{
    Q_OBJECT

public:
    gyro_widget(QWidget *parent = 0);
    QSize sizeHint() const;
    int gyro_x;
    int gyro_y;
    void read_gyro(int,int);
    int action();
    int pos_x;
    int pos_y;


public slots:
    void refreshPixmap();
    void reset_position();

protected:
    void paintEvent(QPaintEvent *event);
    
private:

    void drawGrid(QPainter *painter);
    void drawSquare(QPainter *painter);
    QPixmap pixmap;
};

#endif
