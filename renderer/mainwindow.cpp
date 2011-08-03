#include "mainwindow.h"

mainwindow::mainwindow(QWidget *parent)
  : QWidget(parent)
{

  main_layout = new QHBoxLayout;
  left_layout = new QVBoxLayout;
  renderer_layout = new QVBoxLayout;
  buttons_layout = new QHBoxLayout;
  renderer = new Renderer;
  play_button = new QPushButton("Play");
  toggle_button = new QPushButton("Toggle Averaging");
  connect_button = new QPushButton("Connect Emotiv");
  connect_Kat = new QPushButton("Connect Kat's Headset");
  connect_Will = new QPushButton("Connect Will's Headset");
  connect_server = new QPushButton("Connect to Server");
  tabs = new QTabWidget;
  page1 = new QWidget;
  page2 = new QWidget;
  setup_page1();
  setup_page2();
  tabs->addTab(page1, "Waveforms");
  tabs->addTab(page2, "FFT");
  setup_mainwindow();
  this->setLayout(main_layout);
}

void mainwindow::setup_page1()
{
  buttons_layout->addWidget(play_button);
  buttons_layout->addWidget(toggle_button);
  
  renderer_layout->addWidget(renderer);
  renderer_layout->addLayout(buttons_layout);

  page1->setLayout(renderer_layout);
  
  QObject::connect(play_button,SIGNAL(clicked()),
		   this, SLOT(play_pause_renderer()));
   QObject::connect(toggle_button,SIGNAL(clicked()),
		   renderer, SLOT(setShowAvg()));
  QObject::connect(connect_button, SIGNAL(clicked()),
		   renderer, SLOT(connectEmotiv()));
  QObject::connect(connect_Will, SIGNAL(clicked()),
		   renderer, SLOT(setKey_Will()));
  QObject::connect(connect_Kat, SIGNAL(clicked()),
		   renderer, SLOT(setKey_Kat()));
  QObject::connect(connect_server, SIGNAL(clicked()),
		   this, SLOT(connect_disconnect_server()));
}

void mainwindow::setup_page2()
{
  /*
    Well nothing here yet
  */
}

void mainwindow::setup_mainwindow()
{
  QLabel *title_label = new QLabel("<h2>Winlab<em>EEG</em></h2>"
		    "<p>An open-source EEG GUI and<br />"
		    "Pattern Detection Toolpak</p>"
		    "<p>Brought to you by:<br/>"
		    "William Pan<br/>"
		    "Jordan Romvary<br/>"
		    "Katherine Liu</p>");
 
  left_layout->addWidget(title_label);
  left_layout->addWidget(connect_button);
  left_layout->addWidget(connect_Will);
  left_layout->addWidget(connect_Kat);
  left_layout->addWidget(connect_server);
  left_layout->addStretch();

  main_layout->addLayout(left_layout);
  main_layout->addWidget(tabs);
}

void mainwindow::play_pause_renderer()
{
  if(renderer->playing)
    {
      play_button->setText(tr("Play"));
      renderer->play_pause();
    }
  else
    {
      play_button->setText(tr("Pause"));
      renderer->play_pause();
    }
}

void mainwindow::connect_disconnect_server()
{
  renderer->connect_server();
  if( (renderer->server_connected) == 1)
    connect_server->setText(tr("Disconnect from Server"));
  else
    connect_server->setText(tr("Connect to Server"));
  
      
}
