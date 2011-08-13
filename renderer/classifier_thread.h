#ifndef CLASSIFIER_THREAD_H
#define CLASSIFIER_THREAD_H

#include <QThread>
#include <utility>
#include <deque>
#include <map>
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>

using namespace std;

class classifier_thread : public QThread
{

 public:
  classifier_thread();
  volatile bool stopped;
  void stop();
  map< string , deque<int> > sensor_data;

 protected:
  void run();
  
 };

#endif
