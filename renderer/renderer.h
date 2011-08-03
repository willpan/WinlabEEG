#ifndef RENDERER_H
#define RENDERER_H

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
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <libudev.h>         //Libudev
#include <openssl/aes.h>     //Libcrypto
#include <QMessageBox>

#define rectwidth 129
#define scrollInt 4
#define WAIT_TIME 10
#define FIRST_CONNECT 0
#define DISCONNECT 1
#define RECONNECT  2


using namespace std;

class Renderer : public QWidget
{
  Q_OBJECT

 public:
  Renderer(QWidget *parent = 0);
  QSize sizeHint() const;
  deque<int> * Buffer[14];
  map <string, deque<int> > SensorBits;
  void setKey(unsigned char *);
  bool playing;
  int server_connected;

 protected:
  void paintEvent(QPaintEvent *event);
  void timerEvent(QTimerEvent *event);
  
 public slots:

  void setShowAvg();
  void setup();
  bool connectEmotiv();
  void play_pause();
  void setKey_Kat();
  void setKey_Will();
  int connect_server();

 private:
  int offset;
  int myTimerId;
  QPen pen;
  const int * offsets;

  unsigned char emotivBuffer[32];
  void decrypt(unsigned char *in, unsigned char *out);
  AES_KEY decryptKey;
  int emotiv_fd;
  unsigned char * readBuffer();
  bool showAvg; 
  int node_data[14];

  QMessageBox * error_msg;
  int sockfd;
  int portno;
  struct sockaddr_in serv_addr;
  struct hostent *server;
  char message[32];
  void error(const char *);
};

#endif
