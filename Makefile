# OpenCV based Image Processing Algorithm.
# GitHub: techping
# The top level Makefile.

CXX = g++
OPENCV_LIBS = $(shell pkg-config --cflags --libs opencv)
export CXX OPENCV_LIBS

subdirs = $(shell find src/ -maxdepth 1 -type d)
dirs = $(filter-out src/, ${subdirs})

.PHONY: all clean

all:
	@if [ ! -n "${PACKAGE}" ]; then \
		echo "No $\{PACKAGE} environmet found. Compile all..."; \
		for dir in ${dirs}; do \
			${MAKE} -C $$dir; \
		done \
	else \
		if [ -d src/${PACKAGE} ]; then \
			echo "Compile ${PACKAGE}..."; \
			${MAKE} -C src/${PACKAGE}; \
		else \
			echo "Source code for '${PACKAGE}' is not existed. Exit."; \
		fi \
	fi

clean:
	@if [ ! -n "${PACKAGE}" ]; then \
		echo "No $\{PACKAGE} environmet found. Clean all..."; \
		for dir in ${dirs}; do \
			${MAKE} -C $$dir clean; \
		done \
	else \
		if [ -d src/${PACKAGE} ]; then \
			echo "Clean ${PACKAGE}..."; \
			${MAKE} -C src/${PACKAGE} clean; \
		else \
			echo "Source code for '${PACKAGE}' is not existed. Exit."; \
		fi \
	fi
