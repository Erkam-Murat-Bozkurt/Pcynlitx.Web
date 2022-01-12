


#ifndef HEAD_WRITER_H
#define HEAD_WRITER_H

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
#include "Cpp_FileOperations.h"
#include "StringOperator.h"

class head_writer
{
public:
 head_writer();
 head_writer(const head_writer & orig);
 virtual ~head_writer();
 void Receive_Template_File_Path(char * path);
 void Collect_Information();
 void Write_HTML_Header(char * new_path);
 char * Get_Title_Name();
 char * Get_Style_Sheet(int num);
 int    Get_Style_Sheet_Number();
private:
 void Clear_Dynamic_Memory();
 void Read_Template_File();
 void C_String_Constructor(char ** target_string, std::string std_string);
 void Determine_StyleSheet_Number();
 void extract_header_informations();
 void extract_stylesheets();
 void extract_title_name();
 Cpp_FileOperations FileManager;
 StringOperator StringManager;
 char ** template_file_content;
 char * template_file_path;
 char * title;
 char ** stylesheets;
 int  stylesheet_number;
 int  File_Line_Number;
 bool Memory_Delete_Condition;
};

#endif /* HEAD_WRITER_H */
