#include "mainwindow.h"
#include <QtGui>

mainwindow::mainwindow(QWidget *parent)
  : QWidget(parent)
{
  training = 0;
  setAutoFillBackground(false);
  playing = false;
  timer    = new QTimer;
  reader   = new Reader;
  renderer = new Renderer;
  gyro  = new gyro_widget;
  tabs  = new QTabWidget;
  page1 = new QWidget;
  page2 = new QWidget;
  connect_will_button = new QPushButton("Connect Will's Emotiv");
  connect_kat_button  = new QPushButton("Connect Kat's Emotiv");
  start_stop_button   = new QPushButton("Start/Stop");
  toggle_print        = new QPushButton("Toggle Print");
  training_button     = new QPushButton("Start Training");
  reset_gyro = new QPushButton("Reset Gyro Position");
  thread = 0;
  setup_page1();
  setup_page2();
  tabs->addTab(page1, "Waveforms");
  tabs->addTab(page2, "Gyro");
  QObject::connect(timer,SIGNAL(timeout()),
		   reader,SLOT(read_buffer()));
  QObject::connect(timer,SIGNAL(timeout()),
		   this,SLOT(update_widgets()));
  QObject::connect(training_button,SIGNAL(clicked()),
		   this, SLOT(thread_do_stuff()));
  QObject::connect(connect_will_button,SIGNAL(clicked()),
		   this, SLOT(connect_will()));
  QObject::connect(connect_kat_button, SIGNAL(clicked()),
		   this, SLOT(connect_kat()));
  QObject::connect(start_stop_button,SIGNAL(clicked()),
		   this, SLOT(start_stop_reader()));
  QObject::connect(toggle_print, SIGNAL(clicked()),
		   reader, SLOT(toggle_print()));
  QObject::connect(reset_gyro, SLOT(clicked()),
		   gyro, SLOT(reset_pos()));
  setup_mainwindow();
  this->setLayout(main_layout);
}

void mainwindow::setup_page1()
{

  renderer->new_data = reader->node_data;
  QVBoxLayout * page1_layout = new QVBoxLayout;
  page1_layout->addWidget(renderer);
  page1->setLayout(page1_layout);
}

void mainwindow::setup_page2()
{
  QVBoxLayout * page2_layout = new QVBoxLayout;
  page2_layout->addWidget(gyro);
  
  QHBoxLayout * reset_layout = new QHBoxLayout;
  reset_layout->addWidget(reset_gyro);
  reset_layout->addStretch();
  page2_layout->addLayout(reset_layout);

  page2->setLayout(page2_layout);
}

void mainwindow::setup_mainwindow()
{
  main_layout = new QHBoxLayout;
  
  QVBoxLayout * left_layout = new QVBoxLayout; 
  QLabel * WEEG_label = new QLabel("<h2>Winlab<em>EEG</em></h2>"
				   "<p>An open source EEG GUI <br />"
				   "and Patter Detection Toolpak. <br />"
				   "Brought to you by: <br />"
				   "William Pan<br /> "
				   "Jordan Romvary<br />"
				   "Katherine Liu<br />"
				   "John Ting<br />"
				   "Krishna Yelleyi</p>"
				   "<p> Usage:</p>");
  left_layout->addWidget(WEEG_label);
  left_layout->addWidget(connect_will_button);
  left_layout->addWidget(connect_kat_button);
  left_layout->addWidget(start_stop_button);
  left_layout->addWidget(toggle_print);
  left_layout->addWidget(training_button);
  left_layout->addStretch();
  
  QVBoxLayout * right_layout = new QVBoxLayout;
  QLabel * desc_label = new QLabel;
  right_layout->addWidget(desc_label);
  right_layout->addWidget(tabs);
  
  main_layout->addLayout(left_layout);
  main_layout->addLayout(right_layout);
}

void mainwindow::start_stop_reader()
{
  if(!playing)
    {
      cout << "Starting timer\n";
      timer->start(5);
    }
  else
    {
      cout << "Stopping timer\n";
      timer->stop();
    }
  playing = !playing;
}

void mainwindow::thread_do_stuff()
{
  if(thread == 0)
    {
      thread = new classifier_thread;
      training_button->setEnabled(false);
      training = true;
      cout << "start training\n";
    }
}

void mainwindow::connect_emotiv()
{
  reader->connect_emotiv();
  reader->set_key_Kat();
  cout << "Emotiv connected\n";
}

void mainwindow::update_widgets()
{
  reader->read_buffer();
  renderer->update();
  gyro->read_gyro(reader->gyro_x,reader->gyro_y);
  gyro->refreshPixmap();
  if(training)
    {
      static int counter = 0;
      static const char * sensor_names[] = {"AF3","AF4","F3","F4","F7",
					    "F8","FC5","FC6","O1","O2",
					    "P7","P8","T7","T8"};
      for(int i = 0; i < 14; i++)
	{
	  thread->sensor_data[sensor_names[i]].push_back(reader->node_data[i]);
	}
      counter++;
      if(counter == 1000)
	{
	  training = false;
	  counter = 0;
	  training_button->setEnabled(true);
	  thread->start();
	  cout << "end training\n";
	}
    }
}

void mainwindow::connect_will()
{
  reader->connect_emotiv();
  reader->set_key_Will();
  renderer->offsets = reader->offsets;
  start_stop_reader();
}

void mainwindow::connect_kat()
{
  reader->connect_emotiv();
  reader->set_key_Kat();
  renderer->offsets = reader->offsets;
  start_stop_reader();
}
