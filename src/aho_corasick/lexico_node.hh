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
    int _node_id;  /* Numero du noeud, sert a identifier le noeud lors de l'affichage de l'arbre */
    char _character;  /* Variable contenant le caractere */
    std::unordered_set<const std::string *> _keywords;  /* Si la taille de Sate est 0 alors le noeud n'est pas un
                                                         un etat terminal, sinon State contient le ou les mots correspondants
                                                         a l'etat terminal */
public:
    

    
    LexicoNode * _failure_node;  /* Pointeur sur le noeud de suppleance */
    LexicoNode * _father_node;  /* Pointeur sur le noeud pere */
    
    std::unordered_map<char, LexicoNode *> _children; // Children nodes

    LexicoNode( void );
    LexicoNode( int, char, LexicoNode *);
    ~LexicoNode( void );

    LexicoNode * addChild( LexicoNode * );
    LexicoNode * getChild( const char& );
    
    const char& getCharacter( void );
    const int& getNodeId( void );

    LexicoNode * setFailureNode( LexicoNode * );
    void addKeyword( const std::string * );
    
    const std::unordered_set<const std::string *> & getKeywords( void );
    
    bool isLeaf( void );
    bool hasFailureNode( void );
    
};





/**************************** Services **************************/


/*
* Constructeur vide, initialisation du noeud.
*/
LexicoNode::LexicoNode( void ) : _children(), _keywords() {
    _character    = '\0';  /* Character = caractere NULL */
    _failure_node = NULL;  /* Les deux pointeurs pointent sur NULL */
    _father_node  = NULL;
    _node_id      = 0;
}

LexicoNode::LexicoNode( int new_node_id, char new_character, LexicoNode * new_father ) : _children(), _keywords() {
    _character    = new_character;
    _failure_node = NULL;
    _father_node  = new_father;
    _node_id      = new_node_id;
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
LexicoNode * LexicoNode::getChild( const char & to_search ) {
    auto it = _children.find( to_search );
    
    if( it != _children.end() ) {
        return it->second;
    } else {
        return NULL;
    }
}

LexicoNode * LexicoNode::addChild( LexicoNode * new_child ) {
    if( auto [iter, wasAdded] = _children.insert({new_child->_character, new_child}); wasAdded ) {
        return new_child;
    } // else raise an exception ?
}


const char& LexicoNode::getCharacter( void ) {
    return _character;
}

const int& LexicoNode::getNodeId( void ) {
    return _node_id;
}

LexicoNode * LexicoNode::setFailureNode(LexicoNode * new_failure_node) {
    _failure_node = new_failure_node;
    
    if( _failure_node->_keywords.size() ) {
        _keywords.insert( _failure_node->_keywords.begin(), _failure_node->_keywords.end() );
    }
    
    return new_failure_node;
}

void LexicoNode::addKeyword(const std::string * new_keyword ) {
    _keywords.insert( new_keyword );
}

const std::unordered_set<const std::string *> & LexicoNode::getKeywords( void ) {
    return _keywords;
}

bool LexicoNode::isLeaf( void ) {
    return (_children.size() == 0);
}

bool LexicoNode::hasFailureNode( void ) {
    return (_failure_node != NULL);
}

} // end of aho_corasick namespace


//#endif /* __LEXICO_NODE_HH__ */
