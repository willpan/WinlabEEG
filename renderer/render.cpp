
#include <QtGui>
#include "render.h"

Render::Render(QWidget *parent)
  : QWidget(parent)
{
  offset = 0;
  myTimerId = 0;
  filedescriptor = 0;
  setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
  setAutoFillBackground(TRUE);
  showAvg = FALSE;
  setup();
  pen.setWidth(1);
  pen.setColor(Qt::blue);
}

void Render::setup()
{
  unsigned char willKey[]="\x31\x00\x39\x54\x38\x10\x37"
    "\x42\x31\x00\x39\x48\x38\x00\x37\x50";
  setKey(willKey);
  printf("Initializing vectors\n");
  //***********************New Stuff*********************//

  
  int sensorMask_F3[]  = {10, 11, 12, 13, 14, 15, 0, 1, 2, 3, 4, 5, 6, 7}; 
  int sensorMask_FC6[] = {214, 215, 200, 201, 202, 203, 204, 205, 206, 207, 192, 193, 194, 195}; 
  int sensorMask_P7[]  = {84, 85, 86, 87, 72, 73, 74, 75, 76, 77, 78, 79, 64, 65};
  int sensorMask_T8[]  = {160, 161, 162, 163, 164, 165, 166, 167, 152, 153, 154, 155, 156, 157};
  int sensorMask_F7[]  = {48, 49, 50, 51, 52, 53, 54, 55, 40, 41, 42, 43, 44, 45};
  int sensorMask_F8[]  = {178, 179, 180, 181, 182, 183, 168, 169, 170, 171, 172, 173, 174, 175};
  int sensorMask_T7[]  = {66, 67, 68, 69, 70, 71, 56, 57, 58, 59, 60, 61, 62, 63};
  int sensorMask_P8[]  = {158, 159, 144, 145, 146, 147, 148, 149, 150, 151, 136, 137, 138, 139};
  int sensorMask_AF4[] = {196, 197, 198, 199, 184, 185, 186, 187, 188, 189, 190, 191, 176, 177};
  int sensorMask_F4[]  = {216, 217, 218, 219, 220, 221, 222, 223, 208, 209, 210, 211, 212, 213};
  int sensorMask_AF3[] = {46, 47, 32, 33, 34, 35, 36, 37, 38, 39, 24, 25, 26, 27};
  int sensorMask_O2[]  = {140, 141, 142, 143, 128, 129, 130, 131, 132, 133, 134, 135, 120, 121};
  int sensorMask_O1[]  = {102, 103, 88, 89, 90, 91, 92, 93, 94, 95, 80, 81, 82, 83};
  int sensorMask_FC5[] = {28, 29, 30, 31, 16, 17, 18, 19, 20, 21, 22, 23, 8, 9};

  vector<int> vector_F3(sensorMask_F3, sensorMask_F3 + sizeof(sensorMask_F3)/sizeof(int));
  vector<int> vector_FC6(sensorMask_FC6, sensorMask_FC6 + sizeof(sensorMask_FC6)/sizeof(int));
  vector<int> vector_P7(sensorMask_P7, sensorMask_P7 + sizeof(sensorMask_P7)/sizeof(int));
  vector<int> vector_T8(sensorMask_T8, sensorMask_T8 + sizeof(sensorMask_T8)/sizeof(int));
  vector<int> vector_F7(sensorMask_F7, sensorMask_F7 + sizeof(sensorMask_F7)/sizeof(int));
  vector<int> vector_F8(sensorMask_F8, sensorMask_F8 + sizeof(sensorMask_F8)/sizeof(int));
  vector<int> vector_T7(sensorMask_T7, sensorMask_T7 + sizeof(sensorMask_T7)/sizeof(int));
  vector<int> vector_P8(sensorMask_P8, sensorMask_P8 + sizeof(sensorMask_P8)/sizeof(int));
  vector<int> vector_AF4(sensorMask_AF4, sensorMask_AF4 + sizeof(sensorMask_AF4)/sizeof(int));
  vector<int> vector_F4(sensorMask_F4, sensorMask_F4 + sizeof(sensorMask_F4)/sizeof(int));
  vector<int> vector_AF3(sensorMask_AF3, sensorMask_AF3 + sizeof(sensorMask_AF3)/sizeof(int));
  vector<int> vector_O2(sensorMask_O2, sensorMask_O2 + sizeof(sensorMask_O2)/sizeof(int));
  vector<int> vector_O1(sensorMask_O1, sensorMask_O1 + sizeof(sensorMask_O1)/sizeof(int));
  vector<int> vector_FC5(sensorMask_FC5, sensorMask_FC5 + sizeof(sensorMask_FC5)/sizeof(int));


  SensorBits["F3"] = vector_F3;
  SensorBits["FC6"] = vector_FC6;
  SensorBits["P7"] = vector_P7;
  SensorBits["T8"] = vector_T8;
  SensorBits["F7"] = vector_F7;
  SensorBits["F8"] = vector_F8;
  SensorBits["T7"] = vector_T7;
  SensorBits["P8"] = vector_P8;
  SensorBits["AF4"] = vector_AF4;
  SensorBits["F4"] = vector_F4;
  SensorBits["AF3"] = vector_AF3;
  SensorBits["O2"] = vector_O2;
  SensorBits["O1"] = vector_O1;
  SensorBits["FC5"] = vector_FC5;
 
  /*
  nodeData["F3"] = 0;
  nodeData["FC6"] = 0;
  nodeData["P7"] = 0;
  nodeData["T8"] = 0;
  nodeData["F7"] = 0;
  nodeData["F8"] = 0;
  nodeData["T7"] = 0;
  nodeData["P8"] = 0;
  nodeData["AF4"] = 0;
  nodeData["F4"] = 0;
  nodeData["AF3"] = 0;
  nodeData["O2"] = 0;
  nodeData["O1"] = 0;
  nodeData["FC5"] = 0;
  */

  for(int i = 0; i < 14; ++i)
    {
      nodeData.push_back(0);
    }


  for(int x = 0; x < 100; ++x)
    {
      Buffer0.append(0);
      Buffer1.append(0);
      Buffer2.append(0);
      Buffer3.append(0);
      Buffer4.append(0);
      Buffer5.append(0);
      Buffer6.append(0);
      Buffer7.append(0);
    }
  printf("Starting timer\n");
  myTimerId = startTimer(waitTime);
  printf("Exiting setup\n");
}

