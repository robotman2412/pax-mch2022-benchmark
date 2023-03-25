
#include "testing.h"

#include "esp_vfs.h"
#include "esp_vfs_fat.h"
#include "esp_system.h"

#include <pax_gui.hpp>

static const char *TAG = "testing";

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
		case RP2040_INPUT_BUTTON_SELECT:	return gui::SELECT;
		case RP2040_INPUT_JOYSTICK_LEFT:	return gui::LEFT;
		case RP2040_INPUT_JOYSTICK_PRESS:	return gui::CENTER;
		case RP2040_INPUT_JOYSTICK_DOWN:	return gui::DOWN;
		case RP2040_INPUT_JOYSTICK_UP:		return gui::UP;
		case RP2040_INPUT_JOYSTICK_RIGHT:	return gui::RIGHT;
	}
}

static wl_handle_t s_wl_handle = WL_INVALID_HANDLE;

esp_err_t mount_internal_filesystem() {
	const esp_partition_t* fs_partition = esp_partition_find_first(ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_DATA_FAT, "locfd");
	if (fs_partition == NULL) {
		ESP_LOGE(TAG, "failed to mount locfd: partition not found");
		return ESP_FAIL;
	}
	
	const esp_vfs_fat_mount_config_t mount_config = {
		.format_if_mount_failed = true,
		.max_files              = 5,
		.allocation_unit_size   = CONFIG_WL_SECTOR_SIZE,
	};
	
	esp_err_t res = esp_vfs_fat_spiflash_mount("/internal", "locfd", &mount_config, &s_wl_handle);
	if (res != ESP_OK) {
		ESP_LOGE(TAG, "failed to mount locfd (%d)", res);
		return res;
	}
	
	return ESP_OK;
}


extern "C" void testing() {
	pax::Buffer cbuf(&buf);
	
	cbuf.background(0);
	cbuf.fillColor = 0xffffffff;
	cbuf.lineColor = 0xffffffff;
	
	// gui::Button base({20, 20, 100, 20}, "A Button!", [](gui::Button &b) -> void {
	// 	ESP_LOGI("lololol", "BUTTON PRESSED!");
	// });
	
	// gui::ColorPicker base({10, 10, 200, 200}, true, true);
	// base.setHSV(32, 127, 127);
	
	// pax_buf_t imgbuf;
	// pax_decode_png_buf(&imgbuf, smile_png_start, smile_png_end - smile_png_start, PAX_BUF_16_4444ARGB, 0);
	// gui::Image base(&imgbuf);
	
	// gui::GridContainer base({0, 0, 320, 240}, 4, 3);
	// base.doWrap = true;
	// base.appendChildT(gui::Button({0, 0, 80, 20}, "Button A"));
	// base.appendChildT(gui::Button({0, 0, 80, 20}, "Button B"));
	// base.appendChildT(gui::Button({0, 0, 80, 20}, "Button C"));
	// base.appendChildT(gui::Button({0, 0, 80, 20}, "Button D"));
	// base.appendChildT(gui::Button({0, 0, 80, 20}, "Button E"));
	// base.appendChildT(gui::Button({0, 0, 80, 20}, "Button F"));
	// base.appendChildT(gui::Button({0, 0, 80, 20}, "Button G"));
	
	// mount_internal_filesystem();
	// gui::addDefaultIconGetters();
	// gui::FilePicker base({0, 0, 320, 240}, "/internal");
	// base.onSelect = [](gui::FilePicker &base) {
	// 	auto x = base.getPath();
	// 	puts(x.c_str());
	// };
	
	gui::Keyboard base({0, 0, 320, 240});
	
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
