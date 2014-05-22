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

example::example(void)
{
}

void example::extract(vector<double> rule)
{
	for (int i = 0; i < _examples.size(); i++)
	{
		if (check(rule, _examples[i]))
		{
			_examples.erase(_examples.begin() + i);
		}
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

double example::get_up(int index)
{
	return _up[index];
}

double example::get_low(int index)
{
	return _low[index];
}

vector<double> example::operator[](int index)
{
	return _examples[index];
}

const vector<double> example::operator[](int index) const
{
	return _examples[index];
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
	_classes = 10;
}