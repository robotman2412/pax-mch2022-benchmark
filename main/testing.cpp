
#include "testing.h"


static const char *TAG = "testing";

extern uint8_t smile_png_start[] asm("_binary_smile_png_start");
extern uint8_t smile_png_end[]   asm("_binary_smile_png_end");



uint32_t dummy_shader_promise(pax_buf_t *buf, pax_col_t color, void *args) {
	return PAX_PROMISE_IGNORE_BASE | PAX_PROMISE_IGNORE_UVS | PAX_PROMISE_OPAQUE;
}

pax_col_t dummy_shader_cb(pax_col_t existing, pax_col_t tint, int x, int y, float u, float v, void *args) {
	return (x ^ y) & 1 ? 0xff00ff00 : 0xff0000ff;
}

pax_shader_t dummy_shader = {
	.schema_version    = 1,
	.schema_complement = (uint8_t) ~1,
	.renderer_id       = PAX_RENDERER_ID_SWR,
	.promise_callback  = (void*) dummy_shader_promise,
	.callback          = (void*) dummy_shader_cb,
	.callback_args     = NULL,
	.alpha_promise_0   = false,
	.alpha_promise_255 = true,
};

void thing(int64_t now, bool shaded) {
	pax_push_2d(&buf);
	
	// Rotate centerr.
	float a0 = now % 10000 / 10000.0f * 2 * M_PI;
	float r0 = 30;
	pax_apply_2d(&buf, matrix_2d_translate(80 + r0*cosf(a0), 120 - r0*sinf(a0)));
	
	// Draw circle.
	float a1 = now % 2000 / 2000.0f * 2 * M_PI;
	float r2 = 15 + 5*sinf(a1);
	if (shaded) {
		pax_draw_circle(&buf, 0xffff0000, 0, 0, r2);
	} else {
		pax_shade_circle(&buf, 0, &dummy_shader, NULL, 0, 0, r2);
	}
	
	pax_pop_2d(&buf);
}

extern "C" void testing() {
	while (1) {
		pax_background(&buf, 0);
		int64_t now = esp_timer_get_time() / 1000;
		pax_recti clip = { 40, 80, 80, 80 };
		
		pax_draw_rect(&buf, 0xff3f3f3f, clip.x, clip.y, clip.w, clip.h);
		pax_clip(&buf, clip.x, clip.y, clip.w, clip.h);
		thing(now, false);
		pax_noclip(&buf);
		
		pax_push_2d(&buf);
		pax_apply_2d(&buf, matrix_2d_translate(160, 0));
		
		pax_draw_rect(&buf, 0xff3f3f3f, clip.x, clip.y, clip.w, clip.h);
		pax_clip(&buf, clip.x+160, clip.y, clip.w, clip.h);
		thing(now, true);
		pax_noclip(&buf);
		
		pax_pop_2d(&buf);
		
		disp_flush();
		
		rp2040_input_message_t msg;
		if (xQueueReceive(get_rp2040()->queue, &msg, 1)) {
			if (msg.input == RP2040_INPUT_BUTTON_HOME) break;
		}
	}
}



/*
extern "C" void testing() {
	pax_background(&buf, 0);
	
	pax_draw_text(&buf, 0xffffffff, pax_font_sky, 9, 5, 5, "Hi Ther.");
	pax_draw_rect(&buf, 0xffff0000, 0, 20, 40, 5);
	pax_draw_rect(&buf, 0xff00ff00, 0, 25, 40, 5);
	pax_draw_rect(&buf, 0xff0000ff, 0, 30, 40, 5);
	
	pax_draw_rect(&buf, 0xff000000, 0*3, 0, 3, 3);
	
	pax_draw_rect(&buf, 0xffff0000, 2*3, 0, 3, 3);
	pax_draw_rect(&buf, 0xff00ff00, 3*3, 0, 3, 3);
	pax_draw_rect(&buf, 0xff0000ff, 4*3, 0, 3, 3);
	
	pax_draw_rect(&buf, 0xff00ffff, 6*3, 0, 3, 3);
	pax_draw_rect(&buf, 0xffff00ff, 7*3, 0, 3, 3);
	pax_draw_rect(&buf, 0xffffff00, 8*3, 0, 3, 3);
	
	pax_draw_rect(&buf, 0xffffffff, 10*3, 0, 3, 3);
	
	size_t pngsize;
	void *pngbuf;
	bool res = pax_encode_png_buf(&buf, &pngbuf, &pngsize, 0, 0, 40, 40);
	if (!res) {
		printf("Error: %s\n", pax_desc_err(pax_last_error));
	} else {
		pax_insert_png_buf(&buf, pngbuf, pngsize,  0, 60, 0);
		pax_insert_png_buf(&buf, pngbuf, pngsize, 40, 60, 0);
		pax_insert_png_buf(&buf, pngbuf, pngsize, 80, 60, 0);
	}
	
	disp_flush();
}
*/



