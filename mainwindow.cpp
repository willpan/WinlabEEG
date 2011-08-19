#include "mainwindow.h"
#include <QtGui>

mainwindow::mainwindow(QWidget *parent)
  : QWidget(parent)
{
  accept_commands = false;
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
  reset_button        = new QPushButton("Reset Gyro Position");
  commands_button     = new QPushButton("Accept Commands");
  thread = 0;
  setup_page1();
  setup_page2();
  tabs->addTab(page1, "Waveforms");
  tabs->addTab(page2, "Gyro");
  start_stop_button->setEnabled(false);

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
  QObject::connect(reset_button, SIGNAL(clicked()),
		   gyro, SLOT(reset_position()));
  QObject::connect(commands_button, SIGNAL(clicked()),
		   this, SLOT(commands()));
  
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
  QSlider *sliderx = new QSlider(Qt::Horizontal);
  QSlider *slidery = new QSlider(Qt::Horizontal);
  sliderx->setRange(1,20);
  slidery->setRange(1,20);
  QLabel * xlabel = new QLabel("Gyro sensitivity (X, Y):  ");

  QVBoxLayout * page2_layout = new QVBoxLayout;
  page2_layout->addWidget(gyro);

  QHBoxLayout * buttons = new QHBoxLayout;
  buttons->addWidget(reset_button);
  buttons->addWidget(commands_button);
  buttons->addStretch();
  buttons->addWidget(xlabel);
  buttons->addWidget(sliderx);
  buttons->addWidget(slidery);
  
  QObject::connect(sliderx,SIGNAL(valueChanged(int)),
		   gyro, SLOT(set_x(int)));
  QObject::connect(slidery,SIGNAL(valueChanged(int)),
		   gyro, SLOT(set_y(int)));
  sliderx->setValue(10);
  slidery->setValue(10);

  page2_layout->addLayout(buttons);

  page2->setLayout(page2_layout);
  gyro->refreshPixmap();
}

void mainwindow::setup_mainwindow()
{
  main_layout = new QHBoxLayout;

  QVBoxLayout * left_layout = new QVBoxLayout;
  QLabel * label = new QLabel("<h2>Winlab<em>EEG</em></h2>"
			      "<p>An open source EEG GUI and <br />"
			      "Patter Detection Tookpak. <br />"
			      "<br /> Brought to you by: <br />"
			      "William Pan, <br />"
			      "Jordan Romvary <br />"
			      "Katherine Liu <br />"
			      "John Ting <br />"
			      "Krishna Yelleyi</p>"
			      "<p> Usage:</p>");
  left_layout->addWidget(label);
  left_layout->addWidget(connect_will_button);
  left_layout->addWidget(connect_kat_button);
  left_layout->addWidget(start_stop_button);
  left_layout->addWidget(toggle_print);
  left_layout->addWidget(training_button);
  left_layout->addStretch();
  
  main_layout->addLayout(left_layout);
  main_layout->addWidget(tabs);
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
  if(thread != 0)
    thread->~classifier_thread();
  thread = 0;
  thread = new classifier_thread;
  training_button->setEnabled(false);
  training = true;
  cout << "start training\n";
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
  if(accept_commands)
    {
      do_action();
    }
  if(training)
    {
      train();
    }
}

void mainwindow::connect_will()
{
  reader->connect_emotiv();
  reader->set_key_Will();
  renderer->offsets = reader->offsets;
  start_stop_reader();
  start_stop_button->setEnabled(true);
}

void mainwindow::connect_kat()
{
  reader->connect_emotiv();
  reader->set_key_Kat();
  renderer->offsets = reader->offsets;
  start_stop_reader();
  start_stop_button->setEnabled(true);
}

void mainwindow::commands()
{
  accept_commands = !accept_commands;
  if(accept_commands)
    {
      commands_button->setText("Stop Commands");
    }
  else
    {
      commands_button->setText("Accept Commands");
    }
}

void mainwindow::do_action()
{
  static ofstream output;
  static int count = 0;
  static int last_action = -1;
  static int curr_action;
  curr_action = gyro->action();

  if( curr_action == 0 ) //User is looking forward
    {
      if(count == 0)
	{
	  output.open("./scripts/temp.csv");
	  output << "AF3,F4,F3,F4,F7,F8,FC5,FC6,O1,O2,P7,P8,T7,T8" << endl;
	}
      count++;
      if(count % 5 == 0)
	{
	  for(int i = 0; i < 14; i++)
	    {
	      output << reader->node_data[i] << ",";
	    }
	  output << endl;
	}
      if(count == 100)
	{
	  output.close();
	  system("./scripts/ *****************************  &");
	  count = 0; 
	}
    } 
  else if( curr_action == 1 && last_action != curr_action)
    {
      system("./scripts/commands.sh 1 &");
      count = 0;
      if(output.is_open())
	{
	  output.close();
	  count = 0;
	}
      last_action = curr_action;
    }
  else if( curr_action == 2 && last_action != curr_action)
    {
      system("./scripts/commands.sh 2 &");
      count = 0;
      if(output.is_open())
	{
	  output.close();
	  count = 0;
	}
      last_action = curr_action;
    }
  /*  if( curr_action != last_action )
      {
      switch(curr_action)
	{
	case 0:
	  break;
	case 1:
	  system("./scripts/commands.sh 1");
	  break;
	case 2:
	  system("./scripts/commands.sh 2");
	  break;
	case 3:
	  system("./scripts/commands.sh 3");
	  break;
	case 4:
	  system("./scripts/commands.sh 4");
	  break;	  
	}
      last_action = curr_action;    
    }
  */
}

void mainwindow::train()
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
