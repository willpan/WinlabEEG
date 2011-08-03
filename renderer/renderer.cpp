
#include <QtGui>
#include "renderer.h"

Renderer::Renderer(QWidget *parent)
  : QWidget(parent)
{
  offset = 0;
  myTimerId = 0;
  emotiv_fd = 0;
  setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
  setAutoFillBackground(TRUE);
  showAvg = FALSE;
  setup();
  server_connected = 0;
  pen.setWidth(1);
  pen.setColor(Qt::blue);
  playing = FALSE;
 }

void Renderer::setup()
{
  setKey_Will();
  printf("Initializing vectors\n");
  //***********************New Stuff*********************//

  
  int sensorMask_F3[]  = {10, 11, 12, 13, 14, 15,
			  0, 1, 2, 3, 4, 5, 6, 7}; 
  int sensorMask_FC6[] = {214, 215, 200, 201, 202, 203, 204, 
			  205, 206, 207, 192, 193, 194, 195}; 
  int sensorMask_P7[]  = {84, 85, 86, 87, 72, 73, 74, 
			  75, 76, 77, 78, 79, 64, 65};
  int sensorMask_T8[]  = {160, 161, 162, 163, 164, 165, 166, 
			  167, 152, 153, 154, 155, 156, 157};
  int sensorMask_F7[]  = {48, 49, 50, 51, 52, 53, 54, 
			  55, 40, 41, 42, 43, 44, 45};
  int sensorMask_F8[]  = {178, 179, 180, 181, 182, 183, 168,
			  169, 170, 171, 172, 173, 174, 175};
  int sensorMask_T7[]  = {66, 67, 68, 69, 70, 71, 56,
			  57, 58, 59, 60, 61, 62, 63};
  int sensorMask_P8[]  = {158, 159, 144, 145, 146, 147, 148, 
			  149, 150, 151, 136, 137, 138, 139};
  int sensorMask_AF4[] = {196, 197, 198, 199, 184, 185, 186, 
			  187, 188, 189, 190, 191, 176, 177};
  int sensorMask_F4[]  = {216, 217, 218, 219, 220, 221, 222,
			  223, 208, 209, 210, 211, 212, 213};
  int sensorMask_AF3[] = {46, 47, 32, 33, 34, 35, 36, 
			  37, 38, 39, 24, 25, 26, 27};
  int sensorMask_O2[]  = {140, 141, 142, 143, 128, 129, 130,
			  131, 132, 133, 134, 135, 120, 121};
  int sensorMask_O1[]  = {102, 103, 88, 89, 90, 91, 92, 
			  93 , 94 , 95, 80, 81, 82, 83};
  int sensorMask_FC5[] = {28, 29, 30, 31, 16, 17, 18,
			  19, 20, 21, 22, 23, 8, 9};

  deque<int> deque_F3(sensorMask_F3, 
		      sensorMask_F3 + sizeof(sensorMask_F3)/sizeof(int));
  deque<int> deque_FC6(sensorMask_FC6, 
		       sensorMask_FC6 + sizeof(sensorMask_FC6)/sizeof(int));
  deque<int> deque_P7(sensorMask_P7, 
		      sensorMask_P7 + sizeof(sensorMask_P7)/sizeof(int));
  deque<int> deque_T8(sensorMask_T8, 
		      sensorMask_T8 + sizeof(sensorMask_T8)/sizeof(int));
  deque<int> deque_F7(sensorMask_F7, 
		      sensorMask_F7 + sizeof(sensorMask_F7)/sizeof(int));
  deque<int> deque_F8(sensorMask_F8, 
		      sensorMask_F8 + sizeof(sensorMask_F8)/sizeof(int));
  deque<int> deque_T7(sensorMask_T7, 
		      sensorMask_T7 + sizeof(sensorMask_T7)/sizeof(int));
  deque<int> deque_P8(sensorMask_P8, 
		      sensorMask_P8 + sizeof(sensorMask_P8)/sizeof(int));
  deque<int> deque_AF4(sensorMask_AF4, 
		       sensorMask_AF4 + sizeof(sensorMask_AF4)/sizeof(int));
  deque<int> deque_F4(sensorMask_F4, 
		      sensorMask_F4 + sizeof(sensorMask_F4)/sizeof(int));
  deque<int> deque_AF3(sensorMask_AF3, 
		       sensorMask_AF3 + sizeof(sensorMask_AF3)/sizeof(int));
  deque<int> deque_O2(sensorMask_O2, 
		      sensorMask_O2 + sizeof(sensorMask_O2)/sizeof(int));
  deque<int> deque_O1(sensorMask_O1, 
		      sensorMask_O1 + sizeof(sensorMask_O1)/sizeof(int));
  deque<int> deque_FC5(sensorMask_FC5, 
		       sensorMask_FC5 + sizeof(sensorMask_FC5)/sizeof(int));


  SensorBits["F3"] = deque_F3;
  SensorBits["FC6"] = deque_FC6;
  SensorBits["P7"] = deque_P7;
  SensorBits["T8"] = deque_T8;
  SensorBits["F7"] = deque_F7;
  SensorBits["F8"] = deque_F8;
  SensorBits["T7"] = deque_T7;
  SensorBits["P8"] = deque_P8;
  SensorBits["AF4"] = deque_AF4;
  SensorBits["F4"] = deque_F4;
  SensorBits["AF3"] = deque_AF3;
  SensorBits["O2"] = deque_O2;
  SensorBits["O1"] = deque_O1;
  SensorBits["FC5"] = deque_FC5;

  memset(node_data, 0, sizeof(node_data));

  for(int i = 0; i < 14; ++i)
    {
      Buffer[i] = new deque<int>(100,0);
    }

  for( map< string, deque<int> >::iterator ii = SensorBits.begin(); 
       ii != SensorBits.end(); ++ii)
    {
      cout << (*ii).first << "\t" ;
    }
  puts("");
 

}

