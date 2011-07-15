#include "emotiv.h"

emotiv::emotiv()
{
	/* Opens Emotiv if Emotiv is only HID Device attached */
	key = 0;
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
		res = read(filedescriptor, buffer, 32);
		if(res < 0) {perror("read");}
	}
	else 
	{
		perror("Unable to open device");
	}
}

emotiv::emotiv(unsigned char * keyinput)
{
	key = keyinput;
	AES_set_decrypt_key(key, 128, &decryptKey);
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
		res = read(filedescriptor, buffer, 32);
		if(res < 0) {perror("read");}
	}
	else 
	{
		perror("Unable to open device");
	}
}

void emotiv::setKey(unsigned char * keyinput)
{
	key = keyinput;
	AES_set_decrypt_key(key, 128, &decryptKey);
}

void emotiv::decrypt(unsigned char * in, unsigned char * out)
{
	AES_ecb_encrypt(in, out, &decryptKey, AES_DECRYPT);
    AES_ecb_encrypt(&in[AES_BLOCK_SIZE], &out[AES_BLOCK_SIZE], &decryptKey, AES_DECRYPT);
}

unsigned char * emotiv::readBuffer()
{
	res = read( filedescriptor , buffer , 32);
	unsigned char temp[32];
	decrypt(buffer,temp);
	for(int i = 0; i < 32 ; ++i)
	{
		printf("%02x ",temp[i]);
	}
	printf("\n\n");
}

