
#include "testing.h"

#include <pax_cxx.hpp>

extern "C" void testing() {
	pax::Buffer cbuf(&buf);
	
	pax::Outline outline;
	outline.push_back((pax_vec1_t) { 1, -1});
	outline.push_back((pax_vec1_t) { 0, -1});
	outline.push_back((pax_vec1_t) {-1, -1});
	outline.push_back((pax_vec1_t) {-1,  0});
	outline.push_back((pax_vec1_t) {-1,  1});
	outline.push_back((pax_vec1_t) { 0,  1});
	outline.push_back((pax_vec1_t) { 1,  1});
	outline.push_back((pax_vec1_t) { 1,  0});
	
	pax::Circle    from;
	pax::Shape     to(outline);
	pax::LerpShape device(from, to, 0.5);
	
	cbuf.fillColor = 0x7fffffff;
	
	uint64_t start = esp_timer_get_time();
	cbuf.clearMatrix();
	while (1) {
		pax_background(&buf, 0);
		
		float coeff = (esp_timer_get_time() - start) / 1000000.0 / 2;
		coeff = fmodf(coeff, 4);
		if (coeff >= 2 && coeff <= 3) {
			coeff = 1 - (coeff - 2);
		} else if (coeff > 3) {
			coeff = 0;
		} else if (coeff > 1 && coeff < 2) {
			coeff = 1;
		}
		device = device.withCoeff(coeff);
		
		cbuf.pushMatrix();
			cbuf.translate(cbuf.width() / 2.0, cbuf.height() / 2.0);
			cbuf.scale(25);
			
			cbuf.draw(-3, 0, from);
			cbuf.draw( 0, 0, device);
			cbuf.draw( 3, 0, to);
		cbuf.popMatrix();
		
		disp_flush();
	}
}



/*
	pax_quad_t beziers[] = {
		{ 160,  64,   176,  32,   192,  16,   224,  16 },
		{ 224,  16,   256,  16,   272,  48,   272,  64 },
		{ 272,  64,   272,  96,   192, 208,   160, 224 },
		{ 160, 224,   128, 208,    48, 112,    48,  64 },
		{  48,  64,    48,  48,    64,  16,    96,  16 },
		{  96,  16,   128,  16,   144,  32,   160,  64 },
	};
	size_t n_bezier      = sizeof(beziers) / sizeof(pax_quad_t);
	size_t point_per_bez = 7;
	size_t n_point       = point_per_bez * n_bezier + 1;
	pax_vec1_t points[n_point];
	
	// Vectorise beziers.
	for (size_t i = 0; i < n_bezier; i++) {
		pax_vectorise_bezier(points + (i * point_per_bez), point_per_bez + 1, beziers[i]);
	}
	
	// Triangulate points.
	size_t *triang   = NULL;
	size_t  n_triang = pax_triang_concave(&triang, n_point - 1, points);
	
	uint64_t start = esp_timer_get_time();
	while (1) {
		pax_background(&buf, 0);
		
		float coeff = (esp_timer_get_time() - start) / 1000000.0 * 2;
		
		if (coeff >= 2) {
			if (triang) pax_draw_shape_triang(&buf, 0xffffffff, n_point-1, points, n_triang, triang);
		} else if (coeff > 1) {
			coeff --;
			pax_col_t col = 0xff000000 | (0x010101 * (uint8_t) (coeff * 255));
			if (triang) pax_draw_shape_triang(&buf, col, n_point-1, points, n_triang, triang);
			pax_outline_shape(&buf, 0xffffffff, n_point, points);
		} else {
			pax_outline_shape_part(&buf, 0xffffffff, n_point, points, 0, coeff);
		}
		
		disp_flush();
	}
*/