QSize Renderer::sizeHint() const
{
  return QSize(600,540);
}

void Renderer::paintEvent(QPaintEvent * /* event */)
{
  static const QColor colors[12] = { 
    Qt::red, Qt::green, Qt::blue, Qt::cyan, Qt::magenta, Qt::yellow, Qt::darkRed,
    Qt::darkGreen, Qt::darkBlue, Qt::darkCyan, Qt::darkMagenta, Qt::darkYellow 
  };

  
  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);
  if( offset % rectwidth == 0)
    {
      painter.setPen(Qt::black);
      painter.drawLine(width()-1,0,width()-1,height());
      offset = 0;
    } 
  painter.setPen(pen);
 
  for(int i = 0; i < 14; ++i)
    {
      Buffer[i]->pop_front();
    }
  	
 if(emotiv_fd != 0)
    {
      readBuffer();
      for(int i = 0; i < 14; ++i)
	{
	  Buffer[i]->push_back(node_data[i]);
	}
    }
 else
   {
     for(int i = 0; i < 14; ++i)
       {
	 Buffer[i]->push_back(offsets[i]+200);
       }
   }
 
 for(int i = 0; i < 14; ++i)
   {
     painter.setPen(colors[i%12]);
     painter.drawLine(width()-scrollInt,i*35+(((*Buffer[i])[98]-offsets[i])/5),
		      width(),i*35+(((*Buffer[i])[99])-offsets[i])/5);
   }
}



void Renderer::timerEvent(QTimerEvent *event)
{
  if (event->timerId() == myTimerId)
    {
      ++offset;
      scroll(-scrollInt, 0);
    }
  else 
    {
      QWidget::timerEvent(event);
    }
}

bool Renderer::connectEmotiv()
{
   /* Opens Emotiv if Emotiv is only HID Device attached */
  struct udev_device *dev;
  struct udev *udev;
  struct udev_list_entry *devices, *dev_list_entry;
  struct udev_enumerate *enumerate;
  udev = udev_new();
  enumerate = udev_enumerate_new(udev);
  udev_enumerate_add_match_subsystem(enumerate, "hidraw");
  udev_enumerate_scan_devices(enumerate);
  devices = udev_enumerate_get_list_entry(enumerate);
  udev_list_entry_foreach(dev_list_entry, devices) 
    {
      const char * path = udev_list_entry_get_name(dev_list_entry);
      dev = udev_device_new_from_syspath(udev,path);
    }
  emotiv_fd = open(udev_device_get_devnode(dev), O_RDONLY);
  if (emotiv_fd > 0) 
    {
      int res = read(emotiv_fd, emotivBuffer, 32);
      if(res < 0) {perror("read"); return false;}
      return true;
    }
  else 
    {
      perror("Unable to open device");
      return false;
    }
}

