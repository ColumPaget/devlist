#include "display.h"
#include "command-line.h"

void DeviceDisplay(int Flags, TDevice *dev)
{
    char *Tempstr=NULL, *Output=NULL, *Power=NULL;
    const char *color="";
    float val;


    Output=CopyStr(Output, dev->Address);
    Output=TerminalPadStr(Output, ' ', 13);
    if (StrValid(dev->DevNode)) Output=MCatStr(Output, " ~g", dev->DevNode, "~0", NULL);
    Output=TerminalPadStr(Output, ' ', 22);

    if (strcmp(dev->BusType, "pci")==0) Output=MCatStr(Output, " ~b", dev->BusType, "~0 ", NULL);
    else if (strcmp(dev->BusType, "power")==0) Output=MCatStr(Output, " ~m", dev->BusType, "~0 ", NULL);
    else if (strncmp(dev->BusType, "usb3", 4)==0) Output=MCatStr(Output, " ~g", dev->BusType, "~0 ", NULL);
    else if (strncmp(dev->BusType, "usb2", 4)==0) Output=MCatStr(Output, " ~y", dev->BusType, "~0 ", NULL);
    else if (strncmp(dev->BusType, "usb1", 4)==0) Output=MCatStr(Output, " ~m", dev->BusType, "~0 ", NULL);
    else if (strncmp(dev->BusType, "scsi", 4)==0) Output=MCatStr(Output, " ~c", dev->BusType, "~0 ", NULL);
    else Output=MCatStr(Output, " ", dev->BusType, NULL);
    Output=TerminalPadStr(Output, ' ', 31);

    if (dev->Size == 0) Output=CatStr(Output, "    -- ");
    else
    {
        Tempstr=FormatStr(Tempstr, " % 6s", ToMetric((double) dev->Size, 1));
        Output=CatStr(Output, Tempstr);
    }


    if (strcmp(dev->BusType, "power")==0)
    {
        val=dev->PowerNow * 100.0 / dev->PowerMax;
        if (val > 75) color="~g";
        else if (val > 40) color="~y";
        else if (val > 20) color="~r";
        else "~w~R";
        Tempstr=FormatStr(Tempstr, "   %s %5.1f%% full~0    ~c%s:~0 %s - ~y%s~0", color, val, dev->Class, dev->Description, dev->Vendor);
    }
    else
    {
        Power=CopyStr(Power, ToMetric((double) dev->PowerMax,1));
        Tempstr=FormatStr(Tempstr, " % 7s/s %5smA ~c%s:~0 %s - ~y%s~0", ToIEC((double) dev->Speed, 1), Power, dev->Class, dev->Description, dev->Vendor);
    }

    Output=CatStr(Output, Tempstr);

    if (StrValid(dev->Status)) Output=MCatStr(Output, "  ", dev->Status, NULL);
    if (StrValid(dev->Serial)) Output=MCatStr(Output, "  serial=", dev->Serial, NULL);

    if (strcmp(dev->BusType, "power")==0)
		{
			Tempstr=FormatStr(Tempstr, "  %0.1f%% healthy", dev->PowerMax * 100.0 / dev->Size);
			Output=CatStr(Output, Tempstr);
		}
    Output=CatStr(Output, "\n");


    TerminalPutStr(Output, NULL);

    Destroy(Tempstr);
    Destroy(Power);
    Destroy(Output);
}

