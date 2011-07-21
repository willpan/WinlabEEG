#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);
  mainwindow MainProgram;
  MainProgram.show();
  return app.exec();

}