void Renderer::setKey(unsigned char * keyinput)
{
  AES_set_decrypt_key( keyinput , 128 , &decryptKey );
}

void Renderer::decrypt(unsigned char *in, unsigned char *out)
{
  AES_ecb_encrypt(in, out, &decryptKey, AES_DECRYPT);
  AES_ecb_encrypt(&in[AES_BLOCK_SIZE], &out[AES_BLOCK_SIZE], &decryptKey, AES_DECRYPT);
}

unsigned char * Renderer::readBuffer()
{
  read( emotiv_fd , emotivBuffer , 32);
  static int n;
  static unsigned char data[32];

  if(server_connected==1)
    {
      n = write(sockfd, emotivBuffer, 32); 
      if (n < 0) 
	error("ERROR writing to socket");
      bzero(node_data,sizeof(node_data));
      n = read(sockfd, node_data, sizeof(node_data));
      if (n < 0) 
	error("ERROR reading from socket");
    }
  else
    {
      decrypt(emotivBuffer, data);
      int j = 0;
      for( map< string, deque<int> >::iterator ii = SensorBits.begin(); 
	   ii != SensorBits.end(); ++ii)
	{
	  int level = 0;
	  for(int i = 13; i > -1; --i)
	    {
	      level <<= 1;
	      int b = (((*ii).second)[i] / 8 + 1), o = (((*ii).second)[i] % 8);
	      level |= ((data[b] >> o) & 1);
	    } 
	  node_data[j] = level;
	  printf("%d\t",level);
  	  ++j;
	}
      puts("");
    }
  return 0;
}


void Renderer::play_pause()
{
  if(playing)
    {
      killTimer(myTimerId);
      myTimerId = 0;
    }
  else
    myTimerId = startTimer(WAIT_TIME);
  playing = !playing;
}


void Renderer::setShowAvg()
{
  showAvg = !showAvg;
}

void Renderer::setKey_Kat()
{
  static unsigned char katKey[]="\x4D\x00\x47\x54\x38\x10\x38\x42"
    "\x4D\x00\x47\x48\x38\x00\x38\x50";
  setKey(katKey);
  static const int Kat_offsets[] = {
    8220, 8050, 7950, 7980, 8850, 8210, 8690, 
    8340, 9060, 8570, 8800, 8360, 8740, 7545
  };
  offsets = Kat_offsets;
}

void Renderer::setKey_Will()
{
  static unsigned char willKey[]="\x31\x00\x39\x54\x38\x10\x37"
    "\x42\x31\x00\x39\x48\x38\x00\x37\x50";
  setKey(willKey);
  static const int Will_offsets[] = {
    7860, 8630, 8100, 8610, 9000, 8500, 8420,
    8820, 7330, 8920, 7800, 8150, 9180, 8750
  };
  offsets = Will_offsets;
}

int Renderer::connect_server()
{
  if(server_connected == 0)
    { 
      portno = 2000;
      sockfd = socket(AF_INET, SOCK_STREAM, 0);
      if (sockfd < 0) 
        {
	  printf("ERROR, cannot open socket\n");
	  return 1;
	}
      server = gethostbyname("localhost");//local host 192.168.207.89
      if (server == NULL) {
        printf("ERROR, no such host\n");
        return 1;
      }
      bzero((char *) &serv_addr, sizeof(serv_addr));
      serv_addr.sin_family = AF_INET;
      bcopy((char *)server->h_addr, 
	    (char *)&serv_addr.sin_addr.s_addr,
	    server->h_length);
      serv_addr.sin_port = htons(portno);
      if (::connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
	{
	  error("ERROR connecting");
	  return 1;
	}
      server_connected = 1; // Use Server to process data
    }
  else if(server_connected == 1)
    server_connected = 2; // connected but do not use server to process data
  else
    server_connected = 1; // connected, use server to process data
  return 0;
}


void Renderer::error(const char *msg)
{
  perror(msg);
}
//192.168.206.124
