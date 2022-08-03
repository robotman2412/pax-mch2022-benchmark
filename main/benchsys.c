
#include "benchsys.h"
#include "esp_system.h"
#include "esp_timer.h"
#include "esp_log.h"

static const char *TAG = "benchsys";

static double subtract_time = 0;
pax_buf_t *benchmark_buf = NULL;

// A dummy function used to calibrate benchmarks.
bool dummy_bench_func(pax_buf_t *buf, int iteration, void *args) {
	// Yep, this does nothing.
	return true;
}

// Calibrate the benchmark system to improve accuracy.
void bench_calibrate() {
	ESP_LOGI(TAG, "Starting calibration.");
	subtract_time  = 0;
	benchres_t res = bench_run(dummy_bench_func, NULL);
	if (res.success) {
		subtract_time  = res.average_time;
		ESP_LOGI(TAG, "Calibration finished!");
	} else {
		ESP_LOGE(TAG, "Calibration failed!");
	}
}

// Run a benchmark for about 2 seconds or at least 10 iterations.
benchres_t bench_run(benchfunc_t func, void *args) {
	if (!benchmark_buf) {
		ESP_LOGE(TAG, "Failed to run benchmark: Framebuffer not provided.");
		return (benchres_t) { 0,0,0,0,0,0 };
	}
	
	// Setup.
	int      iter  = 0;
	uint64_t start = esp_timer_get_time();
	uint64_t limit = start + 1000 * 1000;
	uint64_t now   = start;
	
	// Run benchmark.
	do {
		if (!func(benchmark_buf, iter, args)) {
			ESP_LOGE(TAG, "Benchmark function reports failure.");
			uint64_t took = now - start - iter * subtract_time;
			return (benchres_t) {
				.per_second       = (double) iter / took * 2,
				.average_time     = (double) took / iter,
				.total_iterations = iter,
				.runtime_us       = took,
				.runtime_ms       = took / 1000,
				.success          = false,
			};
		}
		iter ++;
		now  = esp_timer_get_time();
	} while (now < limit || iter < 10);
	
	// Adjust time againt number of iterations.
	uint64_t took = now - start - iter * subtract_time;
	
	// Pack the data and send it.
	return (benchres_t) {
		.per_second       = (double) iter / took * 1000 * 1000,
		.average_time     = (double) took / iter,
		.total_iterations = iter,
		.runtime_us       = took,
		.runtime_ms       = took / 1000,
		.success          = true,
	};
}

// Dump benchmark results to log.
void bench_log_res(const char *TAG, benchres_t res) {
	ESP_LOGI(TAG, "%d in %llums:", res.total_iterations, res.runtime_ms);
	ESP_LOGI(TAG, "%lf/s, %lfus", res.per_second, res.average_time);
}