QSize Render::sizeHint() const
{
  return QSize(600,560);
}

void Render::paintEvent(QPaintEvent * /* event */)
{
  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);
  if( offset % rectwidth == 0)
    {
      painter.setPen(Qt::black);
      painter.drawLine(width()-1,0,width()-1,height());
      offset = 0;
    } 
  painter.setPen(pen);
 
  
  Buffer0.pop_front();
  Buffer1.pop_front();
  Buffer2.pop_front();
  Buffer3.pop_front();
  Buffer4.pop_front();
  Buffer5.pop_front();
  Buffer6.pop_front();
  Buffer7.pop_front();
  
  //Buffer8.pop_front();
  //Buffer9.pop_front();
  //Buffer10.pop_front();
  //Buffer11.pop_front();
	
 if(filedescriptor != 0)
    {
      readBuffer();
      
      Buffer0.append(nodeData[0]);
      Buffer1.append(nodeData[1]);
      Buffer2.append(nodeData[2]);
      Buffer3.append(nodeData[3]);
      Buffer4.append(nodeData[4]);
      Buffer5.append(nodeData[5]);
      Buffer6.append(nodeData[6]);
      Buffer7.append(nodeData[7]);
    }
 else
   {

     Buffer0.append(0);
     Buffer1.append(0);
     Buffer2.append(0);
     Buffer3.append(0);
     Buffer4.append(0);
     Buffer5.append(0);
     Buffer6.append(0);
     Buffer7.append(0);
   }
 
  painter.drawLine(width()-scrollInt,3+Buffer0[98]/100,
		   width(),3+Buffer0[99]/100);
  painter.drawLine(width()-scrollInt,73+Buffer1[98]/100,
		   width(),73+Buffer1[99]/100);
  painter.drawLine(width()-scrollInt,143+Buffer2[98]/100,
		   width(),143+Buffer2[99]/100);
  painter.drawLine(width()-scrollInt,213+Buffer3[98]/100,
		   width(),213+Buffer3[99]/100);
  painter.drawLine(width()-scrollInt,283+Buffer4[98]/100,
		   width(),283+Buffer4[99]/100);
  painter.drawLine(width()-scrollInt,353+Buffer5[98]/100,
		   width(),353+Buffer5[99]/100);
  painter.drawLine(width()-scrollInt,423+Buffer6[98]/100,
		   width(),423+Buffer6[99]/100);
  painter.drawLine(width()-scrollInt,493+Buffer7[98]/100,
		   width(),493+Buffer7[99]/100);

  printf("%d  %d  %d  %d  %d  %d  %d  %d\n ",Buffer0[98],Buffer1[98],Buffer2[98],Buffer3[98],Buffer4[98],Buffer5[98],Buffer6[98],Buffer7[98]);
  

  if(showAvg)
    {
      painter.setPen(Qt::red);
      Avg0.append((Buffer0[99]+Buffer0[98]+Buffer0[97]+Buffer0[96])/4);
      Avg1.append((Buffer1[99]+Buffer1[98]+Buffer1[97]+Buffer1[96])/4);
      Avg2.append((Buffer2[99]+Buffer2[98]+Buffer2[97]+Buffer2[96])/4);
      Avg3.append((Buffer3[99]+Buffer3[98]+Buffer3[97]+Buffer3[96])/4);
      Avg4.append((Buffer4[99]+Buffer4[98]+Buffer4[97]+Buffer4[96])/4);
      painter.drawLine(width()-scrollInt,Avg0[0]/4,
		       width(),Avg0[1]/4);
      painter.drawLine(width()-scrollInt,73+Avg1[0]/4,
		       width(),73+Avg1[1]/4);
      painter.drawLine(width()-scrollInt,143+Avg2[0]/4,
		       width(),143+Avg2[1]/4);
      painter.drawLine(width()-scrollInt,213+Avg3[0]/4,
		       width(),213+Avg3[1]/4);
      painter.drawLine(width()-scrollInt,283+Avg4[0]/4,
		       width(),283+Avg4[1]/4);
      Avg0.pop_front();
      Avg1.pop_front();
      Avg2.pop_front();
      Avg3.pop_front();
      Avg4.pop_front();
    }
}



