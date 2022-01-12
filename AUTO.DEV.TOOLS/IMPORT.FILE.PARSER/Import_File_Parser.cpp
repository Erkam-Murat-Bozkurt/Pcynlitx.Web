


/*

Copyright ©  2021,  Erkam Murat Bozkurt

This file is part of the research project which is carried by Erkam Murat Bozkurt.

This is a free software: you can redistribute it and/or modify it under the terms
of the GNU General Public License as published by the Free Software Foundation
either version 3 of the License, or any later version.

This software is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE.

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program. If not, see <http://www.gnu.org/licenses/>.

*/

#include "Import_File_Parser.h"

Import_File_Parser::Import_File_Parser(){

     this->Memory_Delete_Condition = true;

     this->import_data_pointer = nullptr;
}

Import_File_Parser::Import_File_Parser(const Import_File_Parser & orig){}

Import_File_Parser::~Import_File_Parser(){

     if(!this->Memory_Delete_Condition){

        this->Clear_Dynamic_Memory();
     }
}

void Import_File_Parser::Clear_Dynamic_Memory(){

     if(!this->Memory_Delete_Condition){

         this->Memory_Delete_Condition = true;

         this->FileManager.Clear_Dynamic_Memory();
     }
}

void Import_File_Parser::Receive_HTML_File_Path(char * path){

    if(*path != '\0'){

       this->Memory_Delete_Condition = false;

       this->infocol.Receive_HTML_File_Path(path);
    }
    else{
           std::cout << "\n\n ERROR:";

           std::cout << "\n A valid file name must be entered..";

           std::cout << "\n\n";

           exit(0);
    }
}

void Import_File_Parser::Construct_New_HTML(char * new_path){

     this->Collect_Information();

     this->Write_New_HTML(new_path);
}

void Import_File_Parser::Collect_Information(){

     this->infocol.Collect_Information();

     this->allocate_memory_for_input_data_pointer();

     this->Determine_Import_File_Sizes();

     this->allocate_memory_for_imput_file_data();

     int Imp_File_Num = this->infocol.Get_Input_File_Number();

     for(int i=0;i<Imp_File_Num;i++){

         this->Read_Import_File(this->infocol.Get_Input_File_Path(i),i);
     }
}

void Import_File_Parser::Read_Import_File(std::string path, int index){

     this->FileManager.SetFilePath(path);

     this->FileManager.FileOpen(Rf);

     int file_line_index = 0;

     do {

          std::string string_line = this->FileManager.ReadLine();

          this->C_String_Constructor(&(this->import_data_pointer[index].Import_File_Content[file_line_index]),string_line);

          file_line_index++;

     }while(!this->FileManager.Control_End_of_File());

     this->FileManager.FileClose();
}

void Import_File_Parser::Write_New_HTML(char * new_path){

     this->FileManager.SetFilePath(new_path);

     this->FileManager.FileOpen(RWCf);

     int import_file_index = 0;

     for(int i=0;i<this->infocol.Get_File_Size();i++){

         int import_line = 0;

         if(import_file_index < this->infocol.Get_Input_File_Number()){

            import_line = this->infocol.Get_Input_File_Position(import_file_index);
         }

         if(i != import_line -1){

            this->FileManager.WriteToFile(this->infocol.Get_File_Content(i));

            this->FileManager.WriteToFile("\n");
         }
         else{

               int loop_size = this->import_data_pointer[import_file_index].imput_line_number;

               for(int k=0;k<loop_size;k++){

                   char * pointer = this->import_data_pointer[import_file_index].Import_File_Content[k];

                    this->FileManager.WriteToFile(pointer);

                    this->FileManager.WriteToFile("\n");
               }

               import_file_index++;

               i++;
         }
     }
}

void Import_File_Parser::C_String_Constructor(char ** target_string, std::string std_string){

     size_t string_size = std_string.length();

     *target_string = new char [2*string_size];

     for(size_t i=0;i<string_size;i++){

         (*target_string)[i] = std_string[i];
     }

     (*target_string)[string_size] = '\0';
}

void Import_File_Parser::Determine_Import_File_Sizes(){

     int input_file_number = this->infocol.Get_Input_File_Number();

     for(int i=0;i<input_file_number;i++){

         std::string path = this->infocol.Get_Input_File_Path(i);

         char * target_file_path = this->FileManager.Conver_Std_String_To_Char(path);

         if(this->FileManager.Is_Path_Exist(target_file_path)){

            this->FileManager.SetFilePath(path);

            this->FileManager.FileOpen(Rf);

            this->import_data_pointer[i].imput_line_number = 0;

            do {

                 this->FileManager.ReadLine();

                 this->import_data_pointer[i].imput_line_number++;

            }while(!this->FileManager.Control_End_of_File());

            this->FileManager.FileClose();
         }
         else{

              std::cout << "\n the file path :" << path

                        << " does not exist .." << std::endl;

              std::cout << std::endl;

              exit(EXIT_FAILURE);
         }
     }
}

void Import_File_Parser::allocate_memory_for_imput_file_data(){

     int input_file_number = this->infocol.Get_Input_File_Number();

     for(int i=0;i<input_file_number;i++){

         int size = this->import_data_pointer[i].imput_line_number;

         this->import_data_pointer[i].Import_File_Content = new char * [5*size];
     }
}

void Import_File_Parser::allocate_memory_for_input_data_pointer(){

     int Imp_File_Num = this->infocol.Get_Input_File_Number();

     this->import_data_pointer = new import_file_data [2*Imp_File_Num];

}
