/*
 * This example code is in the Public Domain (or CC0 licensed, at your option.)
 * Unless required by applicable law or agreed to in writing, this
 * software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
 * CONDITIONS OF ANY KIND, either express or implied.
 */

#pragma once


// For graphics.
#include "pax_gfx.h"
// For PNG images.
#include "pax_codecs.h"

#ifdef __cplusplus
extern "C" {
#endif

// For pin mappings.
#include "hardware.h"
// The screen driver.
#include "ili9341.h"
// For all system settings and alike.
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_system.h"
#include "nvs.h"
#include "nvs_flash.h"
#include "rp2040.h"
// For WiFi connectivity.
#include "wifi_connect.h"
#include "wifi_connection.h"
// For exiting to the launcher.
#include "soc/rtc.h"
#include "soc/rtc_cntl_reg.h"
// Benchmarking system.
#include "benchsys.h"

#ifdef __cplusplus
}
#endif

// Testing stuff.
#include "testing.h"

#ifdef __cplusplus
extern "C" {
#endif

extern pax_buf_t buf;

// Updates the screen with the last drawing.
void disp_flush();

// Exits the app, returning to the launcher.
void exit_to_launcher();

// Performs all benchmarks.
void perform_benchmark();

// Run the TechDemo in a loop.
void td_ok();

// Run the TechDemo version 2 in a loop.
void td_new();

// Runs a benchmark with some nice visual flair added.
void fancy_benchmark(const char *name, benchfunc_t func, void *args);

#ifdef __cplusplus
}
#endif
