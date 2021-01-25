/**
* Class LexicographicTree:
*  Cette classe est un arbre lexicographique, elle permet de construire un
*  automate base sur l'algorithme de Aho et Corasick.
*
 
 it is used to implement the AC algorithm.
 Le root of this tree is the initial state and the empty word.
 
*  La racine de l'arbre Root est consideree comme l'etat initial et le mot vide.
*/

#pragma once

//#ifndef __LEXICOGRAPHIC_TREE_HH__
//#define __LEXICOGRAPHIC_TREE_HH__

#include "lexico_node.hh"

#include <string>
#include <iostream>
#include <sstream>
#include <unordered_set>

namespace aho_corasick {

class LexicographicTree {
private:

    int _nCount;  /* Number of nodes in the tree // Nombre de noeuds de l'arbre */
    LexicoNode * _scanner;  /* Pointer used to move within the tree // Pointeur servant a se deplacer dans l'arbre */
    LexicoNode * _root;  /* Root // Racine de l'arbre */
    std::unordered_set<std::string> _dictionary;

    LexicoNode * _NewLexicoNode( void );
    void _Print( LexicoNode * );
    const std::stringstream _getGraphVizDescription_rec( LexicoNode *, bool, bool );
    void _BuildSupplys( LexicoNode * );
    void _FindSupply( LexicoNode *, LexicoNode * );

public:

    LexicographicTree( void );
    ~LexicographicTree( void );
    
