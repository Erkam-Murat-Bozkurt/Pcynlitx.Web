
#include "head_writer.h"

head_writer::head_writer(){

    this->template_file_path = nullptr;

    this->stylesheets = nullptr;

    this->stylesheet_number = 0;

    this->File_Line_Number = 0;
}

head_writer::head_writer(const head_writer & orig){


}

head_writer::~head_writer(){

    if(!this->Memory_Delete_Condition){

       this->Clear_Dynamic_Memory();
    }
}

void head_writer::Clear_Dynamic_Memory(){

     if(!this->Memory_Delete_Condition){

         this->Memory_Delete_Condition = true;

         this->FileManager.Clear_Dynamic_Memory();
     }
}

void head_writer::Receive_Template_File_Path(char * path){

     size_t path_name_size = strlen(path);

     this->template_file_path = new char [5*path_name_size];

     for(size_t i=0;i<path_name_size;i++){

         this->template_file_path[i] = path[i];
     }

     this->template_file_path[path_name_size] = '\0';
}

void head_writer::Collect_Information(){

      this->Read_Template_File();

      this->Determine_StyleSheet_Number();

      this->extract_stylesheets();

      this->extract_title_name();
}


void head_writer::Write_HTML_Header(char * target_html_path){

     char head_marker [] = "<head>";

     char head_close_marker [] = "</head>";

     char title_marker [] = "<title>";

     char title_close_marker [] = "</title>";

     char meta_config [] = "<meta charset=\"utf-8\">";

     this->FileManager.SetFilePath(target_html_path);

     this->FileManager.FileOpen(RWCf);

     this->FileManager.WriteToFile(head_marker);

     this->FileManager.WriteToFile("\n");

     this->FileManager.WriteToFile(title_marker);

     this->FileManager.WriteToFile(this->Get_Title_Name());

     this->FileManager.WriteToFile(title_close_marker);

     this->FileManager.WriteToFile("\n");

     this->FileManager.WriteToFile(meta_config);

     this->FileManager.WriteToFile("\n");

     for(int i=0;i<this->Get_Style_Sheet_Number();i++){

         this->FileManager.WriteToFile("<link rel=\"stylesheet\" href=\"");

         this->FileManager.WriteToFile(this->Get_Style_Sheet(i));

         this->FileManager.WriteToFile("\">");

         this->FileManager.WriteToFile("\n");
     }

     this->FileManager.WriteToFile(head_close_marker);

     this->FileManager.FileClose();
}

void head_writer::Read_Template_File(){

      this->FileManager.SetFilePath(this->template_file_path);

      this->FileManager.FileOpen(Rf);

      this->File_Line_Number = 0;

      do {

          this->FileManager.ReadLine();

          this->File_Line_Number++;

      }while(!this->FileManager.Control_End_of_File());

      this->FileManager.FileClose();


      this->template_file_content = new char * [5*this->File_Line_Number];

      this->FileManager.FileOpen(Rf);

      int file_line_index = 0;

      do {

           std::string string_line = this->FileManager.ReadLine();

           size_t string_size = string_line.length();

           this->template_file_content[file_line_index] = new char [5*string_size];

           this->C_String_Constructor(&(this->template_file_content[file_line_index]),string_line);

           file_line_index++;

      }while(!this->FileManager.Control_End_of_File());

       this->FileManager.FileClose();
}

void head_writer::C_String_Constructor(char ** target_string, std::string std_string){

     size_t string_size = std_string.length();

     *target_string = new char [2*string_size];

     for(size_t i=0;i<string_size;i++){

         (*target_string)[i] = std_string[i];
     }

     (*target_string)[string_size] = '\0';
}

void head_writer::extract_title_name(){

     char KeyWord [] = "#title:";

     for(int i=0;i<this->File_Line_Number;i++){

        if(this->StringManager.CheckStringInclusion(this->template_file_content[i],KeyWord)){

           size_t string_size = strlen(this->template_file_content[i]);

           this->title = new char [5*string_size];

           size_t title_start_position = 0;

           size_t end_position = 0;

           for(size_t k=0;k<string_size;k++){

              if(this->template_file_content[i][k] == ':'){

                 title_start_position = k+1;

                 break;
              }
           }

           if(title_start_position == 0){

              std::cout << "\n Syntax Error in Template file ..";

              exit(0);
           }

           for(size_t k=title_start_position;k<string_size;k++){

              if(this->template_file_content[i][k] == ';'){

                 end_position = k;

                 break;
              }
           }

           if(end_position == 0){

              std::cout << "\n Syntax Error in Template file ..";

              exit(0);
           }

           int index = 0;

           for(size_t k=title_start_position;k<end_position;k++){

               this->title[index] = this->template_file_content[i][k];

               index++;
           }

           this->title[index] = '\0';

           break;
         }
       }
}

void head_writer::extract_stylesheets(){

     char KeyWord [] = "#stylesheet:";

     int stylesheet_index = 0;

     for(int i=0;i<this->File_Line_Number;i++){

        if(this->StringManager.CheckStringInclusion(this->template_file_content[i],KeyWord)){

           size_t string_size = strlen(this->template_file_content[i]);

           this->stylesheets[stylesheet_index] = new char [5*string_size];

           size_t style_start_position = 0;

           size_t end_position = 0;

           for(size_t k=0;k<string_size;k++){

              if(this->template_file_content[i][k] == ':'){

                 style_start_position = k+1;

                 break;
              }
           }

           if(style_start_position == 0){

              std::cout << "\n Syntax Error in Template file ..";

              exit(0);
           }

           for(size_t k=style_start_position;k<string_size;k++){

              if(this->template_file_content[i][k] == ';'){

                 end_position = k;

                 break;
              }
           }

           if(end_position == 0){

              std::cout << "\n Syntax Error in Template file ..";

              exit(0);
           }

           int index = 0;

           for(size_t k=style_start_position;k<end_position;k++){

              this->stylesheets[stylesheet_index][index] = this->template_file_content[i][k];

              index++;
           }

           this->stylesheets[stylesheet_index][index] = '\0';

           stylesheet_index++;
        }
     }
}

void head_writer::Determine_StyleSheet_Number(){

     char KeyWord [] = "#stylesheet";

     for(int i=0;i<this->File_Line_Number;i++){

        if(this->StringManager.CheckStringInclusion(this->template_file_content[i],KeyWord)){

           this->stylesheet_number++;
        }
     }

     this->stylesheets = new char * [2*this->stylesheet_number];

     // Memory allocation for the stylesheet file names;
}

char * head_writer::Get_Title_Name(){

       return this->title;
}

char * head_writer::Get_Style_Sheet(int num){

      return this->stylesheets[num];
}

int head_writer::Get_Style_Sheet_Number(){

    return this->stylesheet_number;
}
