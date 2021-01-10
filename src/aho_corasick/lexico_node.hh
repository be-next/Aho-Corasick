/*
*  Classe LexicoNode:
*   Cette classe represente un noeud dans l'arbre lexicographique.
*   Elle est munie d'une fonction de recherche dichotomique sur ses fils.
*/


#ifndef __LEXICO_NODE_HH__
#define __LEXICO_NODE_HH__

#include <vector>
#include <string>

class LexicoNode {
private:
    
    LexicoNode * _Dicho( const char &, int, int );
    
public:

    int _node_id;  /* Numero du noeud, sert a identifier le noeud lors de l'affichage de l'arbre */
    std::vector<std::string *> _state;  /* Si la taille de Sate est 0 alors le noeud n'est pas un
                              un etat terminal, sinon State contient le ou les mots correspondants
                              a l'etat terminal */
    char _character;  /* Variable contenant le caractere */
    LexicoNode * _supply;  /* Pointeur sur le noeud de suppleance */
    LexicoNode * _father;  /* Pointeur sur le noeud pere */
    std::vector<LexicoNode *> _children;  /* Tableau trie des noeuds fils */


    LexicoNode( void );
    ~LexicoNode( void );

    int operator <= ( LexicoNode & );
    int operator > ( LexicoNode & );
    int operator < ( LexicoNode & );
    int operator == ( LexicoNode & );
    int operator == ( char const & );

    LexicoNode * TestChilds( const char & );

};


//
// Fonction de recherche dichotomique
//
LexicoNode * LexicoNode::_Dicho( const char & to_search, int down, int up ) {
    if( down <= up )  /* si la borne basse est plus petite que la borne haute */
    {
        int center = ( down + up ) / 2;  /* Trouver le milieu des 2 bornes */
        
        if( to_search == _children[ center ]->_character )  /* si le char cherche = le char du milieu */
            return _children[ center ];                      /* alors retourner son LexicoNode */
        else                                            /* si non */
            if( to_search < _children[ center ]->_character ) /* si le char cherche est plus petit */
                return _Dicho( to_search, down, center -1 ); /* alors recommencer dans la moitie basse */
            else                                          /* si non */
                return _Dicho( to_search, center +1, up );   /* recommencer dans la moitie haute */
    }
    else                                                /* si non */
        return ( LexicoNode * ) NULL;                     /* retourner NULL */
}


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
  return _Dicho( to_search, 0, int(_children.size() -1) );
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


#endif /* __LEXICO_NODE_HH__ */
