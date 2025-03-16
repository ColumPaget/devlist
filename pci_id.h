#ifndef DEVLIST_PCIID_H
#define DEVLIST_PCIID_H

#include "common.h"

ListNode *PCILoadIDFile(const char *Path);
ListNode *PCILoadIDs(const char *Dirs, const char *FileNames);


#endif

