#pragma once

#include "systemc.h"
#include "IO.h"

using namespace std;


SC_MODULE(bus) {

	sc_in<bool>  clk_i;

	sc_in<float> data_i_IO;
	sc_out<bool> wr_o_IO;
	sc_out<float> data_o_IO;
	sc_in<bool> wr_i_IO;
	sc_in<bool> rd_i_IO;
	sc_in<int> addr_i_IO;

	sc_in<bool> wr_i_core[cores_count];
	sc_in<bool> rd_i_core[cores_count];
	sc_in<int> addr_i_core[cores_count];
	sc_out<bool> wr_o_core[cores_count];
	sc_out<float> data_ow_core[cores_count];
	sc_out<float> data_o_core[cores_count];
	sc_out<int> data_len_o_core[cores_count];
	sc_in <float> data_i_core[cores_count];

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
		//	ЯДРА
		//-------------------------------------------------------------------------------------

		int core_inp_len[cores_count];
		int core_out_len[cores_count];
		for (int i = 0; i < cores_count; i++)
		{
			
			// чтение длины входных данных ядра
			wr_o_core[i].write(1);
			while (!rd_i_core[i].read()) wait();
			addr_o_mem.write(addr_i_core[i].read());
			wr_len_o_mem.write(1);
			wait();
			wr_len_o_mem.write(0);
			wait();
			data_ow_core[i].write(data_len_i_mem.read());
			core_inp_len[i] = data_len_i_mem.read();
			wr_o_core[i].write(0);
			wait();

			// чтение длины выходных данных ядра
			wr_o_core[i].write(1);
			while (!rd_i_core[i].read()) wait();
			addr_o_mem.write(addr_i_core[i].read());
			wr_len_i_mem.write(1);
			wait();
			wr_len_i_mem.write(0);
			wait();
			float a = data_len_i_mem.read();
			// std::cout << a << "\n";
			data_ow_core[i].write(a);
			core_out_len[i] = data_len_i_mem.read();
			wr_o_core[i].write(0);
			wait();

			// чтение весов ядра
			for (int j = 0; j < core_out_len[i]; j++)
			{
				for (int k = 0; k < core_inp_len[i]; k++)
				{
					while (!rd_i_core[i].read()) wait();
					addr_o_mem.write(addr_i_core[i].read());
					rd_o_mem.write(1);
					wait();
					rd_o_mem.write(0);
					wait();
					float a = data_i_mem.read();
					// std::cout << a << "\n";
					data_ow_core[i].write(a);
				}
			}
			cout << sc_time_stamp() << "  CORE: " << i << endl;
		}

		while(true)
		{
			// запись входных данных
			
			for (int i = 0; i < core_inp_len[0]; i++)
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
			auto start = sc_time_stamp();
			// wr_o_IO.write(0);

			
			for (int i = 0; i < cores_count; i++)
			{
				sc_core::sc_time start = sc_time_stamp();
				// cout << sc_time_stamp() << " START CORE: " << i << endl;

				// передача входных данных в ядро
				wr_o_core[i].write(1);
				for (int j = 0; j < core_inp_len[i]; j++)
				{
					while (!rd_i_core[i].read()) wait();
					addr_o_mem.write(addr_i_core[i].read());
					rd_o_mem.write(1);
					wait();
					rd_o_mem.write(0);
					wait();
					data_o_core[i].write(data_i_mem.read());
				}
				wr_o_core[i].write(0);

				// запись выходных данных ядра в память
				for (int j = 0; j < core_out_len[i]; j++)
				{
					while (!wr_i_core[i].read()) wait();
					addr_o_mem.write(addr_i_core[i].read());
					data_o_mem.write(data_i_core[i].read());
					// std::cout <<"j: " << i << " " << data_co_core1 << "\n";
					wr_o_mem.write(1);
					wait();
					wr_o_mem.write(0);
					wait();
				}

				while (!rd_i_core[i].read()) wait();
				
			}

			// запись выходных данных в устройство вывода

			cout << sc_time_stamp()-start << " время вычисления" << endl;
			while (!rd_i_IO.read()) wait();
			

			for (int i = 0; i < core_out_len[cores_count-1]; i++)
			{
				int addr = (out_addr + cores_count) << 8;
				addr |= i;
				addr_o_mem.write(addr);
				rd_o_mem.write(1);
				wait();
				rd_o_mem.write(0);
				wait();
				data_o_IO.write(data_i_mem.read());
				wait();
				wr_o_IO.write(1);
				wait();
				wr_o_IO.write(0);
				// cout << data_i_mem.read() << endl;
			}
			
		}
	}

	SC_CTOR(bus) {

		wr_o_IO.initialize(0);
		
		data_o_mem.initialize(0);
		wr_o_mem.initialize(0);
		for (int i = 0; i < 3; i++)
		{
			
			data_o_IO.initialize(0);
			
		}

		for (int i = 0; i < cores_count; i++)
			data_o_core[i].initialize(0);

		addr_o_mem.initialize(0);

		SC_CTHREAD(bus_process, clk_i.pos());
	}

};