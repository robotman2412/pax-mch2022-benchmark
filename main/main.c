/*
 * This example code is in the Public Domain (or CC0 licensed, at your option.)
 * Unless required by applicable law or agreed to in writing, this
 * software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
 * CONDITIONS OF ANY KIND, either express or implied.
 */

// This file contains a simple Hello World app which you can base you own
// native Badge apps on.

#include "main.h"

static pax_buf_t buf;
xQueueHandle buttonQueue;

#include <esp_log.h>
static const char *TAG = "pax-esp32-benchmark";

#ifndef pax_font_sky_mono
#define pax_font_sky_mono pax_get_font("sky mono")
#endif

#ifndef pax_font_sky
#define pax_font_sky pax_get_font("sky")
#endif

// Updates the screen with the latest buffer.
void disp_flush() {
    ili9341_write(get_ili9341(), buf.buf);
}

// Exits the app, returning to the launcher.
void exit_to_launcher() {
    REG_WRITE(RTC_CNTL_STORE0_REG, 0);
    esp_restart();
}

// Circle area: 25446
static bool benchmark_circle_big(pax_buf_t *buf, int iteration, void *args) {
    pax_draw_circle(buf, (pax_col_t) args, 123, 200, 90);
    return !pax_last_error;
}

// Circle area: 1256
static bool benchmark_circle_small(pax_buf_t *buf, int iteration, void *args) {
    pax_draw_circle(buf, (pax_col_t) args, 123, 200, 20);
    return !pax_last_error;
}

// Total lines length: 929
static bool benchmark_lines(pax_buf_t *buf, int iteration, void *args) {
    pax_col_t col = (pax_col_t) args;
    
    pax_draw_line(buf, col, 3, 83, 200, 91);
    pax_draw_line(buf, col, 300, 83, 10, 91);
    pax_draw_line(buf, col, 200, 200, 100, 100);
    pax_draw_line(buf, col, 20, 0, 20, 300);
    
    return !pax_last_error;
}

extern uint8_t sample_png_start[] asm("_binary_my_image_png_start");
extern uint8_t sample_png_end[]   asm("_binary_my_image_png_end");

// File size: 130kb, Resolution: 320x240
static bool benchmark_png(pax_buf_t *buf, int iteration, void *args) {
    return pax_insert_png_buf(buf, sample_png_start, sample_png_end - sample_png_start, 0, 0, 0);
}

// Resolution: 320x240
static bool benchmark_backg(pax_buf_t *buf, int iteration, void *args) {
    pax_background(buf, 0);
    return !pax_last_error;
}

// Resolution: 320x240
static bool benchmark_bufcp(pax_buf_t *base, int iteration, void *args) {
    pax_draw_image(base, &buf, 0, 0);
    return !pax_last_error;
}

void idklol();
void app_main() {
  
    
    ESP_LOGI(TAG, "Welcome to the template app!");

    // Initialize the screen, the I2C and the SPI busses.
    bsp_init();

    // Initialize the RP2040 (responsible for buttons, etc).
    bsp_rp2040_init();
    
    // This queue is used to receive button presses.
    buttonQueue = get_rp2040()->queue;
    
    // Initialize graphics for the screen.
    pax_buf_init(&buf, NULL, 320, 240, PAX_BUF_16_565RGB);
    if (pax_last_error) exit_to_launcher();
    pax_enable_multicore(1);
    pax_background(&buf, 0);
    
    // idklol();
    
    // Initialise graphics for the benchmark.
    benchmark_buf = malloc(sizeof(pax_buf_t));
    pax_buf_init(benchmark_buf, NULL, 320, 240, PAX_BUF_16_565RGB);
    if (pax_last_error) exit_to_launcher();
    
    // Calibrate benchmark.
    bench_calibrate();
    
    // Circles.
    fancy_benchmark("Circle TU (area 25.446)", benchmark_circle_big,   (void *) 0x3f00afcf);
    fancy_benchmark("Circle SU (area 25.446)", benchmark_circle_big,   (void *) 0xff00afcf);
    fancy_benchmark("Circle TU (area 1.256)",  benchmark_circle_small, (void *) 0x3f00afcf);
    fancy_benchmark("Circle SU (area 1.256)",  benchmark_circle_small, (void *) 0xff00afcf);
    
    // Lines.
    fancy_benchmark("Line T (len. 929)",  benchmark_lines, (void *) 0x3f00afcf);
    fancy_benchmark("Line S (len. 929)",  benchmark_lines, (void *) 0xff00afcf);
    
    // Full buffer shenanigans.
    fancy_benchmark("PNG decode (320x240)",  benchmark_png,   NULL);
    fancy_benchmark("Background",            benchmark_backg, NULL);
    fancy_benchmark("Buffer copy (320x240)", benchmark_bufcp, NULL);
    
}

static float bench_res_y  = 5;
static float bench_res_x  = 160;

