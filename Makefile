PORT ?= /dev/ttyACM0
BUILDDIR ?= build
IDF_PATH ?= $(shell pwd)/esp-idf
IDF_EXPORT_QUIET ?= 0
SHELL := /usr/bin/env bash

.PHONY: prepare clean build monitor menuconfig

all: prepare build install

prepare:
	git submodule update --init --recursive
	cd esp-idf; bash install.sh

clean:
	rm -rf "$(BUILDDIR)"

paxclean:
	rm -rf "$(BUILDDIR)/esp-idf/pax-graphics/"

build:
	source "$(IDF_PATH)/export.sh" && idf.py build

install: build
	python3 tools/webusb_push.py "PAX GFX benchmark" build/main.bin --run

monitor:
	source "$(IDF_PATH)/export.sh" && idf.py monitor -p $(PORT)

menuconfig:
	source "$(IDF_PATH)/export.sh" && idf.py menuconfig
