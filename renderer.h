#ifndef RENDERER_H
#define RENDERER_H

#include <string>
#include <map>
#include <deque>
#include <utility>
#include <QWidget>
#include <QPen>

#define SCROLL_WIDTH 2
#define RECT_WIDTH 129

class Renderer : public QWidget
{
  Q_OBJECT

 public:
  Renderer(QWidget *parent = 0);
  QSize sizeHint() const;
  int * new_data;
  int old_data[14];
  int * offsets;
    
 protected:
  void paintEvent(QPaintEvent *event);
  
 public slots:
  void update();

 private:
  int sync;
  QPen pen;
  
};

#endif
