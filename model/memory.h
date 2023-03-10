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
		}			
	}

	void mem_write()
	{
		if (wr_i.read())
		{
			int addr = addr_bi.read();
			int core_num = (addr >> 16) & 0x000000ff;
			int addr1 = (addr >> 8) & 0x000000ff;
			int addr2 = addr & 0x000000ff;
			mem[addr1][addr2] = data_bi.read();
			// cout << "Data write: " << data_bi.read() << " " << addr << endl;
		}	
	}

	void read_data()
	{
		ifstream fin2(filename);
		while (!fin2.eof()) {
			for (int n = 0; n < cores_count; n++)
				for (int i(0); i < cores_o_size[n]; i++)
					for (int j(0); j < cores_i_size[n]; j++)
						fin2 >> mem[i+weight_base_addr*(n+1)][j];
		}
	}

	void read_len()
	{
		if (wr_len_i.read())
		{
			int addr = addr_bi.read();
			int core_num = (addr >> 16) & 0x000000ff;
			data_len.write(cores_i_size[core_num-1]);
		}
		if (wr_len_o.read())
		{
			int addr = addr_bi.read();
			int core_num = (addr >> 16) & 0x000000ff;
			data_len.write(cores_o_size[core_num-1]);
		}
	}

	SC_CTOR(memory) 
	{
		data_bo.initialize(0);
		data_len.initialize(0);
		mem.resize(300);
		for (int i(0); i < 300; i++)
			mem[i].resize(49);

		SC_METHOD(mem_write);
		sensitive << clk_i.pos();

		SC_METHOD(mem_read);
		sensitive << clk_i.pos();

		SC_METHOD(read_len);
		sensitive << clk_i.pos();

		SC_CTHREAD(read_data, clk_i.pos());
	}


private:
	vector<vector<float>> mem;
};