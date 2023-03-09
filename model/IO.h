#pragma once

#include "systemc.h"
#include <vector>
//#include "neural_net/Process.h"

#define train_dim 49
#define ideal_dim 3
#define data_dim 52
#define set_size 3

#define weight_base_addr 49
#define out_addr 5

#define cores_count 2

#define core1_i_size 49
#define core1_o_size 10

#define core2_i_size 10
#define core2_o_size 5

#define corelast_i_size 5
#define corelast_o_size 3

#define for_data() for(int i(0); i < data_dim; i++)
#define for_train() for(int i(0); i < train_dim; i++)
#define for_ideal() for(int i(0); i < ideal_dim; i++)

using namespace std;

SC_MODULE(IO) {

	sc_in<bool>  clk_i;
	sc_out <float> data_o;
	sc_in <bool> wr_i;
	sc_in<float> data_i[3];	
	sc_out<bool> rd_bo;	
	sc_out<bool> wr_bo;						
	sc_out<int>  addr_bo;			

	vector<float> test_arr;
	vector<float> prediction;

	void parsing_result(vector<float> out) {
		if (out[0] > out[1] && out[0] > out[2])
			cout << "CIRCLE";

		else if (out[1] > out[0] && out[1] > out[2])
			cout << "SQUARE";

		else if (out[2] > out[1] && out[2] > out[0])
			cout << "TRIANGLE";

		else
			cout << "CAN'T RECOGNIZE";
		cout << endl;
	}
	void send_data(string file_name)
	{		
		ifstream fin(file_name);
		while (!fin.eof()) {
			for (int i(0); i < train_dim; i++)
				fin >> test_arr[i];
		}

		for (int i = 0; i < 49; i++)
		{
			int addr = out_addr << 8;
			// cout << i+(core_num-1)*10 << endl;
			addr |= i;
			addr_bo.write(addr);
			data_o.write(test_arr[i]);
			wr_bo.write(1);
			wait();
			wait();
			wr_bo.write(0);
			wait();
			wait();
			
			
			// std::cout << i << " " << " " << j << " " <<weight[i][j] << "\n";
		}

		// for_train()
		// {
		// 	data_o[i].write(test_arr[i]);
		// }
	}
	void result_predict()
	{
		while (!wr_i.read()) wait();
		cout << sc_time_stamp() << endl;
		for (int i(0); i < 3; i++)
		{
			prediction[i] = data_i[i].read();
		}
		for (int i = 0; i < 3; i++)
		{
			cout << prediction[i] << " ";
		}
		cout << endl;
		parsing_result(prediction);
		cout << endl;
	}

	// input main thread
	void core_write() 
	{
		for (int i = 0; i < 20000; i++) wait();
		//SQUARE
		send_data("data/test_square.txt");
		cout << sc_time_stamp() << "  SQUARE" << endl;
		wait();
		for (int i = 0; i < 600; i++) wait();
		wait();
		cout << endl << "PREDICTION: " << endl;
		result_predict();		
		wait();

		send_data("data/test_circle.txt");
		cout << sc_time_stamp() << "  CIRCLE" << endl;
		wait();
		for (int i = 0; i < 600; i++) wait();
		wait();
		cout << endl << "PREDICTION: " << endl;
		result_predict();		
		wait();

		send_data("data/test_triangle.txt");
		cout << sc_time_stamp() << "  TRIANGLE" << endl;
		wait();
		for (int i = 0; i < 600; i++) wait();
		wait();
		cout << endl << "PREDICTION: " << endl;
		result_predict();		
		wait();
	}

	SC_CTOR(IO) {

		rd_bo.initialize(0);	
		wr_bo.initialize(0);						
		addr_bo.initialize(0);

		// for_train()
		{
			data_o.initialize(0);
		}

		test_arr.resize(train_dim);
		prediction.resize(3);

		SC_CTHREAD(core_write, clk_i.pos());

	}

};


