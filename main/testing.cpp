
#include "testing.h"
#include <pax_cxx.hpp>

extern "C" void testing() {
	pax::Buffer cbuf(&buf);
	
	pax_background(&buf, 0);
	cbuf.fillColor = 0xffff007f;
	cbuf.drawCircle(20, 20, 8);
	
	disp_flush();
}
