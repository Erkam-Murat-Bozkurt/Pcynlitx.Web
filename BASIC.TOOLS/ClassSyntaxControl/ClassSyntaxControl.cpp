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

#include "ClassSyntaxControl.h"

ClassSyntaxControl::ClassSyntaxControl(){

     this->Class_Word_Line = nullptr;

     this->Memory_Delete_Condition = true;
}

ClassSyntaxControl::ClassSyntaxControl(const ClassSyntaxControl & orig){

}

ClassSyntaxControl::~ClassSyntaxControl(){

     if(!this->Memory_Delete_Condition){

         this->Clear_Dynamic_Memory();
     }
}

void ClassSyntaxControl::Clear_Dynamic_Memory(){

     if(!this->Memory_Delete_Condition){

         this->Memory_Delete_Condition = true;

         if(this->Class_Word_Line != nullptr){

            delete [] this->Class_Word_Line;

            this->Class_Word_Line = nullptr;
         }

         this->FileManager.Clear_Dynamic_Memory();
     }
}

void ClassSyntaxControl::CheckClassSyntax(char * HeaderFile){

     this->Memory_Delete_Condition = false;

     this->ClassSyntaxErrorStatus = false;

     this->is_there_a_public_key_word = false;

     this->is_there_a_private_key_word = false;

     this->is_there_a_protected_key_word = false;

     this->FileManager.SetFilePath(HeaderFile);

     this->StringOperations.SetFilePath(HeaderFile);

     if(!this->GetClassSyntaxErrorStatus()){

          this->CheckStartBrace();
     }

     if(!this->GetClassSyntaxErrorStatus()){

         this->DetermineClassWordLine();
     }

     if(!this->GetClassSyntaxErrorStatus()){

         this->Read_Class_Word_Line();
     }

     if(!this->GetClassSyntaxErrorStatus()){

         this->CheckClassWord();
     }

     if(!this->GetClassSyntaxErrorStatus()){

         this->CheckClassName();
     }

     if(!this->GetClassSyntaxErrorStatus()){

         this->CheckEndBrace();
     }

     if(!this->GetClassSyntaxErrorStatus()){

         this->CheckPublicKeyWord();
     }

     if(!this->GetClassSyntaxErrorStatus()){

         this->CheckPrivateKeyWord();
     }

     if(!this->GetClassSyntaxErrorStatus()){

        this->CheckProtectedKeyWord();
     }

     !((this->isTherePublicKeyWord())

          || (this->isTherePrivateKeyWord()) ||

          (this->isThereProtectedKeyWord()));

     if(!this->GetClassSyntaxErrorStatus()){

         if(!((this->isTherePublicKeyWord())

              || (this->isTherePrivateKeyWord()) ||

              (this->isThereProtectedKeyWord()))){

              this->ClassSyntaxErrorStatus = true;
         }
     }

     this->FileManager.Clear_Dynamic_Memory();

     this->CharacterOperations.Clear_Dynamic_Memory();

     this->StringOperations.Clear_Dynamic_Memory();
}

void ClassSyntaxControl::CheckStartBrace(){

     this->FileManager.FileOpen(Rf);

     this->FirstBraceLine = 1;

     int FirstBraceLine = 1;

     char * linePointer = this->FileManager.ReadLine_as_Cstring();

     if(this->CharacterOperations.CharacterCheck(linePointer,'{')){

        this->FirstBraceLine = FirstBraceLine;
     }
     else{
            while(!this->CharacterOperations.CharacterCheck(linePointer,'{')){

                   FirstBraceLine++;

                   linePointer = this->FileManager.ReadLine_as_Cstring();

                   if(this->CharacterOperations.CharacterCheck(linePointer,'{')){

                      this->FirstBraceLine = FirstBraceLine;

                      break;
                   }

                   if(this->FileManager.Control_End_of_File()){

                      this->ClassSyntaxErrorStatus = true;

                      break;
                   }
           }
     }

     this->FileManager.FileClose();
}

void ClassSyntaxControl::DetermineClassWordLine(){

     char class_word [] = "class ";

     char comment_start [] = "/*";

     char comment_end [] = "*/";

     bool class_specifier_exist = false;

     this->FileManager.FileOpen(Rf);

     this->ClassWordLineNumber = 0;

     int line_index = 0;

     do{

         char * file_line = this->FileManager.ReadLine_as_Cstring();

         bool is_this_a_comment_start = this->StringOperations.CheckStringInclusion(file_line,comment_start);

         if(is_this_a_comment_start){

            bool is_comment_end = false;

            do{

               is_comment_end =  this->StringOperations.CheckStringInclusion(file_line,comment_end);

               if(is_comment_end){

                 break;
               }
               else{

                   file_line = this->FileManager.ReadLine_as_Cstring();

                   line_index++;
               }

            }while(is_comment_end);

            file_line = this->FileManager.ReadLine_as_Cstring();

            line_index++;
         }

         bool is_class_specifer_exist = this->StringOperations.CheckStringInclusion(file_line,class_word);

         if(is_class_specifer_exist){

            class_specifier_exist = true;

            this->FileManager.FileClose();

            this->ClassWordLineNumber = line_index+1;

            return;
         }
         else{

              line_index++;
         }

     }while(!this->FileManager.Control_End_of_File());

     if(!class_specifier_exist){

        this->ClassSyntaxErrorStatus = true;

        this->FileManager.FileClose();

        return;
     }
}

