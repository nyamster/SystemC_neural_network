#pragma once
#include <iostream>
#include <vector>
#include "main.h"
#include <fstream>
using namespace std;

vector<int> convert(vector<float> out) {
	vector<int> int_out(out.size());
	for (int i(0); i < int_out.size(); i++) {
		if (out[i] > 0.90)
			int_out[i] = 1;
		else
			int_out[i] = 0;
	}

	return int_out;
}

vector<vector<float>> from_file(string filename, int set_size, int elements_in_set) {
	ifstream fin(filename);
	vector<vector<float>> dataset(set_size, vector<float>(elements_in_set));
	while (!fin.eof()) {
		for (int i(0); i < set_size; i++) {
			for (int j(0); j < elements_in_set; j++)
				fin >> dataset[i][j];
		}
	}

	return dataset;
}