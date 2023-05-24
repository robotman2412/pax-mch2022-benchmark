PORT ?= /dev/ttyACM0
BUILDDIR ?= build
IDF_PATH ?= $(shell pwd)/esp-idf
IDF_EXPORT_QUIET ?= 0
SHELL := /usr/bin/env bash

.PHONY: prepare clean build install monitor menuconfig

all: prepare build install

prepare:
	git submodule update --init --recursive
	cd esp-idf; bash install.sh

clean:
	rm -rf "$(BUILDDIR)"

paxclean:
	rm -rf "$(BUILDDIR)/esp-idf/pax-graphics/" "$(BUILDDIR)/esp-idf/pax-codecs/"

build:
	source "$(IDF_PATH)/export.sh" && idf.py build

install: build
	python3 tools/app_push.py build/main.bin pax_gfx_bench "PAX GFX benchmark" 1 --run

monitor:
	source "$(IDF_PATH)/export.sh" && idf.py monitor -p $(PORT)

menuconfig:
	source "$(IDF_PATH)/export.sh" && idf.py menuconfig
