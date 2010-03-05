
#ifndef _dstegr_H_
#define _dstegr_H_


/* This file links to to the LAPACK dstegr routein */
extern"C" {
void dstegr_(unsigned char JOBZ, /*'N' or 'V', 'V' for eigenvectors*/
             unsigned char RANGE,/*'A' to find all eigenvalues*/
             int N,              /*Matrix Order*/
             double* D,          /*N  diagonal elements*/
             double* E,          /*N-1 subdiagonal elements*/
             double VL,          /*Only relevent if RANGE='V'*/
             double VU,          /*Only relevent if RANGE='V'*/
             int IL,             /*Only relevent if RANGE='U'*/
             int IU,             /*Only relevent if RANGE='U'*/
             double ABSTOL,      /*Not used*/
             int* M,             /*Nuber of eigenvalues found*/
             double* W,          /*Array of M eigenvalues, max size N*/
             double* Z,          /*Eigenvectors*/
             int LDZ,            /*Dimension of Z, should be N*/
             int* ISUPPZ,        /*2*M*/
             double* WORK,       /*Workspace, see LWORK*/
             int LWORK,          /*Best size if 18*N for eigenvector operations*/
             int* IWORK,         /*More workspace*/
             int LIWORK,         /*Dimension of LIWORK workspase should be 10*N*/
             int* INFO);         /*=0 of sucessful exit*/
}
void tridiagonal_eigenvalues(int size);


#endif

