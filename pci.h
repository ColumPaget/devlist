
#ifndef DEVLIST_PCI_H
#define DEVLIST_PCI_H

#include "common.h"

TDevice *PCIDeviceLoad(const char *Path, ListNode *PCIids);
void PCIShow(int Flags);

#endif
