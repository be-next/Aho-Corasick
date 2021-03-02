/**
 *  Classe LexicoNode:
 *   Cette classe represente un noeud dans l'arbre lexicographique.
 *   Elle est munie d'une fonction de recherche dichotomique sur ses fils.
 *
 * @author Jérôme Ramette
 */

#pragma once

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
    LexicoNode * _failure_node;  /* Pointeur sur le noeud de suppleance */
    std::unordered_map<char, LexicoNode *> _children; // Children nodes
    LexicoNode * _father_node;  /* Pointeur sur le noeud pere */
    
public:
    LexicoNode( void );
    explicit LexicoNode( int, char, LexicoNode *);
    ~LexicoNode( void );

    LexicoNode * addChild( LexicoNode * );
    LexicoNode * getChild( const char& ) const;
    
    const char& getCharacter( void ) const;
    const int& getNodeId( void ) const;

    const LexicoNode * setFailureNode( LexicoNode * );
    void addKeyword( const std::string * );
    
    const std::unordered_set<const std::string *> & getKeywords( void ) const;
    
    bool isLeaf( void ) const;
    bool hasFailureNode( void ) const;
    LexicoNode * getFailureNode( void ) const;
    
    const std::unordered_map<char, LexicoNode *> getChildren( void ) const;
    const LexicoNode * getFatherNode( void ) const;
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
LexicoNode * LexicoNode::getChild( const char & to_search ) const {
    auto it = _children.find( to_search );
    
    if( it != _children.end() ) {
        return it->second;
    } else {
        return NULL;
    }
}

LexicoNode * LexicoNode::addChild( LexicoNode * new_child ) {
    if( auto [iter, wasAdded] = _children.insert({new_child->_character, new_child}); !wasAdded ) {
        throw std::logic_error("Node insertion failure!");
    }
    
    return new_child;
}


const char& LexicoNode::getCharacter( void ) const {
    return _character;
}

const int& LexicoNode::getNodeId( void ) const {
    return _node_id;
}

const LexicoNode * LexicoNode::setFailureNode( LexicoNode * new_failure_node) {
    _failure_node = new_failure_node;
    
    if( _failure_node->_keywords.size() ) {
        _keywords.insert( _failure_node->_keywords.begin(), _failure_node->_keywords.end() );
    }
    
    std::cout << "set node (" << _node_id << "-" << _character << ") with failure node (" << new_failure_node->_node_id << "-" << new_failure_node->_character << ")" << std::endl;
    
    return new_failure_node;
}

void LexicoNode::addKeyword(const std::string * new_keyword ) {
    _keywords.insert( new_keyword );
}

const std::unordered_set<const std::string *> & LexicoNode::getKeywords( void ) const {
    return _keywords;
}

bool LexicoNode::isLeaf( void ) const {
    return (_children.size() == 0);
}

bool LexicoNode::hasFailureNode( void ) const {
    return (_failure_node != NULL);
}

LexicoNode * LexicoNode::getFailureNode( void ) const {
    return _failure_node;
}

const std::unordered_map<char, LexicoNode *> LexicoNode::getChildren( void ) const {
    return _children;
}

const LexicoNode * LexicoNode::getFatherNode( void ) const {
    return _father_node;
}


} // end of aho_corasick namespace
