//from my MAC -> copied into the template in VM

#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <stdbool.h> 
#include <sys/wait.h>
#define DELIMS " \t\r\n"  //delimeters 
//#define CHDR 6
#define EXIT 0
#define TYPE 1
#define COPY 2
#define DELETE 3
//#define EXECUTE 4 //--> NEED TO IMPLEMENT
#define ERROR 5

//list of functions implemented
void error(char *command);
void delete(char *filename);
void type(char *filename);
void copy(char *name1, char *name2);//issues?!
//void execute(char *filename); //-->NEED TO IMPLEMENT
//void chdirec(char *command); 
char *fFilename(char *command, int cmdType);
char *secFilename(char *command, int cmdType);
bool fileExists(char *filename);
int cmdArgSize(char *command);
int commandType(char *command);

char *name1;//first file name
char *name2;//2 file name

/*void chdirec(char *command) {
    if(command = strtok(command, DELIMS)){
        if(strcmp(command, "cd") == 0) {
    char *arg = strtok(0, DELIMS);
    if(!arg)
      printf("MISSING ARGUMENT!\n FORMAT: cd <argument>\n");
    else
      chdir(arg);
  }
  return;
}
}*/
//displays an error message if command not found
void error(char *command) {
  printf("COMMAND NOT FOUND: %s\n", command);
}

//deletes a file (filename) 
void delete(char *filename) {
  //if the file exists
  if(fileExists(filename)) {
    remove(filename);//removes the file
    printf("FILE DELETED: \'%s\'\n", filename);//let the user know which file was just deleted
  }else//otherwise the file never existed
    printf("FILE DOES NOT EXIST: \'%s\'\n", filename);
}

//prints the contents of filename
void type(char *filename) {
  //if file exists 
  if(fileExists(filename)) {//should check if file exists
    int lex;
    FILE *file;
    file = fopen(filename, "r");//should read file
    //if open
    if(file) {
      //while there is something in the file
      while ((lex = getc(file)) != EOF)
        putchar(lex);//puts the character on the screen

      printf("\n");//goes to next line
      fclose(file);//close the file
    }
   }else//otherwise alert user file does NOT exist
    printf("FILE DOES NOT EXIST: \'%s\'", filename);
}
//NOT WORKING RIGHT!!!!!!
//looked at various examples online but not 1 specific - in my head it should work :/
//copies a file (contents of file) from source to a 'new' destination 
void copy(char *name1, char *name2) {
  //if the file exists 
  if(fileExists(name1)) 
    printf("SOURCE FILE DOES NOT EXIST: \'%s\'\n", name1);
  else{//otherwise alert user the source file does NOT exist
    int lex;
    char c;
    FILE *sfile;
    FILE *dfile;
    sfile = fopen(name1, "r");//opens the original source file
    dfile = fopen(name2, "w");//opens 'new' destination file
    //if files both open
    if (sfile && dfile) {
      /*while((c = fgetc(sfile)) != EOF) {
        fputc(c, dfile);
      }*/
      /*while((lex = getc(sfile)) != EOF){
        lex= fgetc(sfile);
        if(!EOF(sfile))
          fputc(lex, dfile);
        else
          break;
      }*/
      //while the file still has something
      while ((lex = getc(sfile)) != EOF){
        putc(lex, dfile);//prints character to destination file (the copy)
      }
      fclose(sfile);//close the source file
      fclose(dfile);//close the destination file
    }
  }
}

//returns second actual word in command line input (1st filename)
char *fFilename(char *command, int cmdType) {
  char *word = command + cmdArgSize(command) + 1;
  char *numArg;
  if(cmdType == TYPE || cmdType == DELETE)
    numArg = strchr(word, '\0');
  else
    numArg = strchr(word, ' ');
  int nameLength = numArg - word + 1;
  name1 = (char *)malloc(nameLength);
  strncpy(name1, word, nameLength);
  return name1;//returns 1st name
}
 
