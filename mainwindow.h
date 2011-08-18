#include <QWidget>
#include <QLabel>
#include <QLayout>
#include <QTabWidget>
#include <QPushButton>
#include <fstream>
#include <iostream>
#include <QSlider>
#include "reader.h"
#include "renderer.h"
#include "classifier_thread.h"
#include "gyro_widget.h"

class mainwindow : public QWidget
{
  Q_OBJECT

    public:
  mainwindow(QWidget *parent = 0);
  gyro_widget * gyro; 

  public slots:
  void start_stop_reader();
  void thread_do_stuff();


 private slots:
  void connect_emotiv();
  void update_widgets();
  void connect_will();
  void connect_kat();
  void commands();
  
 private:
  QHBoxLayout * main_layout;
  void setup_page1();
  void setup_page2();
  void setup_mainwindow();
  void do_action(int);
  QPushButton * connect_will_button;
  QPushButton * connect_kat_button;
  QPushButton * start_stop_button;
  QPushButton * toggle_print;
  QPushButton * training_button;
  QPushButton * reset_button;
  QPushButton * commands_button;
  QTabWidget * tabs;
  QWidget * page1;
  QWidget * page2;
  QLabel * main_label;
  Reader * reader;  
  Renderer * renderer;
  classifier_thread * thread;
  QTimer * timer;
  bool playing;
  bool training;
  bool accept_commands;
 
};
