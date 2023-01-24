
#include "testing.h"

#include <pax_cxx.hpp>

extern uint8_t smile_png_start[] asm("_binary_smile_png_start");
extern uint8_t smile_png_end[]   asm("_binary_smile_png_end");

extern "C" void testing() {
	pax::Buffer cbuf(&buf);
	
	cbuf.background(0);
	cbuf.fillColor = 0xffffffff;
	cbuf.lineColor = 0xffffffff;
	// cbuf.drawStringCentered(pax_font_saira_regular, 18, 320/2, 0, "This is a\nMultiline STRING!!!!!\rwith all\r\nline ending types.");
	
	// pax::TextBox box;
	// box.alignment = pax::JUSTIFY;
	// pax::TextStyle style = box.getStyle();
	// style.font = pax_font_sky;
	// style.fontSize = 9;
	// style.color = 0xffffffff;
	// pax::Rectf bounds{20, 20, 280, 200};
	// box.bounds = bounds;
	// cbuf.outlineRect(bounds.x, bounds.y, bounds.w, bounds.h);
	// box.appendStyle(style);
	// box.appendText("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.");
	// box.draw(cbuf);
	
	pax_buf_t smile;
	pax_decode_png_buf(&smile, smile_png_start, smile_png_end-smile_png_start, PAX_BUF_16_4444ARGB, 0);
	
	pax::TextBox box;
	box.alignment = pax::JUSTIFY;
	pax::Rectf bounds{20, 20, 280, 200};
	box.bounds = bounds;
	cbuf.outlineRect(bounds.x, bounds.y, bounds.w, bounds.h);
	
	box.appendStyle(pax::TextStyle(pax_font_saira_regular, 18, 0xffff0000));
	box.appendText("Red text,");
	box.appendStyle(pax::TextStyle(pax_font_saira_regular, 18, 0xff00ff00));
	box.appendText("Green text,");
	box.appendStyle(pax::TextStyle(pax_font_saira_regular, 18, 0xff0000ff));
	box.appendText("Blue text,");
	box.appendStyle(pax::TextStyle(pax_font_saira_regular, 18, 0xffffffff, true));
	box.appendText("Italic text,");
	box.appendStyle(pax::TextStyle(pax_font_saira_regular, 24, 0xffffffff, false, false, true));
	box.appendText("Underline text,");
	box.append<pax::ImageElement>(pax::ImageElement(&smile));
	box.draw(cbuf);
	
	// // Make a little image.
	// pax_buf_t extra;
	// pax_buf_init(&extra, NULL, 50, 50, PAX_BUF_8_2222ARGB);
	// pax_background(&extra, 0);
	// pax_draw_circle(&extra, 0xffff7f00, 25, 25, 20);
	// pax_draw_rect(&extra, 0xff00ff00, 20, 20, 10, 10);
	// pax_join();
	
	// // Copy it to main buffer.
	// pax_draw_image(&buf, &extra, 0, 0);
	
	// // And now a converted edition.
	// pax_buf_convert(&extra, &extra, PAX_BUF_16_565RGB);
	// pax_draw_image(&buf, &extra, 50, 0);
	
	disp_flush();
}

/*
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
		
		// lololol
		
		disp_flush();
	}
*/

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
