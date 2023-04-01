
#include "testing.h"


static const char *TAG = "testing";

extern uint8_t smile_png_start[] asm("_binary_smile_png_start");
extern uint8_t smile_png_end[]   asm("_binary_smile_png_end");



extern "C" void testing() {
	// LOLOLOL.
	while (1) {
		auto dims = pax::Buffer::stringSize(pax_font_sky, 18, "Hello, World!");
		ESP_LOGI(TAG, "Text size: %f, %f", dims.x, dims.y);
		rp2040_input_message_t msg;
		if (xQueueReceive(get_rp2040()->queue, &msg, portMAX_DELAY)) {
			if (msg.input == RP2040_INPUT_BUTTON_HOME) break;
		}
	}
}



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
