// FrameWork for CRUDS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MENU "Menu.cfg"
#define FIELDS "Fields.cfg"
#define FIELD_VALUE_LENGTH "FieldValueLength.cfg"
#define FIELD_VALUE "CRUDS_FrameWork.dat"
#define CUSTOMISED_MESSAGES "Customised_Messages.cfg"
#define LINE_LENGTH 100

FILE *fpMenu, *fpFields, *fpFieldValueLength, *fpFieldValue, *fpMessage;

int FieldsCount = 0, FieldValueLength, choice, printMessagesCount = 0;
char **FieldsArray, *FieldValue, **PrintMessagesArray;

char** getPrintMessagesArray();
char** getFieldsArray();
int getFieldValueLength();
int getchoice();
char* getFieldData(char []);
int getRecordPosition(char []);
void showMenu();
void addRecords();
void showRecords();
void search(char *);
void update(char *);
void delete(char *);
char* getID();
void printMessage(int, char []);
int PrintMessagesCount();
char* getNewValue();


void main()
{
    fpFields = fopen(FIELDS, "r");
    FieldsCount = getFieldsCount();
    FieldValueLength = getFieldValueLength();
    FieldsArray = getFieldsArray();
    printMessagesCount = getPrintMessagesCount();
    PrintMessagesArray = getPrintMessagesArray();
    while(1)
    {
        showMenu();
    }    
}

int getFieldValueLength()
{
    fpFieldValueLength = fopen(FIELD_VALUE_LENGTH, "r");
    fscanf(fpFieldValueLength, "%d", &FieldValueLength);
    fclose(fpFieldValueLength);
    return FieldValueLength;
}

int getchoice()
{
    printf("\n Enter your choice: ");
    scanf("%d", &choice);
    return choice;
}

int getFieldsCount()
{
    char Fields[LINE_LENGTH];
    rewind(fpFields);
    while(fgets(Fields, LINE_LENGTH, fpFields))
    {
       FieldsCount++;
    } 
    return FieldsCount;   
}

int getPrintMessagesCount()
{
    char printMessage[LINE_LENGTH];
    fpMessage = fopen(CUSTOMISED_MESSAGES, "r");
    while(fgets(printMessage, LINE_LENGTH, fpMessage))
    {
        printMessagesCount++;
    }
    return printMessagesCount;    
}

char** getPrintMessagesArray()
{
    fpMessage = fopen(CUSTOMISED_MESSAGES, "r");
    int print_messages_counter;
    PrintMessagesArray = (char**)malloc(printMessagesCount * sizeof(char*));
    for (print_messages_counter = 0; print_messages_counter < printMessagesCount; print_messages_counter++)
    {
       PrintMessagesArray[print_messages_counter] = (char*)malloc(LINE_LENGTH * sizeof(char));
       fgets(PrintMessagesArray[print_messages_counter], LINE_LENGTH, fpMessage);
       PrintMessagesArray[print_messages_counter][strlen(PrintMessagesArray[print_messages_counter]) - 1] = '\0';
    }    
    return PrintMessagesArray;
}

char** getFieldsArray()
{
    rewind(fpFields);
    int fields_counter;
    FieldsArray = (char**)malloc(FieldsCount * sizeof(char*));
    for (fields_counter = 0; fields_counter < FieldsCount; fields_counter++)
    {
        FieldsArray[fields_counter] = (char*)malloc(LINE_LENGTH * sizeof(char));
        fgets(FieldsArray[fields_counter], LINE_LENGTH, fpFields);
        FieldsArray[fields_counter][strlen(FieldsArray[fields_counter]) - 1] = '\0';
    }
    return FieldsArray;    
}

void showMenu()
{
    char Menu[LINE_LENGTH];
    fpMenu = fopen(MENU, "r");
    while(fgets(Menu, LINE_LENGTH, fpMenu))
    {
        printf("%s", Menu);
    } 
    switch (getchoice())
    {
       case 1: addRecords();
               break;

       case 2: showRecords();
               break;

       case 3: update(getID());
               break;

       case 4: search(getID());
               break;

       case 5: delete(getID());
               break;

       case 0: fclose(fpFields);
               exit(0);        

       default: printf("Invalid option.\n");                                   
    }
    fclose(fpMenu);   
}

void addRecords()
{
   int fields_counter;
   for (fields_counter = 0; fields_counter < FieldsCount; fields_counter++)
   {
      strcpy(FieldValue, getFieldData(FieldsArray[fields_counter]));
      fpFieldValue = fopen(FIELD_VALUE, "a");
      fwrite(FieldValue, FieldValueLength, 1, fpFieldValue);
      if (fields_counter == FieldsCount - 1)
      {
        strcpy(FieldValue, "A");
        fwrite(FieldValue, FieldValueLength, 1, fpFieldValue);
      }  
      rewind(fpFieldValue);
      fread(FieldValue, FieldValueLength, 1, fpFieldValue);
      printMessage(1, FieldValue);
      fclose(fpFieldValue);

   } 
}

char* getFieldData(char *Field)
{
    FieldValue = (char*)malloc(FieldValueLength * sizeof(char));
    printf("Enter %s: ", Field);
    scanf(" %[^\n]s", FieldValue);
    return FieldValue;
}

