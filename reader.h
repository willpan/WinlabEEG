#ifndef READER_H
#define READER_H

#include <map>              //STL map class
#include <utility>          //STL utilities
#include <deque>            //STL deque class
#include <iostream>
#include <QWidget>
#include <QPen>
#include <linux/hidraw.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <libudev.h>         //Libudev
#include <openssl/aes.h>     //Libcrypto

#define RECT_WIDTH 129
#define SCROLL_WIDTH 2
#define WAIT_TIME 5


typedef unsigned char uchar;
using namespace std;

class Reader : public QWidget
{
  Q_OBJECT

 public:
  Reader(QWidget *parent = 0);
  map <string, int * > sensor_bits;
  void setKey(uchar *);
  int gyro_x;
  int gyro_y;
  int node_data[14];
  bool print;
  int * offsets;

 public slots:
  void read_buffer();
  bool connect_emotiv();
  void set_key_Kat();
  void set_key_Will();
  void toggle_print();
  
 private:
  uchar emotiv_buffer[32];
  void decrypt(uchar *in, uchar *out);
  AES_KEY decryptKey;
  int emotiv_fd;
  void set_key(uchar *);


   
};

#endif
