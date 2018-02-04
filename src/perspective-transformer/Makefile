# OpenCV based Image Processing Algorithm.
# GitHub: techping
# grayscale-transform folder.

SRC = $(wildcard *.cc)
OBJ = $(patsubst %cc, %o, ${SRC})

.PHONY: all clean

all: ${OBJ}

%.o: %.cc
	${CXX} $^ -o $@ ${OPENCV_LIBS}

clean:
	-@rm ${OBJ}
