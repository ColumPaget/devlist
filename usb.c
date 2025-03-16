#include "common.h"
#include <sys/ioctl.h>
#include <linux/usbdevice_fs.h>
#include "display.h"

const char *USBLookupClass(int Class)
{
    switch (Class)
    {
    case 0x0:
        return(NULL);
        break;
    case 0x1:
        return("Audio");
        break;
    case 0x2:
        return("Communications");
        break;
    case 0x3:
        return("Human Interface");
        break;
    case 0x5:
        return("Physical");
        break;
    case 0x6:
        return("Image");
        break;
    case 0x7:
        return("Printer");
        break;
    case 0x8:
        return("Mass Storage");
        break;
    case 0x9:
        return("USB Hub");
        break;
    case 0xA:
        return("CDC Data");
        break;
    case 0xB:
        return("Smart Card");
        break;
    case 0xD:
        return("Content Security");
        break;
    case 0xE:
        return("Video");
        break;
    case 0xF:
        return("Health");
        break;
    case 0x10:
        return("Audio/Video");
        break;
    case 0x11:
        return("Billboard");
        break;
    case 0x12:
        return("USB-C Bridge");
        break;
    case 0x13:
        return("Bulk Display");
        break;
    case 0x14:
        return("MCTP over USB");
        break;
    case 0x3C:
        return("I3C");
        break;
    case 0xDC:
        return("Diagnostics");
        break;
    case 0xE0:
        return("WiFi");
        break;
    case 0xEF:
        return("misc");
        break;
    }

    return(NULL);
}





int USBReset(TDevice *dev)
{
    /* Also resets?
    sudo sh -c "echo 0 > /sys/bus/usb/devices/1-4.6/authorized"
    sudo sh -c "echo 1 > /sys/bus/usb/devices/1-4.6/authorized"
    */



    char *Path=NULL;
    int fd, result, RetVal=FALSE;

    Path=FormatStr(Path, "/dev/usb/%03d/%03d", dev->BusNum, dev->DevNum);
    fd=open(Path, O_WRONLY);
    if (fd > -1)
    {
        result = ioctl(fd, USBDEVFS_RESET, 0);
        if (result==0) RetVal=TRUE;
        else perror("Error resetting device");
        close(fd);
    }

    Destroy(Path);
    return(RetVal);
}


TDevice *USBDeviceLoad(const char *Path)
{
    TDevice *dev;
    char *Tempstr=NULL;
    int val;

    dev=(TDevice *) calloc(1, sizeof(TDevice));
    dev->Description=ReadFile(dev->Description, Path, "product");
    dev->Vendor=ReadFile(dev->Vendor, Path, "manufacturer");
    dev->idProduct=ReadFile(dev->idProduct, Path, "idProduct");
    dev->idVendor=ReadFile(dev->idVendor, Path, "idVendor");
    Tempstr=ReadFile(Tempstr, Path, "version");
    dev->BusType=MCopyStr(dev->BusType, "usb", Tempstr, NULL);
    dev->Serial=ReadFile(dev->Serial, Path, "serial");
    dev->Speed=ReadIntegerFile(Path, "speed") * 1024 * 1024;
    dev->BusNum=ReadIntegerFile(Path, "busnum");
    dev->DevNum=ReadIntegerFile(Path, "devnum");
    dev->PowerMax=ReadIntegerFile(Path, "bMaxPower");
    dev->Address=FormatStr(dev->Address, "/%03d/%03d", dev->BusNum, dev->DevNum);

    dev->idClass=ReadFile(dev->idClass, Path, "bDeviceClass");
    val=strtol(dev->idClass, NULL, 16);

    if (val == 0)
    {
        dev->idClass=ReadFile(dev->idClass, Path, "bInterfaceClass");
        val=strtol(dev->idClass, NULL, 16);
    }

    if (val==0)
    {
        Tempstr=MCopyStr(Tempstr, "/", GetBasename(Path), ":1.0/bInterfaceClass", NULL);
        dev->idClass=ReadFile(dev->idClass, Path, Tempstr);
        val=strtol(dev->idClass, NULL, 16);
    }

    dev->Class=CopyStr(dev->Class, USBLookupClass(val));
    LookupAnyDevNode(dev, Path);
		if (! StrValid(dev->DevNode))
		{
			Tempstr=MCopyStr(Tempstr, Path, "/*/", NULL);
    	LookupAnyDevNode(dev, Tempstr);
		}

    Destroy(Tempstr);
    return(dev);
}


ListNode *USBGetList(int Flags)
{
    glob_t Glob;
    char *Tempstr=NULL;
    TDevice *dev;
    int i;

    glob("/sys/bus/usb/devices/[0-9]-*", 0, 0, &Glob);

    for (i=0; i < Glob.gl_pathc; i++)
    {
        dev=USBDeviceLoad(Glob.gl_pathv[i]);
        if (StrLen(dev->Description)) DeviceDisplay(Flags, dev);
        TDeviceDestroy(dev);
    }

    Destroy(Tempstr);
}


