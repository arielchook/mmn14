#include <stdio.h>
#include <messages.h>
#include <util.h>
#include <string.h>

void processFile(char *fname)
{
    FILE *asFile, *amFile;
    char *fnameWext=(char*)safe_malloc(strlen(fname)+4);
    /* add .as extension and try to open the input file */
    sprintf(fnameWext,"%s.as",fname);
    if ((asFile=fopen(fnameWext,"r")) == NULL)
    {
        fprintf(stderr,ERR_FILE_CANT_BE_WRITTEN, fnameWext);
        free(fnameWext);
        return;
    }

    /* add .am extension and try to open the macro output file. 
       no need to allocate a new file name since it's the same length */
    sprintf(fnameWext,"%s.am",fname);
    if ((amFile=fopen(fnameWext,"w")) == NULL)
    {
        fprintf(stderr,ERR_FILE_CANT_BE_WRITTEN, fnameWext);
        free(fnameWext);
        return;
    }

    
}

int main(int argc, char *argv[])
{
    /* make sure the user passed at least one parameter to the program */
    if (argc<2)
    {
        fprintf(stderr,ERR_USAGE,argv[0]);
        exit(1);
    }

    /* We process each file given in arguments independently */
    while(--argc) 
    {
        processFile(argv[argc]);
    }
    return 0;
}