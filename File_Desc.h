//
// Created by Administrator on 7/20/2023.
//

#ifndef COMPILER_PROJ_FILE_DESC_H
#define COMPILER_PROJ_FILE_DESC_H

#include <stdio.h>
#include <stdbool.h>

#define UNSET 0
#define SET 1
#define BUFFER_SIZE 10000000

typedef struct File_Descriptor {
    FILE *fp;
    int line_number; /* line number in the file */
    int char_number; /* character number in the line */
    int flag; /* to prevents two ungets in a row */
    int buf_size; /* stores the buffer size */
    char *buffer; /* buffer to store a line */
    char *file; /* file name, allocate memory for this */
    int flag2;
} File_Desc;

bool fd_is_open(File_Desc* fd);
File_Desc* fd_open(const char* filename);
File_Desc* fd_open_stdin();
void fd_close(File_Desc* fd);
char* fd_get_filename(File_Desc* fd);
void fd_get_next_line(File_Desc* fd);
char* fd_get_curr_line(File_Desc* fd);
int fd_get_line_num(File_Desc* fd);
int fd_get_char_num(File_Desc* fd);
void fd_close_file(File_Desc* fd);
char fd_get_char(File_Desc* fd);
void fd_report_error(File_Desc* fd, const char* msg);
void fd_unget_char(File_Desc* fd, char c);


#endif //COMPILER_PROJ_FILE_DESC_H
