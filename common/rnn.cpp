#include <iostream>
#include <vector>
#include <fstream>
#include "csv.hpp"
#include "network.hpp"
#include "rnn.hpp"

#define CONF "parameters.json"
using namespace std;

void serialize(ofstream &fout, vector<double> &v, int len = -1){
	if(len == -1){
		len = v.size();
	}
	for(int i = 0; i < len; i++){
		fout << v[i] << " ";
	}
	fout << endl;
}	

Executor::Executor(){
	this->raw_time_series = load_data();
        this->conf = load_config(CONF, raw_time_series);
}

pair<vector<vector<double>>, Config> Executor::get_data(){
	return {raw_time_series, conf}; 	
}

void Executor::start(Network model){
	ofstream fout("out.data");

	cout << "Starting.." << endl << endl;
	cout << "Input dim: " << conf.input_dim << endl;
	cout << "Hidden dim: " << conf.hidden_dim << endl;
	cout << "Output dim: " << conf.output_dim << endl;
	cout << "Sequence length: " << raw_time_series.size() << endl;
	cout << "Number of epochs: " << conf.num_epochs << endl;
	cout << "Number of training sequences: " << conf.n_sequences << endl;
	cout << "Length of training sequences: " << conf.batch_size << endl;
	cout << "Learning rate: " << conf.learning_rate << endl << endl;


	vector<double> output;
	for(int i = 1; i < raw_time_series.size()-1; i++){
		output.push_back(raw_time_series[i+1][conf.target_column]);
	}
	
	cout << "Training..." << endl;	
	auto hist = model.train();
	
	cout << endl << "Evaluation..." << endl;
	auto res = model.evaluate();

	cout << "MSE: " << res.second << endl;

	cout << "Serializing to file" << endl;

	serialize(fout, output);
	serialize(fout, res.first, output.size());
	serialize(fout, hist.v_mse);
}
