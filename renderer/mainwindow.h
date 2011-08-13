#include <QWidget>
#include <QLabel>
#include <QLayout>
#include <QTabWidget>
#include <QPushButton>
#include <fstream>
#include <iostream>
#include "renderer.h"
#include "server_options_dialog.h"
#include "classifier_thread.h"
#include "gyro_widget.h"

class mainwindow : public QWidget
{
  Q_OBJECT

    public:
  mainwindow(QWidget *parent = 0);
  int timer;

  public slots:
  void play_pause_renderer();
  void connect_disconnect_server();
  void set_options();
  void thread_do_stuff();
  
 private:
  void setup_page1();
  void setup_page2();
  void setup_mainwindow();
  QHBoxLayout * main_layout;
  QVBoxLayout * left_layout;
  QVBoxLayout * renderer_layout;
  QHBoxLayout * buttons_layout;
  Renderer * renderer;
  gyro_widget * gyro;
  QPushButton * play_button;
  QPushButton * pause_button;
  QPushButton * toggle_button;
  QPushButton * connect_button;
  QPushButton * connect_Kat;
  QPushButton * connect_Will;
  QPushButton * connect_server;
  QPushButton * server_options;
  QPushButton * print_on_off;
  QPushButton * start_thread;
  QTabWidget * tabs;
  QWidget * page1;
  QWidget * page2;
  server_options_dialog * dialog;
  classifier_thread * thread;

  
};
