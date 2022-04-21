/*************************************************************************
        Copyright © [2022] [Konstantinidis Konstantinos]

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.

See the License for the specific language governing permissions and
limitations under the License.
*************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct{
    int code;
    int recNo;
}BinTreeElementType;

typedef struct BinTreeNode *BinTreePointer;

struct BinTreeNode{
    BinTreeElementType Data;
    BinTreePointer LChild, RChild;
} ;

typedef enum{
    FALSE, TRUE
} boolean;

typedef struct{
    int code;
    char surname[20];
    char name[20];
    char sex;
    int year;
    float grade;
}StudentT;

void CreateBST(BinTreePointer *Root);
boolean BSTEmpty(BinTreePointer Root);
void RecBSTInsert(BinTreePointer *Root, BinTreeElementType Item);
void RecBSTSearch(BinTreePointer Root, BinTreeElementType KeyValue, boolean *Found, BinTreePointer *LocPtr);
void RecBSTInorder(BinTreePointer Root);
void menu(int *choice);
int BuildBST(BinTreePointer *Root);
void PrintStudent(int RecNum);
void printStudentsWithGrade();

int main(){
    FILE *fp;
    boolean found;
    BinTreePointer Root, LocPtr;
    int choice, size;
    BinTreeElementType indexRec;
    StudentT student;

    size = BuildBST(&Root);
    do{
        menu(&choice);
        if(choice == 1){
            fp = fopen("students_Sample.dat","a");
            if (fp == NULL){
                printf("Can't open students_Sample.dat\n");
                exit(1);
            }
            else{
                printf("Give student's AM: ");
                scanf("%d", &student.code);
                getchar();
                while(student.code<0){
                    printf("Code can't be a negative number\n");
                    printf("Give student's AM: ");
                    scanf("%d", &student.code);
                    getchar();
                }
                indexRec.code = student.code;
                RecBSTSearch(Root, indexRec, &found, &LocPtr);
                if(found == FALSE){
                    printf("Give student surname: ");
                    fgets(student.surname,20,stdin);
                    student.surname[strlen(student.surname)-1]='\0';

                    printf("Give student name: ");
                    fgets(student.name,20,stdin);
                    student.name[strlen(student.name)-1]='\0';

                    printf("Give student's registration year: ");
                    scanf("%d", &student.year);
                    getchar();

                    printf("Give student's grade(0-20): ");
                    scanf("%g", &student.grade);
                    getchar();
                    while(student.grade<0 || student.grade>20){
                        printf("Give student's grade(0-20): ");
                        scanf("%g", &student.grade);
                        getchar();
                    }

                    printf("Give student sex F/M: ");
                    scanf("%c", &student.sex);
                    getchar();
                    while(student.sex!='F' && student.sex!='M'){
                        printf("Give student sex F/M: ");
                        scanf("%c", &student.sex);
                        getchar();
                    }

                    printf("\nsize = %d\n", size);
                    indexRec.recNo = size;
                    RecBSTInsert(&Root, indexRec);
                    fprintf(fp,"%d, %s, %s, %c, %d, %g%c", student.code, student.surname, student.name,
                            student.sex, student.year, student.grade, '\n');

                    size++;
                    fclose(fp);
                }
                else
                    printf("A student with the same code is already in the tree");
            }
        }
        if(choice == 2){
            printf("Give student's code: ");
            scanf("%d", &student.code);
            indexRec.code = student.code;
            RecBSTSearch(Root, indexRec, &found, &LocPtr);
            if(found == TRUE){
                indexRec.recNo = LocPtr->Data.recNo;
                PrintStudent(indexRec.recNo);
            }
            else
                printf("There is no student with this code");
        }
        if(choice == 3){
            printf("Print all students (Code, Number of record) ascending order(Code): \n");
            RecBSTInorder(Root);
            printf("\n");
        }
        if(choice == 4){
            printf("Print students with a >= given grade: \n");
            printStudentsWithGrade();
        }
    }while(choice!=5);

    system("PAUSE");
}

void menu(int *choice){
    printf("\n                  MENU                  \n");
    printf("-------------------------------------------------\n");
    printf("1. Insert new student\n");
    printf("2. Search for a student\n");
    printf("3. Print all students (Traverse Inorder)\n");
    printf("4. Print students with a >= given grade\n");
    printf("5. Quit\n");
    printf("\nChoice: ");
    do{
        scanf("%d", choice);
        getchar();
    } while (*choice<1 && *choice>4);
}

void CreateBST(BinTreePointer *Root){*Root = NULL;}

boolean BSTEmpty(BinTreePointer Root){return (Root==NULL);}

void RecBSTInsert(BinTreePointer *Root, BinTreeElementType Item){
    if(BSTEmpty(*Root)){
        (*Root) = (BinTreePointer)malloc(sizeof (struct BinTreeNode));
        (*Root) ->Data.code = Item.code;
        (*Root) ->Data.recNo = Item.recNo;
        (*Root) ->LChild = NULL;
        (*Root) ->RChild = NULL;
    }
    else
        if (Item.code < (*Root) ->Data.code)
            RecBSTInsert(&(*Root) ->LChild, Item);
        else if (Item.code > (*Root) ->Data.code)
            RecBSTInsert(&(*Root) ->RChild, Item);
        else
            printf("This code already exists in the tree\n", Item.code);
}

void RecBSTSearch(BinTreePointer Root, BinTreeElementType KeyValue, boolean *Found, BinTreePointer *LocPtr){
    boolean stop;

    if (BSTEmpty(Root))
        *Found=FALSE;
    else
        if (KeyValue.code < Root->Data.code)
            RecBSTSearch(Root->LChild, KeyValue, &(*Found), &(*LocPtr));
        else
            if (KeyValue.code > Root->Data.code)
                RecBSTSearch(Root->RChild, KeyValue, &(*Found), &(*LocPtr));
            else{
                *Found = TRUE;
                *LocPtr = Root;
            }
}

void RecBSTInorder(BinTreePointer Root){
    if (Root!=NULL){
        RecBSTInorder(Root->LChild);
        printf("(%d, %d), ", Root->Data.code, Root->Data.recNo);
        RecBSTInorder(Root->RChild);
    }
}

int BuildBST(BinTreePointer *Root){
    FILE *fp;
    StudentT student;
    BinTreeElementType indexRec;
    int size, nscan;
    char termch;

    CreateBST(Root);

    size = 0;
    fp = fopen("students_Sample.dat","r");
    if (fp == NULL){
        printf("Can't open students_Sample.dat\n");
        exit(1);
    }
    else{
        while(TRUE){
            nscan = fscanf(fp,"%d, %20[^,], %20[^,], %c, %d, %g%c", &student.code, student.surname, student.name, &student.sex, &student.year, &student.grade, &termch);
            if (nscan == EOF) break;
            if (nscan != 7 || termch != '\n'){
                printf("Improper file format\n");
                break;
            }
            else{
                indexRec.code = student.code;
                indexRec.recNo = size;

                RecBSTInsert((Root), indexRec);
                size++;
            }
        }
    }
    fclose(fp);

    return size;
}

void PrintStudent(int RecNum){
    FILE *infile;
    int nscan, lines;
    char termch;
    StudentT student;

    lines = 0;

    infile = fopen("students_Sample.dat", "r");
    if (infile == NULL)
        printf("Can't open students_Sample.dat\n");
    else{
        while(lines <= RecNum){
            nscan = fscanf(infile,"%d, %20[^,], %20[^,], %c, %d, %g%c", &student.code, student.surname, student.name, &student.sex, &student.year, &student.grade, &termch);
            if (nscan == EOF) break;
            if (nscan != 7 || termch != '\n'){
                printf("Improper file format\n");
                break;
            }
            else
                lines++;
        }
        if(lines != RecNum)
            printf("%d, %s, %s, %c, %d, %0.2g\n", student.code, student.surname, student.name, student.sex, student.year, student.grade);
    }
    fclose(infile);
}

void printStudentsWithGrade(){
    FILE *infile;
    int nscan;
    char termch;
    StudentT student;
    float theGrade;

    printf("Give the grade: ");
    scanf("%g", &theGrade);

    infile = fopen("students_Sample.dat", "r");
    if (infile == NULL)
        printf("Can't open students_Sample.dat\n");
    else{
        while(TRUE){
            nscan = fscanf(infile,"%d, %20[^,], %20[^,], %c, %d, %g%c", &student.code, student.surname, student.name, &student.sex, &student.year, &student.grade, &termch);
            if (nscan == EOF) break;
            if (nscan != 7 || termch != '\n'){
                printf("Improper file format\n");
                break;
            }
            else
                if(student.grade >= theGrade)
                    printf("%d, %s, %s, %c, %d, %g\n", student.code, student.surname, student.name, student.sex, student.year, student.grade);
        }
    }
    fclose(infile);
}
