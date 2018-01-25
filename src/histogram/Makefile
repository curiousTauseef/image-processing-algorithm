# OpenCV based Image Processing Algorithm.
# GitHub: techping
# image-interpolation folder.

SRC = $(wildcard *.cc)
OBJ = $(patsubst %cc, %o, ${SRC})

.PHONY: all clean

all: ${OBJ}

%.o: %.cc
	${CXX} $^ -o $@ ${OPENCV_LIBS} --std=c++11

clean:
	-@rm ${OBJ}
