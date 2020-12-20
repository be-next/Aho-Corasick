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

    int Ncount;  /* Nombre de noeuds de l'arbre */
    LexicoNode * Scanner;  /* Pointeur servant a se deplacer dans l'arbre */
    LexicoNode * Root;  /* Racine de l'arbre */

    LexicoNode * NewLexicoNode( void );
    void Print( LexicoNode * );
    const std::stringstream _getGraphVizDescription_rec( LexicoNode * );
    void BuildSupplys( LexicoNode * );
    void FindSupply( LexicoNode *, LexicoNode * );

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
LexicoNode * LexicographicTree::NewLexicoNode( void ) {
  return new LexicoNode;
}


/*
* void Print( LexicoNode * lnode ):
*  Fonction recursive d'affichage de l'arbre
*/
void LexicographicTree::Print( LexicoNode * lnode ) {
    if( lnode != NULL ) { /* Si l'arbre est construit */
        std::cout << "(" << lnode->Character << "-" << lnode->Number;  /* Affiche le contenu */
        std::cout << "[";
        if( lnode->Supply != NULL ) {
            std::cout << lnode->Supply->Character << "-" << lnode->Supply->Number;  /* Affiche la suppleance */
        }
            
        std::cout << "]";

        /* Recommencer avec ses fils s'il y en a */
        for( LexicoNode * ln_rec : lnode->_children ) {
            Print( ln_rec );
        }

        std::cout << ")";
    }
}

const std::stringstream LexicographicTree::_getGraphVizDescription_rec( LexicoNode * lnode ) {
    std::stringstream strStream;
    
    if( lnode != NULL ) {
        // Dessin du noeud courant
        if (lnode->Character == '\0' ) { // cas particulier pour le noeud racine
            strStream << lnode->Number << " [ label=\" NULL \" ]" << std::endl;
        } else {
            strStream << lnode->Number << " [ label=\" " << lnode->Character << " \" ]" << std::endl;
        }
        
        // Tracé des liens entre le noeud courant et ses fils
        for( LexicoNode * lnode_child : lnode->_children ) {
            strStream << lnode->Number << " -> " << lnode_child->Number << std::endl;
            strStream << _getGraphVizDescription_rec( lnode_child ).str();
        }
        
        // Tracé du lien, en rouge, vers le noeud de suppléance
        strStream << lnode->Number << " -> " << lnode->Supply->Number << " [ color = red ] " << std::endl;
        
        // Dessin des mots terminaux dans des boites bleues
        for( std::string * str : lnode->State ) {
            strStream << long(str) << " [ shape=box, color=blue, label=\"" << (*str) << "\" ]" << std::endl;
            strStream << lnode->Number << " -> " << long(str) << " [ color=blue ] " << std::endl;
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
    
    strStream << _getGraphVizDescription_rec( Root ).str();
    
    strStream << "}" << std::endl;
    
    return strStream.str();
}


/*
* void BuildSupplys( LexicoNode * lnode ):
*  Fonction recursive permettant d'attacher les pointeurs de suppleance
*  Supply au noeud correspondant au plus grand bord du prefixe teste.
*/
void LexicographicTree::BuildSupplys( LexicoNode * lnode ) {
    if( lnode->_children.size() ) { /* Si le noeud n'est pas une feuille */
        for( LexicoNode * ln_rec : lnode->_children ) { /* Pour tous ces fils */
            if( ln_rec->Supply == NULL )  /* si la suppleance n'est pas calculee */
                FindSupply( lnode->Supply, ln_rec );  /* la chercher */
            
            BuildSupplys( ln_rec );  /* recommencer avec le fils */
        }
    }
}


/*
* void FindSupply( LexicoNode * child, LexicoNode * lnode ):
*  Fonction de recherche et d'attachement du pointeur suppleance pour
*  un noeud donne.
*/
void LexicographicTree::FindSupply( LexicoNode * snode, LexicoNode * child ) {
    LexicoNode * nextNode;
    
    if( snode->Supply == NULL )  /* Si la suppleance du noeud de suppleance du pere n'existe pas */
        FindSupply( snode->Father->Supply, snode );  /* on la trouve. */

    if(( nextNode = snode->TestChilds( child->Character )) != NULL ) { /* Si la suppleance du pere a un */
        if( child != nextNode )                          /* noeud contenant la lettre de son fils, et que */
        {                                                /* ce noeud n'est pas le fils, alors ce noeud */
            child->Supply = nextNode;                    /* devient le noeud de suppleance du fils. */
            child->State.insert( child->State.end(), child->Supply->State.begin(), child->Supply->State.end());        /* Le fils ajoute a ses etats terminaux ceux de son noeud de suppleance */
        } else {                                         /* si le noeud est le fils, alors la suppleance du fils est Root. */
            child->Supply = Root;
        }
    } else {
        if( snode == Root )        /* Si la suppleance du pere est Root et qu'il n'a pas de noeud contenant la */
            child->Supply = Root;  /* lettre du fils, alors la suppleance du fils est egale a Root. */
        else
            FindSupply( snode->Supply, child );  /* si non, on recherche la suppleance du fils avec la suppleance */
    }                                            /* de la suppleance du pere recursivement. */
}


/*
* Constructeur vide, creation de l'arbre vide
*/
LexicographicTree::LexicographicTree( void ) {
  Ncount = 0;

  Root = NewLexicoNode();
  Root->Supply = Root;
  Scanner = Root;

  Root->Number = Ncount++;
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
    LexicoNode * currentNode = Root;
    LexicoNode * nextNode = Root; // Pour pas etre a zero !
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
            newNode = NewLexicoNode();
            newNode->Number = Ncount++;
            newNode->Character = currentCharacter;
            currentNode->_children.push_back( newNode );
            newNode->Father = currentNode;
            currentNode = newNode;
        }
    }
    
    currentNode->State.push_back( new std::string( new_word ) );  /* On ajoute le mot a la liste des etats terminaux */
}                                                                 /* du dernier noeud trouve */


/*
* void BuildSupplys( void ):
*  Permet d'attacher le pointeur Supply de chacun des noeuds
*  au noeud correspondant au plus grand bord.
*/
void LexicographicTree::BuildSupplys( void ) {
  BuildSupplys( Root );
}


/*
* Fonction d'affichage pour le debug
*/
void LexicographicTree::Print( void ) {
    Print( Root );
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
    if(( nextNode = Scanner->TestChilds( newCharacter )) != NULL ) {
        return ( Scanner = nextNode )->State;  /* on retourne sa liste d'etat */
    } else {
        if( Scanner == Root ) { /* Si le noeud en cours est Root et qu'il n'a pas de fils avec la lettre */
            return Scanner->State;  /* on retourne sa liste d'etat */
        } else {
            Scanner = Scanner->Supply;  /* si le noeud en cours n'est pas Root, alors on recommence */
            return Transition( newCharacter );  /* recursivement avec son noeud de suppleance */
        }
    }
}

void LexicographicTree::cancelCurrentSearch( void ) {
    Scanner = Root;
}


#endif /* __LEXICOGRAPHIC_TREE_HH__ */
