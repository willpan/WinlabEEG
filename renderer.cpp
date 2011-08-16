#include <QtGui>
#include "renderer.h"

Renderer::Renderer(QWidget *parent)
  : QWidget(parent)
{
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
  setAutoFillBackground(TRUE);
  pen.setWidth(1);
  pen.setColor(Qt::blue);
 
  sync = 0;
  static int Will_offsets[]={
    7760, 8530, 8000, 8510, 8900, 8400, 8320,
    8720, 7230, 8820, 7700, 8050, 9080, 8650
  };
  static int Kat_offsets[]={
    8120, 7950, 7850, 7880, 8750, 8110, 8590, 
    8240, 8960, 8470, 8700, 8260, 8640, 7445
  };
  offsets = Kat_offsets;
  new_data = offsets;
  memset(old_data, 0, sizeof(old_data));
}

QSize Renderer::sizeHint() const
{
  return QSize(600,540);
}

void Renderer::update()
{
  sync++;
  scroll(-SCROLL_WIDTH,0);
  
}

void Renderer::paintEvent(QPaintEvent * /* event */)
{
  static const QColor colors[12] = { 
    Qt::red, Qt::green, Qt::blue, Qt::cyan, Qt::magenta, Qt::yellow, Qt::darkRed,
    Qt::darkGreen, Qt::darkBlue, Qt::darkCyan, Qt::darkMagenta, Qt::darkYellow 
  };
  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);
  if( sync % RECT_WIDTH == 0)
    {
      painter.setPen(Qt::black);
      painter.drawLine(width()-1,0,width()-1,height());
      sync = 0;
    }
 
  for( int i = 0; i < 14; i++)
    {
      painter.setPen(colors[i%12]);
      painter.drawLine(width()-SCROLL_WIDTH,
		      i*35+( old_data[i]-offsets[i]  )/5  ,
		      width(),
		      i*35+( new_data[i]-offsets[i]  )/5  );
    }
  memcpy(old_data,new_data,sizeof(old_data));
}
