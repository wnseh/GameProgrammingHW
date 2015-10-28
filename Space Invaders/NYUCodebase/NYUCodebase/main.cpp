#include "ClassDemoApp.h"

int main(int argc, char *argv[])
{
	ClassDemoApp spaceInvaders;
	while (!spaceInvaders.UpdateAndRender()) {}
	return 0;
}
