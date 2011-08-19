
#include <QtGui>
#include "reader.h"

Reader::Reader(QWidget *parent)
  : QWidget(parent)
{
  emotiv_fd = 0;
  gyro_x = 0;
  gyro_y = 0;
  memset(node_data, 0, sizeof(node_data));
  const char * sensor_names[] = {"F3", "FC6", "P7", "T8", "F7", "F8", "T7",
			       "P8", "AF4", "F4", "AF3", "O2", "O1", "FC5"};
  for(int i = 0; i < 14; i++)
    sensor_bits[sensor_names[i]] = 0;

  map< string, int * >::iterator it;
  for( it = sensor_bits.begin(); it != sensor_bits.end(); it++)
    {
      cout << (*it).first << "\t" ;
    }
  cout << "Gyro X\tGyro Y\t" << endl;

  print = true;
  
 }


bool Reader::connect_emotiv()
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
      int res = read(emotiv_fd, emotiv_buffer, 32);
      if(res < 0) {perror("read"); return false;}
      return true;
    }
  else 
    {
      perror("Unable to open device");
      return false;
    }
}

void Reader::set_key(uchar * keyinput)
{
  AES_set_decrypt_key( keyinput , 128 , &decryptKey );
}

void Reader::decrypt(uchar *in, uchar *out)
{
  AES_ecb_encrypt(in, out, &decryptKey, AES_DECRYPT);
  AES_ecb_encrypt(&in[AES_BLOCK_SIZE], &out[AES_BLOCK_SIZE], &decryptKey, AES_DECRYPT);
}

void Reader::read_buffer()
{
  
  static int sensorMask_F3[]  = {10, 11, 12, 13, 14, 15,
				 0, 1, 2, 3, 4, 5, 6, 7}; 
  static int sensorMask_FC6[] = {214, 215, 200, 201, 202, 203, 204, 
				 205, 206, 207, 192, 193, 194, 195}; 
  static int sensorMask_P7[]  = {84, 85, 86, 87, 72, 73, 74, 
				 75, 76, 77, 78, 79, 64, 65};
  static int sensorMask_T8[]  = {160, 161, 162, 163, 164, 165, 166, 
				 167, 152, 153, 154, 155, 156, 157};
  static int sensorMask_F7[]  = {48, 49, 50, 51, 52, 53, 54, 
				 55, 40, 41, 42, 43, 44, 45};
  static int sensorMask_F8[]  = {178, 179, 180, 181, 182, 183, 168,
				 169, 170, 171, 172, 173, 174, 175};
  static int sensorMask_T7[]  = {66, 67, 68, 69, 70, 71, 56,
				 57, 58, 59, 60, 61, 62, 63};
  static int sensorMask_P8[]  = {158, 159, 144, 145, 146, 147, 148, 
				 149, 150, 151, 136, 137, 138, 139};
  static int sensorMask_AF4[] = {196, 197, 198, 199, 184, 185, 186, 
				 187, 188, 189, 190, 191, 176, 177};
  static int sensorMask_F4[]  = {216, 217, 218, 219, 220, 221, 222,
				 223, 208, 209, 210, 211, 212, 213};
  static int sensorMask_AF3[] = {46, 47, 32, 33, 34, 35, 36, 
				 37, 38, 39, 24, 25, 26, 27};
  static int sensorMask_O2[]  = {140, 141, 142, 143, 128, 129, 130,
				 131, 132, 133, 134, 135, 120, 121};
  static int sensorMask_O1[]  = {102, 103, 88, 89, 90, 91, 92, 
				 93 , 94 , 95, 80, 81, 82, 83};
  static int sensorMask_FC5[] = {28, 29, 30, 31, 16, 17, 18,
				 19, 20, 21, 22, 23, 8, 9};
  if(sensor_bits["F3"]==0)
    {
      sensor_bits["F3"] = sensorMask_F3;
      sensor_bits["FC6"] = sensorMask_FC6;
      sensor_bits["P7"] = sensorMask_P7;
      sensor_bits["T8"] = sensorMask_T8;
      sensor_bits["F7"] = sensorMask_F7;
      sensor_bits["F8"] = sensorMask_F8;
      sensor_bits["T7"] = sensorMask_T7;
      sensor_bits["P8"] = sensorMask_P8;
      sensor_bits["AF4"] = sensorMask_AF4;
      sensor_bits["F4"] = sensorMask_F4;
      sensor_bits["AF3"] = sensorMask_AF3;
      sensor_bits["O2"] = sensorMask_O2;
      sensor_bits["O1"] = sensorMask_O1;
      sensor_bits["FC5"] = sensorMask_FC5;
    }

  static uchar data[32];
  read( emotiv_fd , emotiv_buffer , 32);
  decrypt(emotiv_buffer, data);
  gyro_x = data[29] - offsets[14];
  gyro_y = offsets[15] - data[30];
  
  int j = 0;
  map< string, int *>::iterator it;
  for( it = sensor_bits.begin(); 
       it != sensor_bits.end(); ++it)
    {
      int level = 0;
      for(int i = 13; i > -1; --i)
	{
	  level <<= 1;
	  int b = ((*it).second[i] / 8 + 1);
	  int o = ((*it).second[i] % 8);
	  level |= ((data[b] >> o) & 1);
	} 
      node_data[j] = level;
      j++;
      if(print)
	cout << level << "\t";
    }
  if(print)
    cout << gyro_x << "\t" << gyro_y << endl;
}

void Reader::set_key_Kat()
{
  static uchar katKey[]="\x4D\x00\x47\x54\x38\x10\x38\x42"
    "\x4D\x00\x47\x48\x38\x00\x38\x50";
  set_key(katKey);
  
  static int Kat_offsets[] = {
    8120, 7950, 7850, 7880, 8750, 8110, 8590, 
    8240, 8960, 8470, 8700, 8260, 8640, 7445,
    104, 103
  };
  offsets = Kat_offsets;
  
}

void Reader::set_key_Will()
{
  static uchar willKey[]="\x31\x00\x39\x54\x38\x10\x37"
    "\x42\x31\x00\x39\x48\x38\x00\x37\x50";
  set_key(willKey);
  
  static int Will_offsets[] = {
    7760, 8530, 8000, 8510, 8900, 8400, 8320,
    8720, 7230, 8820, 7700, 8050, 9080, 8650,
    104, 105
  };
  offsets = Will_offsets;
  
}

void Reader::toggle_print()
{
  print = !print;
}
