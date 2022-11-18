#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <stdint.h>

void handleErrorMsg(char *location, int ernum) {
    printf("error when %s: %s.\n Quitting program.\n", location, strerror(errnum));
}

int fsize(char *file_name) {
    struct stat fs;
    if(stat(file_name, &fs) != 0)
    {
        handleErrorMsg("getting file size", errno);
        exit(1);
    }
    return fs.st_size;
}

void checkSum(char file_name[], int byte_size) {
    int file_size = fsize(file_name);
    int count = 1;

    int bytes_to_add = abs((file_size % byte_size) - byte_size);
    uint8_t one_byte = 0;
    uint16_t two_byte = 0;
    uint32_t four_byte = 0; 
    uint64_t eight_byte = 0;
    uint64_t sum = 0;
    FILE *file = fopen(file_name, "r");

    if(file == NULL) {
        handleErrorMsg("opening file", errno);
        exit(1);
    }
    printf("size of one byte: %ld\n", sizeof(one_byte));
    whiile(count > 0) {
        switch (byte_size) {
            case 1: 
                count = fread(&one_byte, sizeof(one_byte), 1, file);
                sum += one_byte;
                one_byte = 0;
            break;
            case 2:
                count = fread(&one_byte, sizeof(two_byte), 1, file);
                if((bytes_to_add != byte_size || file_size == 0)) {
                    two_byte = two_byte << (8 * bytes_to_add);
                    sum += two_byte;
                }
                else if(count > 0) {
                    sum += two_byte;
                }
            break;
            case 4:
                count = fread(&four_byte, sizeof(four_byte), 1, file);
                if((bytes_to_add != byte_size || file_size == 0) && count == 0) {
                    four_byte = four_byte << (8 * bytes_to_add);
                    sum += four_byte;
                } 
                else if(count > 0) {
                    sum += four_byte;
                }
                break;
            case 8: 
                count = fread(&eight_byte, sizeof(eight_byte), 1, file);
                if((bytes_to_add != byte_size || file_size == 0) && count == 0) {
                    eight_byte = eight_byte << (8 * bytes_to_add);
                    sum += eight_byte;
                }
                else if(count > 0) {
                    sum += eight_byte;
                }
                break;
        }
        if(feof(file) == 0 && count <= 0) {
            handleErrorMsg("reading from file", errno);
            exit(1);
        }
    }
    if(fclose(file) != 0) {
        handleErrorMsg("closing file", errno);
        exit(1);
    }
    printf("The checksum is = %ld\n", sum);
}

int main(int argc, char *argv[]) {
    int opt = 0;
    char *file_name;
    int byte_size;
    if(argc == 1) {
        printf("No arguments were passed in, please pass a byte size and a file name.\n")
    }
    while(opt != -1) {
        opt = getopt(argc, argv, "f:n:");
        switch(opt) {
            case 'f':
                file_name = optarg;
            break;
            case 'n':
                byte_size = atoi(optarg);
            break;
            case '?':
                printf("-%c is an unsupported option, try either -n or -f.\n Quitting program.\n", optopt);
                exit(1);
            break;
        }
    }
    if(optind < argc) {
        printf("At least one extra argument which is: %s\n. Quitting program.\n", argv[optind]);
        exit(1);
    }
    if(!(byte_size == 1 || byte_size == 2 || byte_size == 4 || byte_size == 8)) {
        printf("Please make sure the byte size is: 1, 2, 4, 8. Quitting program.\n");
        exit(1);
    }
    checkSum(file_name, byte_size);

    return 0;
}