#include <iostream>
#include "systemc.h"
#include "model/core.h"
#include "model/core_last.h"
#include "model/memory.h"
#include "model/bus.h"
#include "model/IO.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int sc_main(int argc, char* argv[]) {

	IO IO("IOput");
	bus bus("bus");
	core core1("core1"),core2("core2");//,core3("core3");
	core_last core_last("core_last");
	memory memory("memory");
	
	
	sc_clock clk("clk", sc_time(10, SC_NS));
	sc_signal<bool> wr_i_IO;
	sc_signal<float> data_i_IO;
	sc_signal<bool> wr_o_IO;
	sc_signal<float> data_o_IO[3];
	sc_signal<bool> rd_bo_IO;	
	sc_signal<bool> wr_bo_IO;						
	sc_signal<int>  addr_bo_IO;

	sc_signal<bool> wr_i_core1;
	sc_signal<bool> rd_i_core1;
	sc_signal<int> addr_i_core1;
	sc_signal<bool> wr_o_core1;
	sc_signal<float> data_ow_core1;
	sc_signal<float> data_o_core1;
	sc_signal<int> data_len_o_core1;
	sc_signal <float> data_co_core1;
	sc_signal <bool> wr_co_core1;

	sc_signal<bool> wr_i_core2;
	sc_signal<bool> rd_i_core2;
	sc_signal<int> addr_i_core2;
	sc_signal<bool> wr_o_core2;
	sc_signal<float> data_ow_core2;
	sc_signal<float> data_o_core2;
	sc_signal<int> data_len_o_core2;
	sc_signal<float> data_co_core2;
	sc_signal <bool> wr_co_core2;

	// sc_signal<bool> wr_i_core3;
	// sc_signal<bool> rd_i_core3;
	// sc_signal<bool> wr_o_core3;
	// sc_signal<int> addr_i_core3;
	// sc_signal<float> data_ow_core3;
	// sc_signal<float> data_o_core3[49];
	// sc_signal<int> data_len_o_core3;

	sc_signal<bool> wr_i_corelast;
	sc_signal<bool> rd_i_corelast;
	sc_signal<int> addr_i_corelast;
	sc_signal<float> data_i_corelast[3];
	sc_signal<float> data_ow_corelast;
	sc_signal <float> data_ci_corelast;

	sc_signal<bool> wr_o_mem;
	sc_signal<bool> wr_len_o_mem;
	sc_signal<bool> wr_len_i_mem;
	sc_signal<int>  data_len_i_mem;
	sc_signal<bool> rd_o_mem;
	sc_signal<int> addr_o_mem;
	sc_signal<float> data_i_mem;
	sc_signal<float> data_o_mem;

	// sc_signal <bool> wr_co_core[3];
	
	//INPUT

	IO.clk_i(clk);
	// for_train()
	{
		IO.data_o(data_i_IO);
	}
	for (int i = 0; i < 3; i++)
	{
		IO.data_i[i](data_o_IO[i]);
	}
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
	for (int i = 0; i < 3; i++)
	{
		bus.data_o_IO[i](data_o_IO[i]);
	}	

	bus.wr_i_core1(wr_i_core1);
	bus.rd_i_core1(rd_i_core1);
	bus.addr_i_core1(addr_i_core1);
	bus.wr_o_core1(wr_o_core1);
	bus.data_ow_core1(data_ow_core1);
	bus.data_o_core1(data_o_core1);
	bus.data_co_core1(data_co_core1);
	bus.data_len_o_core1(data_len_o_core1);
	bus.wr_co_core1(wr_co_core1);

	bus.wr_i_core2(wr_i_core2);
	bus.rd_i_core2(rd_i_core2);
	bus.addr_i_core2(addr_i_core2);
	bus.wr_o_core2(wr_o_core2);
	bus.data_ow_core2(data_ow_core2);
	bus.data_o_core2(data_o_core2);
	bus.data_co_core2(data_co_core2);
	bus.data_len_o_core2(data_len_o_core2);
	bus.wr_co_core2(wr_co_core2);

	// bus.wr_i_core3(wr_i_core3);
	// bus.rd_i_core3(rd_i_core3);
	// bus.addr_i_core3(addr_i_core3);
	// bus.wr_o_core3(wr_o_core3);
	// bus.data_ow_core3(data_ow_core3);
	// for (int i = 0; i < 49; i++)
	// {
		
	// 	bus.data_o_core3[i](data_o_core3[i]);
	// }
	// bus.data_len_o_core3(data_len_o_core3);

	bus.wr_i_corelast(wr_i_corelast);
	bus.rd_i_corelast(rd_i_corelast);
	bus.addr_i_corelast(addr_i_corelast);
	//bus.wr_o_corelast(wr_o_core_last);
	for (int i = 0; i < 3; i++)
	{
		bus.data_i_corelast[i](data_i_corelast[i]);
	}
	bus.data_ow_corelast(data_ow_corelast);
	bus.data_ci_corelast(data_ci_corelast);

	bus.wr_o_mem(wr_o_mem);
	bus.wr_len_o_mem(wr_len_o_mem);
	bus.wr_len_i_mem(wr_len_i_mem);
	bus.data_len_i_mem(data_len_i_mem);
	bus.rd_o_mem(rd_o_mem);
	bus.addr_o_mem(addr_o_mem);
	bus.data_i_mem(data_i_mem);
	bus.data_o_mem(data_o_mem);

	// CORE 1

	core1.clk_i(clk);
	core1.wr_ci(wr_o_core1);
	core1.data_bi(data_ow_core1);
	core1.data_ci(data_o_core1);
	core1.wr_co(wr_co_core1);
	core1.data_co(data_co_core1);
	core1.rd_bo(rd_i_core1);
	core1.wr_bo(wr_i_core1);
	core1.addr_bo(addr_i_core1);
	core1.data_len(data_len_o_core1);
	
	//CORE 2

	core2.clk_i(clk);
	core2.wr_ci(wr_o_core2);
	core2.data_bi(data_ow_core2);
	core2.data_ci(data_o_core2);
	core2.wr_co(wr_co_core2);
	core2.data_co(data_co_core2);
	core2.rd_bo(rd_i_core2);
	core2.wr_bo(wr_i_core2);
	core2.addr_bo(addr_i_core2);
	core2.data_len(data_len_o_core2);

	//CORE 3

	// core3.clk_i(clk);
	// core3.wr_ci(wr_o_core3);
	// core3.data_bi(data_ow_core3);
	// for_train()
	// {
	// 	core3.data_ci[i](data_o_core3[i]);
	// }
	// core3.wr_co(wr_co_core[2]);
	// for (int i(0); i < 10; i++)
	// {
	// 	core3.data_co[i](data_co_core[i+20]);
	// }
	// core3.rd_bo(rd_i_core3);
	// core3.wr_bo(wr_i_core3);
	// core3.addr_bo(addr_i_core3);

	// core3.data_len(data_len_o_core3);

	core_last.clk_i(clk);
	core_last.data_bi(data_ow_corelast);
	core_last.data_ci(data_ci_corelast);
	core_last.wr_ci(wr_co_core2);
	for (int i(0); i < 3; i++)
	{
		
		core_last.data_bo[i](data_i_corelast[i]);
	}
	core_last.rd_bo(rd_i_corelast);
	core_last.wr_bo(wr_i_corelast);
	core_last.addr_bo(addr_i_corelast);

   	
	// MEMORY
	
	memory.clk_i(clk);
	memory.addr_bi(addr_o_mem);	
	// for (int i = 0; i < 3; i++)
	{
		memory.data_bi(data_o_mem);
	}
	memory.rd_i(rd_o_mem);
	memory.wr_len_i(wr_len_o_mem);
	memory.wr_len_o(wr_len_i_mem);
	memory.data_len(data_len_i_mem);	
	memory.wr_i(wr_o_mem);
	memory.data_bo(data_i_mem);
	

	// TRACING

	//sc_trace_file *wf = sc_create_vcd_trace_file("wave");

	cout << endl << "_________________________" << endl << endl << "Calculating..." << endl << "_________________________" << endl;
	sc_start(sc_time(400000, SC_NS));

	//sc_close_vcd_trace_file(wf);
	
	return(0);
}