void Render::timerEvent(QTimerEvent *event)
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

bool Render::connectEmotiv()
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
	
  filedescriptor = open(udev_device_get_devnode(dev), O_RDONLY);
  if (filedescriptor > 0) 
    {
      res = read(filedescriptor, emotivBuffer, 32);
      if(res < 0) {perror("read"); return false;}
      return true;
    }
  else 
    {
      perror("Unable to open device");
      return false;
    }
}

void Render::setKey(unsigned char * keyinput)
{
  AES_set_decrypt_key( keyinput , 128 , &decryptKey );
}

void Render::decrypt(unsigned char *in, unsigned char *out)
{
  AES_ecb_encrypt(in, out, &decryptKey, AES_DECRYPT);
  AES_ecb_encrypt(&in[AES_BLOCK_SIZE], &out[AES_BLOCK_SIZE], &decryptKey, AES_DECRYPT);
}

unsigned char * Render::readBuffer()
{
  res = read( filedescriptor , emotivBuffer , 32);
  static unsigned char data[32];
  decrypt(emotivBuffer,data);
  int n = 0;
  for( map< string, vector<int> >::iterator ii = SensorBits.begin(); ii != SensorBits.end(); ++ii)
  {
    int level = 0;
    for(int i = 13; i > -1; --i)
      {
	level <<= 1;
	int b = (((*ii).second)[i] / 8 + 1), o = (((*ii).second)[i] % 8);
	level |= ((data[b] >> o) & 1);
      } 
    nodeData[n] = level;
    ++n;
  }
  return 0;
}

void Render::Pause()
{
  killTimer(myTimerId);
  myTimerId = 0;
}

void Render::Play()
{
  myTimerId = startTimer(waitTime);
}


void Render::setShowAvg()
{
  showAvg = !showAvg;
  if(showAvg)
    {
      Avg0.append((Buffer0[99]+Buffer0[98]+Buffer0[97]+Buffer0[96])/4);
      Avg1.append((Buffer1[99]+Buffer1[98]+Buffer1[97]+Buffer1[96])/4);
      Avg2.append((Buffer2[99]+Buffer2[98]+Buffer2[97]+Buffer2[96])/4);
      Avg3.append((Buffer3[99]+Buffer3[98]+Buffer3[97]+Buffer3[96])/4);
      Avg4.append((Buffer4[99]+Buffer4[98]+Buffer4[97]+Buffer4[96])/4);
    }
}

