#include "pci.h"
#include "pci_id.h"
#include "display.h"

void PCIDeviceLoadSpeed(const char *Path, TDevice *dev)
{
    char *Tempstr=NULL, *Token=NULL;
    const char *ptr;
    double val;

    Tempstr=ReadFile(Tempstr, Path, "current_link_speed");
    ptr=GetToken(Tempstr, "\\S", &Token, 0);
    val=atof(Token);
    ptr=GetToken(ptr, "\\S", &Token, 0);
    if (strcmp(Token, "GT/s")==0) val *= 1024 * 1024 * 1024;
    if (strcmp(Token, "MT/s")==0) val *= 1024 * 1024;
    if (strcmp(Token, "kT/s")==0) val *= 1024;
    dev->Speed=(uint64_t) val;

    Destroy(Tempstr);
    Destroy(Token);

}



TDevice *PCIDeviceLoad(const char *Path, ListNode *PCIids)
{
    TDevice *dev;
    char *Tempstr=NULL, *Key=NULL;
    glob_t Glob;
    const char *ptr;

    dev=(TDevice *) calloc(1, sizeof(TDevice));
    dev->BusType=CopyStr(dev->BusType, "pci");
    dev->Address=CopyStr(dev->Address, GetBasename(Path));
    dev->idProduct=ReadFile(dev->idProduct, Path, "device");
    dev->idVendor=ReadFile(dev->idVendor, Path, "vendor");
    dev->idClass=ReadFile(dev->idClass, Path, "class");
    dev->Serial=ReadFile(dev->Serial, Path, "serial");
    dev->BusNum=ReadIntegerFile(Path, "busnum");
    dev->DevNum=ReadIntegerFile(Path, "devnum");
    dev->Serial=ReadFile(dev->Serial, Path, "serial");

    PCIDeviceLoadSpeed(Path, dev);

    Key=FormatStr(Key, "P%04x%04x", strtol(dev->idVendor, NULL, 16), strtol(dev->idProduct, NULL, 16));
    ptr=GetVar(PCIids, Key);

    if (! StrValid(ptr))
    {
        Tempstr=FormatStr(Tempstr, "P%04x%04x", strtol(dev->idVendor, NULL, 16), strtol(dev->idProduct, NULL, 16));
        Key=CopyStrLen(Key, Tempstr, 7);
        ptr=GetVar(PCIids, Key);
    }
    dev->Description=CopyStr(dev->Description, ptr);

    Tempstr=FormatStr(Tempstr, "C%06x", strtol(dev->idClass, NULL, 16));
    dev->Class=CopyStr(dev->Class, Tempstr);

    ptr=GetVar(PCIids, Tempstr);

    if (! StrValid(ptr))
    {
        Key=CopyStrLen(Key, Tempstr, 5);
        ptr=GetVar(PCIids, Key);

        if (! StrValid(ptr))
        {
            Key=CopyStrLen(Key, Tempstr, 3);
            ptr=GetVar(PCIids, Key);
        }
    }
    dev->Class=CopyStr(dev->Class, ptr);

    LookupAnyDevNode(dev, Path);

    Key=FormatStr(Key, "V%x", strtol(dev->idVendor, NULL, 16));
    dev->Vendor=CopyStr(dev->Vendor, GetVar(PCIids, Key));

    Destroy(Tempstr);
    Destroy(Key);

    return(dev);
}





void PCIShow(int Flags)
{
    glob_t Glob;
    char *Tempstr=NULL;
    ListNode *PCIids;
    TDevice *dev;
    int i;

    if (StrValid(PciIDsFile)) PCIids=PCILoadIDFile(PciIDsFile);
    else PCIids=PCILoadIDs(PciIDsDirs, "pci.ids.gz:pci.ids.bz2:pci.ids");

    glob("/sys/bus/pci/devices/*", 0, 0, &Glob);

    for (i=0; i < Glob.gl_pathc; i++)
    {
        dev=PCIDeviceLoad(Glob.gl_pathv[i], PCIids);
        DeviceDisplay(Flags, dev);
        TDeviceDestroy(dev);
    }

    Destroy(Tempstr);

}
