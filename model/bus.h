#pragma once

#include "systemc.h"
#include "IO.h"

using namespace std;


SC_MODULE(bus) {

	sc_in<bool>  clk_i;

	sc_in<float> data_i_IO;
	sc_out<bool> wr_o_IO;
	sc_out<float> data_o_IO[corelast_o_size];
	sc_in<bool> wr_i_IO;
	sc_in<bool> rd_i_IO;
	sc_in<int> addr_i_IO;

	sc_in<bool> wr_i_core1;
	sc_in<bool> rd_i_core1;
	sc_in<int> addr_i_core1;
	sc_in<bool> wr_co_core1;
	sc_out<bool> wr_o_core1;
	sc_out<float> data_ow_core1;
	sc_out<float> data_o_core1;
	sc_out<int> data_len_o_core1;
	sc_in <float> data_co_core1;
	

	sc_in<bool> wr_i_core2;
	sc_in<bool> rd_i_core2;
	sc_in<int> addr_i_core2;
	sc_in<bool> wr_co_core2;
	sc_out<bool> wr_o_core2;
	sc_out<float> data_ow_core2;
	sc_out<float> data_o_core2;
	sc_out<int> data_len_o_core2;
	sc_in <float> data_co_core2;

	// sc_in<bool> wr_i_core3;
	// sc_in<bool> rd_i_core3;
	// sc_out<bool> wr_o_core3;
	// sc_in<int> addr_i_core3;
	// sc_out<float> data_ow_core3;
	// sc_out<float> data_o_core3[49];
	// sc_out<int> data_len_o_core3;

	sc_in<bool> wr_i_corelast;
	sc_in<bool> rd_i_corelast;
	sc_in<int> addr_i_corelast;
	sc_in<float> data_i_corelast[corelast_o_size];
	sc_out<float> data_ow_corelast;
	sc_out <float> data_ci_corelast;

	sc_out<bool> wr_o_mem;
	sc_out<bool> wr_len_o_mem;
	sc_out<bool> wr_len_i_mem;
	sc_in<int> data_len_i_mem;
	sc_out<bool> rd_o_mem;
	sc_out<int> addr_o_mem;
	sc_in<float> data_i_mem;
	sc_out<float> data_o_mem;


	// bus main thread
	void bus_process()
	{
		//-------------------------------------------------------------------------------------
		//	1 ЯДРО
		//-------------------------------------------------------------------------------------

		// чтение длины входных данных 1 ядра
		while (!rd_i_core1.read()) wait();
		addr_o_mem.write(addr_i_core1.read());
		wr_len_o_mem.write(1);
		wait();
		wr_len_o_mem.write(0);
		wait();
		data_ow_core1.write(data_len_i_mem.read());

		// чтение длины выходных данных 1 ядра
		while (!rd_i_core1.read()) wait();
		addr_o_mem.write(addr_i_core1.read());
		wr_len_i_mem.write(1);
		wait();
		wr_len_i_mem.write(0);
		wait();
		float a = data_len_i_mem.read();
		// std::cout << a << "\n";
		data_ow_core1.write(a);

		// чтение весов 1 ядра
		for (int i = 0; i < core1_o_size; i++)
		{
			for (int j = 0; j < core1_i_size; j++)
			{
				while (!rd_i_core1.read()) wait();
				addr_o_mem.write(addr_i_core1.read());
				rd_o_mem.write(1);
				wait();
				rd_o_mem.write(0);
				wait();
				float a = data_i_mem.read();
				// std::cout << a << "\n";
				data_ow_core1.write(a);
			}
		}
		cout << sc_time_stamp() << "  CORE1" << endl;

		//-------------------------------------------------------------------------------------
		//	2 ЯДРО
		//-------------------------------------------------------------------------------------

		// // чтение длины входных данных 2 ядра
		// while (!rd_i_core2.read()) wait();
		// addr_o_mem.write(addr_i_core2.read());
		// wr_len_o_mem.write(1);
		// wait();
		// wr_len_o_mem.write(0);
		// wait();
		// data_ow_core2.write(data_len_i_mem.read());

		// // чтение длины выходных данных 2 ядра
		// while (!rd_i_core2.read()) wait();
		// addr_o_mem.write(addr_i_core2.read());
		// wr_len_i_mem.write(1);
		// wait();
		// wr_len_i_mem.write(0);
		// wait();
		// float a = data_len_i_mem.read();
		// // std::cout << a << "\n";
		// data_ow_core1.write(a);

		// // чтение весов 2 ядра
		// for (int i = 0; i < core2_o_size; i++)
		// {
		// 	for (int j = 0; j < core2_i_size; j++)
		// 	{
		// 		while (!rd_i_core2.read()) wait();
		// 		addr_o_mem.write(addr_i_core2.read());
		// 		rd_o_mem.write(1);
		// 		wait();
		// 		rd_o_mem.write(0);
		// 		wait();
		// 		float a = data_i_mem.read();
		// 		// std::cout << a << "\n";
		// 		data_ow_core2.write(a);
		// 	}
		// }
		// cout << sc_time_stamp() << "  CORE2" << endl;

		//-------------------------------------------------------------------------------------
		//	ПОСЛЕДНЕЕ ЯДРО
		//-------------------------------------------------------------------------------------
		for (int i = 0; i < corelast_o_size; i++)
		{
			for (int j = 0; j < corelast_i_size; j++)
			{
				while (!rd_i_corelast.read()) wait();
				addr_o_mem.write(addr_i_corelast.read());
				rd_o_mem.write(1);
				wait();
				rd_o_mem.write(0);
				wait();
				data_ow_corelast.write(data_i_mem.read());
			}
		}
		
		cout << sc_time_stamp() << "  CORE LAST" << endl;

		while(true)
		{
			while (!wr_i_IO.read()) wait();
			for (int i = 0; i < core1_i_size; i++)
			{
				while (!wr_i_IO.read()) wait();
				addr_o_mem.write(addr_i_IO.read());
				data_o_mem.write(data_i_IO.read());
				// std::cout <<"j: " << i << " " << data_i_IO[0] << "\n";
				wr_o_mem.write(1);
				wait();
				wr_o_mem.write(0);
				wait();
			}

			wr_o_core1.write(1);

			for (int i = 0; i < core1_i_size; i++)
			{
				while (!rd_i_core1.read()) wait();
				addr_o_mem.write(addr_i_core1.read());
				rd_o_mem.write(1);
				wait();
				rd_o_mem.write(0);
				wait();
				data_o_core1.write(data_i_mem.read());
			}
			wr_o_core1.write(0);

			for (int i = 0; i < core1_o_size; i++)
			{
				while (!wr_i_core1.read()) wait();
				addr_o_mem.write(addr_i_core1.read());
				data_o_mem.write(data_co_core1.read());
				// std::cout <<"j: " << i << " " << data_co_core1[0] << "\n";
				wr_o_mem.write(1);
				wait();
				wr_o_mem.write(0);
				wait();
			}

			while (!rd_i_core1.read()) wait();

			for (int i = 0; i < corelast_i_size; i++)
			{
				while (!rd_i_corelast.read()) wait();
				addr_o_mem.write(addr_i_corelast.read());
				rd_o_mem.write(1);
				wait();
				rd_o_mem.write(0);
				wait();
				float a = data_i_mem.read();
				// cout << a << endl;
				data_ci_corelast.write(a);
			}

			// for (int i = 0; i < 30; i++)
			// {
			// 	data_ci_corelast[i].write(data_co_core1[i].read());
			// }
			wait();
		}
		

	}

	void bus_O()
	{
		if (wr_i_corelast.read())
		{
			for (int i = 0; i < corelast_o_size; i++)
			{
				// wr_o_mem.write(1);
				// addr_o_mem.write(addr_i_corelast.read());
				// data_o_mem.write(data_i_corelast.read());
				wr_o_IO.write(1);
				data_o_IO[i].write(data_i_corelast[i].read());
			}
		}
		else
		{
			wr_o_IO.write(0);
		}
	}



	SC_CTOR(bus) {

		wr_o_IO.initialize(0);
		
		data_o_mem.initialize(0);
		wr_o_mem.initialize(0);
		for (int i = 0; i < corelast_o_size; i++)
		{
			
			data_o_IO[i].initialize(0);
			
		}

		data_o_core1.initialize(0);
		data_ow_corelast.initialize(0);
		data_ci_corelast.initialize(0);

		addr_o_mem.initialize(0);

		SC_CTHREAD(bus_process, clk_i.pos());

		// SC_METHOD(bus_I);
		// sensitive << clk_i.pos();

		SC_METHOD(bus_O);
		sensitive << clk_i.pos();

		//SC_METHOD(bus_core1, clk_i.pos());
		//sensitive << clk_i.pos();

		//SC_METHOD(bus_core2, clk_i.pos());
		//sensitive << clk_i.pos();

		//SC_METHOD(bus_core3, clk_i.pos());
		//sensitive << clk_i.pos();

		//SC_METHOD(bus_corelast, clk_i.pos());
		//sensitive << clk_i.pos();

	}

	//private:
	//	int mem[9];

};