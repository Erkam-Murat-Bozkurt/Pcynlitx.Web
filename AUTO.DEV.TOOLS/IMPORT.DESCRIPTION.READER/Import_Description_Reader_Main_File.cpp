
#include <iostream>
#include <string>
#include <cstring>
#include "Import_Description_Reader.h"

int main(){

    Import_Description_Reader infocol;

    char test_file_path [] = "html_test_file.html";

    infocol.Receive_HTML_File_Path(test_file_path);

    infocol.Collect_Information();

    int number = infocol.Get_Input_File_Number();

    std::cout << "\n input number:" << number;

    std::cin.get();

    std::cout << "\n\n";

    for(int i=0;i<number;i++){

        std::cout << "\n Input [" << i << "]:"

        << infocol.Get_Input_File_Path(i);

        std::cin.get();

        std::cout << "\n Input file position:"

                  << infocol.Get_Input_File_Position(i);

        std::cout << "\n\n";
    }

    return 0;
}
