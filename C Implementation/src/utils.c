#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//Common

typedef enum{FALSE, TRUE} bool;

char *multiplyChar(char c, int n)
{
	int i;
	char *res = malloc( n );
	
	for(i = 0; i < n; i++) res[i] = c;
	
	return res;
}

long rand_limit(long max) 
{
	unsigned long
    // max <= RAND_MAX < ULONG_MAX, so this is okay.
	num_bins = (unsigned long) max + 1,
	num_rand = (unsigned long) RAND_MAX + 1,
    bin_size = num_rand / num_bins,
    defect   = num_rand % num_bins;

	long x;
	do {
		x = random();
	}
	// This is carefully written not to overflow
	while (num_rand - defect <= (unsigned long)x);

	// Truncated division is intentional
	return x/bin_size;
}

//Vector

typedef struct 
{
	double X;
	double Y;
} vector2;

typedef struct 
{
	double X;
	double Y;
	double Z;
} vector3;

vector3 cross3(vector3 *a, vector3 *b)
{
	vector3 res;
	
	res.X = (a->Y * b->Z) - (a->Z * b->Y);
	res.Y = (a->Z * b->X) - (a->X * b->Z);
	res.Z = (a->X * b->Y) - (a->Y * b->X);
	
	return res;
}

double cross2(vector2 *a, vector2 *b)
{
	return (a->X * b->Y) - (b->X * a->Y);
}

double scalar3(vector3 *a, vector3 *b)
{
	return (a->X * b->X) + (a->Y * b->Y) + (a->Z * b->Z);
}

double scalar2(vector2 *a, vector2 *b)
{
	return (a->X * b->X) + (a->Y * b->Y);
}

//Matrix

#define MAT_ROW_SEPARATOR '-'
#define MAT_COLUMN_SEPARATOR '|'

typedef struct
{
	int rows;		//Zeilen
	int columns;	//Spalten
	int **data;		
} matrix;

//Matrix Common

matrix *mat_create(int cRows, int cColumns)
{
	int i;
	matrix *mat = NULL;
	
	if(cRows < 1 || cColumns < 1) return mat;
	
	mat = malloc( sizeof(matrix) );		//Allocate memory for the struct
	
	mat->rows = cRows;
	mat->columns = cColumns;
	
    mat->data = malloc(cRows * sizeof(*(mat->data)) );	//Allocate memory for the pointer to columns
	for(i = 0; i < cRows; i++)
	{
		mat->data[i] = calloc(cColumns, sizeof(**(mat->data)) );	//Allocate memory for the columns 
	}
	
	return mat;
}

matrix *mat_getUnit(int n)
{
	matrix *res;
	int i;
	
	res = mat_create(n, n);
	
	for(i = 0; i < n; i++)
	{
		res->data[i][i] = 1;
	}
	
	return res;
}

bool mat_set(matrix *mat, int m, int n, int value)
{
	if(m > mat->rows || n > mat->columns || m < 1 || n < 1) return FALSE;
	
	mat->data[m-1][n-1] = value;
	
	return TRUE;
}

int mat_get(matrix *mat, int m, int n)
{
	if(m > mat->rows || n > mat->columns || m < 1 || n < 1) return -1;
	
	return mat->data[m-1][n-1];
}

matrix *mat_getRandom(int cRows, int cColumns, int limit)
{
	int m, n;
	matrix *res;
	res = mat_create(cRows, cColumns);
	
	srand( time(NULL) );
	
	for(m = 0; m < res->rows; m++)
	{
		for(n = 0; n < res->columns; n++) res->data[m][n] = rand_limit(limit);
	}
	
	return res;
}

void mat_print(matrix *mat)
{
	int m, n;
	char *rowSeparator = multiplyChar(MAT_ROW_SEPARATOR, (3 * mat->columns) + 1);
	
	for(m = 0; m < mat->rows; m++)
	{
		printf("%s\n", rowSeparator);
		for(n = 0; n < mat->columns; n++)
		{
			printf("%c%2i", MAT_COLUMN_SEPARATOR, mat->data[m][n]);
		}
		printf("%c\n", MAT_COLUMN_SEPARATOR);
	}
	printf("%s\n", rowSeparator);
	
	free(rowSeparator);
}

//Matrix Operations

bool mat_equals(matrix *mat1, matrix *mat2)
{
	int m, n;
	
	if( (mat1->rows != mat2->rows) || (mat1->columns != mat2->columns) ) return FALSE;
	
	for(m = 0; m < mat1->rows; m++)
	{
		for(n = 0; n < mat1->columns; n++)
		{
			if( mat1->data[m][n] != mat2->data[m][n] ) return FALSE;
		}
	}
	
	return TRUE;
}

matrix *mat_multiply(matrix *left, matrix *right)
{
	matrix *res;
	int m, n;
	int nLeft, mRight;
	int value;
	
	if( left->columns != right->rows ) return NULL;
	
	res = mat_create( left->rows, right->columns );
	
	for(m = 0; m < res->rows; m++)
	{
		for(n = 0; n < res->columns; n++)
		{
			value = 0;
			nLeft = 0;
			mRight = 0;
			
			while(nLeft < left->columns)
			{
				value += left->data[m][nLeft] * right->data[mRight][n];
				
				nLeft++;
				mRight++;
			}
			
			res->data[m][n] = value;
		}
	}
	
	return res;
}

matrix *mat_factor(matrix *mat, int factor, bool reziproke)
{
	matrix *res;
	int m, n;
	
	res = mat_create(mat->rows, mat->columns);
	
	for(m = 0; m < mat->rows; m++)
	{
		for(n = 0; n < mat->columns; n++)
		{
			if( reziproke ) res->data[m][n] = mat->data[m][n] / factor;
			else res->data[m][n] = mat->data[m][n] * factor;
		}
	}
	
	return res;
}

matrix *mat_sum(matrix *left, matrix *right)
{
	matrix *res;
	int m, n;
	
	if( (left->rows != right->rows) || (left->columns != right->columns) ) return NULL; 
	
	res = mat_create(left->rows, left->columns);
	
	for(m = 0; m < res->rows; m++)
	{
		for(n = 0; n < res->columns; n++)
		{
			res->data[m][n] = left->data[m][n] + right->data[m][n];
		}
	}
	
	return res;
}
