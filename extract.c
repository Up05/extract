#include "str.h"

#include <assert.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>

#define assertf(EXPR, ...) if(!EXPR) { fprintf(stderr, __VA_ARGS__); exit(1); }

#define __repeat(NUM, VAR, INDEX) for(int VAR ## INDEX = 0; VAR ## INDEX < NUM; VAR ## INDEX ++)
#define __repeat_expand(NUM, VAR, INDEX) __repeat(NUM, VAR, INDEX)
#define repeat(NUM) __repeat_expand(NUM, I, __COUNTER__)

#define TMP_DIR "tmp/"

void try_unzip(char* ext, char* exe, char* flag, char* file) {
    if(str_ends_with(file, ext)) {
        if(system(str_concat(3, "which ", exe, " > /dev/null 2>&1"))) {
            printf("You do not have '%s', cannot unzip files with '%s' extension. Skipping '%s'...\n", exe, ext, file);
            if(str_eq(exe, "7z")) 
                puts("Tip: 7z package is often called 'p7zip' or 'p7zip-full', try e.g.: run0 pacman -S p7zip");
            return;
        }
        int exit_code = system(str_concat(5, exe, " ", flag, " ", file));
        if(exit_code) {
            printf("Failed to unzip '%s'! Skipping...\n", file);
            return;
        }
        
        exit_code = rename(file, str_concat(2, TMP_DIR, file));
        if(exit_code) {
            printf("Failed to move file '%s' to '%s%s'! Skipping...\n", file, TMP_DIR, file);
            puts("Tip: '.gz' and, probably, other files get cleaned up automatically");
        }
    }
}

int main(int argc, char** argv) {
    char* filter;

    if(argc < 2) {
        filter = "";
        goto handling;
    }

    if(str_eq(argv[1], "-h") || str_eq(argv[1], "--help")) {
        puts(
            "Usage: 'extract [FILTER]'. " 
            "if a filename in cwd does not contain FILTER, it will be ignored"
        );
        return 0;
    }

    filter = argv[1];

    
    handling:

    DIR* dir;
    assertf((dir = opendir(".")), "Can't open directory: '.'");

    { // makes the ./tmp dir for later moving original zipped files into 
        struct stat st = { };
        if (stat(TMP_DIR, &st) == -1) {
            mkdir(TMP_DIR, 0700);
        }
    }

    // 5 -- amount of try_unzip calls
    repeat(5) {
        rewinddir(dir);
        struct dirent* dir_ptr;
        while((dir_ptr = readdir(dir))) {
            char* filename = dir_ptr->d_name;

            try_unzip(".gz",  "gzip",  "-d",  filename);     
            try_unzip(".tar", "tar",   "-xf", filename);     
            try_unzip(".rar", "unrar", "",    filename);     
            try_unzip(".zip", "unzip", "",    filename);     
            try_unzip(".7z",  "7z",    "x",   filename);     
        }
    }

    return 0;
}
