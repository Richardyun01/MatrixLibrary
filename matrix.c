#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int rows, cols;
    float **data2d;
    float *data1d;
} MATRIX;

void matAlloc(MATRIX *A, int rows, int cols);
void matPrint(MATRIX *A);
int matSave(char *path, MATRIX *A);
int matLoad(char *path, MATRIX *A);
int matAdd(MATRIX *A, MATRIX *B, MATRIX *C);
int matSub(MATRIX *A, MATRIX *B, MATRIX *C);
int matMul(MATRIX *A, MATRIX *B, MATRIX *C);

int main()
{
    MATRIX A = {0}, B = {0}, C = {0};

    int i, j;
    int r, c;

    printf("A row: ");
    scanf("%d", &r);
    printf("A col: ");
    scanf("%d", &c);

    matAlloc(&A, r, c);

    for (i = 0; i < r; i++) {
        for (j = 0; j < c; j++) {
            scanf("%f", &A.data2d[i][j]);
        }
    }

    printf("B row: ");
    scanf("%d", &r);
    printf("B col: ");
    scanf("%d", &c);

    matAlloc(&B, r, c);

    for (i = 0; i < r; i++) {
        for (j = 0; j < c; j++) {
            scanf("%f", &B.data2d[i][j]);
        }
    }

    printf("\n");

    int op;

    printf("1+, 2-, 3*: ");
    scanf("%d", &op);

    switch (op) {
    case 1:
        matAdd(&A, &B, &C);
        matPrint(&C);
        break;
    case 2:
        matSub(&A, &B, &C);
        matPrint(&C);
        break;
    case 3:
        matMul(&A, &B, &C);
        matPrint(&C);
        break;
    }

    matSave("A.txt", &A);
    matLoad("A.txt", &A);
    matSave("B.txt", &B);
    matLoad("B.txt", &B);


    return 0;
}

void matAlloc(MATRIX *A, int rows, int cols)
{
    A->rows = rows;
    A->cols = cols;
    A->data1d = (float*)realloc(A->data1d, rows*cols*sizeof(float));
    A->data2d = (float**)realloc(A->data2d, rows*sizeof(float*));
    for (int i = 0; i < rows; i++) {
        A->data2d[i] = A->data1d + i*cols;
    }
}

void matPrint(MATRIX *A)
{
    for (int i = 0; i < A->rows; i++) {
        for (int j = 0; j < A->cols; j++) {
            printf("%f ", A->data2d[i][j]);
        }
        printf("\n");
    }
}

int matSave(char *path, MATRIX *A)
{
    FILE *fp = fopen(path, "wb");
    if (!fp) return -1;

    fwrite(&(A->rows), sizeof(int), 1, fp);
    fwrite(&(A->cols), sizeof(int), 1, fp);
    fwrite(A->data1d, sizeof(float), A->rows*A->cols, fp);
    fclose(fp);
    return 0;
}

int matLoad(char *path, MATRIX *A)
{
    FILE *fp = fopen(path, "rb");
    if (!fp) return -1;

    int rows, cols;
    fread(&rows, sizeof(int), 1, fp);
    fread(&cols, sizeof(int), 1, fp);

    matAlloc(A, rows, cols);
    fread(A->data1d, sizeof(float), rows*cols, fp);
    fclose(fp);
    return 0;
}

int matAdd(MATRIX *A, MATRIX *B, MATRIX *C)
{
    if (A->rows != B->rows || A->cols != B->cols) return -1;

    matAlloc(C, A->rows, A->cols);
    for (int i = 0; i < C->rows; i++) {
        for (int j = 0; j < C->cols; j++) {
            C->data2d[i][j] = A->data2d[i][j] + B->data2d[i][j];
        }
    }

    return 0;
}

int matSub(MATRIX *A, MATRIX *B, MATRIX *C)
{
    if (A->rows != B->rows || A->cols != B->cols) return -1;

    matAlloc(C, A->rows, A->cols);
    for (int i = 0; i < C->rows; i++) {
        for (int j = 0; j < C->cols; j++) {
            C->data2d[i][j] = A->data2d[i][j] - B->data2d[i][j];
        }
    }

    return 0;
}

int matMul(MATRIX *A, MATRIX *B, MATRIX *C)
{
    if (A->rows != B->rows || A->cols != B->cols) return -1;

    matAlloc(C, A->rows, A->cols);
    for (int i = 0; i < C->rows; i++) {
        for (int j = 0; j < C->rows; j++) {
            float sum = 0;
            for (int k = 0; k < A->cols; A++) {
                sum += A->data2d[i][k] * B->data2d[k][j];
            }
            C->data2d[i][j] = sum;
        }
    }

    return 0;
}
