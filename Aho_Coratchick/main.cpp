#include <string>

#include "lexicographic_tree.hh"
#include "file_to_buffer.hh"

int main(int argc, char **argv) {
 
    if (argc < 3) {
        std::cerr << "ERROR" << std::endl;
        std::cerr << argv[0] << " world_1 [ world_2 ... world_n ] file_name" << std::endl;
        exit(1);
    }
    
    std::string file_buffer;
    String buffer;
    File_To_Buffer fb;
    LexicographicTree lt;
    
    for (int count = 1; count < argc - 1; count++) /* Pour tous les mots a chercher */
    {
        buffer = argv[count];
        lt.AddWord(buffer); /* Les ajouter a l'arbre lexicographique */
    }

    lt.BuildSupplys(); /* Calcul des suppleances */
    lt.Print();        /* Affichage de l'arbre */

    file_buffer = fb.Load2stdstring(argv[argc - 1]); /* Chargement du texte */
    
    
    int line = 1;  /* Compteur de ligne */
    int place = 1; /* Compteur de deplacement sur une ligne */
    int words = 1;

    for (int count = 0; count < file_buffer.size(); count++, place++) /* Pour toutes les lettres du texte */
    {
        if (file_buffer[count] == '\n') /* si fin de ligne */
        {
            line++;    /* incrementer le compteur de ligne */
            place = 1; /* Reinitialiser le compteur de deplacement */
        }
        else /* si non */
        {
            Vector<String *> &result = lt.Transition(file_buffer[count]); /* Calculer la transition */

            for (int lcount = result.GetSize() - 1; lcount >= 0; lcount--) /* Afficher tous les resultats */
            {
                std::cout << words++ << "_ ";
                std::cout << *(result[lcount]);
                std::cout << " ---> ligne " << line;
                std::cout << " en position " << (place - result[lcount]->GetSize()) << std::endl;
            }
        }
    }
}
