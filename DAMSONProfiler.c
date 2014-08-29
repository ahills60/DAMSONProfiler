// Standard I/O defines
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>

// For POSIX piping
#include <unistd.h>

// Constants
#include "DAMSONProfiler.h"

// Defines
#define MAX_CHARS            65536

// Variables
unsigned long long int StartTime;
// For holding a time object
struct timeval tv;
// Text buffer
char ScreenText[256];

// Prototypes
void Error(const char* format, ...);
void print(const char* format, ...);

// Function for printing error messages
void Error(const char* format, ...)
{
    va_list argpointer;
    va_start(argpointer, format);
    vsnprintf(ScreenText, 255, format, argpointer);
    printf("%s", ScreenText);
    va_end(argpointer);
}

// Function for printing statements with a time stamp
void print(const char* format, ...)
{
    va_list argpointer;
    unsigned long long int timeElapsed;
    
    gettimeofday(&tv, NULL);
    
    timeElapsed = 1000000LL * tv.tv_sec + tv.tv_usec - StartTime;
    
    va_start(argpointer, format);
    vsnprintf(ScreenText, 255, format, argpointer);
    printf("%llu: %s", timeElapsed, ScreenText);
    va_end(argpointer);
}

int main(int argc, char *argv[])
{
    char *currObj, *parVal = "", *filename = "\0";
    int i, n, a, isParam;
    FILE *fp;
    
    printf("\nDAMSON Profiler ");
    printf("Version %i.%i.%i (%s)\n", VERSION_MAJOR, VERSION_MINOR, VERSION_BUILD, VERSION_DATE);
    printf("Author: Andrew Hills (a.hills@sheffield.ac.uk)\n\n");
    
    // Go through arguments if any:
    for (i = 0; i < argc; i++)
    {
        currObj = argv[i];
        isParam = 0;
        a = strlen(argv[i]);
        // Detect the presence of an argument starter (i.e. "-"):
        for (n = 0; n < a; n++)
        {
            if (argv[i][n] == '-')
                isParam = 1;
            else
                break;
        }
        
        // An argument starter may have been found, so check:
        if (isParam)
        {
            // Yes, argument starter was found
            memmove(&currObj[0], &currObj[n], strlen(currObj) - n + 1);
            
            parVal = currObj;
        }
        else
        {
            // No argument starter was found. This could be the result of an option.
            if (strcmp(parVal, ""))
            {
                // There was previously a parameter. Determine what was set:
                if (!strcmp(parVal, "filename"))
                    filename = currObj;
                else
                    Error("Unrecognised input \"%s\"\n", parVal);
                
                parVal = "";
            }
            else // No parameter was defined. Skip to the next argument
                continue;
        }
    }
    
    // Check for piped input:
    if (isatty(fileno(stdin)))
    {
        // Connection is via a terminal session.
        Error("Invalid usage. Pipe DAMSON output into profiler.\n\n");
        return -1;
    }
    else
    {
        // Connection is not via a terminal. Could be a pipe.
        
    }
    
    // Note down the start time:
    gettimeofday(&tv, NULL);
    StartTime = 1000000LL * tv.tv_sec + tv.tv_usec;
    
    // Now check to see if a filename was defined:
    if (filename[0] == '\0')
    {
        // Print to screen
        
    }
    else
    {
        // Save to file
        fp = fopen(filename, "w+");
        if (!fp)
        {
            Error("Unable to open file stream to save output.");
            return -2;
        }
        
        // If here, we're able to open the file stream.
        
        // Finally close the file stream
        fclose(fp);
    }
    
    return 0;
}