void showRecords()
{
    FieldValue = (char*)malloc(FieldValueLength * sizeof(char));
    int fields_counter;
    fpFieldValue = fopen(FIELD_VALUE, "r");
    printf("------------Student Details------------\n");
    while(fread(FieldValue, FieldValueLength, 1, fpFieldValue))
    { 
       fseek(fpFieldValue, (FieldsCount - 1) * FieldValueLength, SEEK_CUR);
       fread(FieldValue, FieldValueLength, 1, fpFieldValue);
       if(strcmp(FieldValue, "A") == 0) 
       {
         fseek(fpFieldValue, -((FieldsCount + 1) * FieldValueLength), SEEK_CUR);
         fread(FieldValue, FieldValueLength, 1, fpFieldValue);
         for (fields_counter = 0; fields_counter < FieldsCount; fields_counter++)
         {
           printf("%s: %s\n", FieldsArray[fields_counter], FieldValue);
           fread(FieldValue, FieldValueLength, 1, fpFieldValue);
         }
         printf("\n-------------------------------------------------------\n");
        } 
    }
    fclose(fpFieldValue);       
}

int getRecordPosition(char ID[])
{
    int RecordPosition = -1;
    fpFieldValue = fopen(FIELD_VALUE, "r");
    FieldValue = (char*)malloc(FieldValueLength * sizeof(char));
    while(fread(FieldValue, FieldValueLength, 1, fpFieldValue))
    {
        if (strcmp(FieldValue, ID) == 0)
        {
          fseek(fpFieldValue, (FieldsCount - 1) * FieldValueLength, SEEK_CUR);
          fread(FieldValue, FieldValueLength, 1, fpFieldValue);
          if (strcmp(FieldValue, "A") == 0)
           {
              fseek(fpFieldValue, -((FieldsCount + 1) * FieldValueLength), SEEK_CUR);
              RecordPosition = ftell(fpFieldValue);
           } 
            break;
        }
        fseek(fpFieldValue, FieldsCount * FieldValueLength, SEEK_CUR);
    }
    fclose(fpFieldValue); 
    return RecordPosition;   
}

char* getID()
{
    char *ID = (char*)malloc(FieldValueLength * sizeof(char));
    printf("Enter %s: ", FieldsArray[0]);
    scanf("%s", ID);
    return ID;
}

void search(char ID[])
{
    int fields_counter, RecordPosition = getRecordPosition(ID);
    fpFieldValue = fopen(FIELD_VALUE, "r");
    if (RecordPosition > -1)
    {
       FieldValue = (char*)malloc(FieldValueLength * sizeof(char));
       fseek(fpFieldValue, RecordPosition, 0);
       fread(FieldValue, FieldValueLength, 1, fpFieldValue);
       for (fields_counter = 0; fields_counter < FieldsCount; fields_counter++)
       {
          printf("%s: %s\n", FieldsArray[fields_counter], FieldValue);
          fread(FieldValue, FieldValueLength, 1, fpFieldValue); 
       }    
    }
    else
    {
        printMessage(4, ID);
    } 
    fclose(fpFieldValue);   
}

char* getNewValue()
{
    char *NewValue = (char*)malloc(FieldValueLength * sizeof(char));
    printf("Enter new %s: ", FieldsArray[FieldsCount - 1]);
    scanf("%s", NewValue);
    return NewValue;
}

void update(char ID[])
{
    int fields_counter, RecordPosition = getRecordPosition(ID);
    fpFieldValue = fopen(FIELD_VALUE, "r+");
    if (RecordPosition > -1)
    {
       FieldValue = (char*)malloc(FieldValueLength * sizeof(char));
       fseek(fpFieldValue, RecordPosition, 0);
       fseek(fpFieldValue, (FieldsCount - 1) * FieldValueLength, SEEK_CUR);
       fread(FieldValue, FieldValueLength, 1, fpFieldValue);
       strcpy(FieldValue, getNewValue());
       fseek(fpFieldValue, -FieldValueLength, SEEK_CUR);
       fwrite(FieldValue, FieldValueLength, 1, fpFieldValue);
       fclose(fpFieldValue);
       printMessage(2, ID);
    } 
    else
    {
        printMessage(4, ID);
    }   
}

void delete(char ID[])
{
    int fields_counter, RecordPosition = getRecordPosition(ID);
    fpFieldValue = fopen(FIELD_VALUE, "r+");
    if (RecordPosition > -1)
    {
       FieldValue = (char*)malloc(FieldValueLength * sizeof(char));
       fseek(fpFieldValue, RecordPosition, 0);
       fseek(fpFieldValue, FieldsCount * FieldValueLength, SEEK_CUR);
       fread(FieldValue, FieldValueLength, 1, fpFieldValue);
       strcpy(FieldValue, "D");
       fseek(fpFieldValue, -FieldValueLength, SEEK_CUR);
       fwrite(FieldValue, FieldValueLength, 1, fpFieldValue);
       fclose(fpFieldValue);
       printMessage(3, ID);
    } 
    else
    {
        printMessage(4, ID);
    }    
}

void printMessage(int choice, char ID[])
{
  printf("%s- %s ", FieldsArray[0], ID);
  if (choice == 1)
  {
    printf("%s\n", PrintMessagesArray[0]);
  }
  else if (choice == 2)
  {
    printf("%s\n", PrintMessagesArray[1]);
  }
  else if (choice == 3)
  {
    printf("%s\n", PrintMessagesArray[2]);
  }  
  else
  {
    printf("%s\n", PrintMessagesArray[3]);
  }  
}