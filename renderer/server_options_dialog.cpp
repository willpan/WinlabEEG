#include <QtGui>
#include <string>
#include "server_options_dialog.h"

server_options_dialog::server_options_dialog(QWidget *parent)
  : QDialog(parent)
{ 
  port_label = new QLabel(tr("Port Number:  "));
  portno_edit = new QLineEdit("2000");
  port_label->setBuddy(portno_edit);

  host_label = new QLabel(tr("Host Address: "));
  host_edit = new QLineEdit("localhost");
  host_label->setBuddy(host_edit);

  accept_button = new QPushButton(tr("Accept"));
  accept_button->setDefault(true);
  accept_button->setEnabled(true);
  cancel_button = new QPushButton(tr("Cancel"));
  
  connect(portno_edit, SIGNAL(textChanged(const QString &)),
	  this, SLOT(enable_accept_button(const QString &)));
  connect(accept_button, SIGNAL(clicked()),
	  this, SLOT(accept_clicked()));
  connect(accept_button, SIGNAL(clicked()),
	  this, SLOT(close()));
  connect(host_edit, SIGNAL(textChanged(const QString &)),
	  this, SLOT(enable_accept_button(const QString &)));
  connect(accept_button, SIGNAL(clicked()),
	  this, SLOT(accept_clicked()));
  connect(cancel_button, SIGNAL(clicked()),
	  this, SLOT(close()));

  QHBoxLayout * topLayout = new QHBoxLayout;
  topLayout->addWidget(port_label);
  topLayout->addWidget(portno_edit);
  
  QHBoxLayout * middleLayout = new QHBoxLayout;
  middleLayout->addWidget(host_label);
  middleLayout->addWidget(host_edit);

  QHBoxLayout * bottomLayout = new QHBoxLayout;
  bottomLayout->addWidget(accept_button);
  bottomLayout->addWidget(cancel_button);

  QVBoxLayout * mainLayout = new QVBoxLayout;
  mainLayout->addLayout(topLayout);
  mainLayout->addLayout(middleLayout);
  mainLayout->addLayout(bottomLayout);
  setLayout(mainLayout);

  setWindowTitle(tr("Options"));
  setFixedHeight(sizeHint().height());
}

void server_options_dialog::accept_clicked()
{
  std::string str0 = (portno_edit->text()).toStdString();
  int portno = atoi(str0.c_str());
  
  std::string str1 = (host_edit->text()).toStdString();
  const char * host_addr = str1.c_str();

  emit accept_options(portno, host_addr);
}

void server_options_dialog::enable_accept_button(const QString &text)
{
  accept_button->setEnabled(!text.isEmpty());
}
