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
    mutable LexicoNode * _scanner;  /* Pointer used to move within the tree // Pointeur servant a se deplacer dans l'arbre */
    LexicoNode * _root;  /* Root // Racine de l'arbre */
    std::unordered_set<std::string> _dictionary;

    void _BuildSupplys( LexicoNode * );
    void _findFailureNode( LexicoNode *, LexicoNode * );
    void _findFailureNodesOnThisBranchBefore( LexicoNode *);

public:

    LexicographicTree( void );
    ~LexicographicTree( void );
    
    void addKeyword( const std::string & );
    void finalize( void );
    const std::unordered_set<const std::string *> & Transition( char ) const;
    void cancelCurrentSearch( void );
    
    const LexicoNode * getRoot( void ) const;
};



/*
* void BuildSupplys( LexicoNode * lnode ):
*  Fonction recursive permettant d'attacher les pointeurs de suppleance
*  Supply au noeud correspondant au plus grand bord du prefixe teste.
 
 Recursive function used to attach pointers to the node corresponding to the bigest edge of the prefix being tested
*/
void LexicographicTree::_BuildSupplys( LexicoNode * lnode ) {
    if( !lnode->isLeaf() ) { /* Si le noeud n'est pas une feuille */
        for( const auto& [character, child] : lnode->getChildren() ) { // For all of this children
            if( !child->hasFailureNode() ) {  /* si la suppleance n'est pas calculee */
                _findFailureNode( lnode->getFailureNode(), child );  /* la chercher */
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
void LexicographicTree::_findFailureNode( LexicoNode * snode, LexicoNode * child ) {
    LexicoNode * nextNode;
    
    std::cout << "snode : " << snode->getNodeId() << "-" << snode->getCharacter() << " / child : " << child->getNodeId() << "-" << child->getCharacter() << std::endl;
    
    if( !snode->hasFailureNode() ) { /* Si la suppleance du noeud de suppleance du pere n'existe pas */
        _findFailureNode( snode->getFatherNode()->getFailureNode(), snode );  /* on la trouve. */
    }
    
    if(( nextNode = snode->getChild( child->getCharacter() )) != NULL ) { /* Si la suppleance du pere a un */
        if( child != nextNode )                          /* noeud contenant la lettre de son fils, et que */
        {                                                /* ce noeud n'est pas le fils, alors ce noeud */
            if( !nextNode->hasFailureNode() ) {
                _findFailureNodesOnThisBranchBefore( nextNode );
            }
            
            child->setFailureNode( nextNode );
            //child->_supply = nextNode;                    /* devient le noeud de suppleance du fils. */
            //child->_state.insert( child->_supply->_state.begin(), child->_supply->_state.end());        /** Le fils ajoute a ses etats terminaux ceux de son noeud de suppleance */
        } else {                                         /* si le noeud est le fils, alors la suppleance du fils est Root. */
            child->setFailureNode( _root );
        }
    } else {
        if( snode == _root ) {        /* Si la suppleance du pere est Root et qu'il n'a pas de noeud contenant la */
            //child->_supply = _root;  /* lettre du fils, alors la suppleance du fils est egale a Root. */
            child->setFailureNode( _root );
        } else {
            _findFailureNode( snode->getFailureNode(), child );  /* si non, on recherche la suppleance du fils avec la suppleance */
        }                                            /* de la suppleance du pere recursivement. */
    }
}

void LexicographicTree::_findFailureNodesOnThisBranchBefore( LexicoNode * node ) {
    const LexicoNode * father = node->getFatherNode();
    const LexicoNode * soon = node;
    
    // follow up the branch from the node which the failure node must be found, to the root
    while( father != _root ) {
        soon = father;
        father = soon->getFatherNode();
    }
    
    LexicoNode * child = father->getChild(soon->getCharacter());
    
    if( !child->hasFailureNode() ) {
        _findFailureNode( _root, child );
    }
    
    _BuildSupplys(child);
}

/*
* Constructeur vide, creation de l'arbre vide
*/
LexicographicTree::LexicographicTree( void ) {
    _nCount  = 0;
    _root    = new LexicoNode( _nCount++, '\0', NULL );
    _scanner = _root;
    _root->setFailureNode( _root );
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
    LexicoNode * nextNode;
    LexicoNode * currentNode = _root;
    auto it = new_keyword.cbegin();
    
    // Only if keyword hasn't been already added in the tree (a keyword must be added only once)
    if( auto [dictionary_it, wasAdded] = _dictionary.emplace( new_keyword ); wasAdded ) {
        // As long as the current node has a child corresponding to the current keuword's letter (*it)
        while( (it != new_keyword.cend()) && ((nextNode = currentNode->getChild( *it)) != NULL) ) {
            currentNode = nextNode; // Go down into the tree following the keyword's letters, node by node
            it++;
        }
        // While it remains letters in the keyword
        while( it != new_keyword.cend() ) {
            // A new node is created with the current letter and added to the children set of the current node
            currentNode = currentNode->addChild( new LexicoNode( _nCount++, *it, currentNode) );
            it++;
        }

        // The keyword is added to the set of final states of the last created node
        currentNode->addKeyword( &(*dictionary_it) );
    }
}

/*
* void BuildSupplys( void ):
*  Permet d'attacher le pointeur Supply de chacun des noeuds
*  au noeud correspondant au plus grand bord.
*/
void LexicographicTree::finalize( void ) {
  _BuildSupplys( _root );
}



/*
* String Transition( char newCharacter ):
*  Fonction de transition permettant de se deplacer
*  dans l'automate forme par l'arbre lexicographique
*/
const std::unordered_set<const std::string *> & LexicographicTree::Transition( char currentCharacter ) const {
    LexicoNode * nextNode;

    /* s'il existe un noeud suivant avec la lettre donnee en argument */
    if(( nextNode = _scanner->getChild( currentCharacter )) != NULL ) {
        return ( _scanner = nextNode )->getKeywords();  /* on retourne sa liste d'etat */
    } else {
        if( _scanner == _root ) { /* Si le noeud en cours est Root et qu'il n'a pas de fils avec la lettre */
            return _scanner->getKeywords();  /* on retourne sa liste vide d'etat */
        } else {
            _scanner = _scanner->getFailureNode();  /* si le noeud en cours n'est pas Root, alors on recommence */
            return Transition( currentCharacter );  /* recursivement avec son noeud de suppleance */
        }
    }
}

/// Reinit current seach by setting scanner to the root node
void LexicographicTree::cancelCurrentSearch( void ) {
    _scanner = _root;
}

const LexicoNode * LexicographicTree::getRoot( void ) const {
    return _root;
}

} // end of aho_corasick namespace

//#endif /* __LEXICOGRAPHIC_TREE_HH__ */
