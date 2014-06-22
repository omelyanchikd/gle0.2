#include "example.h"

bool operator==(const vector<double> lhs, const vector<double> rhs)
{
	if (!(lhs.size() == rhs.size()))
		return false;
	for (int i = 0; i < lhs.size(); i++)
	{
		if (!(lhs[i] == rhs[i]))
			return false;
	}
	return true;
}

bool check(vector<double> rule, vector<double> example)
{
	for (int i = 0; i < rule.size(); i += 2)
	{
		if (!((rule[i] <= example[i/2]) && (example[i/2] < rule[i + 1])))
			return false;
	}
	return true;
}

bool error(vector<double> rule, vector<double> example)
{
	for (int i = 0; i < rule.size() - 2; i+= 2)
	{
		if (!((rule[i] <= example[i/2]) && (example[i/2] < rule[i + 1])))
			return false;
	}
	if (!((rule[rule.size() - 2] <= example[example.size()-1]) && (example[example.size()-1] < rule[rule.size()-1])))
			return true;
	return false;
}

void check_error(bool &correct, bool &error, vector<double> rule, vector<double> example)
{
	correct = false;
	error = false;
	int i = 0;
	for (i = 0; i < rule.size() - 2; i+= 2)
	{
		if (!((rule[i] <= example[i/2]) && (example[i/2] < rule[i + 1])))
			break;
	}
	if (i == rule.size() - 2)
	{
		if (!((rule[rule.size() - 2] <= example[example.size()-1]) && (example[example.size()-1] < rule[rule.size()-1])))
			error = true;
		else
			correct = true;
	}
}

example::example(void)
{
}

void example::extract(vector<double> rule)
{
    int i = 0;
	while (i < _examples.size())
	{
		bool correct = false;
		bool error = false;
		check_error(correct, error, rule, _examples[i]);
		if (correct || error)
		{
			_examples.erase(_examples.begin() + i);
		}
		else
			i++;
	}
}

int example::size()
{
	return _examples.size();
}

int example::get_classes()
{
	return _classes;
}

vector<vector<double>> example::get_examples()
{
	return _examples;
}

vector<double> example::get_up()
{
	return _up;
}

vector<double> example::get_low()
{
	return _low;
}

double example::get_up(int index)
{
	return _up[index];
}

double example::get_low(int index)
{
	return _low[index];
}

double example::get_heom()
{
	return _heom;
}

vector<double> example::operator[](int index)
{
	return _examples[index];
}

const vector<double> example::operator[](int index) const
{
	return _examples[index];
}

double heom(vector<double> ex1, vector<double> ex2, vector<double> up, vector<double> low)
{
	double dist = 0;
	for (int i = 0; i < ex1.size(); i++)
	{
		double diff= ((ex1[i] - ex2[i]) / (up[i] - low[i]));
		dist += diff*diff;
	}
	return dist;
}

double heom(vector<vector<double>> examples, vector<double> up, vector<double> low)
{
	double min = heom(examples[0], examples[1], up,low);
	for (int i = 0; i < examples.size(); i++)
		for (int j = 0; j < examples.size()/2; j++)
		{
			if (i == j)
				continue;
			if (heom(examples[i], examples[j],up,low) < min)
				min = heom(examples[i], examples[j], up, low);
		}
	return min;
}

example::example(string filename1, string filename2)
{
	ifstream fin;
	fin.open(filename1);
	while (!fin.eof())
	{
		string line;
		getline(fin, line);
		stringstream input(line);
		vector<double> values;
		while (!input.eof())
		{
			double val;
			input>>val;
			values.push_back(val);
		}
		_examples.push_back(values);
		values.clear();
		input.str("");
	}
	fin.close();
	fin.open(filename2);
	string line;
	getline(fin, line);
	stringstream input(line);
	while (!input.eof())
	{
		double val;
		input>>val;
		_up.push_back(val);
	}
	getline(fin, line);
	input.clear();
	input.str(line);
	while (!input.eof())
	{
		double val;
		input>>val;
		_low.push_back(val);
	}

	fin.close();
	_classes = 5;
//	_heom = heom(_examples, _up, _low);
}