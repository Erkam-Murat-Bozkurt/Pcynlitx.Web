
#include "Import_Description_Reader.h"

Import_Description_Reader::Import_Description_Reader(){

     this->Memory_Delete_Condition = false;

     this->FilePath = nullptr;

     this->File_Content = nullptr;

     this->Input_File_Number = 0;

     this->File_Line_Number = 0;

     this->Data_Pointer = nullptr;

     this->Input_File_Paths = nullptr;
}

Import_Description_Reader::Import_Description_Reader(const Import_Description_Reader & orig){

}

Import_Description_Reader::~Import_Description_Reader(){

    if(!this->Memory_Delete_Condition){

       this->Clear_Dynamic_Memory();
    }
}

void Import_Description_Reader::Clear_Dynamic_Memory(){

     if(!this->Memory_Delete_Condition){

         this->Memory_Delete_Condition = true;

         if(this->File_Line_Number > 0){

           if(this->FilePath != nullptr){

              delete [] this->FilePath;
           }

           if(this->File_Content != nullptr){

             for(int i=0;i<this->File_Line_Number;i++){

                 delete [] this->File_Content[i];
             }

             delete [] this->File_Content;
           }
         }

         if(this->Input_File_Number){

           if(this->Data_Pointer != nullptr){

              for(int i=0;i<this->Input_File_Number;i++){

                  delete [] this->Data_Pointer[i].file_name;

                  delete [] this->Data_Pointer[i].html_string_line;
              }

              delete [] this->Data_Pointer;
           }

           if(this->Input_File_Paths != nullptr){

              delete [] this->Input_File_Paths;
           }
         }

         this->StringManager.Clear_Dynamic_Memory();

         this->FileManager.Clear_Dynamic_Memory();
     }
}

void Import_Description_Reader::Collect_Information(){

     this->Determine_HTML_File_Size();

     this->Allocate_Memory_For_Reading();

     this->Read_HTML_File();

     this->Determine_Imput_File_Number();

     this->Determine_Input_Files_Positions();

     this->Extract_Input_File_Paths();

     this->List_Input_File_Names();
}

void Import_Description_Reader::Receive_HTML_File_Path(char * path){

    if(*path != '\0'){

       this->Memory_Delete_Condition = false;

       this->File_Path_Receive_Status = true;

       this->FileManager.SetFilePath(path);
    }
    else{
           std::cout << "\n\n ERROR:";

           std::cout << "\n A valid file name must be entered..";

           std::cout << "\n\n";

           exit(0);
    }
}

void Import_Description_Reader::Determine_HTML_File_Size(){

     this->File_Line_Number = 0;

     this->FileManager.FileOpen(Rf);

     do {

          this->FileManager.ReadLine();

          this->File_Line_Number++;

     }while(!this->FileManager.Control_End_of_File());

     this->FileManager.FileClose();
}

void Import_Description_Reader::Allocate_Memory_For_Reading(){

     this->File_Content = new char * [5*this->File_Line_Number];

     for(int i=0;i<5*this->File_Line_Number;i++){

         this->File_Content[i] = nullptr;
     }
}

void Import_Description_Reader::Read_HTML_File(){

     this->FileManager.FileOpen(Rf);

     int index = 0;

     do {

          std::string string_line = this->FileManager.ReadLine();

          string_line.append(1,'\n');

          this->C_String_Constructor(&(this->File_Content[index]),string_line);

          index++;

     }while(!this->FileManager.Control_End_of_File());

     this->FileManager.FileClose();
}

void Import_Description_Reader::Determine_Imput_File_Number(){

     char KeyWord [] = "#import(";

     for(int i=0;i<this->File_Line_Number;i++){

        if(this->StringManager.CheckStringInclusion(this->File_Content[i],KeyWord)){

          this->Input_File_Number++;
        }
     }

     this->Data_Pointer = new Input_File_Data [2*this->Input_File_Number];

     // Memory allocation for the input file data
}

