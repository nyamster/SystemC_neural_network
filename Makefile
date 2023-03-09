# export SYSTEMC_HOME=/usr/local/systemc-2.3.1

default:
	g++ -I. -I${SYSTEMC_HOME}/include -L. -L${SYSTEMC_HOME}/lib-linux64 -Wl,-rpath=${SYSTEMC_HOME}/lib-linux64 -o hello system_start.cpp -lsystemc -lm