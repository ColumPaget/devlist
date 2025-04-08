#include "battery.h"
#include "display.h"

TDevice *BatteryDeviceLoad(const char *Path)
{
    TDevice *dev;
    char *Tempstr=NULL, *Key=NULL;
    const char *ptr;
    glob_t Glob;
    int val;

    dev=(TDevice *) calloc(1, sizeof(TDevice));
    dev->BusType=CopyStr(dev->BusType, "power");
    dev->Address=CopyStr(dev->Address, GetBasename(Path));
    dev->Class=ReadFile(dev->Class, Path, "technology");
    Tempstr=ReadFile(Tempstr, Path, "type");
    dev->Class=MCatStr(dev->Class, " ", Tempstr, NULL);
    dev->Vendor=ReadFile(dev->Vendor, Path, "manufacturer");
    dev->Serial=ReadFile(dev->Serial, Path, "serial_number");
    dev->Size=ReadIntegerFile(Path, "charge_full_design");
    if (dev->Size==0) dev->Size=ReadIntegerFile(Path, "energy_full_design");
    dev->PowerMax=ReadIntegerFile(Path, "charge_full");
    if (dev->PowerMax==0) dev->PowerMax=ReadIntegerFile(Path, "energy_full");
    dev->PowerNow=ReadIntegerFile(Path, "charge_now");
    if (dev->PowerNow==0) dev->PowerNow=ReadIntegerFile(Path, "energy_now");

    Tempstr=ReadFile(Tempstr, Path, "status");
		if (strcasecmp(Tempstr, "discharging")==0) dev->Status=CopyStr(dev->Status, "~rdischarging~0");
		else if (strcasecmp(Tempstr, "charging")==0) dev->Status=CopyStr(dev->Status, "~gcharging~0");
		else dev->Status=CopyStr(dev->Status, Tempstr);

    dev->Description=ReadFile(dev->Description, Path, "model_name");


    Destroy(Tempstr);
    Destroy(Key);

    return(dev);
}





void BatteryShow(int Flags)
{
    glob_t Glob;
    char *Tempstr=NULL;
    TDevice *dev;
    int i;

    glob("/sys/class/power_supply/BAT*", 0, 0, &Glob);

    for (i=0; i < Glob.gl_pathc; i++)
    {
        dev=BatteryDeviceLoad(Glob.gl_pathv[i]);
        if (StrLen(dev->Description)) DeviceDisplay(Flags, dev);
        TDeviceDestroy(dev);
    }

    Destroy(Tempstr);

}
