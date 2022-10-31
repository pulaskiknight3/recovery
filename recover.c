#include <stdio.h>
#include <stdlib.h>

#define BLOCK_SIZE 512
int main(int argc, char *argv[])
{
    // worked with Elizabeth Stanish and Kai Wei Zhang

    // If user doesn't type in one CLI argument, return error
    if (argc != 2)
    {
        printf("Usage: ./recover IMAGE\n");
        return 1;
    }

    if (argv[1] == NULL)
    {
        printf("Could not open file.\n");
        return 2;
    }

    // opens up a raw file.
    FILE *raw = fopen(argv[1], "r");

    // declare the number of images generated.
    int imgcounter = 0;

    // create an output file that for now doesn't exist.
    FILE *output = NULL;

    // Declare the names of strings and the buffer of 512 bytes
    unsigned char buffer[BLOCK_SIZE];
    char filename[8];

    // Check if 512 bytes is the start of JPEG file
    while ((fread(buffer, 1, BLOCK_SIZE, raw)) == BLOCK_SIZE)
    {
        // Check for 512 bytes by checking the first four bytes
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            // If output file doesn't exist and input file already exists, close the output file.
            // If not closed, creates valgrind errors.
            if (output != NULL)
            {
                fclose(output);
            }
            // If new JPEG file found, create new file and write to it. Uses ###.jpg format for filename.
            // Increment image counter by 1
            sprintf(filename, "%03i.jpg", imgcounter);
            output = fopen(filename, "w+");
            imgcounter++;
        }
        // No JPEG but have been already writing, so continue writing.
        if (output != NULL)
        {
            fwrite(buffer, 1, BLOCK_SIZE, output);
        }
    }
    // Close all files and return 0 to exit the program.
    fclose(output);
    fclose(raw);
    return 0;
}