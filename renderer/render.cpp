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
      
      //Buffer8.append(temp[8]);
      //Buffer9.append(temp[9]);
      //Buffer10.append(temp[10]);
      //Buffer11.append(temp[11]);
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
      unsigned char * temp = readBuffer();
  
      Buffer0.append(temp[0]);
      Buffer1.append(temp[1]);
      Buffer2.append(temp[2]);
      Buffer3.append(temp[3]);
      Buffer4.append(temp[4]);
      Buffer5.append(temp[5]);
      Buffer6.append(temp[6]);
      Buffer7.append(temp[7]);
      //Buffer8.append(temp[8]);
      //Buffer9.append(temp[9]);
      //Buffer10.append(temp[10]);
      //Buffer11.append(temp[11]);
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
     //Buffer8.append(temp[8]);
     //Buffer9.append(temp[9]);
     //Buffer10.append(temp[10]);
     //Buffer11.append(temp[11]);
   }
 
  painter.drawLine(width()-scrollInt,3+Buffer0[98]/4,
		   width(),3+Buffer0[99]/4);
  painter.drawLine(width()-scrollInt,73+Buffer1[98]/4,
		   width(),73+Buffer1[99]/4);
  painter.drawLine(width()-scrollInt,143+Buffer2[98]/4,
		   width(),143+Buffer2[99]/4);
  painter.drawLine(width()-scrollInt,213+Buffer3[98]/4,
		   width(),213+Buffer3[99]/4);
  painter.drawLine(width()-scrollInt,283+Buffer4[98]/4,
		   width(),283+Buffer4[99]/4);
  painter.drawLine(width()-scrollInt,353+Buffer5[98]/4,
		   width(),353+Buffer5[99]/4);
  painter.drawLine(width()-scrollInt,423+Buffer6[98]/4,
		   width(),423+Buffer6[99]/4);
  painter.drawLine(width()-scrollInt,493+Buffer7[98]/4,
		   width(),493+Buffer7[99]/4);
  /*
  painter.drawLine(width()-scrollInt,563+Buffer8[98]/4,
		   width(),563+Buffer8[99]/4);
  painter.drawLine(width()-scrollInt,633+Buffer9[98]/4,
		   width(),633+Buffer9[99]/4);
  painter.drawLine(width()-scrollInt,703+Buffer10[98]/4,
		   width(),703+Buffer10[99]/4);
  painter.drawLine(width()-scrollInt,773+Buffer11[98]/4,
		   width(),773+Buffer11[99]/4);
  */
  if(showAvg)
    {
      painter.setPen(Qt::red);
      printf("ok?\n");
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
      printf("still ok?\n");
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
  static unsigned char temp[32];
  decrypt(emotivBuffer,temp);
  return temp;
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
