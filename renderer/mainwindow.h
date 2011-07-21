#include <QWidget>
#include <QLabel>
#include <QLayout>
#include <QTabWidget>
#include <QPushButton>
#include "render.h"

class mainwindow : public QWidget
{
  Q_OBJECT

    public:
  mainwindow(QWidget *parent = 0);


 private:
  void setup_page1();
  void setup_page2();
  void setup_mainwindow();
  QHBoxLayout * main_layout;
  QVBoxLayout * left_layout;
  QVBoxLayout * renderer_layout;
  QHBoxLayout * buttons_layout;
  Render * renderer;
  QPushButton * play_button;
  QPushButton * pause_button;
  QPushButton * toggle_button;
  QPushButton * connect_button;
  QPushButton * connect_Kat;
  QPushButton * connect_Will;
  QTabWidget * tabs;
  QWidget * page1;
  QWidget * page2;

};
