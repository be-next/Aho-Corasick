#ifndef __FILE_TO_BUFFER_HH__
#define __FILE_TO_BUFFER_HH__

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "string.hh"

class File_To_Buffer : private std::fstream {
private:

    void Open( char * , int );
    void Close( int );

public:

    File_To_Buffer( void ){};
    
    virtual ~File_To_Buffer( void ){};

    String Load( char * );
    void Save( char *, String & );

    std::string Load2stdstring( char * );
};


/*
* Open( char *, int ):
*  Ouvre le fichier suivant les specifications donnees
*  en argument.
*/
void File_To_Buffer::Open( char * name, int mask ) {
  open( name, mask );

//  if( !is_open() )
//    {
//      cerr << "Cannot open input file \"" << name << "\"\n";
//      exit( 1 );
//    }
}


/*
* Close():
*  Ferme le fichier correspondant a la classe
*/
void File_To_Buffer::Close( int error ) {
    if( error || fail() ) {
        std::cerr << "Something strange happened\n";
        clear();
    }
    
    close();
}


/************************ Services ************************/


/*
* Load( char * ):
*  Remplie le Buffer avec le fichier donne en argument
*/
String File_To_Buffer::Load( char * name ) {
    Open( name, std::ios::in );
    String buffer;
    char c;

    while( !eof() && (( c=get() ) != '\377' ))
        buffer += c;
  
    Close( !eof() );
    
    return buffer;
}

std::string File_To_Buffer::Load2stdstring( char * file_name ) {
    this->Open( file_name, std::ios::in );
    
    std::stringstream strStream;
    strStream << this->rdbuf(); //read the file
    
    return strStream.str();
}


/*
* Save( char * ):
*  Remplie le fichier donne en argument avec le Buffer
*/
void File_To_Buffer::Save( char * name, String & buffer ) {
    Open( name, std::ios::out );

    for( int count = 0; count < buffer.GetSize(); count++ ) {
        put( buffer[ count ] );
    }
    
    Close( 0 );
}
      
#endif /* __FILE_TO_BUFFER_HH__ */

