
#include "testing.h"

#include <pax_cxx.hpp>
#include <pax_gui.hpp>

extern uint8_t smile_png_start[] asm("_binary_smile_png_start");
extern uint8_t smile_png_end[]   asm("_binary_smile_png_end");

namespace gui = pax::gui;

gui::InputButton translateInput(rp2040_input_message_t msg) {
	switch (msg.input) {
		default: return gui::UNKNOWN;
		case RP2040_INPUT_BUTTON_HOME:		return gui::HOME;
		case RP2040_INPUT_BUTTON_MENU:		return gui::MENU;
		case RP2040_INPUT_BUTTON_START:		return gui::START;
		case RP2040_INPUT_BUTTON_ACCEPT:	return gui::ACCEPT;
		case RP2040_INPUT_BUTTON_BACK:		return gui::BACK;
		case RP2040_INPUT_FPGA_CDONE:		return gui::UNKNOWN;
		case RP2040_INPUT_BATTERY_CHARGING:	return gui::UNKNOWN;
		case RP2040_INPUT_BUTTON_SELECT:	return gui::SELECT;
		case RP2040_INPUT_JOYSTICK_LEFT:	return gui::LEFT;
		case RP2040_INPUT_JOYSTICK_PRESS:	return gui::CENTER;
		case RP2040_INPUT_JOYSTICK_DOWN:	return gui::DOWN;
		case RP2040_INPUT_JOYSTICK_UP:		return gui::UP;
		case RP2040_INPUT_JOYSTICK_RIGHT:	return gui::RIGHT;
	}
}

extern "C" void testing() {
	pax::Buffer cbuf(&buf);
	
	// for (int s = 127; s < 256; s+=32) {
	// 	for (int v = 63; v < 256; v+=32) {
	// 		for (int h = 7; h < 256; h+=8) {
	// 			pax::Color rgb = pax::hsv(h, s, v);
	// 			uint8_t un_h, un_s, un_v;
	// 			pax::undo_hsv(rgb, un_h, un_s, un_v);
	// 			printf("HSV(%3d, %3d, %3d) -> #%06x -> HSV(%3d, %3d, %3d)\n",
	// 				h * 360 / 255, s * 100 / 255, v * 100 / 255,
	// 				rgb & 0xffffff,
	// 				un_h * 360 / 255, un_s * 100 / 255, un_v * 100 / 255
	// 			);
	// 		}
	// 	}
	// }
	
	for (int h = 0; h < 256; h++) {
		int s=127, v=255;
		pax::Color rgb = pax::hsv(h, s, v);
		uint8_t un_h, un_s, un_v;
		uint8_t r = rgb>>16, g = rgb>>8, b = rgb;
		pax::undo_hsv(rgb, un_h, un_s, un_v);
		printf("%d,%d,%d,%d,%d\n", h, un_h, r, g, b);
	}
	
	cbuf.background(0);
	cbuf.fillColor = 0xffffffff;
	cbuf.lineColor = 0xffffffff;
	
	// gui::Button base({20, 20, 100, 20}, "A Button!", [](gui::Button &b) -> void {
	// 	ESP_LOGI("lololol", "BUTTON PRESSED!");
	// });
	
	gui::ColorPicker base({10, 10, 200, 200}, true, true);
	base.setHSV(32, 127, 127);
	
	bool needsADraw = true;
	uint64_t prev = esp_timer_get_time() / 1000;
	while (1) {
		uint64_t now = esp_timer_get_time() / 1000;
		
		// Draw GUI stuffs.
		if (needsADraw) {
			cbuf.background(gui::getTheme()->backgroundColor);
			base.draw(cbuf);
			disp_flush();
			needsADraw = false;
		}
		
		// Do a ticking.
		needsADraw |= base.tick(now, prev - now);
		
		// Await input.
		rp2040_input_message_t msg;
		if (xQueueReceive(get_rp2040()->queue, &msg, 1)) {
			// Translate MCH2022 input to PAX GUI input.
			auto input = translateInput(msg);
			// Home button is still exit app.
			if (input == gui::HOME) break;
			// Forward event.
			base.buttonChange(input, msg.state);
			needsADraw = true;
		}
		
		prev = now;
	}
}
