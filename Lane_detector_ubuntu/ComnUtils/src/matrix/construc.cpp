/****************************************************************************/
/*                                                                          */
/* Projet : Librairie Matrices                                              */
/*                                                                          */
/* Auteur : Stephane Christy                                                */
/*                                                                          */
/* Date   : Aout 1994                                                       */
/*                                                                          */
/* Module : constructeurs et destructeur                                    */
/*                                                                          */
/****************************************************************************/


#include "matrix.h"


Matrix::Matrix(void)                           // constructeur
{
  rows = cols = 0; tabMat = NULL;
}


Matrix::Matrix(int nr, int nc)               // creation et allocation
{
  rows = cols = 0; tabMat = NULL;
  create(nr, nc);
}


Matrix::Matrix(int nr, int nc, double val)   // creation avec valeur par defaut
{
  rows = cols = 0; tabMat = NULL;
  create(nr, nc, val);
}


Matrix::Matrix(const Matrix& M)                // creation par recopie
{
  rows = cols = 0; tabMat = NULL;
                  // necessaire car l'affectation appelle la fonction effacer
  *this = M;
}


Matrix::Matrix(int nr, int nc, const double** M)
{
  int i, j;

  rows = cols = 0; tabMat = NULL;
  create(nr, nc);
  for(i = 1; i <= nr; i++)
    for(j = 1; j <= nc; j++)
      (*this)(i, j) = M[i][j];
}

Matrix::Matrix(const vector<vector<float> >& M)
{
  int i, j;
  int nr, nc;
  
  nr = (int)M.size();
  if (nr>0) nc = (int)M[0].size(); else nc=0;

  rows = cols = 0; tabMat = NULL;
  create(nr, nc);
  for(i = 1; i <= nr; i++)
    for(j = 1; j <= nc; j++)
      (*this)(i, j) = M[i-1][j-1];
}

Matrix::Matrix(const vector<float>& M)
{
  int j;
  int nr, nc;
  
  nr = 1;
  nc = (int)M.size();

  rows = cols = 0; tabMat = NULL;
  create(nr, nc);
  for(j = 1; j <= nc; j++)
      (*this)(1, j) = M[j-1];
}

Matrix::Matrix(const char* filename)
{
  rows = cols = 0; tabMat = NULL;
  read(filename);
}


Matrix::~Matrix(void)                          // destructeur
{
  clear();
}
