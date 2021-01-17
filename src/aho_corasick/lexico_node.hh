/**
 *  Classe LexicoNode:
 *   Cette classe represente un noeud dans l'arbre lexicographique.
 *   Elle est munie d'une fonction de recherche dichotomique sur ses fils.
 *
 * @author Jérôme Ramette
 */

#pragma once

//#ifndef __LEXICO_NODE_HH__
//#define __LEXICO_NODE_HH__

#include <string>
#include <unordered_map>
#include <unordered_set>

namespace aho_corasick {

class LexicoNode {
private:
    
public:

    int _node_id;  /* Numero du noeud, sert a identifier le noeud lors de l'affichage de l'arbre */
    
    // Replace vector by unordered_set
    std::unordered_set<std::string *> _state;  /* Si la taille de Sate est 0 alors le noeud n'est pas un
                              un etat terminal, sinon State contient le ou les mots correspondants
                              a l'etat terminal */
    char _character;  /* Variable contenant le caractere */
    LexicoNode * _supply;  /* Pointeur sur le noeud de suppleance */
    LexicoNode * _father;  /* Pointeur sur le noeud pere */
    
    std::unordered_map<char, LexicoNode *> _children; // Children nodes

    LexicoNode( void );
    ~LexicoNode( void );

    int operator <= ( LexicoNode & );
    int operator > ( LexicoNode & );
    int operator < ( LexicoNode & );
    int operator == ( LexicoNode & );
    int operator == ( char const & );

    LexicoNode * TestChilds( const char & );

};





/**************************** Services **************************/


/*
* Constructeur vide, initialisation du noeud.
*/
LexicoNode::LexicoNode( void ) : _children(), _state() {
    _character = '\0';  /* Character = caractere NULL */
    _supply    = NULL;  /* Les deux pointeurs pointent sur NULL */
    _father    = NULL;
    _node_id   = 0;
}


/*
* Destructeur
*/
LexicoNode::~LexicoNode( void ) {
}


/*
* int TestChilds( char tChar )
*  Teste si un des fils du noeud a comme Character to_search et retourne
*  un pointeur sur ce dernier, si non, retourne NULL.
*/
LexicoNode * LexicoNode::TestChilds( const char & to_search ) {
    std::unordered_map<char, LexicoNode *>::iterator it;
    
    it = _children.find( to_search );
    
    if( it != _children.end() ) {
        return it->second;
    } else {
        return NULL;
    }
}


/*
* Definition de l'operateur <=
*/
int LexicoNode::operator <= ( LexicoNode & instance ) {
  if( _character <= instance._character )
    return 1;
  else
    return 0;
}


/*
* Definition de l'operateur >
*/
int LexicoNode::operator > ( LexicoNode & instance ) {
  if( _character > instance._character )
    return 1;
  else
    return 0;
}


/*
 * Define < operator in order to use std::sort to sort _children std::vector
 */
int LexicoNode::operator < ( LexicoNode & instance ) {
    if( _character < instance._character )
        return 1;
    else
        return 0;
}

/*
 * Define == operator in order to implement find function on _children std::vector
 */
int LexicoNode::operator == ( LexicoNode & instance ) {
    if( _character == instance._character )
        return 1;
    else
        return 0;
}

int LexicoNode::operator == ( char const & c ) {
    if( _character == c )
        return 1;
    else
        return 0;
}

} // end of aho_corasick namespace


//#endif /* __LEXICO_NODE_HH__ */
