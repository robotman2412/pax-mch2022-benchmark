
#include "testing.h"
#include <pax_cxx.hpp>

pax_col_t cShader(pax_col_t tint, int x, int y, float u, float v, void *args) {
	return pax_col_rgb(u * 255, v * 255, 0);
}

void quantum(pax::Buffer &cbuf, int limit) {
	if (limit == 0) return;
	cbuf.pushMatrix();
	
	float a = (esp_timer_get_time() % 5000000) / 1000000.0 / 5.0;
	cbuf.rotate(M_PI * 2.0 * a);
	cbuf.drawLine(pax::hsv(limit * 10 + a * 255, 255, 255), 50, 0, 0, 0);
	cbuf.translate(50, 0);
	cbuf.scale(0.8);
	quantum(cbuf, limit - 1);
	
	cbuf.popMatrix();
}

extern "C" void testing() {
	pax::Buffer cbuf(&buf);
	
	cbuf.background(pax::rgb(0, 0, 0));
	cbuf.fillColor = 0xffff007f;
	cbuf.lineColor = 0xffffffff;
	
	// pax::Shader shader(&cShader, NULL);
	// pax_quad_t uvs = {
	// 	0, 0,
	// 	1, 0,
	// 	1, 1,
	// 	0, 1,
	// };
	// cbuf.drawCircle(&shader, &uvs, 60, 60, 50);
	
	// cbuf.drawCircle(&shader, NULL, 170, 60, 50);
	
	// pax_join();
	// disp_flush();
	
	pax_apply_2d(&buf, matrix_2d_scale(4, 4));
	pax_vec1_t shape[] = {
		{ 15, 35 },
		{ 35, 45 },
		{ 40, 40 },
		
		{ 20, 20 },
		{ 30, 25 },
		{ 40, 15 },
	};
	size_t n_shape = sizeof(shape) / sizeof(pax_vec1_t);
	
	pax_draw_shape(&buf, 0xffff0000, n_shape, shape);
	pax_outline_shape(&buf, 0xffffffff, n_shape, shape);
	pax_draw_line(&buf, 0xffffffff, shape[0].x, shape[0].y, shape[n_shape-1].x, shape[n_shape-1].y);
	
	disp_flush();
	
	// while (1) {
	// 	cbuf.background(pax::rgb(0, 0, 0));
		
	// 	cbuf.pushMatrix();
	// 	cbuf.translate(buf.width / 2, buf.height / 2);
	// 	quantum(cbuf, 9);
	// 	cbuf.popMatrix();
		
	// 	disp_flush();
	// }
}
