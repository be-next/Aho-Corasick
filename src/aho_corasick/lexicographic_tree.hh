/*
* Classe LexicographicTree:
*  Cette classe est un arbre lexicographique, elle permet de construire un
*  automate base sur l'algorithme de Aho et Corasick.
*
*  La racine de l'arbre Root est consideree comme l'etat initial et le mot vide.
*/

#pragma once

#ifndef __LEXICOGRAPHIC_TREE_HH__
#define __LEXICOGRAPHIC_TREE_HH__

#include "lexico_node.hh"

#include <vector>
#include <string>
#include <iostream>
#include <sstream>


class LexicographicTree {
private:

    int _nCount;  /* Nombre de noeuds de l'arbre */
    LexicoNode * _scanner;  /* Pointeur servant a se deplacer dans l'arbre */
    LexicoNode * _root;  /* Racine de l'arbre */

    LexicoNode * _NewLexicoNode( void );
    void _Print( LexicoNode * );
    const std::stringstream _getGraphVizDescription_rec( LexicoNode * );
    void _BuildSupplys( LexicoNode * );
    void _FindSupply( LexicoNode *, LexicoNode * );

public:

    LexicographicTree( void );
    ~LexicographicTree( void );
    
    void AddWord( const std::string & );
    void BuildSupplys( void );
    void Print( void );
    const std::string getGraphVizDescription( void );
    std::vector<std::string *> & Transition( char );
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
        std::cout << "(" << lnode->_character << "-" << lnode->_node_id;  /* Affiche le contenu */
        std::cout << "[";
        if( lnode->_supply != NULL ) {
            std::cout << lnode->_supply->_character << "-" << lnode->_supply->_node_id;  /* Affiche la suppleance */
        }
            
        std::cout << "]";

        /* Recommencer avec ses fils s'il y en a */
        for( LexicoNode * ln_rec : lnode->_children ) {
            _Print( ln_rec );
        }

        std::cout << ")";
    }
}

const std::stringstream LexicographicTree::_getGraphVizDescription_rec( LexicoNode * lnode ) {
    std::stringstream strStream;
    
    if( lnode != NULL ) {
        // Dessine le noeud courant
        if (lnode->_character == '\0' ) { // cas particulier pour le noeud racine
            strStream << lnode->_node_id << " [ label=\" NULL \" ]" << std::endl;
        } else {
            strStream << lnode->_node_id << " [ label=\" " << lnode->_character << " \" ]" << std::endl;
        }
        
        // Trace les liens entre le noeud courant et ses fils
        for( LexicoNode * lnode_child : lnode->_children ) {
            strStream << lnode->_node_id << " -> " << lnode_child->_node_id << std::endl;
            strStream << _getGraphVizDescription_rec( lnode_child ).str();
        }
        
        // Trace le lien, en rouge, vers le noeud de suppléance
        strStream << lnode->_node_id << " -> " << lnode->_supply->_node_id << " [ color = red ] " << std::endl;
        
        // Dessine les mots terminaux dans des boites bleues
        for( std::string * str : lnode->_state ) {
            strStream << long(str) << " [ shape=box, color=blue, label=\"" << (*str) << "\" ]" << std::endl;
            strStream << lnode->_node_id << " -> " << long(str) << " [ color=blue ] " << std::endl;
        }
    }
    
    return strStream;
}

// 
const std::string LexicographicTree::getGraphVizDescription( void ) {
    std::stringstream strStream;
    
    strStream << "digraph finite_state_machine {" << std::endl;
    strStream << "ratio = auto;" << std::endl;
    strStream << "node [shape = circle];" << std::endl;
    
    strStream << _getGraphVizDescription_rec( _root ).str();
    
    strStream << "}" << std::endl;
    
    return strStream.str();
}


