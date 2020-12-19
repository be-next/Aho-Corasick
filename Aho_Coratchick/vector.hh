#ifndef __VECTOR_HH__
#define __VECTOR_HH__

#include <iostream>
#include "quicksort.hh"

template <class Type>
class Vector : private QuickSort<Vector<Type>, Type>
{

private:
  int Size;

  Type *Data;

public:
  Vector(void);
  Vector(int);
  Vector(const Vector<Type> &);

  virtual ~Vector(void);

  Vector<Type> &operator=(const Vector<Type> &);
  Vector<Type> &operator+=(const Vector<Type> &);
  Vector<Type> &operator+=(Type);
  Type &operator[](int);

  int GetSize(void);
  void SetSize(int);
 // void Sort(void);

  friend std::ostream &operator<<(std::ostream &os, Vector &v)
  {
    for (int count = 0; count < v.GetSize(); count++)
    {
      //	os << *(v[ count ]) << " ";
    }
    return os;
  }
};

/********************  services  ********************/

/*
* Constructeur vide
*/
template <class Type>
Vector<Type>::Vector(void)
{
  Data = NULL;
  Size = 0;
}

/*
* Constructeur avec la taille en argument
*/
template <class Type>
Vector<Type>::Vector(int size)
{
  Size = size;
  Data = (Type *)new Type[Size];
}

/*
* Constructeur de copie
*/
template <class Type>
Vector<Type>::Vector(const Vector<Type> &instance)
{
  Size = instance.Size;
  Data = (Type *)new Type[Size];

  for (int count = 0; count < Size; count++)
    Data[count] = instance.Data[count];
}

/*
* Destructeur
*/
template <class Type>
Vector<Type>::~Vector()
{
  if (Size != 0)
    delete[] Data;
}

/*
* Redefinition de l'operateur de copie =
*/
template <class Type>
Vector<Type> &Vector<Type>::operator=(const Vector<Type> &instance)
{
  if (Size != 0)
    delete[] Data;

  Size = instance.Size;
  Data = (Type *)new Type[Size];

  for (int count = 0; count < Size; count++)
    Data[count] = instance.Data[count];

  return (*this);
}

/*
* Concatenation de 2 Vector<Type>
*/
template <class Type>
Vector<Type> &Vector<Type>::operator+=(const Vector<Type> &instance)
{
  Vector<Type>::SetSize(Size + instance.Size);

  for (int count = 0; count < instance.Size; count++)
    Data[Size + count - instance.Size] = instance.Data[count];

  QuickSort<Vector<Type>, Type>::Sort(*this);

  return *this;
}

/*
* Redefinition de l'operateur []
*/
template <class Type>
Type &Vector<Type>::operator[](int add)
{
  return Data[add];
}

/*
* Redefinition de l'operateur +=
* Ajoute un element en agrandissant la taille
* et en maintenant le tableau trie.
*/
template <class Type>
Vector<Type> &Vector<Type>::operator+=(Type new_type)
{
  Vector<Type>::SetSize(Size + 1);
  Data[Size - 1] = new_type;
  QuickSort<Vector<Type>, Type>::Sort(*this);

  return (*this);
}

/*
* Change la taille de Data
*/
template <class Type>
void Vector<Type>::SetSize(int size)
{
  if (size < Size)
    delete[](Data + (Size - size) * sizeof(Type));

  Data = (Type *)realloc(Data, size * sizeof(Type));
  Size = size;
}

/*
* Retourne la taille de Data
*/
template <class Type>
int Vector<Type>::GetSize(void)
{
  return Size;
}

/*
* Trie le tableau Data
*
template <class Type>
void Vector<Type>::Sort(void)
{
  QuickSort<Vector<Type>, Type>::Sort(*this);
}*/

#endif /* __VECTOR_HH__ */