void Import_Description_Reader::Determine_Input_Files_Positions(){

     char KeyWord [] = "#import(";

     int index = 0;

     for(int i=0;i<this->File_Line_Number;i++){

         if(this->StringManager.CheckStringInclusion(this->File_Content[i],KeyWord)){

            this->place_c_string(&(this->Data_Pointer[index].html_string_line),

            this->File_Content[i]);

            this->Data_Pointer[index].input_position = i;

            index++;
         }
     }
}

void Import_Description_Reader::Extract_Input_File_Paths(){

     for(int i=0; i<this->Input_File_Number; i++){

         char * string_line = this->Data_Pointer[i].html_string_line;

         // string_line is The readed HTML file line

         size_t start_position = 0;

         size_t end_position = 0;

         this->Find_Imput_Start_Point(string_line,&start_position);

         this->Find_Imput_End_Point(string_line,start_position,&end_position);

         this->Find_Imput_File_Path(string_line,start_position,end_position,i);
      }
}

void Import_Description_Reader::Find_Imput_Start_Point(char * string_line,

     size_t * start){

     size_t sharp_position = 0;

     size_t string_size = strlen(string_line);

     for(size_t i=0;i<string_size;i++){

         if(string_line[i] == '#'){

             sharp_position = i;

             break;
         }
     }

     size_t first_brace_position = 0;

     for(size_t i=sharp_position;i<string_size;i++){

         if(string_line[i] == '('){

             first_brace_position = i;

             break;
         }
     }

     *start = first_brace_position + 1;
}

void Import_Description_Reader::Find_Imput_End_Point(char * string_line,

     size_t start_pos, size_t * end_pos)
     {

     bool end_point_find_status = false;

     size_t string_size = strlen(string_line);

     for(size_t i=start_pos; i<string_size;i++){

         if(string_line[i] == ' '){

           end_point_find_status = false;

           break;
         }

         if(string_line[i] == ')')
         {
            *end_pos = i;

            end_point_find_status = true;

            break;
         }
     }

     if(!end_point_find_status){

         std::cout << "\n the file name can not be readed ";

         exit(EXIT_FAILURE);
     }
}

void Import_Description_Reader::Find_Imput_File_Path(char * string_line,

     size_t start_pos,

     size_t end_pos, int index){

     size_t string_size = end_pos - start_pos;

     this->Data_Pointer[index].file_name = new char [2*string_size];

     int name_index = 0;

     for(size_t i=start_pos; i<end_pos; i++){

         this->Data_Pointer[index].file_name[name_index] = string_line[i];

         name_index++;
     }

     this->Data_Pointer[index].file_name[name_index] = '\0';
}

void Import_Description_Reader::List_Input_File_Names(){

     this->Input_File_Paths = new char * [2*this->Input_File_Number];

     for(int i=0;i<this->Input_File_Number;i++){

        this->place_c_string(&(this->Input_File_Paths[i]),

             this->Data_Pointer[i].file_name);
     }
}

void Import_Description_Reader::C_String_Constructor(char ** target_string, std::string std_string){

     size_t string_size = std_string.length();

     *target_string = new char [2*string_size];

     for(size_t i=0;i<string_size;i++){

         (*target_string)[i] = std_string[i];
     }

     (*target_string)[string_size] = '\0';
}

void Import_Description_Reader::place_c_string(char ** pointer, char * string){

     size_t string_size = strlen(string);

     *pointer = new char [2*string_size];

     for(size_t i=0;i<string_size;i++){

        (*pointer)[i] = string[i];
     }

     (*pointer)[string_size] = '\0';
}

char * Import_Description_Reader::Get_Input_File_Path(int num){

     return this->Input_File_Paths[num];
}

int Import_Description_Reader::Get_Input_File_Position(int num){

    return this->Data_Pointer[num].input_position;
}

int Import_Description_Reader::Get_Input_File_Number(){

    return this->Input_File_Number;
}

char * Import_Description_Reader::Get_File_Content(int num){

     return this->File_Content[num];
}

int Import_Description_Reader::Get_File_Size(){

    return this->File_Line_Number;
}
