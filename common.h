#ifndef DEVLIST_COMMON_H
#define DEVLIST_COMMON_H

#include "libUseful-5/libUseful.h"
#include <glob.h>


#define FLAG_REMOVABLE 1
#define FLAG_READONLY  2

#define VERSION "1.0"

extern char *PciIDsFile;
extern char *PciIDsDirs;

typedef struct
{
    unsigned long BusNum;
    unsigned long DevNum;
    unsigned long PowerMax;
    uint64_t Speed;
    uint64_t Size;
		int Flags;
    char *BusType;
    char *Address;
    char *DevNode;
    char *Serial;
    char *idClass;
    char *Class;
    char *idVendor;
    char *Vendor;
    char *idProduct;
    char *Description;
} TDevice;


char *ReadFile(char *RetStr, const char *Dir, const char *File);
int ReadIntegerFile(const char *Dir, const char *File);
uint64_t ReadUint64File(const char *Dir, const char *File);
char *LookupDevNode(char *RetStr, const char *Path, const char *SubDir);
void LookupAnyDevNode(TDevice *dev, const char *Path);
void TDeviceDestroy(void *p_Dev);

#endif
