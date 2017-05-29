#include "utils.h"
#include <time.h>

#define COUNT 20

int main(void)
{
	matrix *mat1 = mat_getRandom(3,5, 10);
	matrix *mat2 = mat_getUnit(5);
		
	mat_print(mat1);
	mat_print(mat2);
	
	mat_print( mat_multiply(mat1, mat2) );
	
	return 0;
}
