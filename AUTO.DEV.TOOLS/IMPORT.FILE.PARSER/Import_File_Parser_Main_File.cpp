

 #include <iostream>
 #include "Import_File_Parser.h"

 int main(int argc, char** argv){

     if(argc < 2){

        std::cout << "\n\n The usage : html_parser.exe <file_path>";

        exit(0);
     }

     Import_File_Parser parser;

     char new_path [] = "new_html.html";

     parser.Receive_HTML_File_Path(argv[1]);

     parser.Construct_New_HTML(new_path);

     return 0;
 };
