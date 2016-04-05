/* 
 * File:   main.cpp
 * Author: william
 *
 * Created on 2016年4月2日, 下午 8:27
 */

#include <stdlib.h>
# include <stdio.h>
# include <iostream>
# include <string.h>

using namespace std;

enum TokenType { IDENTIFIER = 34512, CONSTANT = 87232, SPECIAL = 29742, NOTHING = 11111 } ;

typedef char * charPtr ;

struct Column {
  int column ;
  Column * next ; // 下一個此token出現過的Column(同一Line)
} ;

typedef Column * ColumnPtr ;

struct Line {
  int line ;
  ColumnPtr firstAppearAt ; //第一次出現所在的Column
  ColumnPtr lastAppearAt ; //最後出現所在的Column
  Line * next ;
} ;

typedef Line * LinePtr ;

struct Token {
  charPtr tokenStr ;
  TokenType type ;
  LinePtr firstAppearOn ;
  LinePtr lastAppearOn ;
  Token * next ;
} ;

typedef Token * TokenPtr ;

TokenPtr gFront = NULL ;
TokenPtr gRear = NULL ;
int gColumn = 1 ;
int gLine = 1 ;
bool gMoreToken = false ;
char gMoreTokenChar = '\0' ;

typedef char Str100[100] ;

charPtr GetToken( TokenType & type ) ;
void CreateToken( TokenPtr & walkr, charPtr input, TokenType type ) ;
void CreateLine( LinePtr & head, LinePtr & tail, int length ) ;
void CreateColumn( ColumnPtr & head, ColumnPtr & tail, int length ) ;
void PrintToken( TokenPtr walkr ) ;
void PrintLine( LinePtr head ) ;
void PrintColumn( int line, ColumnPtr head ) ;

int main(int argc, char** argv) {
  TokenPtr walkr = NULL ;
  TokenType type ;
  charPtr input ;
  int command = 0 ;
  
  input = new Str100 ;
  strcpy( input, GetToken( type ) ) ; 
 
  while ( strcmp( input, "END_OF_FILE" ) != 0 ) {
    if ( type == IDENTIFIER ) {
      CreateToken( walkr, input, type ) ;
    } // if
    else if ( type == CONSTANT ) {
      CreateToken( walkr, input, type ) ;
    } // if
    else if ( type == SPECIAL ) {
      CreateToken( walkr, input, type ) ;
    } // if
    
    type = NOTHING ;
    strcpy( input, GetToken( type ) ) ; 
  } // while
  
  gFront = walkr ;
  
  PrintToken( walkr ) ;
  
  printf( "歡迎來到\n" ) ;
  
  scanf( "%d", &command ) ;
  while ( command != 5 ) {
    if ( command == 1 ) {
      
    } // if
    else if ( command == 2 ) {
      
    } // if
    else if ( command == 3 ) {
      
    } // if
    else if ( command == 4 ) {
      
    } // if
    
    scanf( "%d", &command ) ;
  } // while  
  
  printf( "Sucess!!" ) ;
  return 0;
} // main()


