#ifndef GYRO_WIDGET_H
#define GYRO_WIDGET_H

#include <QPixmap>
#include <QWidget>
#include <iostream>
#include <unistd.h>
#include <QLabel>

class gyro_widget : public QLabel
{
    Q_OBJECT

public:
    gyro_widget(QWidget *parent = 0);
    QSize sizeHint() const;
    int gyro_x;
    int gyro_y;
    void read_gyro(int,int);

public slots:
    void refreshPixmap();
    void reset_pos();

protected:
    void paintEvent(QPaintEvent *event);
    int pos_x;
    int pos_y;


private:

    void drawGrid(QPainter *painter);
    void drawSquare(QPainter *painter);
    QPixmap pixmap;
};

#endif
