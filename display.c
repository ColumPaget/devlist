#include "display.h"
#include "command-line.h"

void DeviceDisplay(int Flags, TDevice *dev)
{
    char *Tempstr=NULL, *Output=NULL;


    Output=CopyStr(Output, dev->Address);
    Output=TerminalPadStr(Output, ' ', 13);
    Output=MCatStr(Output, " ~g", dev->DevNode, "~0", NULL);
    Output=TerminalPadStr(Output, ' ', 20);

    if (strcmp(dev->BusType, "pci")==0) Output=MCatStr(Output, " ~b", dev->BusType, "~0 ", NULL);
    else if (strncmp(dev->BusType, "usb3", 4)==0) Output=MCatStr(Output, " ~g", dev->BusType, "~0 ", NULL);
    else if (strncmp(dev->BusType, "usb2", 4)==0) Output=MCatStr(Output, " ~y", dev->BusType, "~0 ", NULL);
    else if (strncmp(dev->BusType, "usb1", 4)==0) Output=MCatStr(Output, " ~m", dev->BusType, "~0 ", NULL);
    else if (strncmp(dev->BusType, "scsi", 4)==0) Output=MCatStr(Output, " ~c", dev->BusType, "~0 ", NULL);
    else Output=MCatStr(Output, " ", dev->BusType, NULL);
    Output=TerminalPadStr(Output, ' ', 29);

    if (dev->Size == 0) Output=CatStr(Output, "    -- ");
    else
    {
        Tempstr=FormatStr(Tempstr, " % 6s", ToMetric((double) dev->Size, 1));
        Output=CatStr(Output, Tempstr);
    }

    Tempstr=FormatStr(Tempstr, " % 7s/s % 5lumA ~c%s:~0 %s - ~y%s~0", ToIEC((double) dev->Speed, 1), dev->PowerMax, dev->Class, dev->Description, dev->Vendor);
	  Output=CatStr(Output, Tempstr);
    if (StrValid(dev->Serial)) Output=MCatStr(Output, "  serial=", dev->Serial, NULL);
    Output=CatStr(Output, "\n");


    TerminalPutStr(Output, NULL);

    Destroy(Tempstr);
    Destroy(Output);
}

