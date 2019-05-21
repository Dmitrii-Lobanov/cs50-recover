#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        fprintf(stderr, "Please enter file to open.\n");
        return 1;
    }

    //Open the file entered in the command line
    FILE *file = fopen(argv[1], "r");

    //Throw an error if the file does not exist
    if(file == NULL)
    {
        fprintf(stderr, "Could not open the file %s.\n", argv[1]);
        return 1;
    }

    //Create an output file for images
    FILE *img = NULL;

    //Create buffer and output file arrays
    //filename contains 8 bytes (3 for name, 1 for dot, 3 for extension, and 1 for \0)
    unsigned char buffer[512];
    char filename[8];

    //Counter for filename
    int counter = 0;

    //Flag
    bool flag = false;

    //Read the file
    while(fread(buffer, 512, 1, file) == 1)
    {
        //Check if the cursor at the beginning of JPEG
        if(buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            //Close current JPEG, so we can start reading the next one
            if(flag == true)
            {
                fclose(img);
            }
            else
            {
                flag = true;
            }

            sprintf(filename, "%03i.jpg", counter);
            img = fopen(filename, "w");
            counter++;
        }

        if(flag == true)
        {
            fwrite(&buffer, 512, 1, img);
        }
    }

    fclose(file);
    fclose(img);

    return 0;
}
