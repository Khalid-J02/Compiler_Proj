#include "File_Desc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void resetBuffer(File_Desc *fd) {
    fd->buffer[0] = '\0';
    fd->char_number = 0;
    fd->flag2 = UNSET;
}

// Helper function to resize the buffer
void resizeBuffer(File_Desc *fd) {
    fd->buf_size *= 2;
    fd->buffer = (char *) realloc(fd->buffer, fd->buf_size);
    if (fd->buffer == NULL) {
        fprintf(stderr, "Memory allocation error!");
        exit(0);
    }
}

bool fd_is_open(File_Desc *fd) {
    if (fd != NULL) {
        if (fd->fp != NULL && !ferror(fd->fp)) {
            return true;
        } else {
            return false;
        }
    } else {
        return false;
    }
}

char *fd_get_filename(File_Desc *fd) {
    return fd->file;
}

char *fd_get_curr_line(File_Desc *fd) {
    if (feof(fd->fp)) {
        return fd->buffer;
    }
    return NULL;
}

int fd_get_line_num(File_Desc *fd) {
    return fd->line_number;
}

int fd_get_char_num(File_Desc *fd) {
    return fd->char_number;
}

File_Desc *fd_open_stdin() {
    File_Desc *fd = (File_Desc *) malloc(sizeof(File_Desc));
    if (fd == NULL) {
        fprintf(stderr, "Memory allocation error!");
        return NULL;
    }

    fd->file = NULL;
    fd->fp = stdin;
    fd->line_number = 1;
    fd->char_number = 0;
    fd->flag = UNSET;
    fd->buf_size = BUFFER_SIZE;
    fd->buffer = (char *) malloc(fd->buf_size);
    if (fd->buffer == NULL) {
        fprintf(stderr, "Memory allocation error!");
        free(fd);
        return NULL;
    }
    resetBuffer(fd);
    fd->flag2 = UNSET;

    return fd;
}
// this is the constructor for the description file

File_Desc *fd_open(const char *filename) {
    File_Desc *fd = (File_Desc *) malloc(sizeof(File_Desc));
    if (fd == NULL) {
        fprintf(stderr, "Memory allocation error!");
        return NULL;
    }

    fd->file = strdup(filename);
    fd->fp = fopen(filename, "r");
    fd->line_number = 1;
    fd->char_number = 0;
    fd->flag = UNSET;
    fd->buf_size = BUFFER_SIZE;
    fd->buffer = (char *) malloc(fd->buf_size);
    if (fd->buffer == NULL) {
        fprintf(stderr, "Memory allocation error!");
        free(fd->file);
        free(fd);
        return NULL;
    }
    resetBuffer(fd);
    fd_get_next_line(fd);

    fd->flag2 = UNSET;


    return fd;
}
// Constructor to open the file

void fd_close_file(File_Desc *fd) {
    if (fd->fp != stdin) {
        fclose(fd->fp);
        fd->fp = NULL;
    }
}

void fd_close(File_Desc *fd) {
    if (fd == NULL) {
        return;
    }
    if (fd->file) {
        free(fd->file);
    }
    if (fd->buffer) {
        free(fd->buffer);
    }
    if (fd->fp != stdin) {
        fclose(fd->fp);
    }

    free(fd);
}
// Destructor to close the file and free memory

char fd_get_char(File_Desc *fd) {
    char c;
    c = fd->buffer[fd->char_number++];

    if (c == '\n') {
        fd->line_number++;
        fd->char_number = 0;
        resetBuffer(fd);
        fd_get_next_line(fd);
    }

    return c;
}
// Gets the current character in the file


void fd_report_error(File_Desc *fd, const char *msg) {
    if (fd && fd->file) {
        fprintf(stderr, "Error in file '%s' at line %d, character %d: %s\n",
                fd->file, fd->line_number, fd->char_number, msg);

        for(int i=0; fd->buffer[i]!='\n' && fd->buffer[i]!='\0' ; i++){
            printf("%c", fd->buffer[i]);
        }
        printf("\n");
        for (int i = 0; i < fd->char_number - 1; i++) {
            fprintf(stderr, " ");
        }
        fprintf(stderr, "^");
    }
    fd_close(fd);
    exit(1);
}
// Reports the error specifying the current line and character

void fd_unget_char(File_Desc *fd, char c) {
    if (c != '\0') {
        fd->buffer[--fd->char_number] = c;
        fd->flag2 = SET;
    }
}
// Puts back the current character, modifies the character number

void fd_get_next_line(File_Desc *fd) {

    int i = 0;
    char c;
    while ((c = fgetc(fd->fp)) != EOF) {
        if (c == '\n') {
            fd->buffer[i] = '\n';
            return;
        }
        fd->buffer[i++] = c;
    }
    fd->buffer[i] = '\0';

}