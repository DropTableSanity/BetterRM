// BetterRM: rm with trash safety
// author: chance nelson
// TODO: removing folders
//       better support for removing binary files


#include <stdio.h>
#include <unistd.h>
#include <pwd.h>
#include <sys/types.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>


// desc: check the arguments given for validity
// return: boolean; true for valid args, false for invalid
// argc: arg count
// argv: arg list
bool check_args(int argc, char** argv) {
    for(int i = 1; i < argc; i++) {
        // check to see if each file exists
        FILE *file = fopen(argv[i], "r");
        if(file == NULL) {
            fclose(file);
            return false;

        }
        
        fclose(file);

    }

    return true;

}


int main(int argc, char** argv) {
    // check args for validity
    // if args not valid, print a description
    if(!check_args(argc, argv)) {
        printf("BetterRM - so you dont go batshit crazy when you accidentaly delete important stuff\n");
        printf("Usage: brm file1 file2 ... filen");

    }

    // get the home directory
    const char *homeDir;
    homeDir = getpwuid(getuid())->pw_dir;

    // loop through each file, copy it to the home trash folder, and unlink the original version
    for(int i = 1; i < argc; i++) {
        FILE *original = fopen(argv[i], "r");

        // set up string that describes the path to a trash folder in /home/$USER
        char *backupPath = malloc(strlen(homeDir) + strlen(argv[i]) + strlen("/trash/")); 
        strcpy(backupPath, homeDir);
        strcat(backupPath, "/trash/");

        // find out if the trash folder exists or not
        // if it doesnt, create it
        // TODO: different trash folder locations? maybe user specified optionally
        struct stat st = {0};
        if(stat(backupPath, &st) == -1) {
            mkdir(backupPath, 0700);

        }

        strcat(backupPath, argv[i]);

        // create backup file
        FILE *backup = fopen(backupPath, "w");

        // copy original file contents to the backup area
        char c = fgetc(original);
        while(c != EOF) {
            fputc(c, backup);
            c = fgetc(original);

        }

        // unlink original file (spooky part)
        remove(argv[i]);

    }

    return 0;

}
