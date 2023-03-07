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
		// cout <<"core: "<< core_num<<" input: "<<sc_time_stamp() << endl;
		for (int i = 0; i < core1_i_size; i++)
		{
			int addr = (train_addr + (core_num-1)) << 16;
			addr |= i;
			addr_bo.write(addr);
			rd_bo.write(1);
			wait();
			wait();
			rd_bo.write(0);
			wait();
			wait();
			test[i] = (float)data_ci.read();
		}
		wait();

		// for_train()
		// {
		// 	test[i] = (float)data_ci[i].read();
		// }			
		for (int i(0); i < core1_o_size; i++)
		{
			out_hidden[i] = 0;
			for (int j(0); j < core1_i_size; j++)
			{
				out_hidden[i] += test[j] * weight[i][j];
			}
			out_hidden[i] = 1 / (1 + exp(-out_hidden[i]));
		}
		
		for (int i(0); i < core1_o_size; i++)
		{
			int addr = (out_addr + core_num) << 16;
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

			
			// data_co[i].write(out_hidden[i]);
			// cout << out_hidden[i] << endl;
		}
		wait();
		rd_bo.write(1);
		wait();
		rd_bo.write(0);
		wr_co.write(1);
		wait();
		// else
		// {
		// 	wr_co.write(0);
		// }

	}
	//core main thread
	void weight_read() {
		for (int i = 0; i < (core_num - 1) * 4000; i++) wait();
		for (int i(0); i < core1_o_size; i++)
		{
			for (int j = 0; j < core1_i_size; j++)
			{
				int addr = i << 16;
				// cout << i+(core_num-1)*10 << endl;
				addr |= j;
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
		// inp_len = data_len.read();
		// cout << "Hui: " << inp_len <<endl;
		// cout << sc_time_stamp() << endl;
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
		weight.resize(core1_o_size);
		for(int i(0);i<core1_o_size;i++)
		{
			weight[i].resize(core1_i_size);
		}
		out_hidden.resize(core1_o_size);
		test.resize(core1_i_size);
		SC_CTHREAD(weight_read, clk_i.pos());
		// SC_METHOD(input_read);
		// sensitive << clk_i.pos();

	}

private:
	vector<vector<float>> weight;
	vector<float> out_hidden;
	vector<float> test;
	int inp_len;
};

int core::count = 0;


