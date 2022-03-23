#ifndef MMM_H_
#define MMM_H_

// globals (anything here would be shared with all threads) */
// I would declare the pointers to the matrices here (i.e., double **A, **B, **C),
// as well as the size of the matrices, etc.
double **A;
double **B;
double **C;
double **D;
int size;

struct parParams{
    int s_row;
    int f_row;
};

void mmm_init();
void mmm_reset(double **);
void mmm_freeup();
void mmm_seq();
void *mmm_par(void *args);
double mmm_verify();
void printMatrix(double **);

#endif /* MMM_H_ */

