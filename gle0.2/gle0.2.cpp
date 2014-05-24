// gle0.2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

using namespace std;

const double epf = 0.05;

int class_error(vector<double> rule, example examples)
{
	int number = 0;
	for (int i = 0; i < examples.size(); i++)
	{
		if (error(rule, examples[i]))
			number++;
	}
	return number;
}

int correct(vector<double> rule, example examples)
{
	int number = 0;
	for (int i = 0; i < examples.size(); i++)
	{
		if (check(rule, examples[i]))
			number++;
	}
	return number;
}


double coverage(vector<double> rule, example examples)
{
	double cov = 1;
	for (int i = 0; i < rule.size(); i += 2)
	{
		cov *= (rule[i + 1] - rule[i]) / (examples.get_up(i/2) - examples.get_low(i/2));
	}
	return cov;
}

double fitness(vector<double> rule, example examples)
{
	int _examples = examples.size();
	int error_class = class_error(rule, examples);
	int correctness = correct(rule,examples);
	double cov = coverage(rule, examples);
	return (2 * (_examples - error_class) + correctness + cov);//(2 * (examples.size() - class_error(rule, examples)) + correct(rule, examples) + coverage(rule, examples));
}

vector<double> fitness(vector<vector<double>> population, example examples)
{
	vector<double> fit;
	for (int i = 0; i < population.size(); i++)
	{
		fit.push_back(fitness(population[i], examples));
	}
	return fit;
}

void init(vector<vector<double>> &population, example examples, int n)
{
	int choice = rand() % examples.size();
	population.resize(n);
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < examples[choice].size(); j++)
		{
			double k1 = (double)rand()/RAND_MAX * ((double)examples.size()/examples.get_classes());
			double k2 = (double)rand()/RAND_MAX * ((double)examples.size()/examples.get_classes());
			double value = examples[choice][j];
			double up = examples.get_up(j);
			double low = examples.get_low(j);
			int classes = examples.get_classes();
			population[i].push_back(examples[choice][j] - (examples.get_up(j) - examples.get_low(j))/examples.size() * k1);
			population[i].push_back(examples[choice][j] + (examples.get_up(j) - examples.get_low(j))/examples.size() * k2);
		}
	}

}

vector<double> best(vector<vector<double>> population, example examples)
{
	vector<double> max = population[0];
	for (int i = 1; i < population.size(); i++)
	{
		if (fitness(population[i], examples) > fitness(max, examples))
			max = population[i];
	}
	return max;
}

int select(vector<double> population)
{
	double sum = 0;
	for (int i = 0; i < population.size(); i++)
	{
		sum += population[i];		
	}
	vector<double> probabilities;
	for (int i = 0; i < population.size(); i++)
	{
		probabilities.push_back(population[i]/sum);
	}
	vector<double> distribution;
	distribution.push_back(0);
	for (int i = 0; i < probabilities.size(); i++)
	{
		distribution.push_back(distribution[i] + probabilities[i]);
	}
	double choice = (double)rand()/RAND_MAX;
	for (int i = 0; i < distribution.size(); i++)
	{
		if ((distribution[i] <= choice) && (choice < distribution[i+1]))
			return i;
	}
	return (population.size() - 1);
}

vector<double> select(vector<vector<double>> population, example examples)
{
	return population[select(fitness(population, examples))];
}

double min(double a, double b)
{
	if (a > b)
		return b;
	return a;
}

double max(double a, double b)
{
	if (a > b)
		return a;
	return b;
}

vector<double> crossover(vector<double> parent1, vector<double> parent2)
{
	vector<double> child;
	for (int i = 0; i < parent1.size(); i += 2)
	{
		double left1 = min(parent1[i], parent2[i]);
		double left2 = max(parent1[i], parent2[i]);
		double right1 = min(parent1[i + 1], parent2[i + 1]);
		double right2 = max(parent1[i + 1], parent2[i + 1]);
		double l = left1 + (double)rand()/RAND_MAX * (left2 - left1);
		double r = right1 + (double)rand()/RAND_MAX * (right2 - right1);
		child.push_back(l);
		child.push_back(r);
	}
	return child;
}

vector<double> recombine(vector<vector<double>> population, example examples)
{
	return crossover(population[select(fitness(population, examples))], population[select(fitness(population, examples))]);
}

vector<double> evo_alg(example examples)
{
	int i = 0;
	int n = 10;
	vector<vector<double>> old_population;
	init(old_population, examples, n);
	vector<vector<double>> new_population;
	while (i < 20)
	{
		i++;
		new_population.push_back(best(old_population, examples));
		for (int j = 1; j < old_population.size()/2; j++)
		{
			new_population.push_back(select(old_population, examples));
		}
		for (int j = old_population.size()/2; j < old_population.size(); j++)
		{
			new_population.push_back(recombine(old_population, examples));
		}
		old_population.clear();
		old_population = new_population;
		new_population.clear();
	}
	return best(old_population, examples);
}

void output(vector<double> rule, string filename)
{
	ofstream fout;
	ostringstream fstream;	
	for (int i = 0; i < rule.size(); i++)
	{
		fstream<<rule[i]<<" ";
		fstream<<endl;
		fout.open(filename, ios_base::app);
		fout<<fstream.str();
		fout.close();
		fstream.str("");
	}
}

vector<vector<double>> hider(example examples)
{
	vector<vector<double>> rules;
	int n = examples.size();
	int i = 0;
	vector<double> rule;
	while (examples.size() > n * epf)
	{
		rule = evo_alg(examples);
		output(rule, "output.txt");
		rules.push_back(rule);
		examples.extract(rule);
		rule.clear();
		i++;
	}
	return rules;
}

void output(vector<vector<double>> rules, string filename)
{
	ofstream fout;
	ostringstream fstream;	
	for (int i = 0; i < rules.size(); i++)
	{
		for (int j = 0; j < rules[i].size(); j++)
		{
			fstream<<rules[i][j]<<" ";
		}
		fstream<<endl;
		fout.open(filename, ios_base::app);
		fout<<fstream.str();
		fout.close();
		fstream.str("");
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	example examples("poland.txt", "bounds.txt");
	vector<vector<double>> rules = hider(examples);
//	output(rules, "output.txt");
	int n;
	cin>>n;
	return 0;
}

