#ifndef BUSLIST_COMMAND_LINE_H
#define BUSLIST_COMMAND_LINE_H

#include "common.h"


#define SHOW_BUS_PCI  1
#define SHOW_BUS_USB  2
#define SHOW_BUS_SCSI 4
#define SHOW_DETAILS 16


#define SHOW_BUS_ALL (SHOW_BUS_PCI | SHOW_BUS_USB | SHOW_BUS_SCSI)

int ParseCommandLine(int argc, char *argv[]);

#endif