void ClassSyntaxControl::Read_Class_Word_Line(){

     if(!this->GetClassSyntaxErrorStatus()){

         this->FileManager.FileOpen(Rf);

         this->CharacterOperations.ForwardFilePointer(&this->FileManager,this->GetClassWordLineNumber());

         char * buffer = this->FileManager.ReadLine_as_Cstring();

         int Line_Size = strlen(buffer);

         this->Class_Word_Line = new char [10*Line_Size];

         for(int i=0;i<Line_Size;i++){

             this->Class_Word_Line[i] = buffer[i];
         }

         this->Class_Word_Line[Line_Size] = '\0';

         int Position = 0, Start_Point = 0;

         this->Find_Next_Character_Position(this->Class_Word_Line,&Position,Start_Point);

         this->Shift_List(&this->Class_Word_Line,Position,Start_Point);

         Line_Size = strlen(this->Class_Word_Line);

         int index_counter = Line_Size;

         while((this->Class_Word_Line[index_counter] == ' ') || (this->Class_Word_Line[index_counter] == ' ')){

                this->Class_Word_Line[index_counter] = '\0';

                index_counter--;
         }

         int Space_Number = this->Space_Counter(this->Class_Word_Line);

         Position = 0;

         for(int i=0;i<Space_Number;i++){

             Start_Point = Position;

             Find_Next_Space_Position(this->Class_Word_Line,&Position,Start_Point);

             Start_Point = Position;

             Find_Next_Character_Position(this->Class_Word_Line,&Position,Start_Point);

             Shift_List(&this->Class_Word_Line,Position-Start_Point-1,Start_Point);
         }

         Line_Size = strlen(this->Class_Word_Line);

         if(this->Class_Word_Line[Line_Size-1] == '\n'){

            this->Class_Word_Line[Line_Size-1] = '\0';
         }

         Line_Size = strlen(this->Class_Word_Line);

         if(this->Class_Word_Line[Line_Size-1] == '{'){

            if(this->Class_Word_Line[Line_Size -2] == ' '){

               this->Class_Word_Line[Line_Size -2] = '{';

               this->Class_Word_Line[Line_Size-1] = '\0';
            }
         }

         this->FileManager.FileClose();
     }
     else{

           int Line_Size = 10;

           this->Class_Word_Line = new char [10*Line_Size];

           for(int i=0;i<Line_Size;i++){

               this->Class_Word_Line[i] = '\0';
           }
     }
}

void ClassSyntaxControl::CheckClassWord(){

     char class_word[] = {'c','l','a','s','s','\0'};

     int ListLength = strlen(this->Get_Class_Word_Line());

     bool is_There_a_space = false;

     for(int i=0;i<ListLength;i++){

         if(this->Get_Class_Word_Line()[i] == ' '){

            is_There_a_space = true;
         }
     }

     if(!is_There_a_space){

        this->ClassSyntaxErrorStatus = true;
     }
     else
     {

       int FirstCharacterPoint = 0, NextSpacePoint =0;

       while(this->Get_Class_Word_Line()[FirstCharacterPoint]==' '){

           FirstCharacterPoint++;
       }

       NextSpacePoint = FirstCharacterPoint;

       while(this->Get_Class_Word_Line()[NextSpacePoint]!= ' '){

            NextSpacePoint++;
       }

       int FirstWordSize = NextSpacePoint - FirstCharacterPoint;

       char FirstWord [5*FirstWordSize];

       for(int i=0;i<FirstWordSize;i++){

           FirstWord[i] = this->Get_Class_Word_Line()[FirstCharacterPoint+i];
       }

       FirstWord[FirstWordSize]= '\0';

       if(this->CharacterOperations.CompareString(FirstWord,class_word)){

           this->is_there_a_class_word = true;
       }
       else
       {
           this->ClassSyntaxErrorStatus = true;
       }
     }
}

void ClassSyntaxControl::CheckClassName(){

     if(!this->GetClassSyntaxErrorStatus()){

         bool is_there_a_first_space = false, is_there_a_character_after_space = false;

         int FirstCharacterPoint = 0, FirstSpacePoint =0;

         while(this->Get_Class_Word_Line()[FirstCharacterPoint] ==' '){

               FirstCharacterPoint++;
         }

         int ListLength = strlen(this->Get_Class_Word_Line());

         for(int i=FirstCharacterPoint;i<ListLength;i++){

             if(this->Get_Class_Word_Line()[i] == ' '){

                is_there_a_first_space = true;

                FirstSpacePoint = i;

                break;
             }
         }

         if(is_there_a_first_space){

            for(int i=FirstSpacePoint;i<ListLength;i++){

                if(this->Get_Class_Word_Line()[i] != ' '){

                   is_there_a_character_after_space = true;

                   break;
                }
            }
         }

         if((is_there_a_first_space) && (is_there_a_character_after_space)){

             this->is_there_a_class_name = true;
         }
         else{

             this->ClassSyntaxErrorStatus = true;
         }
      }
}

