


#ifndef IMPORT_DESCRIPTION_READER_H
#define IMPORT_DESCRIPTION_READER_H

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
#include "StringOperator.h"
#include "Cpp_FileOperations.h"


struct Input_File_Data
{
  char * file_name;
  char * html_string_line;
  int input_position;
};

class Import_Description_Reader
{
public:
 Import_Description_Reader();
 Import_Description_Reader(const Import_Description_Reader & orig);
 virtual ~Import_Description_Reader();
 void Collect_Information();
 void Receive_HTML_File_Path(char * html_path);
 void Determine_Head_Position();
 void Determine_Input_Files_Positions();
 void Clear_Dynamic_Memory();
 char * Get_Input_File_Path(int num);
 int Get_Input_File_Number();
 int Get_Input_File_Position(int num);
 int Get_File_Size();
 char * Get_File_Content(int num);
 void C_String_Constructor(char ** target_string, std::string std_string);
private:
 void Find_Imput_File_Path(char * string_line, size_t start_pos,size_t end_pos, int index);
 void Find_Imput_Start_Point(char * string_line,size_t * start);
 void Find_Imput_End_Point(char * string_line, size_t start_pos, size_t * end_pos);
 void Determine_Imput_File_Number();
 void Determine_HTML_File_Size();
 void Allocate_Memory_For_Reading();
 void Read_HTML_File();
 void Extract_Input_File_Paths();
 void List_Input_File_Names();
 void place_c_string(char ** pointer, char * string);
 char * FilePath;
 char ** input_lines;
 char ** File_Content;
 char ** Input_File_Paths;
 Cpp_FileOperations FileManager;
 StringOperator StringManager;
 CharOperator Char_Manager;
 Input_File_Data * Data_Pointer;
 bool Memory_Delete_Condition;
 bool File_Path_Receive_Status;
 int  File_Line_Number;
 int  Input_File_Number;
};

#endif /* IMPORT_DESCRIPTION_READER_H */
