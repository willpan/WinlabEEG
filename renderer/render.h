#ifndef RENDER_H
#define RENDER_H


#include <QVector>
#include <QWidget>
#include <iostream>
#include <QPen>

/* Linux */
#include <linux/types.h>
#include <linux/input.h>
#include <linux/hidraw.h>

/* Unix */
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

/* C */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

/* Libudev */
#include <libudev.h>
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <unistd.h>

/* OpenSSL */

#include <openssl/aes.h>

#define rectwidth 129
#define scrollInt 2
#define waitTime 5

class Render : public QWidget
{
  Q_OBJECT

    public:
  Render(QWidget *parent = 0);
  QSize sizeHint() const;
  QVector<int> Buffer0;
  QVector<int> Buffer1;
  QVector<int> Buffer2;
  QVector<int> Buffer3;
  QVector<int> Buffer4;
  QVector<int> Buffer5;
  QVector<int> Buffer6;
  QVector<int> Buffer7;
  QVector<int> Avg;
  //QVector<int> Buffer8;
  //QVector<int> Buffer9;
  //QVector<int> Buffer10;
  //QVector<int> Buffer11;
  //int byteNum;

  void setKey(unsigned char *);

 protected:
  void paintEvent(QPaintEvent *event);
  void timerEvent(QTimerEvent *event);

  public slots:
  void Pause();
  void Play();
  void setShowAvg();
  void setup();
  bool connectEmotiv();

 private:
  int offset;
  int myTimerId;
  QPen pen;

  unsigned char emotivBuffer[32];
  void decrypt(unsigned char *in, unsigned char *out);
  AES_KEY decryptKey;
  int filedescriptor;
  int res;
  unsigned char * readBuffer();
  bool showAvg;
};

#endif
