#include "ClassDemoApp.h"

int main(int argc, char *argv[])
{
	ClassDemoApp TileMap;
	while (!TileMap.UpdateAndRender()) {}
	return 0;
}
