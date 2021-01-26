#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int rows, cols;
    float **data;
    float *raw;
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
            scanf("%f", &A.data[i][j]);
        }
    }

    matSave("A.txt", &A);
    matLoad("A.txt", &A);

    printf("B row: ");
    scanf("%d", &r);
    printf("B col: ");
    scanf("%d", &c);

    matAlloc(&B, r, c);

    for (i = 0; i < r; i++) {
        for (j = 0; j < c; j++) {
            scanf("%f", &B.data[i][j]);
        }
    }

    printf("\n");

    matSave("B.txt", &B);
    matLoad("B.txt", &B);

    int count;

    printf("1+, 2-, 3*: ");
    scanf("%d", &count);

    if (count == 1) {
        matAdd(&A, &B, &C);
        matPrint(&C);
    } else if (count == 2) {
        matSub(&A, &B, &C);
        matPrint(&C);
    } else if (count == 3) {
        matMul(&A, &B, &C);
        matPrint(&C);
    }

    return 0;
}

void matAlloc(MATRIX *A, int rows, int cols)
{
    A->rows = rows;
    A->cols = cols;
    A->raw = (float*)realloc(A->raw, rows*cols*sizeof(float));
    A->data = (float**)realloc(A->data, rows*sizeof(float*));
    for (int i = 0; i < rows; i++) {
        A->data[i] = A->raw + i*cols;
    }
}

void matPrint(MATRIX *A)
{
    for (int i = 0; i < A->rows; i++) {
        for (int j = 0; j < A->cols; j++) {
            printf("%f ", A->data[i][j]);
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
    fwrite(A->raw, sizeof(float), A->rows*A->cols, fp);
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
    fread(A->raw, sizeof(float), rows*cols, fp);
    fclose(fp);
    return 0;
}

int matAdd(MATRIX *A, MATRIX *B, MATRIX *C)
{
    if (A->rows != B->rows || A->cols != B->cols) return -1;

    matAlloc(C, A->rows, A->cols);
    for (int i = 0; i < C->rows; i++) {
        for (int j = 0; j < C->cols; j++) {
            C->data[i][j] = A->data[i][j] + B->data[i][j];
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
            C->data[i][j] = A->data[i][j] - B->data[i][j];
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
                sum += A->data[i][k] * B->data[k][j];
            }
            C->data[i][j] = sum;
        }
    }

    return 0;
}
