/*
File:         tide_analyzer.h
Purpose:      Contains prototypes
Author:       Rutendo Musuka and Hazel Chongoti
Student #s:   76822683 and 53106662
CS Accounts:  k7i2b and m8j2b
Date:         10 Sept 2019
*/

/********************************************************************
YOU MAY EDIT THIS FILE (BUT YOU DON'T NEED TO)
******************************************************************/

#pragma once

/* Insert function prototypes here */
FILE* open_file(FILE* file_pointer, char* fileName, char* mode);
void process_file(double array_to_populate[], FILE* pointer_to_data_file);