/*
* void BuildSupplys( LexicoNode * lnode ):
*  Fonction recursive permettant d'attacher les pointeurs de suppleance
*  Supply au noeud correspondant au plus grand bord du prefixe teste.
*/
void LexicographicTree::_BuildSupplys( LexicoNode * lnode ) {
    
    std::sort( lnode->_children.begin(), lnode->_children.end());
    
    if( lnode->_children.size() ) { /* Si le noeud n'est pas une feuille */
        for( LexicoNode * ln_rec : lnode->_children ) { /* Pour tous ces fils */
            if( ln_rec->_supply == NULL )  /* si la suppleance n'est pas calculee */
                _FindSupply( lnode->_supply, ln_rec );  /* la chercher */
            
            _BuildSupplys( ln_rec );  /* recommencer avec le fils */
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
    
    if( snode->_supply == NULL )  /* Si la suppleance du noeud de suppleance du pere n'existe pas */
        _FindSupply( snode->_father->_supply, snode );  /* on la trouve. */

    if(( nextNode = snode->TestChilds( child->_character )) != NULL ) { /* Si la suppleance du pere a un */
        if( child != nextNode )                          /* noeud contenant la lettre de son fils, et que */
        {                                                /* ce noeud n'est pas le fils, alors ce noeud */
            child->_supply = nextNode;                    /* devient le noeud de suppleance du fils. */
            child->_state.insert( child->_state.end(), child->_supply->_state.begin(), child->_supply->_state.end());        /* Le fils ajoute a ses etats terminaux ceux de son noeud de suppleance */
        } else {                                         /* si le noeud est le fils, alors la suppleance du fils est Root. */
            child->_supply = _root;
        }
    } else {
        if( snode == _root )        /* Si la suppleance du pere est Root et qu'il n'a pas de noeud contenant la */
            child->_supply = _root;  /* lettre du fils, alors la suppleance du fils est egale a Root. */
        else
            _FindSupply( snode->_supply, child );  /* si non, on recherche la suppleance du fils avec la suppleance */
    }                                            /* de la suppleance du pere recursivement. */
}


/*
* Constructeur vide, creation de l'arbre vide
*/
LexicographicTree::LexicographicTree( void ) {
  _nCount = 0;

  _root = _NewLexicoNode();
  _root->_supply = _root;
  _scanner = _root;

  _root->_node_id = _nCount++;
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
void LexicographicTree::AddWord( const std::string & new_word ) {
    int count;
    LexicoNode * currentNode = _root;
    LexicoNode * nextNode = _root; // Pour pas etre a zero !
    char currentCharacter;

    /* Pour toutes les lettres du mot, tant qu'il existe un noeud pour la lettre */
    for( count = 0; (count < new_word.size()) && nextNode != NULL; count++ ) {
        currentCharacter = new_word[count];
        
        if( ( nextNode = currentNode->TestChilds( currentCharacter )) != NULL )
            currentNode = nextNode;  /* On descend dans l'arbre */
    }
    
    if( nextNode == NULL ) {  /* S'il n'existe pas de noeud correspondant a la lettre */
        LexicoNode * newNode; /* On cree un nouveau noeud avec la lettre en question */
        
        for( --count ; count < new_word.size(); count++ ) {
            currentCharacter = new_word[count];
            newNode = _NewLexicoNode();
            newNode->_node_id = _nCount++;
            newNode->_character = currentCharacter;
            currentNode->_children.push_back( newNode );
            newNode->_father = currentNode;
            currentNode = newNode;
        }
    }
    
    currentNode->_state.push_back( new std::string( new_word ) );  /* On ajoute le mot a la liste des etats terminaux */
}                                                                 /* du dernier noeud trouve */


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
std::vector<std::string *> & LexicographicTree::Transition( char newCharacter ) {
    LexicoNode * nextNode;

    /* s'il existe un noeud suivant avec la lettre donnee en argument */
    if(( nextNode = _scanner->TestChilds( newCharacter )) != NULL ) {
        return ( _scanner = nextNode )->_state;  /* on retourne sa liste d'etat */
    } else {
        if( _scanner == _root ) { /* Si le noeud en cours est Root et qu'il n'a pas de fils avec la lettre */
            return _scanner->_state;  /* on retourne sa liste d'etat */
        } else {
            _scanner = _scanner->_supply;  /* si le noeud en cours n'est pas Root, alors on recommence */
            return Transition( newCharacter );  /* recursivement avec son noeud de suppleance */
        }
    }
}

void LexicographicTree::cancelCurrentSearch( void ) {
    _scanner = _root;
}


#endif /* __LEXICOGRAPHIC_TREE_HH__ */