charPtr GetToken( TokenType & type ) {
  charPtr returnPtr = new Str100 ;
  char input ;
  Str100 processStr ;
  
  if ( gMoreToken ) {
    input = gMoreTokenChar ;
    gMoreToken = false ;
  } // if
  else
    scanf( "%c", &input ) ;
  
  if ( input == '?' || input == ':' || input == ';' || input == '#' || input == '^' || 
       input == '.' || input == ',' || input == '(' || input == ')' || input == '[' ||
       input == ']' || input == '{' || input == '}' ) { // 單獨的特殊符號
    sprintf( processStr, "%c", input ) ;
    strcpy( returnPtr, processStr ) ;
    type = SPECIAL ;
    return returnPtr ;
  } // if
  else if ( input == '+' || input == '-' || input == '*' || input == '/' || input == '<' || input == '>' ||
            input == '=' || input == '%' || input == '&' || input == '!' || input == '|' ) { // 可能成對的特殊符號，但當第二個不是時把gMoreToken改成true
    sprintf( processStr, "%c", input ) ;
    strcpy( returnPtr, processStr ) ;
    type = SPECIAL ;
    scanf( "%c", &input ) ;
    if ( input == '=' || input == '&' || input == '>' || input == '<' ||
         input == '|' || input == '+' || input == '-' ) {
      sprintf( processStr, "%c", input ) ;
      strcat( returnPtr, processStr ) ;
      return returnPtr ; 
    } // if
    else if ( input == '/' ) { // 註解(//)
      while ( input != '\n' ) {
        scanf( "%c", &input ) ;
      } // while  
      
      type = NOTHING ;
      gLine = gLine + 1 ;
      gColumn = 1 ;
      return returnPtr ;
    } // if
    else if ( input == '*' ) { //註解(/* */)
      char test = '\0' ;
      scanf( "%c", &input ) ;
      while ( input != '*' || test != '/' ) {
        
        if ( input == '*' ) {
          scanf( "%c", &test ) ;
          if ( test == '\n' ) {
            gLine = gLine + 1 ;
            gColumn = 1 ;
          } // if 
        } // if  
        else {
          if ( input == '\n' ) {
            gLine = gLine + 1 ;
            gColumn = 1 ;
          } // if 
          
          scanf( "%c", &input ) ;        
        } // else       
      } // while  
      
      type = NOTHING ;
      return returnPtr ;
    } // if  
    else {
      gMoreToken = true ;
      gMoreTokenChar = input ;
      return returnPtr ;
    } // else  
  } // if
  else if ( ( ( 65 <= input ) && ( input <= 90 ) ) || 
            ( ( 97 <= input ) && ( input <= 122 ) ) || input == '_' ) { // 英文或是底線
    type = IDENTIFIER ;
    sprintf( processStr, "%c", input ) ;
    strcpy( returnPtr, processStr ) ;
    scanf( "%c", &input ) ;
    
    while ( strcmp( returnPtr, "END_OF_FILE" ) != 0 && ( ( ( 48 <= input ) && ( input <= 57 ) ) ||
            ( ( 65 <= input ) && ( input <= 90 ) ) || ( ( 97 <= input ) && ( input <= 122 ) ) || input == '_' ) ) {
      sprintf( processStr, "%c", input ) ;
      strcat( returnPtr, processStr ) ;
      scanf( "%c", &input ) ;
    } // while
    
    gMoreToken = true ;
    gMoreTokenChar = input ;
    return returnPtr ;
  } // if
  else if ( ( ( 48 <= input ) && ( input <= 57 ) ) || input == '.' ) { // 數字和'.'
    type = CONSTANT ;
    sprintf( processStr, "%c", input ) ;
    strcpy( returnPtr, processStr ) ;
    scanf( "%c", &input ) ;
    
    while ( ( ( 48 <= input ) && ( input <= 57 ) ) || input == '.' ) {
      sprintf( processStr, "%c", input ) ;
      strcat( returnPtr, processStr ) ;
      scanf( "%c", &input ) ;
    } // while
    
    gMoreToken = true ;
    gMoreTokenChar = input ;
    return returnPtr ;
  } // if
  else if ( input == '"' ) { // 字串(雙引號)
    type = CONSTANT ;
    sprintf( processStr, "%c", input ) ;
    strcpy( returnPtr, processStr ) ;
    scanf( "%c", &input ) ;
    
    while ( input != '"' ) {
      sprintf( processStr, "%c", input ) ;
      strcat( returnPtr, processStr ) ;
      if ( input == '\\' ) { // 如果是反斜線
        scanf( "%c", &input ) ;
        sprintf( processStr, "%c", input ) ;
        strcat( returnPtr, processStr ) ;
      } // if
      
      scanf( "%c", &input ) ;   
    } // while
    
    sprintf( processStr, "%c", input ) ;
    strcat( returnPtr, processStr ) ;
    gMoreToken = false ;
    return returnPtr ;
  } // if
  else if ( input == '\'' ) { // 字元(單引號)
    type = CONSTANT ;
    sprintf( processStr, "%c", input ) ;
    strcpy( returnPtr, processStr ) ;
    scanf( "%c", &input ) ;
    
    while ( input != '\'' ) {
      sprintf( processStr, "%c", input ) ;
      strcat( returnPtr, processStr ) ;
      if ( input == '\\' ) { // 如果是反斜線
        scanf( "%c", &input ) ;
        sprintf( processStr, "%c", input ) ;
        strcat( returnPtr, processStr ) ;
      } // if
      
      scanf( "%c", &input ) ;   
    } // while
    
    sprintf( processStr, "%c", input ) ;
    strcat( returnPtr, processStr ) ;
    gMoreToken = false ;
    return returnPtr ;
  } // if
  else if ( input == ' ' ) {
    gColumn = gColumn + 1 ;
    type = NOTHING ;
    return returnPtr ;
  } // if  
  else if ( input == '\n' ) {
    gLine = gLine + 1 ;
    gColumn = 1 ;
    type = NOTHING ;
    return returnPtr ;
  } // if  
} // GetToken()
 

