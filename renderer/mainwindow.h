#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QPushButton>
#include <QLayout>
#include <QLabel>
#include "render.h"
#include <QWidget>


class MainWindow : public QMainWindow
{
  Q_OBJECT
  
 public:
  MainWindow();
  
 
  public slots:
  //void Pause();
  //void Play();
  //  bool connectEmotiv();


 private:
  void create_mainWindow();
  void create_renderingTab();
  void create_fftTab();
  Render *renderer;
  
  //QTimer *timer;
  QHBoxLayout * mainLayout;
  QWidget * page1;
  QWidget * page2;
  QTabWidget tabs;
  QPushButton *playButton;
  QPushButton *pauseButton;
  QPushButton *toggleAverage;
  QPushButton * setupButton;
  QWidget *central_widget;
};

#endif
