#include "scsi.h"
#include "display.h"

TDevice *SCSIDeviceLoad(const char *Path)
{
    TDevice *dev;
    char *Tempstr=NULL, *Key=NULL;
    const char *ptr;
    glob_t Glob;
    int val;

    dev=(TDevice *) calloc(1, sizeof(TDevice));
    dev->BusType=CopyStr(dev->BusType, "scsi");
    dev->Address=CopyStr(dev->Address, GetBasename(Path));
    dev->Vendor=ReadFile(dev->Vendor, Path, "vendor");
    dev->Serial=ReadFile(dev->Serial, Path, "wwid");

    dev->idClass=ReadFile(dev->idClass, Path, "type");
    if (! dev->idClass) dev->idClass=CopyStr(dev->idClass, "");



    LookupAnyDevNode(dev, Path);
    Tempstr=MCopyStr(Tempstr, "/block/", dev->DevNode, "/size", NULL);
    dev->Size=ReadUint64File(Path, Tempstr) * 512;

    Tempstr=MCopyStr(Tempstr, "/block/", dev->DevNode, "/removable", NULL);
    val=ReadIntegerFile(Path, Tempstr);
		if (val) dev->Flags |= FLAG_REMOVABLE;

    Tempstr=MCopyStr(Tempstr, "/block/", dev->DevNode, "/ro", NULL);
    val=ReadIntegerFile(Path, Tempstr);
		if (val) dev->Flags |= FLAG_READONLY;


		if (dev->Flags & FLAG_REMOVABLE)	dev->Class=CatStr(dev->Class, "removable ");
		if (dev->Flags & FLAG_READONLY) dev->Class=CatStr(dev->Class, "read-only ");

    switch (atoi(dev->idClass))
    {
    case 0:
        dev->Class=CatStr(dev->Class, "hardisk");
        break;
    case 1:
        dev->Class=CatStr(dev->Class, "tape");
        break;
    case 3:
        dev->Class=CatStr(dev->Class, "proccessor");
        break;
    case 4:
        dev->Class=CatStr(dev->Class, "worm drive");
        break;
    case 5:
        dev->Class=CatStr(dev->Class, "rom");
        break;
    case 6:
        dev->Class=CatStr(dev->Class, "scanner");
        break;
    case 7:
        dev->Class=CatStr(dev->Class, "dvd/cd");
        break;
    case 8:
        dev->Class=CatStr(dev->Class, "changer");
        break;
    case 0xd:
        dev->Class=CatStr(dev->Class, "enclosure");
        break;
    }


    dev->Description=ReadFile(dev->Description, Path, "model");


    Destroy(Tempstr);
    Destroy(Key);

    return(dev);
}





ListNode *SCSIGetList(int Flags)
{
    glob_t Glob;
    char *Tempstr=NULL;
    TDevice *dev;
    int i;

    glob("/sys/bus/scsi/devices/*", 0, 0, &Glob);

    for (i=0; i < Glob.gl_pathc; i++)
    {
        dev=SCSIDeviceLoad(Glob.gl_pathv[i]);
        if (StrLen(dev->Description)) DeviceDisplay(Flags, dev);
        TDeviceDestroy(dev);
    }

    Destroy(Tempstr);

}
