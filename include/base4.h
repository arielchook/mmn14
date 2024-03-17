#include <stdio.h>

void copyFile(const char *inputFilename, const char *outputFilename) {
    FILE *inputFile, *outputFile;
    char character;

    // Open the input file in read mode
    inputFile = fopen(inputFilename, "r");
    if (inputFile == NULL) {
        printf("Error opening input file\n");
        return;
    }

    // Open the output file in write mode
    outputFile = fopen(outputFilename, "w");
    if (outputFile == NULL) {
        printf("Error opening output file\n");
        fclose(inputFile); // Close the input file before returning
        return;
    }

    // Read the input file character by character and write to the output file
    while ((character = fgetc(inputFile)) != EOF) {
        switch (character) {
            case '0': fputc('*',outputFile);
                        break;
            case '1': fputc('#',outputFile);
                        break;
            case '2': fputc('%',outputFile);
                        break;
            case '3': fputc('!',outputFile);
                        break;
        }
    }

    // Close both files
    fclose(inputFile);
    fclose(outputFile);
}