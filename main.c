#include "common.h"
#include "pci.h"
#include "usb.h"
#include "scsi.h"
#include "command-line.h"

int main(int argc, char *argv[])
{
    int Flags;

		PciIDsDirs=CopyStr(PciIDsDirs, "/usr/share:/usr/share/lshw:/usr/share/misc");
    Flags=ParseCommandLine(argc, argv);

    if (Flags & SHOW_BUS_PCI) PCIGetList(Flags);
    if (Flags & SHOW_BUS_USB) USBGetList(Flags);
    if (Flags & SHOW_BUS_SCSI) SCSIGetList(Flags);
}
