#ifndef RENDER_H
#define RENDER_H

#include <map>
#include <utility>
#include <deque>

#include <QVector>
#include <QWidget>
#include <iostream>
#include <QPen>

#include <string.h>

/* Linux */
#include <linux/hidraw.h>

/* Unix */
#include <fcntl.h>

/* C */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

/* Libudev */
#include <libudev.h>
#include <stdlib.h>

/* OpenSSL */
#include <openssl/aes.h>

#define rectwidth 129
#define scrollInt 2
#define waitTime 5

using namespace std;

class Render : public QWidget
{
  Q_OBJECT

 public:
  Render(QWidget *parent = 0);
  QSize sizeHint() const;
  deque<int> * Buffer[14];
  map <string, deque<int> > SensorBits;
  void setKey(unsigned char *);
 
 protected:
  void paintEvent(QPaintEvent *event);
  void timerEvent(QTimerEvent *event);

 public slots:
  //void update();
  void setShowAvg();
  void setup();
  bool connectEmotiv();
  void Pause();
  void Play();
  void setKey_Kat();
  void setKey_Will();

 private:
  int offset;
  int myTimerId;
  QPen pen;
  const int * offsets;

  unsigned char emotivBuffer[32];
  void decrypt(unsigned char *in, unsigned char *out);
  AES_KEY decryptKey;
  int filedescriptor;
  int res;
  unsigned char * readBuffer();
  bool showAvg;

  
  deque <int> nodeData;
};

#endif