void CreateToken( TokenPtr & walkr, charPtr input, TokenType type ) {
  TokenPtr temp = NULL ;
  int tokenLength = strlen( input ) ;
  
  if ( walkr == NULL ) {
    walkr = new Token ;
    walkr -> tokenStr = new Str100 ;
    strcpy( walkr -> tokenStr, input ) ;
    walkr -> type = type ;
    walkr -> next = NULL ;
    walkr -> firstAppearOn = NULL ;
    walkr -> lastAppearOn = NULL ;
    gRear = walkr ;   
    CreateLine( walkr -> firstAppearOn, walkr -> lastAppearOn, tokenLength ) ;
    return ; 
  } // if
  else if ( strcmp( input, walkr -> tokenStr ) < 0 ) {
    temp = new Token ;
    temp -> next = walkr ;
    walkr = temp ;
    walkr -> tokenStr = new Str100 ;
    strcpy( walkr -> tokenStr, input ) ;
    walkr -> type = type ;
    walkr -> firstAppearOn = NULL ;
    walkr -> lastAppearOn = NULL ;
    CreateLine( walkr -> firstAppearOn, walkr -> lastAppearOn, tokenLength ) ;
    return ;
  } // if  
  else if ( strcmp( input, walkr -> tokenStr ) == 0 )
    return CreateLine( walkr -> firstAppearOn, walkr -> lastAppearOn, tokenLength ) ;
    
  else if ( strcmp( input, walkr -> tokenStr ) > 0 )
    return CreateToken( walkr -> next, input, type ) ;
    
} // CreatToken()


void CreateLine( LinePtr & head, LinePtr & tail, int length ) {
  if ( head == NULL ) {
    head = new Line ;
    head -> next = NULL ;
    tail = head ;
    head -> line = gLine ;
    CreateColumn( head -> firstAppearAt, head -> lastAppearAt, length ) ;
    return ;
  } // if 
  else if ( gLine == head -> line )
    return CreateColumn( head -> firstAppearAt, head -> lastAppearAt, length ) ;   
  else if ( gLine > head -> line )
    return CreateLine( head -> next, tail, length ) ;
  
} // CreatLine()

void CreateColumn( ColumnPtr & head, ColumnPtr & tail, int length ) {
  if ( head == NULL ) {
    head = new Column ;
    head -> next = NULL ;
    tail = head ;
    head -> column = gColumn ;
    gColumn = gColumn + length ;
    return ;
  } // if  
  else 
    return CreateColumn( head -> next, tail, length ) ;   
} // CreateColumn()  
  

void PrintToken( TokenPtr walkr ) {
  if ( walkr == NULL )
    return ;
  else {
    printf( "%s ", walkr -> tokenStr ) ;
    PrintLine( walkr -> firstAppearOn ) ;
    printf( "\n" ) ;
    return PrintToken( walkr -> next ) ;
  } // else
} // PrintToken()  

void PrintLine( LinePtr head ) {
  if ( head == NULL )
    return ;
  else {
    PrintColumn( head -> line, head -> firstAppearAt ) ;
    return PrintLine( head -> next ) ;
  } // else
} // PrintLine()

void PrintColumn( int line, ColumnPtr head ) {
  if ( head == NULL )
    return ;
  else {
    printf( "(%d,%d)", line, head -> column ) ;
    return PrintColumn( line, head -> next ) ;
  } // else
} // PrintColumn()

/* void PrintToken( TokenPtr walkr ) {
  LinePtr lineWalkr = NULL ;
  ColumnPtr columnWalkr = NULL ;
  if ( walkr != NULL ) {
    lineWalkr = walkr -> firstAppearOn ;
    if ( lineWalkr != NULL )
      columnWalkr = lineWalkr -> firstAppearAt ;
    
  } // if  
  
  while ( walkr != NULL ) {
    printf( "%s ", walkr -> tokenStr ) ;
    while ( lineWalkr != NULL ) {
      while ( columnWalkr != NULL ) {
        printf( "(%d,%d)", lineWalkr -> line, columnWalkr -> column ) ;
        columnWalkr = columnWalkr -> next ;
      } // while
      
      lineWalkr = lineWalkr -> next ;
      columnWalkr = lineWalkr -> firstAppearAt ;
    } // while  
    
    walkr = walkr -> next ;
    lineWalkr = walkr -> firstAppearOn ;
  } // while
  
  
  
} // PrintToken()  */
