

#include <iostream>
#include <string>
#include <cstring>
#include "head_writer.h"

int main(){

    head_writer hd_wrt;

    char teplate_file_path [] = "template.txt";

    hd_wrt.Receive_Template_File_Path(teplate_file_path);

    hd_wrt.Collect_Information();

    std::cout << "\n\n Title:" << hd_wrt.Get_Title_Name();

    int number = hd_wrt.Get_Style_Sheet_Number();

    for(int i=0;i<number;i++){

        std::cout << "\n stylesheet [" << i << "]:" << hd_wrt.Get_Style_Sheet(i);
    }

    char target_html_path [] = "head.html";

    hd_wrt.Write_HTML_Header(target_html_path);

    std::cout << "\n\n";

    return 0;
}
