#pragma once

#include "systemc.h"
#include "IO.h"

using namespace std;


SC_MODULE(memory) {

	sc_in<bool>  clk_i;
	sc_in<int>   addr_bi;		// reading address
	sc_in<float>   data_bi;		// reading data
	sc_in<bool>  rd_i;		// reading flag
	sc_in<bool>  wr_i;					// writing flag
	sc_in<bool>  wr_len_i;					// writing len input
	sc_in<bool>  wr_len_o;					// writing len output
	sc_out<int>  data_len;				// data len
	sc_out<float>  data_bo;				// writing data

	// int cnt = 0;

	// SC_HAS_PROCESS(memory);


	// bus main thread
	void mem_read()
	{
		if (rd_i.read())
		{
			int addr = addr_bi.read();
			int core_num = (addr >> 16) & 0x000000ff;
			int addr1 = (addr >> 8) & 0x000000ff;
			int addr2 = addr & 0x000000ff;
			// cout << core_num << endl;
			// std::cout << "Addr1: " << addr1 << " Addr2: " << addr2 << "\n";
			data_bo.write(mem[addr1][addr2]);
			// data_bo[i].write(mem[addr_bi.read()][i]);
		}			
	}

	void mem_write()
	{
		if (wr_i.read())
			// for (int i(0); i < 3; i++)
			{
				int addr = addr_bi.read();
				int core_num = (addr >> 16) & 0x000000ff;
				int addr1 = (addr >> 8) & 0x000000ff;
				int addr2 = addr & 0x000000ff;
				mem[addr1][addr2] = data_bi.read();
				// cout << "Data write: " << data_bi.read() << " " << addr << endl;
				// cnt++;
			}	
	}

	void read_data()
	{
		// ifstream fin("data/weight_old.txt");
		// while (!fin.eof()) {
		// 	for (int i(0); i < 30; i++)
		// 		for (int j(0); j < 49; j++)
		// 		{
		// 			fin >> mem[i+weight_base_addr][j];
		// 		}
		// 	for (int i(0); i < 3; i++)
		// 		for (int j(0); j < 30; j++)
		// 		{
		// 			fin >> mem[i+weight_base_addr*2][j];
		// 		}
		// }

		ifstream fin2("data/weight.txt");
		while (!fin2.eof()) {
			for (int i(0); i < 10; i++)
				for (int j(0); j < 49; j++)
				{
					fin2 >> mem[i+weight_base_addr][j];
				}
			for (int i(0); i < 5; i++)
				for (int j(0); j < 10; j++)
				{
					fin2 >> mem[i+weight_base_addr*2][j];
				}
			for (int i(0); i < 3; i++)
				for (int j(0); j < 5; j++)
				{
					fin2 >> mem[i+weight_base_addr*3][j];
				}
		}
	}

	void read_len()
	{
		if (wr_len_i.read())
		{
			int addr = addr_bi.read();
			int core_num = (addr >> 16) & 0x000000ff;
			if (core_num == 1)
			{
				data_len.write(core1_i_size);
			}
			if (core_num == 2)
			{
				data_len.write(core2_i_size);
			}
		}
		if (wr_len_o.read())
		{
			int addr = addr_bi.read();
			int core_num = (addr >> 16) & 0x000000ff;
			if (core_num == 1)
			{
				data_len.write(core1_o_size);
			}
			if (core_num == 2)
			{
				data_len.write(core2_o_size);
			}
		}
	}

	SC_CTOR(memory) 
	{
		// for (int i(0); i < 49; i++)
		{
			data_bo.initialize(0);
		}
		data_len.initialize(0);
		mem.resize(300);
		for (int i(0); i < 300; i++)
			mem[i].resize(49);

		mem2.resize(300);
		for (int i(0); i < 300; i++)
			mem2[i].resize(49);

		SC_METHOD(mem_write);
		sensitive << clk_i.pos();

		SC_METHOD(mem_read);
		sensitive << clk_i.pos();

		SC_METHOD(read_len);
		sensitive << clk_i.pos();

		// SC_CTHREAD(read_len, clk_i.pos());

		SC_CTHREAD(read_data, clk_i.pos());
	}


private:
	vector<vector<float>> mem;
	vector<vector<float>> mem2;
};