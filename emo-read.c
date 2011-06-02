
/******************************************
 Reading USB Data from Emotiv 
 William Pan, Jordan Romvary, Katherine Liu
 May be used for any purpose.
******************************************/

#include "usb-read.h"
#include "emo-aes.h"

const char *bus_str(int bus);

int main(int argc, char **argv)
{
	int BLOCKS = 1e6, PRINT = 0;
	argc--; argv++;
	while (argc > 0) 
    {
        if ((*argv)[0] == '-') 
        {
            if (!strcmp(*argv, "-help")) { printf("Usage: AES [-trials N] [-print]"); exit(0); }
            else if (!strcmp(*argv, "-trials")) { argc--; argv++; BLOCKS = atoi(*argv); }
            else if (!strcmp(*argv, "-print")) { PRINT = 1; }
            else { fprintf(stderr, "Invalid program argument: %s", *argv); exit(0); }
        }

        argv++; 
        argc--;
	}

	int fd;
	//Open the Device with non-blocking reads. In real life,
	//   don't use a hard coded path; use libudev instead. 
	struct udev_device *dev;
	struct udev *udev;
	struct udev_list_entry *devices, *dev_list_entry;
	struct udev_enumerate *enumerate;
	
	udev = udev_new();

	enumerate = udev_enumerate_new(udev);
	udev_enumerate_add_match_subsystem(enumerate, "hidraw");
	udev_enumerate_scan_devices(enumerate);
	devices = udev_enumerate_get_list_entry(enumerate);

	udev_list_entry_foreach(dev_list_entry, devices) {
	const char * path = udev_list_entry_get_name(dev_list_entry);

	dev = udev_device_new_from_syspath(udev,path);
	}

	
	fd = open(udev_device_get_devnode(dev), O_RDONLY);
	
	if (fd > 0) {
		int i, res, desc_size = 0;
		char buf[32];
		struct hidraw_report_descriptor rpt_desc;
		struct hidraw_devinfo info;

		memset(&rpt_desc, 0, sizeof(rpt_desc));
		memset(&info, 0xff, sizeof(info));
		memset(buf, 0x0, sizeof(buf));
		
		// Get Raw Name 
		res = ioctl(fd, HIDIOCGRAWNAME(256), buf);
		if (res < 0)
			perror("HIDIOCGRAWNAME");
		else {
			printf("Raw Name: %s\n", buf);
		}

		// Get Physical Location 
		res = ioctl(fd, HIDIOCGRAWPHYS(256), buf);
		if (res < 0)
			perror("HIDIOCGRAWPHYS");
		else {
			printf("Raw Phys: %s\n", buf);
		}

		// Get Raw Info 
		res = ioctl(fd, HIDIOCGRAWINFO, &info);
		if (res < 0)
			perror("HIDIOCGRAWINFO");
		else {
			printf("Raw Info:\n");
			printf("\tbustype: %d (%s)\n", info.bustype, bus_str(info.bustype));
			printf("\tvendor: 0x%04hx\n", info.vendor);
			printf("\tproduct: 0x%04hx\n", info.product);
		}
	
		res = read(fd, buf, 32);
		uchar in[32];
		uchar out[32];
		int trial = 0;
		if(res < 0) {perror("read");}
		else {
			printf("read() read %d bytes: \n",res);
			while(1)
			{
				strncpy(in,buf,32);
				Decrypt32(in,out,PRINT);
				int i = 0;
				for(; i < 32 ; ++i)
				{
					printf("%02x ",out[i]);
				}
				printf("\n\n");
				res = read(fd, buf, 32);
			}

		}
		close(fd);
	}
	else {
		perror("Unable to open device");
	}
	
	return 0;
}

const char *
bus_str(int bus)
{
	switch(bus) {
	case BUS_USB:
		return "USB";
		break;
	case BUS_HIL:
		return "HIL";
		break;
	case BUS_BLUETOOTH:
		return "Bluetooth";
		break;
	case BUS_VIRTUAL:
		return "Virtual";
		break;
	default:
		return "Other";
		break;
	}
}
