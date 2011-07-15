#include "mainwindow.h"

MainWindow::MainWindow()
{
  create_mainWindow();
  create_renderingTab();
  create_fftTab();
  mainLayout->addWidget(&tabs);
  central_widget = new QWidget;
  central_widget->setLayout(mainLayout);
  setCentralWidget(central_widget);
  
}

void MainWindow::create_mainWindow()
{
  mainLayout = new QHBoxLayout;
  QVBoxLayout * leftLayout = new QVBoxLayout;
  QLabel titleLabel("<h2>Winlab<em>EEG</em></h2>"
		    "<p>An open-source EEG GUI and<br />"
		    "Pattern Detection Toolpak</p>"
		    "<p>Brought to you by:<br/>"
		    "William Pan<br/>"
		    "Jordan Romvary<br/>"
		    "Katherine Liu</p>");

  setupButton = new QPushButton("Connect");

  leftLayout->addWidget(&titleLabel);
  leftLayout->addWidget(setupButton);
  leftLayout->addStretch();
  mainLayout->addLayout(leftLayout);
  
  page1 = new QWidget;
  page2 = new QWidget;
}

void MainWindow::create_renderingTab()
{
  renderer = new Render;

  /*** Add Buttons ***/
  QHBoxLayout *buttonlayout = new QHBoxLayout;

  QPushButton *pushbutton1 = new QPushButton("Play");
  QPushButton *pushbutton2 = new QPushButton("Pause");
  QPushButton *pushbutton3 = new QPushButton("Toggle Averaging");

  QObject::connect(pushbutton1, SIGNAL(clicked()),
		   renderer, SLOT(Play()));
  QObject::connect(pushbutton2, SIGNAL(clicked()),
		   renderer, SLOT(Pause()));
  QObject::connect(pushbutton3, SIGNAL(clicked()),
		   renderer, SLOT(setShowAvg()));
  buttonlayout->addWidget(pushbutton1);
  buttonlayout->addWidget(pushbutton2);
  buttonlayout->addWidget(pushbutton3);

  /*** Putting Renderer Together ***/

  QVBoxLayout * renderer_layout = new QVBoxLayout;
   
  QObject::connect(setupButton, SIGNAL(clicked()),
		   renderer,SLOT(connectEmotiv()));

  renderer_layout->addWidget(renderer);
  renderer_layout->addLayout(buttonlayout);
  page1->setLayout(renderer_layout);
  tabs.addTab(page1,"Waveforms");
}

void MainWindow::create_fftTab()
{
  tabs.addTab(page2, "FFT");
}