void ClassSyntaxControl::CheckEndBrace(){

     this->FileManager.FileOpen(Rf);

     this->LastBraceLine = 1;

     char * linePointer = this->FileManager.ReadLine_as_Cstring();

     if(!this->CharacterOperations.CharacterCheck(linePointer,'}')){

         while(!this->CharacterOperations.CharacterCheck(linePointer,'}')){

                this->LastBraceLine ++;

                linePointer = this->FileManager.ReadLine_as_Cstring();

                if(this->CharacterOperations.CharacterCheck(linePointer,'}')){

                    break;
                }

                if(this->FileManager.Control_End_of_File()){

                    this->ClassSyntaxErrorStatus = true;

                    break;
                }
         }
     }

     this->FileManager.FileClose();
}

void ClassSyntaxControl::CheckPublicKeyWord(){

     char public_word [] = "public:";

     this->is_there_a_public_key_word =

          this->Control_Str_Word_Existance(public_word);
}

void ClassSyntaxControl::CheckPrivateKeyWord(){

     char private_word [] = "private:";

     this->is_there_a_private_key_word =

          this->Control_Str_Word_Existance(private_word);
}

void ClassSyntaxControl::CheckProtectedKeyWord(){

     char protected_word [] = "protected:";

     this->is_there_a_protected_key_word =

          this->Control_Str_Word_Existance(protected_word);
}

void ClassSyntaxControl::Place_Null_for_Array(char Array [], int Array_Size){

     for(int i=0;i<Array_Size;i++){

         Array[i] = '\0';
     }
}

void ClassSyntaxControl::Place_String(char ** Pointer, char * String){

     int String_Size = strlen(String);

     for(int i=0;i<String_Size;i++){

         (*Pointer)[i] = String[i];
     }

     (*Pointer)[String_Size] = '\0';
}

void ClassSyntaxControl::Shift_List(char ** Pointer, int Step_Size, int Start_Point){

     int List_Size = strlen((*Pointer));

     for(int i=Start_Point;i< List_Size - Step_Size;i++){

         (*Pointer)[i] = (*Pointer)[i+Step_Size];
     }

     (*Pointer)[List_Size - Step_Size] = '\0';
}

void ClassSyntaxControl::Find_Next_Character_Position(char * Pointer, int * Position, int Start_Point){

     int List_Size = strlen(Pointer);

     for(int i=Start_Point;i<List_Size;i++){

         if(Pointer[i] != ' '){

            *Position = i;

            break;
         }
     }
}

void ClassSyntaxControl::Find_Next_Space_Position(char * Pointer,int * Position, int Start_Point){

     int List_Size = strlen(Pointer);

     for(int i=Start_Point;i<List_Size;i++){

         if(Pointer[i] == ' '){

            *Position = i;

             break;
         }
     }
}

int ClassSyntaxControl::Space_Counter(char * Pointer){

    int List_Size = strlen(Pointer);

    this->Seperate_Space_Number = 0;

    for(int i=0;i<List_Size;i++){

        if(Pointer[i] == ' '){

           this->Seperate_Space_Number++;

           while(Pointer[i] == ' '){

                i++;
           }
        }
    }

    return this->Seperate_Space_Number;
}

bool ClassSyntaxControl::Control_Str_Word_Existance(char * string){

     this->is_string_exist = false;

     this->FileManager.FileOpen(Rf);

     do{

       char * file_line = this->FileManager.ReadLine_as_Cstring();

       bool is_string_exist = this->StringOperations.CheckStringInclusion(file_line,string);

       if(is_string_exist){

          this->is_string_exist = true;

          this->FileManager.FileClose();

          return this->is_string_exist;
       }

      }while(!this->FileManager.Control_End_of_File());

      this->FileManager.FileClose();

      return this->is_string_exist;
}

void ClassSyntaxControl::SetClassSyntaxErrorStatus(bool status){

     this->ClassSyntaxErrorStatus = status;
}


bool ClassSyntaxControl::isTherePublicKeyWord(){

     return this->is_there_a_public_key_word;
}

bool ClassSyntaxControl::isTherePrivateKeyWord(){

     return this->is_there_a_private_key_word;
}

bool ClassSyntaxControl::isThereProtectedKeyWord(){

     return this->is_there_a_protected_key_word;
}

bool ClassSyntaxControl::GetClassSyntaxErrorStatus(){

     return this->ClassSyntaxErrorStatus;
}

int ClassSyntaxControl::GetLastBraceLine(){

    return this->LastBraceLine;
}

bool ClassSyntaxControl::isThereClassWord(){

     return this->is_there_a_class_word;
}

bool ClassSyntaxControl::isThereClassName(){

     return this->is_there_a_class_name;
}

int ClassSyntaxControl::GetClassWordLineNumber(){

     return this->ClassWordLineNumber;
}

int ClassSyntaxControl::GetFirstBraceLine(){

     return this->FirstBraceLine;
}

char * ClassSyntaxControl::Get_Class_Word_Line(){

       return this->Class_Word_Line;
}