/*
extern "C" void testing() {
	pax::Buffer gfx {&buf};
	
	// Make a simple box with Saira Regular in 18pt.
	pax::TextBox box({0, 0, 150, 100}, pax_font_saira_regular, 18, pax::RIGHT);
	
	// Add a normal bit of text.
	box.appendText("Saira regular 18pt.");
	
	// Add a red bit of text.
	auto style   = box.getStyle();
	style.italic = true;
	style.color  = 0xffff0000;
	box.appendStyle(style);
	
	box.appendText(" Red italics.");
	
	// Draw the thing.
	gfx.background(0xff000000);
	box.draw(gfx);
	// Send to display.
	disp_flush();
}
*/



/*
pax::Color arrlol[] = {
	0xff000000,
	0xffff0000,
	0xff00ff00,
	0xffffff00,
	0xff0000ff,
	0xffff00ff,
	0xff00ffff,
	0xffffffff,
};

extern "C" void testing() {
	// LOLOLOL.
	auto cbuf = pax::Buffer(&buf);
	auto pal = pax::Buffer(320, 240, PAX_BUF_8_PAL);
	pal.getInternal()->palette = arrlol;
	pal.getInternal()->palette_size = sizeof(arrlol) / sizeof(pax::Color);
	
	// pax::disableMulticore();
	
	while (1) {
		pal.background(0);
		pal.drawCircle(5,  30, 30, 20);
		pal.drawCircle(-1, 30, 40, 20);
		pal.drawArc(5,  60, 30, 20, 0, M_PI/2);
		pal.drawArc(-1, 60, 40, 20, 0, M_PI/2);
		pal.drawString(1,  pax_font_saira_regular, 18, 0,  0, "Color 1");
		pal.drawString(2,  pax_font_saira_regular, 18, 0, 20, "Color 2");
		pal.drawString(3,  pax_font_sky, 18, 0, 40, "Color 3");
		pal.drawString(-1, pax_font_sky, 18, 0,  0, "Invisible");
		pal.drawRect(4,  10, 50, 30, 30);
		pal.drawRect(-1, 15, 55, 30, 30);
		pal.drawLine(7, 0, 0, 100, 100);
		pal.drawLine(-1, 0, 0, 100, 100);
		pax::join();
		
		cbuf.drawImageOpaque(pal, 0, 0);
		disp_flush();
		
		rp2040_input_message_t msg;
		if (xQueueReceive(get_rp2040()->queue, &msg, portMAX_DELAY)) {
			if (msg.input == RP2040_INPUT_BUTTON_HOME) break;
		}
	}
}
*/



/*
extern "C" void testing() {
	int rotation = 0;
	while (1) {
		pax_buf_set_rotation(&buf, rotation);
		pax_background(&buf, 0);
		pax_draw_arc (&buf, 0x7fff0000,  20, 20,  10, M_PI/2, M_PI);
		pax_draw_rect(&buf, 0x7f00ff00,  20, 10,  30, 10);
		pax_draw_tri (&buf, 0x7f0000ff,  10, 20,  50, 20,  10, 40);
		pax_outline_circle(&buf, -1, 100, 100, 50);
		pax_draw_text(&buf, -1, pax_font_sky, 18, 50, 20, "This is, a TEXT.");
		disp_flush();
		rp2040_input_message_t msg;
		if (xQueueReceive(get_rp2040()->queue, &msg, portMAX_DELAY)) {
			if (msg.input == RP2040_INPUT_BUTTON_HOME) break;
		}
		rotation++;
	}
	
	// int p = 0;
	// pax_background(&buf, 0);
	// pax_draw_text(&buf, 0xffffffff, pax_font_sky, 18, 100, 100, "This is, a TEXT.");
	// while (1) {
	// 	// for (int i = 0; i < 4; i++) {
	// 	// 	pax_col_t col = pax_col_hsv(p*255/320, 255, 255);
	// 	// 	pax_buf_scroll(&buf, 0, 0, -1);
	// 	// 	pax_draw_line(&buf, 0xff000000, 0, 239, 319, 239);
	// 	// 	pax_draw_line(&buf, col, p, 239, p+160, 239);
	// 	// 	pax_draw_line(&buf, col, p-320, 239, p-320+160, 239);
	// 	// 	p = (p + 1) % 320;
	// 	// }
	// 	// for (int i = 0; i < 4; i++) {
	// 	// 	pax_col_t col = pax_col_hsv(p*255/240, 255, 255);
	// 	// 	pax_buf_scroll(&buf, 0, 1, 0);
	// 	// 	pax_draw_line(&buf, 0xff000000, 0, 0, 0, 239);
	// 	// 	pax_draw_line(&buf, col, 0, p, 0, p+120);
	// 	// 	pax_draw_line(&buf, col, 0, p-240, 0, p-240+120);
	// 	// 	p = (p + 1) % 240;
	// 	// }
	// 	int dx = esp_random() % 9 - 4;
	// 	int dy = esp_random() % 9 - 4;
	// 	for (int i = esp_random() % 10 + 5; i >= 0; i--) {
	// 		pax_buf_scroll(&buf, 0xff7f7f7f, dx, dy);
	// 		disp_flush();
	// 	}
	// }
}
*/
