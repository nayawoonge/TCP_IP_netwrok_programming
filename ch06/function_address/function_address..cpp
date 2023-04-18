#include "../../common.h"

int function01()
{
	printf("[Function 01]\n");
	return 0;
}

int main(int argc, char* argv[])
{
	function01();

	printf("function01 address: %p\n", function01);

	return 0;
}