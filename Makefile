# export SYSTEMC_HOME=/usr/local/systemc233

default:
	g++ -I. -I$SYSTEMC_HOME/include -L. -L$SYSTEMC_HOME/lib-linux64 -Wl,-rpath=$SYSTEMC_HOME/lib-linux64 -o hello system_start.cpp -lsystemc -lm