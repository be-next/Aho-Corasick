/**
 *
 *
 * @author Jérôme Ramette
 */

#include <string>
#include <iostream>

#include "lexicographic_tree.hh"
#include "file_to_buffer.hh"

int main(int argc, char **argv) {
 
    if (argc < 3) {
        std::cerr << "ERROR" << std::endl;
        std::cerr << argv[0] << " world_1 [ world_2 ... world_n ] file_name" << std::endl;
        exit(1);
    }
 
    //std::string graph_file_name( "./graph.plot" );
    std::string file_buffer;
    std::string buffer;
    aho_corasick::File_To_Buffer fb;
    aho_corasick::LexicographicTree lt;
    
    for (int count = 1; count < argc - 1; count++) {/* Pour tous les mots a chercher */
        buffer = argv[count];
        lt.addKeyword(buffer); /* Les ajouter a l'arbre lexicographique */
        lt.addKeyword(std::string("test"));
    }
    
    fb.Save( "./graph1.plot", lt.getGraphVizDescription( false, true ));

    lt.BuildSupplys(); /* Calcul des suppleances */
    lt.Print();        /* Affichage de l'arbre */
    
    fb.Save( "./graph2.plot", lt.getGraphVizDescription());

    file_buffer = fb.Load(argv[argc - 1]); /* Chargement du texte */
    
    
    int line = 1;  /* Compteur de ligne */
    int place = 1; /* Compteur de deplacement sur une ligne */
    int words = 1;

    for(char& c : file_buffer) { /* Pour toutes les lettres du texte */
        if (c == '\n') { /* si fin de ligne */
            lt.cancelCurrentSearch(); // Repositionne le scanner sur le noeud racine
            line++;    /* incrementer le compteur de ligne */
            place = 1; /* Reinitialiser le compteur de deplacement */
        } else {/* si non */
            for( const std::string * str : lt.Transition(c)) { // Calculer la transition et afficher tous les resultats
                std::cout << words++ << "_ ";
                std::cout << *(str);
                std::cout << " ---> ligne " << line;
                std::cout << " en position " << (place - str->size()) << std::endl;
            }
            
            place++;
        }
    }
}
