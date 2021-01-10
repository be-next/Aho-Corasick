#ifndef __QUICKSORT_HH__
#define __QUICKSORT_HH__


template <class Container_Type, class Enclose_Type>
class QuickSort {
private:

  void R_QuickSort( Container_Type &, int, int );
  void Swap( Container_Type &, int, int );

public:

  QuickSort( void );
  QuickSort( Container_Type & );
  QuickSort( Container_Type &, int, int );

  ~QuickSort( void );

  void Sort( Container_Type & );
  void Sort( Container_Type &, int, int );

};


/*
* Fonction recursive de trie quicksort
*/
template <class Container_Type, class Enclose_Type>
void QuickSort<Container_Type, Enclose_Type>::R_QuickSort( Container_Type & tab, int begin, int end )
{
  int find_to_up = begin+1;
  int find_to_down = end;
  int up_limit = 0, down_limit = 0;
  
  if( begin < end )
    {
      down_limit = find_to_up;
      up_limit   = find_to_down;
      
      while( down_limit <= up_limit )
	{
	  up_limit = find_to_down ;
	  while( ( find_to_up <= up_limit ) && ( *(tab[ find_to_up ]) <= *(tab[ begin ]) ) )
	    find_to_up += 1;
	  
	  down_limit = find_to_up;
	  while( ( find_to_down >= down_limit ) && ( *(tab[ find_to_down ]) > *(tab[ begin ]) ) )
	    find_to_down -= 1;
	  
	  if( find_to_up < find_to_down )
	    {
	      Swap( tab, find_to_up, find_to_down );
	      find_to_up += 1;
	      find_to_down -= 1;
	    }
	}
      
      Swap( tab, up_limit, begin );
      
      R_QuickSort( tab, begin, up_limit - 1 );
      R_QuickSort( tab, up_limit + 1, end );
    }
}


/*
* Echange les deux objets indices par les arguments
*/
template <class Container_Type, class Enclose_Type>
void QuickSort<Container_Type, Enclose_Type>::Swap( Container_Type & instance, int index1, int index2 )
{
  Enclose_Type tmp;

  tmp = instance[ index1 ];
  instance[ index1 ] = instance[ index2 ];
  instance[ index2 ] = tmp;
}


/************************ Services ***************************/


/*
* Constructeur vide
*/
template <class Container_Type, class Enclose_Type>
QuickSort<Container_Type, Enclose_Type>::QuickSort( void )
{}


/*
* Constructeur avec en argument l'objet a trier.
* L'appel de se constructeur trie l'objet !!
*/
template <class Container_Type, class Enclose_Type>
QuickSort<Container_Type, Enclose_Type>::QuickSort( Container_Type & instance )
{
  R_QuickSort( instance, 0, instance.GetSize() - 1 );
}


/*
* Constructeur permettant de trier les Container_Types de base, ou tout 
* objet ne possedant pas de services retournant leur taille.
*/
template <class Container_Type, class Enclose_Type>
QuickSort<Container_Type, Enclose_Type>::QuickSort( Container_Type & instance, int begin, int end )
{
  R_QuickSort( instance, begin, end - 1 );
}


/*
* Destructeur
*/
template <class Container_Type, class Enclose_Type>
QuickSort<Container_Type, Enclose_Type>::~QuickSort( void )
{}


/*
* Fonction non recursive appelant la fonction de trie 
* recursive.
*/
template <class Container_Type, class Enclose_Type>
void QuickSort<Container_Type, Enclose_Type>::Sort( Container_Type & instance )
{
  R_QuickSort( instance, 0, instance.GetSize() - 1);
}


/*
* Fonction permettant de trier les Container_Types de base, ou tout 
* objet ne possedant pas de services retournant leur taille.
*/
template <class Container_Type, class Enclose_Type>
void QuickSort<Container_Type, Enclose_Type>::Sort( Container_Type & instance, int begin, int end )
{
  R_QuickSort( instance, begin, end - 1);
}


#endif /* __QUICKSORT_HH__ */
