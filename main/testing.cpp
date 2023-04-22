
#include "testing.h"


static const char *TAG = "testing";

extern uint8_t smile_png_start[] asm("_binary_smile_png_start");
extern uint8_t smile_png_end[]   asm("_binary_smile_png_end");



extern "C" void testing() {
	pax_background(&buf, 0);
	
	pax_draw_round_rect(&buf, 0xffff0000, 10, 10, 50, 30, 5);
	pax_draw_hollow_arc(&buf, 0xff00ff00, 80, 30, 10, 20, M_PI*0.25, M_PI*1.75);
	pax_draw_round_hollow_arc(&buf, 0xff00ff00, 150, 30, 10, 20, M_PI*0.25, M_PI*1.75);
	
	pax_outline_round_rect(&buf, 0xffffffff, 10, 50, 50, 30, 5);
	pax_outline_hollow_arc(&buf, 0xffffffff, 80, 70, 10, 20, M_PI*0.25, M_PI*1.75);
	pax_outline_round_hollow_arc(&buf, 0xffffffff, 150, 70, 10, 20, M_PI*0.25, M_PI*1.75);
	
	disp_flush();
}



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