    void addKeyword( const std::string & );
    void BuildSupplys( void );
    void Print( void );
    const std::string getGraphVizDescription( bool, bool );
    std::unordered_set<std::string *> & Transition( char );
    void cancelCurrentSearch( void );
};


/*
* LexicoNode * NewLexicoNode( void ):
*  Retourne un pointeur sur un nouveau LexicoNode.
*/
LexicoNode * LexicographicTree::_NewLexicoNode( void ) {
  return new LexicoNode;
}


/*
* void Print( LexicoNode * lnode ):
*  Fonction recursive d'affichage de l'arbre
*/
void LexicographicTree::_Print( LexicoNode * lnode ) {
    if( lnode != NULL ) { /* Si l'arbre est construit */
        std::cout << "(" << lnode->getCharacter() << "-" << lnode->getNodeId();  /* Affiche le contenu */
        std::cout << "[";
        if( lnode->_failure_node != NULL ) {
            std::cout << lnode->_failure_node->getCharacter() << "-" << lnode->_failure_node->getNodeId();  /* Affiche la suppleance */
        }
            
        std::cout << "]";

        /* Recommencer avec ses fils s'il y en a */
        for( const auto& [character, child] : lnode->_children ) {
            _Print( child );
        }

        std::cout << ")";
    }
}

const std::stringstream LexicographicTree::_getGraphVizDescription_rec( LexicoNode * lnode, bool graphSuffix = true, bool graphWord = true ) {
    std::stringstream strStream;
    
    if( lnode != NULL ) {
        // Dessine le noeud courant
        if (lnode->getCharacter() == '\0' ) { // cas particulier pour le noeud racine
            strStream << lnode->getNodeId() << " [ label=\" NULL \" ]" << std::endl;
        } else {
            strStream << lnode->getNodeId() << " [ label=\" " << lnode->getCharacter() << " \" ]" << std::endl;
        }
        
        // Draws links between the current node and its child nodes
        for( const auto& [character, child] : lnode->_children ) {
            strStream << lnode->getNodeId() << " -> " << child->getNodeId() << std::endl;
            strStream << _getGraphVizDescription_rec( child, graphSuffix, graphWord ).str();
        }
        
        // Trace le lien, en rouge, vers le noeud de suppléance
        if( graphSuffix) {
            strStream << lnode->getNodeId() << " -> " << lnode->_failure_node->getNodeId() << " [ color = red ] " << std::endl;
        }
        
        // Dessine les mots terminaux dans des boites bleues
        if( graphWord ) {
            for( const auto& str : lnode->_keywords ) {
                strStream << long(str) << " [ shape=box, color=blue, label=\"" << (*str) << "\" ]" << std::endl;
                strStream << lnode->getNodeId() << " -> " << long(str) << " [ color=blue ] " << std::endl;
            }
        }
    }
    
    return strStream;
}

// 
const std::string LexicographicTree::getGraphVizDescription( bool graphSuffix = true, bool graphWord = true  ) {
    std::stringstream strStream;
    
    strStream << "digraph finite_state_machine {" << std::endl;
    strStream << "ratio = auto;" << std::endl;
    strStream << "node [shape = circle];" << std::endl;
    
    strStream << _getGraphVizDescription_rec( _root, graphSuffix, graphWord ).str();
    
    strStream << "}" << std::endl;
    
    return strStream.str();
}


/*
* void BuildSupplys( LexicoNode * lnode ):
*  Fonction recursive permettant d'attacher les pointeurs de suppleance
*  Supply au noeud correspondant au plus grand bord du prefixe teste.
 
 Recursive function used to attach pointers to the node corresponding to the bigest edge of the prefix being tested
*/
void LexicographicTree::_BuildSupplys( LexicoNode * lnode ) {
    
    if( lnode->_children.size() ) { /* Si le noeud n'est pas une feuille */
        for( auto [character, child] : lnode->_children ) { // For all of this children
            
            if( child->_failure_node == NULL ) {  /* si la suppleance n'est pas calculee */
                _FindSupply( lnode->_failure_node, child );  /* la chercher */
            }
            
            _BuildSupplys( child );  /* recommencer avec le fils */
        }
    }
}


/*
* void FindSupply( LexicoNode * child, LexicoNode * lnode ):
*  Fonction de recherche et d'attachement du pointeur suppleance pour
*  un noeud donne.
*/
void LexicographicTree::_FindSupply( LexicoNode * snode, LexicoNode * child ) {
    LexicoNode * nextNode;
    
    if( snode->_failure_node == NULL )  /* Si la suppleance du noeud de suppleance du pere n'existe pas */
        _FindSupply( snode->_father_node->_failure_node, snode );  /* on la trouve. */

    if(( nextNode = snode->getChild( child->getCharacter() )) != NULL ) { /* Si la suppleance du pere a un */
        if( child != nextNode )                          /* noeud contenant la lettre de son fils, et que */
        {                                                /* ce noeud n'est pas le fils, alors ce noeud */
            child->setFailureNode( nextNode );
            //child->_supply = nextNode;                    /* devient le noeud de suppleance du fils. */
            //child->_state.insert( child->_supply->_state.begin(), child->_supply->_state.end());        /** Le fils ajoute a ses etats terminaux ceux de son noeud de suppleance */
        } else {                                         /* si le noeud est le fils, alors la suppleance du fils est Root. */
            //child->_supply = _root;
            child->setFailureNode( _root );
        }
    } else {
        if( snode == _root ) {        /* Si la suppleance du pere est Root et qu'il n'a pas de noeud contenant la */
            //child->_supply = _root;  /* lettre du fils, alors la suppleance du fils est egale a Root. */
            child->setFailureNode( _root );
        } else {
            _FindSupply( snode->_failure_node, child );  /* si non, on recherche la suppleance du fils avec la suppleance */
        }                                            /* de la suppleance du pere recursivement. */
    }
}


/*
* Constructeur vide, creation de l'arbre vide
*/
LexicographicTree::LexicographicTree( void ) {
    _nCount  = 0;
    _root    = new LexicoNode( _nCount++, '\0', NULL );
    _scanner = _root;
    _root->_failure_node = _root;
}


/*
* Destructeur
*/
LexicographicTree::~LexicographicTree( void ) {
}


/*
* Void AddWorld( String & new_world )
*  Ajoute un mot dans l'arbre lexicographique
*/
void LexicographicTree::addKeyword( const std::string & new_keyword ) {
    LexicoNode * currentNode = _root;
    LexicoNode * nextNode = _root; // Pour pas etre a zero !
    auto it = new_keyword.cbegin();
    
    if( _dictionary.find(new_keyword) == _dictionary.end() ) { // Only if keyword hasn't been already added
        /* Pour toutes les lettres du mot, tant qu'il existe un noeud pour la lettre */
        for(; it != new_keyword.cend() && nextNode != NULL; it++ ) {
            if( ( nextNode = currentNode->getChild( *it )) != NULL ) {
                currentNode = nextNode;  /* On descend dans l'arbre */
            }
        }
    
        if( nextNode == NULL ) {  /* S'il n'existe pas de noeud correspondant a la lettre */
            for( --it; it != new_keyword.cend(); it++ ) { // A new node is created with the current keyword's letter
                currentNode = currentNode->addChild( new LexicoNode( _nCount++, *it, currentNode) );
            }
        }

        _dictionary.emplace( new_keyword );
        currentNode->_keywords.insert( new std::string( new_keyword ) );  /* On ajoute le mot a la liste des etats terminaux */
    }                                                                 /* du dernier noeud trouve */
}

/*
* void BuildSupplys( void ):
*  Permet d'attacher le pointeur Supply de chacun des noeuds
*  au noeud correspondant au plus grand bord.
*/
void LexicographicTree::BuildSupplys( void ) {
  _BuildSupplys( _root );
}


/*
* Fonction d'affichage pour le debug
*/
void LexicographicTree::Print( void ) {
    _Print( _root );
    std::cout << std::endl;
}


/*
* String Transition( char newCharacter ):
*  Fonction de transition permettant de se deplacer
*  dans l'automate forme par l'arbre lexicographique
*/
std::unordered_set<std::string *> & LexicographicTree::Transition( char currentCharacter ) {
    LexicoNode * nextNode;

    /* s'il existe un noeud suivant avec la lettre donnee en argument */
    if(( nextNode = _scanner->getChild( currentCharacter )) != NULL ) {
        return ( _scanner = nextNode )->_keywords;  /* on retourne sa liste d'etat */
    } else {
        if( _scanner == _root ) { /* Si le noeud en cours est Root et qu'il n'a pas de fils avec la lettre */
            return _scanner->_keywords;  /* on retourne sa liste d'etat */
        } else {
            _scanner = _scanner->_failure_node;  /* si le noeud en cours n'est pas Root, alors on recommence */
            return Transition( currentCharacter );  /* recursivement avec son noeud de suppleance */
        }
    }
}

void LexicographicTree::cancelCurrentSearch( void ) {
    _scanner = _root;
}

} // end of aho_corasick namespace

//#endif /* __LEXICOGRAPHIC_TREE_HH__ */
