#include <iostream>
#include "systemc.h"
#include "model/core.h"
#include "model/memory.h"
#include "model/bus.h"
#include "model/IO.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int sc_main(int argc, char* argv[]) {

	IO IO("IOput");
	bus bus("bus");
	sc_vector<core>cores("core",cores_count);
	memory memory("memory");
	
	
	sc_clock clk("clk", sc_time(10, SC_NS));
	sc_signal<bool> wr_i_IO;
	sc_signal<float> data_i_IO;
	sc_signal<bool> wr_o_IO;
	sc_signal<float> data_o_IO;
	sc_signal<bool> rd_bo_IO;	
	sc_signal<bool> wr_bo_IO;						
	sc_signal<int>  addr_bo_IO;

	sc_signal<bool> wr_i_core[cores_count];
	sc_signal<bool> rd_i_core[cores_count];
	sc_signal<int> addr_i_core[cores_count];
	sc_signal<bool> wr_o_core[cores_count];
	sc_signal<float> data_ow_core[cores_count];
	sc_signal<float> data_o_core[cores_count];
	sc_signal<int> data_len_o_core[cores_count];
	sc_signal<float> data_co_core[cores_count];
	sc_signal <bool> wr_co_core[cores_count];

	sc_signal<bool> wr_o_mem;
	sc_signal<bool> wr_len_o_mem;
	sc_signal<bool> wr_len_i_mem;
	sc_signal<int>  data_len_i_mem;
	sc_signal<bool> rd_o_mem;
	sc_signal<int> addr_o_mem;
	sc_signal<float> data_i_mem;
	sc_signal<float> data_o_mem;
	
	//INPUT

	IO.clk_i(clk);
	IO.data_o(data_i_IO);
	IO.data_i(data_o_IO);
	IO.wr_i(wr_o_IO);
	IO.rd_bo(rd_bo_IO);
	IO.wr_bo(wr_bo_IO);
	IO.addr_bo(addr_bo_IO);
	
	//BUS
	bus.clk_i(clk);
	bus.wr_i_IO(wr_bo_IO);
	bus.rd_i_IO(rd_bo_IO);
	bus.addr_i_IO(addr_bo_IO);
	bus.data_i_IO(data_i_IO);
	bus.wr_o_IO(wr_o_IO);
	bus.data_o_IO(data_o_IO);

	for (int i = 0; i < cores_count; i++)
	{
		bus.wr_i_core[i](wr_i_core[i]);
		bus.rd_i_core[i](rd_i_core[i]);
		bus.addr_i_core[i](addr_i_core[i]);
		bus.wr_o_core[i](wr_o_core[i]);
		bus.data_ow_core[i](data_ow_core[i]);
		bus.data_o_core[i](data_o_core[i]);
		bus.data_co_core[i](data_co_core[i]);
		bus.data_len_o_core[i](data_len_o_core[i]);
		bus.wr_co_core[i](wr_co_core[i]);
	}

	bus.wr_o_mem(wr_o_mem);
	bus.wr_len_o_mem(wr_len_o_mem);
	bus.wr_len_i_mem(wr_len_i_mem);
	bus.data_len_i_mem(data_len_i_mem);
	bus.rd_o_mem(rd_o_mem);
	bus.addr_o_mem(addr_o_mem);
	bus.data_i_mem(data_i_mem);
	bus.data_o_mem(data_o_mem);

	//CORES

	for (int i = 0; i < cores_count; i++)
	{
		cores[i].clk_i(clk);
		cores[i].wr_ci(wr_o_core[i]);
		cores[i].data_bi(data_ow_core[i]);
		cores[i].data_ci(data_o_core[i]);
		cores[i].wr_co(wr_co_core[i]);
		cores[i].data_co(data_co_core[i]);
		cores[i].rd_bo(rd_i_core[i]);
		cores[i].wr_bo(wr_i_core[i]);
		cores[i].addr_bo(addr_i_core[i]);
		cores[i].data_len(data_len_o_core[i]);
	}
   	
	// MEMORY
	
	memory.clk_i(clk);
	memory.addr_bi(addr_o_mem);	
	memory.data_bi(data_o_mem);
	memory.rd_i(rd_o_mem);
	memory.wr_len_i(wr_len_o_mem);
	memory.wr_len_o(wr_len_i_mem);
	memory.data_len(data_len_i_mem);	
	memory.wr_i(wr_o_mem);
	memory.data_bo(data_i_mem);
	

	// TRACING

	cout << endl << "_________________________" << endl << endl << "Calculating..." << endl << "_________________________" << endl;
	sc_start(sc_time(400000, SC_NS));

	return(0);
}