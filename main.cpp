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
int SumAllKind() ;
int SumIDENTIFIER() ;
int SumCONSTANT() ;
int SumSPECIAL() ;
void PrintTheToken( charPtr input ) ;
void PrintTheLine( int line ) ;
bool TheTokenHasTheLine( LinePtr head, int line ) ;

int main(int argc, char** argv) {
  TokenPtr walkr = NULL ;
  TokenType type ;
  charPtr input ;
  int command = 0 ;
  int searchLine = 0 ;
  
  input = new Str100 ;
  strcpy( input, GetToken( type ) ) ; 
 
  while ( strcmp( input, "END_OF_FILE" ) != 0 ) {
    if ( type != NOTHING )
      CreateToken( walkr, input, type ) ;
    
    type = NOTHING ;
    strcpy( input, GetToken( type ) ) ; 
  } // while
  
  gFront = walkr ;
  
  PrintToken( walkr ) ;
  
  printf( "\n請輸入指令:\n" ) ;
  printf( "1.總共有多少種 token\n" ) ;
  printf( "2.三種case各有多少 token\n" ) ;
  printf( "3.印出指定 token 的位置 (line number,column number) (要排序)\n)" ) ;
  printf( "4.印出所指定的那一 line 出現哪些 token (要排序)\n" ) ;
  printf( "5.結束\n\n" ) ;
  
  scanf( "%d", &command ) ;
  while ( command != 5 ) {
    if ( command == 1 ) {
      printf( "總共%d種\n\n", SumAllKind() ) ;
    } // if
    else if ( command == 2 ) {
      printf( "Case1 共 %d個\n", SumIDENTIFIER() ) ;
      printf( "Case2 共 %d個\n", SumCONSTANT() ) ;
      printf( "Case3 共 %d個\n\n", SumSPECIAL() ) ;
    } // if
    else if ( command == 3 ) {
      printf( "請輸入要搜尋的 token :" ) ;
      scanf( "%s", input ) ;
      printf( "\n" ) ;
      PrintTheToken( input ) ;
      printf( "\n\n" ) ;
    } // if
    else if ( command == 4 ) {
      printf( "請輸入要指定的 line :" ) ;
      scanf( "%d", &searchLine ) ;
      printf( "\n" ) ;
      PrintTheLine( searchLine ) ;
      printf( "\n" ) ;
    } // if
    
    scanf( "%d", &command ) ;
  } // while  
  
  printf( "byebye" ) ;
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
  else {  // 未知的(大空白?)
    gColumn = gColumn + 1 ;
    type = NOTHING ;
    return returnPtr ;
  } // else  
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



int SumAllKind( ) {
  TokenPtr walkr = gFront ;
  int sum = 0 ;
  
  while ( walkr != NULL ) {
    sum = sum + 1 ;
    walkr = walkr -> next ;
  } // while
  
  return sum ;
} // SumAllKind()

int SumIDENTIFIER() {
  TokenPtr walkr = gFront ;
  int sum = 0 ;
  
  while ( walkr != NULL ) {
    if ( walkr -> type == IDENTIFIER )
      sum = sum + 1 ;
    walkr = walkr -> next ;
  } // while
  
  return sum ;  
} // SumIDENTIFIER()

int SumCONSTANT() {
  TokenPtr walkr = gFront ;
  int sum = 0 ;
  
  while ( walkr != NULL ) {
    if ( walkr -> type == CONSTANT )
      sum = sum + 1 ;
    walkr = walkr -> next ;
  } // while
  
  return sum ; 
} // SumCONSTANT()

int SumSPECIAL() {
  TokenPtr walkr = gFront ;
  int sum = 0 ;
  
  while ( walkr != NULL ) {
    if ( walkr -> type == SPECIAL )
      sum = sum + 1 ;
    walkr = walkr -> next ;
  } // while
  
  return sum ;  
} // SumSPECIAL()

void PrintTheToken( charPtr input ) {
  bool printSuccess = false ;
  TokenPtr walkr = gFront ;
  printf( "%s ", input ) ;
  
  while ( walkr != NULL ) {
    if ( strcmp( walkr -> tokenStr, input ) == 0 ) {
      PrintLine( walkr -> firstAppearOn ) ;
      printSuccess = true ;
    } // if
    
    walkr = walkr -> next ;
  } // while
  
  if ( !printSuccess )
    printf( "查無此token : %s", input ) ;
  return ;
} // PrintTheToken()

void PrintTheLine( int line ) {
  TokenPtr walkr = gFront ;
  
  while ( walkr != NULL ) {
    if ( TheTokenHasTheLine( walkr -> firstAppearOn, line ) )
      printf( "%s\n", walkr -> tokenStr ) ;
    
    walkr = walkr -> next ;
  } // while
} // PrintTheLine()


bool TheTokenHasTheLine( LinePtr head, int line ) {
  while ( head != NULL ) {
    if ( head -> line == line )
      return true ;
    
    head = head -> next ;
  } // while
  
  return false ;
} // TheTokenHasTheLine()