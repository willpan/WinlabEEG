#include <QApplication>
#include <QLayout>
#include <QPushButton>
#include "render.h"
#include <QTabWidget>
#include <QLabel>
#include <QScrollArea>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QWidget * mainwindow = new QWidget;
    QHBoxLayout * mainLayout = new QHBoxLayout;
    QVBoxLayout * leftLayout = new QVBoxLayout;
    QLabel titleLabel("<h2>Winlab<em>EEG</em></h2>"
		      "<p>An open-source EEG GUI and<br />"
		      "Pattern Detection Toolpak</p>"
		      "<p>Brought to you by:<br/>"
		      "William Pan<br/>"
		      "Jordan Romvary<br/>"
		      "Katherine Liu</p>");
   
    
    QTabWidget tabs;
    
    QWidget * page1 = new QWidget;
    Render * render = new Render;
    QPushButton *pushbutton1 = new QPushButton("Play");
    QPushButton *pushbutton2 = new QPushButton("Pause");
    QPushButton *pushbutton3 = new QPushButton("Toggle Averaging");

    QObject::connect(pushbutton1, SIGNAL(clicked()),
		     render, SLOT(Play()));
    QObject::connect(pushbutton2, SIGNAL(clicked()),
		     render, SLOT(Pause()));
    QObject::connect(pushbutton3, SIGNAL(clicked()),
		     render, SLOT(setShowAvg()));

    QVBoxLayout *layout = new QVBoxLayout;
    QHBoxLayout *buttonlayout = new QHBoxLayout;
   
    QPushButton * setupButton = new QPushButton("Connect");
    QObject::connect(setupButton, SIGNAL(clicked()),
		     render,SLOT(connectEmotiv()));

    leftLayout->addWidget(&titleLabel);
    leftLayout->addWidget(setupButton);
    leftLayout->addStretch();
    mainLayout->addLayout(leftLayout);
    buttonlayout->addWidget(pushbutton1);
    buttonlayout->addWidget(pushbutton2);
    buttonlayout->addWidget(pushbutton3);
    layout->addWidget(render);
    layout->addLayout(buttonlayout);
    page1->setLayout(layout);
    
    tabs.addTab(page1,"Waveforms");

    QWidget * page2 = new QWidget;
    tabs.addTab(page2, "FFT");
    
    mainLayout->addWidget(&tabs);
    mainwindow->setLayout(mainLayout);
    mainwindow->show();
    return app.exec();
}