//returns third actual word in command line input (2nd filename)
char *secFilename(char *command, int cmdType) {
  char *word;
  char *numArg;
  word = strchr(command, ' ');
  word = strchr(word + 1, ' ');
  numArg = strchr(word++ , '\0');
  int nameLen = numArg - word + 1;
  name2 = (char *)malloc(nameLen);
  strncpy(name2, word, nameLen);
  return name2;//returns 2nd name
}

//BOOLEAN -> checks if file exists
bool fileExists(char *filename) {
  if(access(filename, F_OK) != -1)
    return 1;//found
  return 0;//not found
}

//# of args
int cmdArg(char *command) {
  char *numArg;
  int i = 0;
  numArg = strchr(command, ' ');
  while(numArg != NULL) {
    i++;
    numArg = strchr(numArg + 1, ' ');
  }
  return i;//numArgs
}

//size of the command args(how many?)
int cmdArgSize(char* command) {
  char *numArg;
  numArg = strchr(command, ' ');
  //if # or args is NOT 0 return #
  if(numArg != NULL)
    return numArg - command;
  else//otherwise return just command
    return strlen(command);
}

//returns command type: in DEFINED: 0=exit, 1=type, 2=copy, 3=delete, 4=execute (order as in assingment)
int commandType(char *command) { 
  if(!strcmp(command, "exit"))//if exit
    return EXIT;
  else if(!strncmp(command, "type", 1))//if type
    return TYPE;
  else if(!strncmp(command, "copy", 2))//if copy
    return COPY;
  else if(!strncmp(command, "delete", 3))//if delete
    return DELETE;
  /*else if(fileExists(command))
    //return EXECUTE;
  else if(!strncmp(command, "cd",6))
    return CHDR;*/
  else
    return ERROR;
}

int main(int argc, char** argv) {
  //char *com;
  char command[100] = {0};
  while (1) {
    memset(command, 0 , strlen(command) + 1);//reset command memory
    printf(">tarciVM$");//"shell"
    scanf("%[^\n]%*c", command);//command input (with spaces)

    switch(commandType(command)) {
      case 0://= EXIT
        printf("BYE-BYE\n");
        abort();//can not just break - it won't break unless aborted
      
      case 1://= TYPE
        //invalid type format
        if(cmdArg(command) != 1) {
          printf("MISSING ARGUMENT!\nFORMAT: \'type <filename>\'\n");
          break;
        }else {
          type(fFilename(command, TYPE));//calls function type
          free(name1);//deallocates memory 
        }break;

      case 2://= COPY
        //invalid copy format
        if(cmdArg(command) != 2) {
          printf("MISSING AN ARGUMENT!\nFORMAT: \'copy <filename1_SOURCE> <filename2_DESTINATION>\'\n");
          break;
        }else {
          copy(fFilename(command, COPY), secFilename(command, COPY));//calls function copy
          free(name1);//deallocates memory 
          free(name2);//deallocates memory 
        }break;

      case 3://= DELETE
        //invalid delete format
        if(cmdArg(command) != 1) {
          printf("MISSING ARGUMENT TO BE DELETED!\n FORMAT:\'delete <filename>\'\n");
          break;
        }else {
          delete(fFilename(command, TYPE));//calls function delete
          free(name1);//deallocates memory 
        }break;
        //NEED TO IMPLEMENT
      /*case 4://= EXECUTE
        execute(command);
        break;*/
      /*case 6:
        chdirec(command);
        /*{
         if(com = strtok(com, DELIMS)){
          if(strcmp(com, "cd") == 0) {
          char *arg = strtok(0, DELIMS);
          if(!arg)
            printf("MISSING ARGUMENT!\n FORMAT: cd <argument>\n");
          else
            chdir(arg);//chnage dirctory
          }
         }*/
        //break;
       
      //default is error
      default:
        error(command);
    }
  }
  return 0;
}