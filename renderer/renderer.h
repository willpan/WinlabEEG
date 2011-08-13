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
#define scrollInt 2
#define WAIT_TIME 5
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
  map< string, deque<int> > Buffer;
  map <string, deque<int> > SensorBits;
  void setKey(unsigned char *);
  bool playing;
  int server_connected;
  int * renderer_timer_id;
  int gyro_x;
  int gyro_y;

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
  void accept_options(int port_number , const char *host_name);
  void print_value_on_off();
  
 private:
  map< string, deque<int> >::iterator it;
  int offset;
  QPen pen;
  const int * offsets;
  bool print_values;

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
