CXX = g++
CXXFLAGS = -Wall -MMD -D__LINUX_ALSA_ -std=c++11 -D__LITTLE_ENDIAN__ -fcompare-debug-second ${LIBS}
LIBS = -I/usr/local/include/stk -lstk -lpigpio -lrt
OBJECTS = main.o matrix.o utils.o wavetable.o blanket.o midi.o qmidi.o
DEPENDS = ${OBJECTS:.o=.d}
EXEC = $(shell basename $(CURDIR))
${EXEC}: ${OBJECTS}
	${CXX} ${OBJECTS} ${CXXFLAGS} -o ${EXEC}
.PHONY: build
build: ${OBJECTS}
	${CXX} ${OBJECTS} ${CXXFLAGS} -o ${EXEC}
-include ${DEPENDS}
.PHONY: clean
clean:
	rm ${OBJECTS} ${DEPENDS} ${EXEC}
