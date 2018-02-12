# OpenCV based Image Processing Algorithm.
# GitHub: techping
# grayscale-transform folder.

SRC = $(wildcard *.cc)
OBJ = $(patsubst %cc, %o, ${SRC})
EIGEN3_LIBS = $(shell pkg-config --cflags --libs eigen3)

.PHONY: all clean

all: ${OBJ}

%.o: %.cc
	${CXX} $^ -o $@ ${EIGEN3_LIBS} ${OPENCV_LIBS}

clean:
	-@rm ${OBJ}
