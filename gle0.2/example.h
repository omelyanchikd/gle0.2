#pragma once

#include <vector>
#include <string>
#include <sstream>
#include <fstream>


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

	void extract(vector<double> rule);

    vector<double> operator[](int index);
    const vector<double> operator[](int index) const;


private:
	
	vector<vector<double>> _examples;

	vector<double> _up;
	vector<double> _low;

	int _classes;

};

bool check(vector<double> rule, vector<double> example);
bool error(vector<double> rule, vector<double> example);