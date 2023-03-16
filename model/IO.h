#pragma once

#include "systemc.h"
#include <vector>

#define weight_base_addr 49
#define out_addr 5

#define cores_count 3

const int cores_i_size[] = {49, 16, 4, 3};
const int cores_o_size[] = {16, 4, 3};

const std::string filename = "data/weight16_4.txt";

using namespace std;

SC_MODULE(IO) {

	sc_in<bool>  clk_i;
	sc_out <float> data_o;
	sc_in <bool> wr_i;
	sc_in<float> data_i;	
	sc_out<bool> rd_bo;	
	sc_out<bool> wr_bo;						
	sc_out<int>  addr_bo;			

	vector<float> test_arr;
	vector<float> prediction;

	void parsing_result(vector<float> out) {
		cout << "Результат: ";
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
			for (int i(0); i < cores_i_size[0]; i++)
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
	}
	void result_predict()
	{
		rd_bo.write(1);
		while (!wr_i.read()) wait();
		cout << "Текущее время: " << sc_time_stamp() << endl;
		for (int i(0); i < cores_o_size[cores_count-1]; i++)
		{
			while (!wr_i.read()) wait();
			prediction[i] = data_i.read();
			wait();
		}
		rd_bo.write(0);
		cout << "Выходные данные: ";
		for (int i = 0; i < cores_o_size[cores_count-1]; i++)
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
		for (int i = 0; i < 800; i++) wait();
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

		test_arr.resize(cores_i_size[0]);
		prediction.resize(cores_o_size[cores_count-1]);

		SC_CTHREAD(core_write, clk_i.pos());

	}

};


