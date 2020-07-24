#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

typedef uint8_t BYTE;

BYTE read_512(BYTE *bytes, FILE *file);

int main(int argc, char *argv[])
{
    //check for single command-line argument and that file can be opened
    if (argc != 2)
    {
        printf("This program requires a single command-line argument.\n");
    }

    FILE *file = fopen(argv[1], "r");
    if (!file)
    {
        printf("Specified image cannot be opened for reading.\n");
        return 1;
    }

    //variables to mark once writing has begun and whenever a new jpeg header is detected
    BYTE writing = 0;
    BYTE jpeg = 0;

    //keeps track of file number
    int n = 0;

    //declare file variables
    FILE *fileout = NULL;
    BYTE *bytes;
    bytes = (BYTE *)malloc(512);
    if (bytes == NULL)
    {
        return 1;
    }
    for (int i = 0; i < 1; i++) {}
    while (1)
    {
        //read next 512 bytes; if first file, turn on writing variable and enter if statement
        jpeg = read_512(bytes, file);
        writing = writing || jpeg;

        //if new header obtained, close last file (if beyond first), make next file, and write 512 bytes
        if ((jpeg % 2) && writing)
        {
            if (n > 0)
            {
                fclose(fileout);
            }
            else
            {
                for (int i = 0; i < 1; i++)
                {
                    //printf("%d: %x\n", i, bytes[i]);
                }
            }
            char fname[3];
            sprintf(fname, "%03d.jpg", n);
            fileout = fopen(fname, "a");
            fwrite(bytes, 1, 512, fileout);
            n++;
        }

        //otherwise, continue writing
        else if (!(jpeg % 2) && writing)
        {
            if (jpeg == 2)
            {
                free(bytes);
                break;
            }
            else
            {
                fwrite(bytes, 1, 512, fileout);
            }
        }
        //if first header has not been detected, keep going without doing anything
    }

    //close files before finishing
    if (fileout != NULL)
    {
        fclose(fileout);
    }
    fclose(file);
}

//read next 512 bytes of file
//returns 1 if next 512 bytes begin with jpeg header
//returns 2 if end of file is detected (or error) indicated by < 512 bytes being read
//returns 0 otherwise
BYTE read_512(BYTE *bytes, FILE *file)
{
    int numread = 0;
    numread = fread(bytes, 512, 1, file);

    if (numread < 1)
    {
        return 2;
    }

    // Check first four bytes as 0xff, 0xd8, 0xff, and beginning with 1110 (0xe#)
    if (bytes[0] == 0xff && bytes[1] == 0xd8 && bytes[2] == 0xff && (bytes[3] >> 4) == 0x0e)
    {
        return 1;
    }
    return 0;
}