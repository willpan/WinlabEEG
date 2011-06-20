#include <QApplication>
#include <QLayout>
#include <QPushButton>
#include "render.h"


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

	
	QWidget *window = new QWidget;
	Render *render = new Render;
	
	
	QPushButton *pushbutton1 = new QPushButton("Play");
	QPushButton *pushbutton2 = new QPushButton("Pause");
	
	QObject::connect(pushbutton1, SIGNAL(clicked()),
	                 render, SLOT(Play()));

	QObject::connect(pushbutton2, SIGNAL(clicked()),
	                 render, SLOT(Pause()));

	
	
	//QVBoxLayout *layout = new QVBoxLayout;
	QHBoxLayout *buttonlayout = new QHBoxLayout;

	buttonlayout->addWidget(pushbutton1);
	buttonlayout->addWidget(pushbutton2);
	//layout->addWidget(ticker);
	//layout->addLayout(buttonlayout);
	window->setLayout(buttonlayout);

	window->show();
	render->show();
    return app.exec();
}
