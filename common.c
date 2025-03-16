#include "common.h"

char *PciIDsFile=NULL;
char *PciIDsDirs=NULL;


char *ReadFile(char *RetStr, const char *Dir, const char *File)
{
    STREAM *S;
    char *Path=NULL;

    RetStr=CopyStr(RetStr, "");
    Path=MCopyStr(Path, Dir, "/", File, NULL);
    S=STREAMOpen(Path, "r");
    if (S)
    {
        RetStr=STREAMReadDocument(RetStr, S);
        StripTrailingWhitespace(RetStr);
        StripLeadingWhitespace(RetStr);

        STREAMClose(S);
    }

    Destroy(Path);
    return(RetStr);
}


int ReadIntegerFile(const char *Dir, const char *File)
{
    char *Tempstr=NULL;
    int val;

    Tempstr=ReadFile(Tempstr, Dir, File);
    val=atoi(Tempstr);

    Destroy(Tempstr);

    return(val);
}

uint64_t ReadUint64File(const char *Dir, const char *File)
{
    char *Tempstr=NULL;
    uint64_t val;

    Tempstr=ReadFile(Tempstr, Dir, File);
    val=strtoll(Tempstr, NULL, 10);
    Destroy(Tempstr);

    return(val);
}


char *LookupDevNode(char *RetStr, const char *Path, const char *SubDir)
{
char *Tempstr=NULL;
glob_t Glob;

		RetStr=CopyStr(RetStr, "");
    Tempstr=MCopyStr(Tempstr, Path, "/", SubDir, "/*", NULL);
    glob(Tempstr, 0, 0, &Glob);
    if (Glob.gl_pathc > 0)
    {
      RetStr=CopyStr(RetStr, GetBasename(Glob.gl_pathv[0]));
			StripTrailingWhitespace(RetStr);
      globfree(&Glob);
    }

Destroy(Tempstr);

return(RetStr);
}


void LookupAnyDevNode(TDevice *dev, const char *Path)
{
    if (! StrValid(dev->DevNode)) dev->DevNode=LookupDevNode(dev->DevNode, Path, "block");
    if (! StrValid(dev->DevNode)) dev->DevNode=LookupDevNode(dev->DevNode, Path, "net");
    if (! StrValid(dev->DevNode)) dev->DevNode=LookupDevNode(dev->DevNode, Path, "bluetooth");
    if (! StrValid(dev->DevNode)) dev->DevNode=LookupDevNode(dev->DevNode, Path, "tty");
    if (! StrValid(dev->DevNode)) dev->DevNode=LookupDevNode(dev->DevNode, Path, "input");
    if (! StrValid(dev->DevNode)) dev->DevNode=LookupDevNode(dev->DevNode, Path, "sound");
    if (! StrValid(dev->DevNode)) dev->DevNode=LookupDevNode(dev->DevNode, Path, "graphics");
    if (! StrValid(dev->DevNode)) dev->DevNode=LookupDevNode(dev->DevNode, Path, "hwmon");
}


void TDeviceDestroy(void *p_Dev)
{
    TDevice *Dev;

    Dev=(TDevice *) p_Dev;
    if (Dev)
    {
        Destroy(Dev->BusType);
        Destroy(Dev->Address);
        Destroy(Dev->Serial);
        Destroy(Dev->Description);
        Destroy(Dev->Vendor);
        Destroy(Dev->idClass);
        Destroy(Dev->idVendor);
        Destroy(Dev->idProduct);
        free(Dev);
    }

}

