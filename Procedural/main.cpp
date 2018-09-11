#include "Procedural.h"
#include "main.h"


int main(void)
{
	Procedural procedural = Procedural();
	procedural.initGL();

	return procedural.render_loop();
}
