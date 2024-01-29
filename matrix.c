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
int matTrans(MATRIX *A, MATRIX *AT);
//int matInverse(MATRIX *A, MATRIX *AI);

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

    int op1;
    int op2;

    printf("1+, 2-, 3*, 4transpose, 5inverse: ");
    scanf("%d", &op1);

    switch (op1) {
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
    case 4:
        printf("1->A, 2->B: ");
        scanf("%d", op2);
        if (op2 == 1) {
            matTrans(&A, &C);
            matPrint(&C);
            break;
        } else if (op2 == 2) {
            matTrans(&B, &C);
            matPrint(&C);
            break;
        }
    case 5:
        printf("1->A, 2->B: ");
        scanf("%d", op2);
        if (op2 == 1) {
            matInverse(&A, &C);
            matPrint(&C);
            break;
        } else if (op2 == 2) {
            matInverse(&B, &C);
            matPrint(&C);
            break;
        }
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
 #if 0
    for (int i = 0; i < C->rows; i++) {
        for (int j = 0; j < C->cols; j++) {
            C->data2d[i][j] = A->data2d[i][j] + B->data2d[i][j];
        }
    }
#else
    for (int i = 0; i < C->rows*C->cols; i++) {
        C->data1d[i] = A->data1d[i] + B->data1d[i];
    }
#endif

    return 0;
}

int matSub(MATRIX *A, MATRIX *B, MATRIX *C)
{
    if (A->rows != B->rows || A->cols != B->cols) return -1;

    matAlloc(C, A->rows, A->cols);
#if 0
    for (int i = 0; i < C->rows; i++) {
        for (int j = 0; j < C->cols; j++) {
            C->data2d[i][j] = A->data2d[i][j] - B->data2d[i][j];
        }
    }
#else
    for (int i = 0; i < C->rows*C->cols; i++) {
        C->data1d[i] = A->data1d[i] - B->data1d[i];
    }
#endif

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

int matTrans(MATRIX *A, MATRIX *AT)
{
    matAlloc(AT, A->cols, A->rows);
    for (int i = 0; i < AT->rows; i++) {
        for (int j = 0; j < AT->cols; j++) {
            AT->data2d[i][j] = A->data2d[j][i];
        }
    }

    return 0;
}

int matInverse(MATRIX *A, MATRIX *AI)
{
    if (A->rows != A->cols) return -1;

    int i, j, k, det, a;
    matAlloc(AI, A->rows, A->cols);

    int n = (A->rows);

    for (i = 0; i < n; i++) {
        for (j = 0; j < 2 * n; j++) {
            if (i == (j - n)) AI[i][j] = 1;
        }
    }

    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            if (i != j) {
                det = AI[j][i] / AI[i][i];
                for (k = 0; k < 2 * n; k++) {
                    AI[j][k] -= det * AI[i][k];
                }
            }
        }
    }

    for (i = 0; i < n; i++) {
        a = AI[i][i];
        for (j = 0; j < 2 * n; j++) {
            AI[i][j] /= a;
        }
    }

    return 0;
}
