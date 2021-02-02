
#pragma once

#include "lexico_node.hh"

namespace aho_corasick {

class DisplayTools {
private :
    void _Print( const LexicoNode * );
    const std::stringstream _getGraphVizDescription_rec( const LexicoNode *, bool, bool );
    
public :
    void Print( const LexicoNode * root );
    const std::string getGraphVizDescription( const LexicoNode *, bool, bool );
};

/*
 * void Print( LexicoNode * lnode ):
 *  Fonction recursive d'affichage de l'arbre
 */
void DisplayTools::_Print( const LexicoNode * lnode ) {
    if( lnode != NULL ) { /* Si l'arbre est construit */
        std::cout << "(" << lnode->getCharacter() << "-" << lnode->getNodeId();  /* Affiche le contenu */
        std::cout << "[";
        if( lnode->hasFailureNode() ) {
            std::cout << lnode->getFailureNode()->getCharacter() << "-" << lnode->getFailureNode()->getNodeId();  /* Affiche la suppleance */
        }
        
        std::cout << "]";
        
        /* Recommencer avec ses fils s'il y en a */
        for( const auto& [character, child] : lnode->getChildren() ) {
            _Print( child );
        }
        
        std::cout << ")";
    }
}

const std::stringstream DisplayTools::_getGraphVizDescription_rec( const LexicoNode * lnode, bool graphSuffix = true, bool graphWord = true ) {
    std::stringstream strStream;
    
    if( lnode != NULL ) {
        // Dessine le noeud courant
        if (lnode->getCharacter() == '\0' ) { // cas particulier pour le noeud racine
            strStream << lnode->getNodeId() << " [ label=\" NULL \" ]" << std::endl;
        } else {
            strStream << lnode->getNodeId() << " [ label=\" " << lnode->getNodeId() << "-" << lnode->getCharacter() << " \" ]" << std::endl;
        }
        
        // Draws links between the current node and its child nodes
        for( const auto& [character, child] : lnode->getChildren() ) {
            strStream << lnode->getNodeId() << " -> " << child->getNodeId() << std::endl;
            strStream << _getGraphVizDescription_rec( child, graphSuffix, graphWord ).str();
        }
        
        // Trace le lien, en rouge, vers le noeud de suppléance
        if( graphSuffix) {
            strStream << lnode->getNodeId() << " -> " << lnode->getFailureNode()->getNodeId() << " [ color = red ] " << std::endl;
        }
        
        // Dessine les mots terminaux dans des boites bleues
        if( graphWord ) {
            for( const auto& str : lnode->getKeywords() ) {
                strStream << long(str) << " [ shape=box, color=blue, label=\"" << (*str) << "\" ]" << std::endl;
                strStream << lnode->getNodeId() << " -> " << long(str) << " [ color=blue ] " << std::endl;
            }
        }
    }
    
    return strStream;
}


/// Services

//
const std::string DisplayTools::getGraphVizDescription( const LexicoNode * lnode, bool graphSuffix = true, bool graphWord = true  ) {
    std::stringstream strStream;
    
    strStream << "digraph finite_state_machine {" << std::endl;
    strStream << "ratio = auto;" << std::endl;
    strStream << "node [shape = circle];" << std::endl;
    
    strStream << _getGraphVizDescription_rec( lnode, graphSuffix, graphWord ).str();
    
    strStream << "}" << std::endl;
    
    return strStream.str();
}

/*
 * Fonction d'affichage pour le debug
 */
void DisplayTools::Print( const LexicoNode * root ) {
    _Print( root );
    std::cout << std::endl;
}



} // end of aho_corasick namespace
