

#ifndef IMPORT_FILE_PARSER_H
#define IMPORT_FILE_PARSER_H

#include <iostream>
#include <fstream>
#include <unistd.h>
#include <string>
#include <cstring>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <windows.h>
#include <winbase.h>
#include <tchar.h>
#include <shellapi.h>
#include <shlwapi.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <errno.h>
#include "Import_Description_Reader.h"


struct import_file_data {

    int imput_line_number;
    char ** Import_File_Content;
};

class Import_File_Parser
{
public:
 Import_File_Parser();
 Import_File_Parser(const Import_File_Parser & orig);
 virtual ~Import_File_Parser();
 void Receive_HTML_File_Path(char * path);
 void Construct_New_HTML(char * new_path);
private:
 void Clear_Dynamic_Memory();
 void Read_Import_File(std::string path, int index);
 void Write_New_HTML(char * new_path);
 void Collect_Information();
 void C_String_Constructor(char ** target_string, std::string std_string);
 void Determine_Import_File_Sizes();
 void allocate_memory_for_imput_file_data();
 void allocate_memory_for_input_data_pointer();
 import_file_data * import_data_pointer;
 Import_Description_Reader infocol;
 Cpp_FileOperations FileManager;
 bool Memory_Delete_Condition;
 int Import_File_Number;
};

#endif /* IMPORT_FILE_PARSER_H */
