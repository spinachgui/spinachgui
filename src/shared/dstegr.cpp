#include <iostream>
#include <shared/dstegr.hpp>
using namespace std;

void tridiagonal_eigenvalues(int size) {
    double* diagonals     = new double[size];
    double* sub_diagonals = new double[size-1];

    double* eigenvalues   = new double[size];
    double* eigenvectors  = new double[size*size];

    double* workspace_d   = new double[18*size];
    int*    workspace_i   = new int[10*size];

    int*    eigenvector_support = new int[2*size];

    int eigenvalues_found;
    int info;
    dstegr_('V','A',size,diagonals,sub_diagonals, 0.0,0.0, 0,0, 0.0,
            &eigenvalues_found,eigenvalues,eigenvectors,size,eigenvector_support,
            workspace_d,18*size,
            workspace_i,10*size,
            &info);

    if(info > 0) {
        std::cerr << "Error calling dstegr_, info=" << info << std::endl;
    }

    delete eigenvector_support;
    delete workspace_d;
    delete workspace_i;

    delete eigenvalues;
    delete eigenvectors;

    delete diagonals;
    delete sub_diagonals;
}