// Runs a benchmark with some nice visual flair added.
void fancy_benchmark(const char *name, benchfunc_t func, void *args) {
    
    // Show some text before benchmark.
    pax_draw_text(&buf, 0xffffffff, pax_font_sky, 9, 5, bench_res_y, name);
    pax_draw_text(&buf, 0xffff7f00, pax_font_sky_mono, 9, bench_res_x, bench_res_y, "RUNNING");
    disp_flush();
    
    // Run the benchmark.
    benchres_t result = bench_run(func, args);
    bench_log_res(name, result);
    
    // Show results.
    pax_col_t col = result.success ? 0xff00af00: 0xffff0000;
    char temp[64];
    snprintf(temp, sizeof(temp), "%s %4.0lf/s, %5.1lfms", result.success ? "OK:  " : "ERR: ", result.per_second, result.average_time / 1000.0);
    pax_draw_rect(&buf, 0xff000000, bench_res_x, bench_res_y, buf.width, 9);
    pax_draw_text(&buf, col, pax_font_sky_mono, 9, bench_res_x, bench_res_y, temp);
    disp_flush();
    
    bench_res_y += 11;
}


/*
#include <sdkconfig.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "esp_vfs_fat.h"
#include "driver/sdmmc_host.h"
#include "driver/sdspi_host.h"
#include "sdmmc_cmd.h"

#include <driver/gpio.h>
#include <rom/gpio.h>

esp_err_t mount_sd(int pin_cmd, int pin_clk, int pin_d0, int pin_pwr, const char* mountpoint, bool format_if_failed, int max_files) {
    const char *TAG = "sdcard";
    esp_err_t res;
    if (pin_pwr >= 0) {
        res = gpio_set_direction(pin_pwr, GPIO_MODE_OUTPUT);
        if (res != ESP_OK) return res;
        res = gpio_set_level(pin_pwr, true);
        if (res != ESP_OK) return res;
    }
    
    sdmmc_host_t host = SDMMC_HOST_DEFAULT();
    sdmmc_slot_config_t slot_config = SDMMC_SLOT_CONFIG_DEFAULT();
    
    host.flags = SDMMC_HOST_FLAG_1BIT;
    slot_config.width = 1;
    
    gpio_pad_select_gpio(pin_cmd);
    gpio_set_direction(pin_cmd, GPIO_MODE_INPUT_OUTPUT_OD);
    gpio_set_pull_mode(pin_cmd, GPIO_PULLUP_ONLY);
    gpio_set_level(pin_cmd, 1);
    
    gpio_pad_select_gpio(pin_clk);
    gpio_set_direction(pin_clk, GPIO_MODE_INPUT_OUTPUT_OD);
    gpio_set_pull_mode(pin_clk, GPIO_PULLUP_ONLY);
    gpio_set_level(pin_clk, 1);
    
    gpio_pad_select_gpio(pin_d0);
    gpio_set_direction(pin_d0,  GPIO_MODE_INPUT_OUTPUT_OD);
    gpio_set_pull_mode(pin_d0,  GPIO_PULLUP_ONLY);
    gpio_set_level(pin_d0,  1);
    
    esp_vfs_fat_sdmmc_mount_config_t mount_config = {
        .format_if_mount_failed = format_if_failed,
        .max_files              = max_files,
        .allocation_unit_size   = 0
    };
    
    sdmmc_card_t* card;
    res = esp_vfs_fat_sdmmc_mount(mountpoint, &host, &slot_config, &mount_config, &card);
    
    if (res != ESP_OK) {
        if (res == ESP_FAIL) {
            ESP_LOGE(TAG, "Failed to mount the SD card filesystem.");
        } else if (res == ESP_ERR_NO_MEM) {
            ESP_LOGE(TAG, "Failed to initialize the SD card: not enough memory).");
        } else if (res == ESP_ERR_INVALID_RESPONSE) {
            ESP_LOGE(TAG, "Failed to initialize the SD card: invalid response).");
        } else if (res == ESP_ERR_INVALID_STATE) {
            ESP_LOGE(TAG, "Failed to initialize the SD card: invalid state).");
        } else {
            ESP_LOGE(TAG, "Failed to initialize the SD card (%s). ", esp_err_to_name(res));
        }
        return res;
    }
    return res;
}

void idklol() {
    
    // Mount it???
    mount_sd(GPIO_SD_CMD, GPIO_SD_CLK, GPIO_SD_D0, GPIO_SD_PWR, "/sdcard", false, 5);
    FILE *fd = fopen("/sdcard/Comic Neue.pax_font", "r");
    pax_font_t *font = pax_load_font(fd);
    pax_background(&buf, 0);
    if (pax_last_error) {
        pax_draw_text(&buf, 0xffff0000, pax_font_sky_mono, 18, 5, 5, "Error.");
    } else {
        pax_draw_text(&buf, 0xffffffff, font, font->default_size, 5, 5, "The quick brown fox jumped\nover the lazy dog.\n\nSphinx of black quartz,\njudge my vow.");
    }
    disp_flush();
    while(1);
    
}
*/
