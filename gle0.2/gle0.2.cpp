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
	for (int i = 0; i < examples[0].size(); i++)
	{
		cov *= (rule[2 * i + 1] - rule[2 * i]) / (examples.get_up(i) - examples.get_low(i));
	}
	return cov;
}

double fitness(vector<double> rule, example examples)
{
	return (2 * (examples.size() - class_error(rule, examples) + correct(rule, examples) + coverage(rule, examples)));
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

void init(vector<vector<double>> population, example examples, int n)
{
	int choice = rand() % examples.size();
	population.resize(n);
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < examples[choice].size(); j++)
		{
			double k1 = (double)rand()/RAND_MAX * ((double)examples.size()/examples.get_classes());
			double k2 = (double)rand()/RAND_MAX * ((double)examples.size()/examples.get_classes());
			population[i].push_back(examples[choice][j] - (examples.get_up(j) - examples.get_low(j))/examples.size() * k1);
			population[i].push_back(examples[choice][j] + (examples.get_up(j) - examples.get_low(j))/examples.size() * k1);
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
		distribution[i+1] = distribution[i] + probabilities[i];
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
		double left1 = min(parent1[2*i], parent2[2*i]);
		double left2 = max(parent1[2*i], parent2[2*i]);
		double right1 = min(parent1[2*i + 1], parent2[2*i + 1]);
		double right2 = max(parent1[2*i + 1], parent2[2*i + 1]);
		double l = (double)rand()/RAND_MAX * left1 + (left2 - left1);
		double r = (double)rand()/RAND_MAX * right1 + (right2 - right1);
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
	int n = 50;
	vector<vector<double>> old_population;
	init(old_population, examples, n);
	vector<vector<double>> new_population;
	while (i < 100)
	{
		i++;
		new_population.push_back(best(old_population, examples));
		for (int j = 1; j < old_population.size()/2; j++)
		{
			new_population.push_back(select(old_population, examples));
		}
		for (int j = old_population.size()/2 + 1; j < old_population.size(); j++)
		{
			new_population.push_back(recombine(old_population, examples));
		}
	}
	return best(new_population, examples);
}

vector<vector<double>> hider(example examples)
{
	vector<vector<double>> rules;
	int n = examples.size();
	while (examples.size() > n * epf)
	{
		vector<double> rule = evo_alg(examples);
		rules.push_back(rule);
		examples.extract(rule);
	}
	return rules;
}

void output(vector<vector<double>> rules)
{
	for (int i = 0; i < rules.size(); i++)
	{
		for (int j = 0; j < rules[i].size(); j++)
		{
			cout<<rules[i][j]<<" ";
		}
		cout<<endl;
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	example examples("examples.txt", "bounds.txt");
	vector<vector<double>> rules = hider(examples);
	output(rules);
	return 0;
}

