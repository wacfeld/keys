CXX = g++
CXXFLAGS = -Wall -D__LINUX_ALSA_ -std=c++11 -D__LITTLE_ENDIAN__ -fcompare-debug-second ${LIBS}
LIBS = -I/usr/local/include/stk -lstk -lpigpio -lrt
E: E.cc
	${CXX} E.cc ${CXXFLAGS} -o E
ports: ports.cc
	${CXX} ports.cc ${CXXFLAGS} -o ports
.PHONY: clean
clean:
	rm E ports
