#pragma once
#include "systemc.h"
#include "main.h"
#include "IO.h"

#define epochs 20

SC_MODULE(core_last) {

	sc_in<bool>  clk_i;
	sc_in <float> data_ci;
	sc_in<bool>	 wr_ci[corelast_o_size];						// reading input flag
	sc_out <float> data_bo[corelast_o_size];	
	sc_out<bool> rd_bo;						// reading input flag
	sc_out<bool> wr_bo;						// writing output flag
	sc_out<int>  addr_bo;			    	// writing address
	sc_in <float>  data_bi;					// weight data


	// print float prediction
	void get_prediction() {
		cout << "Prediction: ";
		for_ideal()
			cout << prediction[i] << " ";
		cout << endl;
	}

	// sending results to the bus 
	void bus_write(int addr, vector<float> data) {
		for (int i = 0; i < corelast_o_size; i++)
		{
			data_bo[i].write(data[i]);
		}
		wr_bo.write(1);

		// for (int i = 0; i < 3; i++)
		// {
		// 	int addr2 = addr << 16;
		// 	addr2 |= i;
		// 	addr_bo.write(addr2);
		// 	data_bo[i].write(data[i]);
		// 	cout << data[i] << endl;
			
		// }
		// wr_bo.write(1);
		// wait();
		// wait();
		// wr_bo.write(0);
		// wait();
		// wait();
	}

	// core main method
	void input_read()
	{
		while (!(wr_ci[0].read()/* && wr_ci[1].read() && wr_ci[2].read()*/)) wait();
		{
			for (int i = 0; i < corelast_i_size; i++)
			{
				int addr = (out_addr + cores_count) << 16;
				addr |= i;
				addr_bo.write(addr);
				rd_bo.write(1);
				wait();
				wait();
				rd_bo.write(0);
				wait();
				wait();
				data_neuron[i] = (float)data_ci.read();
			}
			wait();
			// for (int i(0); i < 30; i++)
			// {
			// 	data_neuron[i] = (float)data_ci[i].read();
			// 	// cout << data_neuron[i] << endl;
			// }
			for (int i(0); i < corelast_o_size; i++)
			{
				prediction[i] = 0;
				for (int j(0); j < corelast_i_size; j++)
				{
					prediction[i] += data_neuron[j] * weight[i][j];
				}
				prediction[i] = 1 / (1 + exp(-prediction[i]));
				// cout << prediction[i] << " ";
			}
			bus_write(33, prediction);
		}

	}
	//core main thread
	void weight_read() {
		for (int i = 0; i < 10000; i++) wait();
		for (int i(0); i < corelast_o_size; i++)
		{
			for (int j = 0; j < corelast_i_size; j++)
			{
				int addr = (i+corelast_i_size) << 16;
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
				// cout << weight[i][j] << endl;
			}
		}

		while (true)
		{
			input_read();
		}

		// while (!(wr_ci[0].read() && wr_ci[1].read() && wr_ci[2].read()))
		// 	wait();
		// bus_write(33, prediction);
		
		//cout << weight[0][0];
	}


	SC_CTOR(core_last) {

		addr_bo.initialize(0);
		rd_bo.initialize(0);
		wr_bo.initialize(0);

		data_neuron.resize(corelast_i_size);
		prediction.resize(corelast_o_size);
		weight.resize(corelast_o_size);
		for (int i(0); i < corelast_o_size; i++)
		{
			data_bo[i].initialize(0);
			weight[i].resize(49);
		}

		SC_CTHREAD(weight_read, clk_i.pos());
		// SC_METHOD(input_read);
		// sensitive << clk_i.pos();

	}

private:
	vector<vector<float>> weight;
	vector<float> data_neuron;
	vector<float> prediction;
};


