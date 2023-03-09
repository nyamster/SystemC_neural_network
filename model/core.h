#pragma once
#include "systemc.h"
#include "main.h"
#include "IO.h"

SC_MODULE(core) {	

	sc_in<bool>  clk_i;
	sc_in <bool> wr_ci;
	sc_in <float> data_ci;
	sc_out <bool> wr_co;
	sc_out <float> data_co;
	sc_out<bool> rd_bo;					
	sc_out<bool> wr_bo;						
	sc_out<int>  addr_bo;				
	sc_in <float>  data_bi;
	sc_in <int>  data_len;

	static int count;
	int core_num;


	// core main method
	void input_read()
	{
		wr_co.write(0);
		while (!(wr_ci.read()))
		{
			wait();
		}
		// cout << "INPUT LEN2: " << inp_len << " " << core_num << endl;
		// cout <<"core: "<< core_num<<" input: "<<sc_time_stamp() << endl;
		for (int i = 0; i < inp_len; i++)
		{
			int addr = (out_addr + (core_num-1)) << 8;
			// cout <<"core: "<< core_num<<" input: "<< (out_addr + (core_num-1)) << " " << i << endl;
			addr |= i;
			addr_bo.write(addr);
			rd_bo.write(1);
			wait();
			wait();
			rd_bo.write(0);
			wait();
			wait();
			test[i] = (float)data_ci.read();
			// cout <<"core: "<< core_num<<" input data: "<< test[i] << endl;
		}
		wait();
			
		for (int i(0); i < out_len; i++)
		{
			out_hidden[i] = 0;
			for (int j(0); j < inp_len; j++)
			{
				out_hidden[i] += test[j] * weight[i][j];
			}
			out_hidden[i] = 1 / (1 + exp(-out_hidden[i]));
		}
		
		for (int i(0); i < out_len; i++)
		{
			int addr = (out_addr + core_num) << 8;
			// cout <<"core: "<< core_num<<" output: "<< (out_addr + core_num) << " " << i << endl;
			// cout << i+(core_num-1)*10 << endl;
			addr |= i;
			addr_bo.write(addr);
			data_co.write(out_hidden[i]);
			wr_bo.write(1);
			wait();
			wait();
			wr_bo.write(0);
			wait();
			wait();

			// cout <<"core: "<< core_num<<" output data: "<< out_hidden[i] << endl;
			// data_co[i].write(out_hidden[i]);
			// cout << out_hidden[i] << endl;
		}
		wait();
		rd_bo.write(1);
		wait();
		rd_bo.write(0);
		wr_co.write(1);
		wait();

	}
	//core main thread
	void weight_read()
	{

		while (!(wr_ci.read()))
		{
			wait();
		}
		int addr = core_num << 16;
		addr_bo.write(addr);
		rd_bo.write(1);
		wait();
		wait();
		rd_bo.write(0);
		wait();
		wait();
		inp_len = data_bi.read();
		// cout << "INPUT LEN: " << inp_len << " " << core_num << endl;

		while (!(wr_ci.read()))
		{
			wait();
		}
		addr = core_num << 16;
		addr_bo.write(addr);
		rd_bo.write(1);
		wait();
		wait();
		rd_bo.write(0);
		wait();
		wait();
		wait();
		out_len = data_bi.read();
		// cout << "OUTPUT LEN: " << out_len << " " << core_num << endl;

		for (int i(0); i < out_len; i++)
		{
			for (int j = 0; j < inp_len; j++)
			{
				int addr = core_num << 16;
				addr |= ((i+core_num*weight_base_addr) << 8) & 0x0000ff00;
				// cout << (i+core_num*weight_base_addr) << endl;
				addr |= j & 0x000000ff;
				addr_bo.write(addr);
				rd_bo.write(1);
				wait();
				wait();
				rd_bo.write(0);
				wait();
				wait();
				weight[i][j] = data_bi.read();
				// std::cout << i << " " << " " << j << " " <<weight[i][j] << "\n";
			}
		}

		while (true)
		{
			input_read();
		}
	}

	SC_CTOR(core) {

		count++;
		core_num = count;

		addr_bo.initialize(0);
		wr_co.initialize(0);
		rd_bo.initialize(0);
		wr_bo.initialize(0);
		addr_bo.initialize(0);
		data_co.initialize(0);
		weight.resize(49);
		for(int i(0);i<49;i++)
		{
			weight[i].resize(49);
		}
		out_hidden.resize(49);
		test.resize(49);
		SC_CTHREAD(weight_read, clk_i.pos());
	}

private:
	vector<vector<float>> weight;
	vector<float> out_hidden;
	vector<float> test;
	int inp_len;
	int out_len;
};

int core::count = 0;


