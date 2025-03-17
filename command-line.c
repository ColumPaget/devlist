#include "command-line.h"


void PrintHelp()
{
    printf("usage:  devlist <options>\n");
    printf("options:\n");
    printf("  -pci              - print PCI devices only\n");
    printf("  -usb              - print USB devices only\n");
    printf("  -scsi             - print SCSI devices only\n");
    printf("  -I <path>         - path to pci.ids file if not in standard places\n");
    printf("  -?                - print this help\n");
    printf("  -help             - print this help\n");
    printf("  --help            - print this help\n");
    printf("  --version         - print version\n");
    printf("the default behavior is to print all device types\n");

    exit(0);
}

void PrintVersion()
{
    printf("devlist version: %s\n\n", VERSION);
    exit(0);
}

int ParseCommandLine(int argc, char *argv[])
{
    CMDLINE *Cmd;
    int Flags=0;
    const char *ptr;

    Cmd=CommandLineParserCreate(argc, argv);
    ptr=CommandLineNext(Cmd);
    while (ptr)
    {
        if (strcasecmp(ptr, "-pci")==0) Flags |= SHOW_BUS_PCI;
        else if (strcasecmp(ptr, "-usb")==0) Flags |= SHOW_BUS_USB;
        else if (strcasecmp(ptr, "-scsi")==0) Flags |= SHOW_BUS_SCSI;
        else if (strcasecmp(ptr, "-I")==0) PciIDsFile=CopyStr(PciIDsFile, CommandLineNext(Cmd));
        else if (strcasecmp(ptr, "-?")==0) PrintHelp();
        else if (strcasecmp(ptr, "-help")==0) PrintHelp();
        else if (strcasecmp(ptr, "--help")==0) PrintHelp();
        else if (strcasecmp(ptr, "--version")==0) PrintVersion();

        ptr=CommandLineNext(Cmd);
    }

    //SHOW_BUS_ALL includes all the 'SHOW_BUS' flags, so
    //this will only happen if none of them are set, which
    //is the default 'show all' condition
    if (! (Flags & SHOW_BUS_ALL)) Flags |= SHOW_BUS_ALL;
    return(Flags);
}
