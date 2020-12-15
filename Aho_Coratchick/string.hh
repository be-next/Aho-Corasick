#ifndef __STRING_HH__
#define __STRING_HH__

#include <stdlib.h>
#include <ctype.h>
#include <iostream>
#include <string>


class String {
private:
  
  char * Data;
  int Size;
  
public:
  
  String( void );
  String( char * );
  String( const String & );
    String( std::string & );
  ~String( void );
  
  String & operator = ( char );
  String & operator = ( char * );
    String & operator = ( std::string & );
  String & operator = ( const String & );

  String & operator += ( char );
  String & operator += ( char * );
  String & operator += ( const String & );

  int operator <= ( const String & );
  int operator > ( const String & );

  char & operator [] ( int );

  int GetSize( void );
  void Purge( void );
  void Print( void );

  String * SelfCopy( void );

friend std::ostream & operator << ( std::ostream & os, String & instance )
  {
    for( int count = 0; count < instance.GetSize(); count++ )
      os << instance[ count ];
    return os;
  }

};


/*************** Services *******************/


/*
* Constructeur vide
*/
String::String( void )
{
  Data = new char[1];
  Data[0] = '\0';
  Size = 0;
}


/*
* Constructeur avec un char *
*/
String::String( char * new_data )
{
  Size = (int) strlen( new_data );
  Data = new char[Size+1];
  strcpy( Data, new_data );
}


/*
* Constructeur de copie
*/
String::String( const String & instance )
{
  Size = instance.Size;
  Data = new char[Size+1];
  strcpy( Data, instance.Data );
}

// Constructeur de copie avec une std::string
String::String( std::string & new_string ) {
    Size = int(new_string.size());
    Data = new char[Size+1];
    strcpy( Data, new_string.data() );
}


/*
* Destructeur
*/
String::~String( void )
{
  delete [] Data;
}


/*
* Operateur de copie avec un char
*/
String & String::operator = ( char new_data )
{
  Size = 1;
  Data = ( char * ) realloc( Data, Size+1 );
  Data[0] = new_data;
  Data[1] = '\0';

  return *this;
}


/*
* Operateur de copie avec un char *
*/
String & String::operator = ( char * new_data )
{
  Size = (int) strlen( new_data );
  Data = ( char * ) realloc( Data, Size+1 );
  strcpy( Data, new_data );

  return (*this);
}

String & String::operator = ( std::string & new_string ) {
    Size = int(new_string.size());
    Data = ( char * ) realloc( Data, Size+1 );
    strcpy( Data, new_string.data() );
    
    return (*this);
}

/*
* operateur de copie avec une String
*/
String & String::operator = ( const String & instance )
{
  Size = instance.Size;
  Data = ( char * ) realloc( Data, Size+1 );
  strcpy( Data, instance.Data );
  
  return (*this);  
}


/*
* Concatenation avec un char
*/
String & String::operator += ( char new_data )
{
  Size += 1;
  Data = ( char * ) realloc( Data, Size+1 );
  Data[Size-1] = new_data;
  Data[Size] = '\0';

  return (*this);
}


/*
* Concatenation avec un char *
*/
String & String::operator += ( char * new_data )
{
  int new_size = Size + (int) strlen( new_data );
  Data = ( char * ) realloc( Data, new_size+1 );
  strcpy( Data + Size, new_data );
  Size = new_size;
  
  return (*this);
}


/*
* Concatenation avec une String
*/
String & String::operator += ( const String & instance )
{
  int new_size = Size + instance.Size;
  Data = ( char * ) realloc( Data, new_size+1 );
  strcpy( Data + Size, instance.Data );
  Size = new_size;
  
  return (*this);
}


/*
* Operateur inferieur ou egal
*/
int String::operator <= ( const String & instance )
{
  if( strcmp( Data, instance.Data ) <= 0 )
    return 1;
  else
    return 0;
}


/*
* Operateur strictement supperieur
*/
int String::operator > ( const String & instance )
{
  if( strcmp( Data, instance.Data ) > 0 )
    return 1;
  else
    return 0;
}


/*
* Operateur d'indexation [], indentique a celui des tableaux
*/
char & String::operator [] ( int address )
{
  return Data[address];
}


/*
* Retourne la taille de la String
*/
int String::GetSize( void )
{
  return Size;  
}


/*
* Vide la String, et l'initialise au caractere vide
*/
void String::Purge( void )
{
  Size = 0;
  Data = ( char * ) realloc( Data, 1 );
  Data[0] = '\0';
}


/*
* Affichage
*/
void String::Print( void )
{
  std::cout << Data << std::endl;
}


/*
* Retourne une copie de this
*/
String * String::SelfCopy( void )
{
  String * to_return = new String;

  *to_return = *this;
  return to_return;
}

#endif
