#include "pci_id.h"

#define STATE_VENDOR 1
#define STATE_DEVICE 2
#define STATE_CLASS  3

ListNode *PCIids=NULL;

static char *PCIReadItem(char *RetStr, const char *Details, const char *Prefix, const char *Parent)
{
    char *Key=NULL;
    const char *ptr;

    ptr=Details;
    while (isspace(*ptr)) ptr++;
    ptr=GetToken(ptr, "\\S", &RetStr, 0);
    Key=MCopyStr(Key, Prefix, Parent, RetStr, NULL);
    if (StrValid(ptr)) SetVar(PCIids, Key, ptr);

    Destroy(Key);

    return(RetStr);
}


static int PCIIDReadClass(const char *Details, char **ParentClass)
{
    char *Token=NULL;

    Token=PCIReadItem(Token, Details, "C", "");
    *ParentClass=CopyStr(*ParentClass, Token);
    Destroy(Token);

    return(STATE_CLASS);
}


static int PCIIDReadSubClass(const char *Details, const char *ParentClass, char **ParentSubClass)
{
    char *Token=NULL, *Tempstr=NULL;
    const char *ptr;

    ptr=Details;
    if (*ptr=='\t')
    {
        ptr++;
        if (*ptr != '\0')
        {
            if (*ptr == '\t')
            {
                Tempstr=MCopyStr(Tempstr, ParentClass, *ParentSubClass, NULL);
                Token=PCIReadItem(Token, Details, "C", Tempstr);
            }
            else
            {
                Token=PCIReadItem(Token, Details, "C", ParentClass);
                *ParentSubClass=CopyStr(*ParentSubClass, Token);
            }
        }
    }

    Destroy(Tempstr);
    Destroy(Token);

    return(STATE_CLASS);
}


static int PCIIDReadVendor(const char *Details, char **Vendor)
{
    *Vendor=PCIReadItem(*Vendor, Details, "V", "");
    return(STATE_VENDOR);
}


static int PCIIDReadProduct(const char *Details, const char *Vendor)
{
    char *Token=NULL;
    const char *ptr;

    ptr=Details;
    if (*ptr=='\t') ptr++;

    //if there are two tabs, then we will have vendor/subvendor/product
    //we don't care about subvendor, so just throw that away
    if (*ptr=='\t') return(STATE_DEVICE);

    /*
    {
    		ptr++;
    		ptr=GetToken(ptr, "\\S", &Token, 0);
    }
    */

    Token=PCIReadItem(Token, ptr, "P", Vendor);

    Destroy(Token);

    return(STATE_DEVICE);

}


ListNode *PCILoadIDFile(const char *Path)
{
    char *Tempstr=NULL;
    STREAM *S;
    char *ParentClass=NULL, *ParentSubClass=NULL, *Vendor=NULL;
    int State=0;

    PCIids=MapCreate(1024, LIST_FLAG_UNIQ | LIST_FLAG_CACHE);

    S=STREAMOpen(Path,  "rz");
    if (S)
    {
        Tempstr=STREAMReadLine(Tempstr, S);
        while (Tempstr)
        {
            StripTrailingWhitespace(Tempstr);

            if (Tempstr[0] == '#') /*comment, do nothing*/ ;
            else if (strncmp(Tempstr, "C ", 2)==0) State=PCIIDReadClass(Tempstr+2, &ParentClass);
            else if (! isspace(Tempstr[0])) State=PCIIDReadVendor(Tempstr, &Vendor);
            else
            {
                if (State==STATE_CLASS) PCIIDReadSubClass(Tempstr, ParentClass, &ParentSubClass);
                else PCIIDReadProduct(Tempstr, Vendor);
            }

            Tempstr=STREAMReadLine(Tempstr, S);
        }
        STREAMClose(S);
    }

    Destroy(Vendor);
    Destroy(Tempstr);
    Destroy(ParentClass);
    Destroy(ParentSubClass);

    return(PCIids);
}

ListNode *PCILoadIDs(const char *Dirs, const char *FileNames)
{
    char *Dir=NULL, *Name=NULL, *Path=NULL;
    const char *ptr, *fptr;
    ListNode *IDList=NULL;

    ptr=GetToken(Dirs, ":", &Dir, 0);
    while (ptr)
    {
        fptr=GetToken(FileNames, ":", &Name, 0);
        while (fptr)
        {
            Path=MCopyStr(Path, Dir, "/", Name, NULL);
            IDList=PCILoadIDFile(Path);
            if (ListSize(IDList) >0) break;
            fptr=GetToken(fptr, ":", &Name, 0);
        }

        if (ListSize(IDList) >0) break;

        ptr=GetToken(ptr, ":", &Dir, 0);
    }

    Destroy(Path);
    Destroy(Name);
    Destroy(Dir);

    return(IDList);
}
