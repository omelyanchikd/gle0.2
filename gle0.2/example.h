#pragma once

#include <vector>
#include <string>
#include <sstream>
#include <fstream>

#include "math.h"


using namespace std;

class example
{
public:
	example(void);

	example(string filename1, string filename2);

	int size();
	int get_classes();

	double get_up(int index);
	double get_low(int index);
	
	vector<vector<double>> get_examples();
	vector<double> get_low();
	vector<double> get_up();

	double get_heom();

	void extract(vector<double> rule);

    vector<double> operator[](int index);
    const vector<double> operator[](int index) const;


private:
	
	vector<vector<double>> _examples;

	vector<double> _up;
	vector<double> _low;

	int _classes;
	double _heom;

};

bool check(vector<double> rule, vector<double> example);
bool error(vector<double> rule, vector<double> example);
void check_error(bool &correct, bool &error, vector<double> rule, vector<double> example);

double heom(vector<vector<double>> examples, vector<double> up, vector<double> low);