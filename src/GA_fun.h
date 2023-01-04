#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include "../Libraries/include/openGA.hpp"
#include "Model1_Agent.h"
#include "Model2_Agent.h"
#include "Model3_Agent.h"
#include "Model4_Agent.h"

using namespace EA;
using namespace mlpack::rl;

namespace GeneticAlgorithm
{
	struct MyMiddleCost
	{
		double Fitness_value;
	};
	struct MySolution_Model1
	{
		double A1_joint[8];
		double A2_joint[8];
		double fi1_joint[8];
		double fi2_joint[8];
		double omega1;

		std::string to_string() const
		{
			std::string str;
			str += "{";
			for (int i = 0; i < 8; i++)
			{
				str += " J" + std::to_string(i + 1) + "_A1: " + std::to_string(A1_joint[i]) + " J" + std::to_string(i + 1) + "_A2: " + std::to_string(A2_joint[i]) +
					" J" + std::to_string(i + 1) + "_fi1: " + std::to_string(fi1_joint[i]) + " J" + std::to_string(i + 1) + "_fi2: " + std::to_string(fi2_joint[i]);

			}
			str += " omega1: " + std::to_string(omega1) + " }";
			return str;
		}
	};
	struct MySolution_Model2or4
	{
		double A1_joint[4];
		double A2_joint[4];
		double fi1_joint[4];
		double fi2_joint[4];
		double omega1;

		std::string to_string() const
		{
			std::string str;
			str += "{";
			for (int i = 0; i < 4; i++)
			{
				str += " J" + std::to_string(i + 1) + "_A1: " + std::to_string(A1_joint[i]) + " J" + std::to_string(i + 1) + "_A2: " + std::to_string(A2_joint[i]) +
					" J" + std::to_string(i + 1) + "_fi1: " + std::to_string(fi1_joint[i]) + " J" + std::to_string(i + 1) + "_fi2: " + std::to_string(fi2_joint[i]);

			}
			str += " omega1: " + std::to_string(omega1) + " }";
			return str;
		}
	};
	struct MySolution_Model3
	{
		double A1_joint[11];
		double A2_joint[10];
		double fi1_joint[11];
		double fi2_joint[10];
		double omega1;

		std::string to_string() const
		{
			std::string str;
			str += "{";
			for (int i = 0; i < 10; i++)
			{
				str += " J" + std::to_string(i + 1) + "_A1: " + std::to_string(A1_joint[i]) + " J" + std::to_string(i + 1) + "_A2: " + std::to_string(A2_joint[i]) +
					" J" + std::to_string(i + 1) + "_fi1: " + std::to_string(fi1_joint[i]) + " J" + std::to_string(i + 1) + "_fi2: " + std::to_string(fi2_joint[i]);

			}
			str += " J11_A1: " + std::to_string(A1_joint[10]) + " J11_fi1: " + std::to_string(fi1_joint[10]);
			str += " omega1: " + std::to_string(omega1) + " }";
			return str;
		}
	};
	struct MySolution_Model5
	{
		double A1_joint[12];
		double A2_joint[12];
		double fi1_joint[12];
		double fi2_joint[12];
		double omega1;

		std::string to_string() const
		{
			std::string str;
			str += "{";
			for (int i = 0; i < 12; i++)
			{
				str += " J" + std::to_string(i + 1) + "_A1: " + std::to_string(A1_joint[i]) + " J" + std::to_string(i + 1) + "_A2: " + std::to_string(A2_joint[i]) +
					" J" + std::to_string(i + 1) + "_fi1: " + std::to_string(fi1_joint[i]) + " J" + std::to_string(i + 1) + "_fi2: " + std::to_string(fi2_joint[i]);

			}
			str += " omega1: " + std::to_string(omega1) + " }";
			return str;
		}
	};
	struct MySolution_Model6
	{
		double A1_joint[5];
		double A2_joint[5];
		double fi1_joint[5];
		double fi2_joint[5];
		double omega1;

		std::string to_string() const
		{
			std::string str;
			str += "{";
			for (int i = 0; i < 5; i++)
			{
				str += " J" + std::to_string(i + 1) + "_A1: " + std::to_string(A1_joint[i]) + " J" + std::to_string(i + 1) + "_A2: " + std::to_string(A2_joint[i]) +
					" J" + std::to_string(i + 1) + "_fi1: " + std::to_string(fi1_joint[i]) + " J" + std::to_string(i + 1) + "_fi2: " + std::to_string(fi2_joint[i]);

			}
			str += " omega1: " + std::to_string(omega1) + " }";
			return str;
		}
	};

	//GA class on templates
	template<
		typename Model_Type,
		typename MySolution_Type>
	class GA_operations
	{
	public:
		using ThisChromosomeType = typename Genetic<MySolution_Type, MyMiddleCost>::thisChromosomeType;

		GA_operations(Model_Type& agent_m, bool& stop_flag, const char* filename, 
						const char* fitness_filename, const char* _best_solution_filename,
						trainParameters::GA_Params* params_) :
						agent(agent_m), user_request_stop(stop_flag), 
						best_solution_filename(_best_solution_filename), 
						params(params_), num_evaluations(0), best_fitness(999.0)
		{
			using Size = typename Model_Type::ContiniousAction;
			if (Size::size == 11)
				vectors_size = Size::size - 1;
			else
				vectors_size = Size::size;

			stop_criteria_min_gen = 2 * params->population_size;
			agent.MaxSteps() = params_->single_eval_steps;
			output_file.open(filename, std::ios::trunc);
			FitnessFile.open(fitness_filename, std::ios::trunc);
			fit_file_open = FitnessFile.is_open();
			agent.training_window->generation = 1;
			agent.training_window->organism_num = 1;

			std::cout << "\n Function for single motor: joint_motor_speed =: \n"
				<< "A1 * sin(omega1 * (dt - fi1)) + A2 * cos(omega1 * (dt - fi2)) \n" << std::endl;

			output_file << "\n Function for single motor: joint_motor_speed =: \n"
				<< "A1 * sin(omega1 * dt - fi1) + A2 * cos(omega1 * dt - fi2) \n"
				<< "step" << "\t" << "cost_avg" << "\t" << "cost_best" << "\t" << "best_genes:" << "\n";
			/*hello darkness myold frined*/
		}
		~GA_operations()
		{
			if (fit_file_open)
				FitnessFile.close();
			output_file.close();
			best_out_file.open(best_solution_filename, std::ios::trunc);
			best_out_file << best_solution.to_string();
			best_out_file.close();
		}

		void init_genes(MySolution_Type& p, const std::function<double(void)>& rnd01)
		{
			for (size_t i = 0; i < vectors_size; i++)
			{
				p.A1_joint[i] = params->max_A_param_value * rnd01() - params->A_param_offset;
				p.A2_joint[i] = params->max_A_param_value * rnd01() - params->A_param_offset;
			}
			if(vectors_size == 10)
				p.A1_joint[10] =  params->max_A_param_value * rnd01() - params->A_param_offset;
			
			for (size_t i = 0; i < vectors_size; i++)
			{
				p.fi1_joint[i] = (2.0 * std::_Pi * rnd01() - std::_Pi) * params->pi_multiplier_for_fi1_range;
				p.fi2_joint[i] = (2.0 * std::_Pi * rnd01() - std::_Pi) * params->pi_multiplier_for_fi1_range;
			}
			if (vectors_size == 10)
				p.fi1_joint[10] = (2.0 * std::_Pi * rnd01() - std::_Pi) * params->pi_multiplier_for_fi1_range;
			p.omega1 = 2 * std::_Pi * params->max_omega1_freq * rnd01();
		}

		bool eval_solution(const MySolution_Type& p, MyMiddleCost& c)
		{
			using State = typename Model_Type::State;
			using Action = typename Model_Type::ContiniousAction;

 			double dt = 0.00;
			double sample_return = 0.1;
			double fitness = 0.0;
			State agent_state;	//all the usless readable data
			State next_state;
			Action action;
			agent_state = agent.InitialSample();

			if (true)	//checking genes can be add here
			{
				for (size_t steps = 0; steps < agent.MaxSteps(); steps++)
				{
					for (size_t i = 0; i < vectors_size; i++)
						action.action[i] = p.A1_joint[i] * sin(p.omega1 * dt - p.fi1_joint[i]) + 
											p.A2_joint[i] * cos(p.omega1 * dt - p.fi2_joint[i]);
					if (vectors_size == 10)
						action.action[10] = agent_state.Angle(6) * p.A1_joint[10] * sin(p.fi1_joint[10]);
					
					fitness = agent.SampleContinious(agent_state, action, next_state);
					agent.training_window->fitness = -fitness;
					agent_state = next_state;
					dt += 0.01;

					if (agent.IsTerminal(next_state))
						break;			//check if terminal state
				}
				if (agent.training_window->exit_training_flag == true && num_evaluations > stop_criteria_min_gen)
					user_request_stop = true;

				num_evaluations++;
				std::cout <<"organism: " << agent.training_window->organism_num + 1 << " fitness: " << - fitness << std::endl;
				c.Fitness_value = -fitness;
				agent.training_window->organism_num += 1;

				return true; // genes are accepted
			}
			else
				return false; // genes are rejected
		}

		MySolution_Type mutate(const MySolution_Type& g_base,
			const std::function<double(void)>& rnd01,
			double shrink_scale)
		{
			MySolution_Type g_new;
			bool in_range_a = true;
			bool in_range_fi = true;
			bool in_range_omega;
			const double mu = 0.2 * shrink_scale; // mutation radius
			long double pi = std::_Pi;
			double omega1_max = 2 * pi * params->max_omega1_freq;
			double fi1_range = std::_Pi * params->pi_multiplier_for_fi1_range;
			double fi2_range = std::_Pi * params->pi_multiplier_for_fi2_range;
			double A_low = params->A_param_offset;
			double A_max = params->max_A_param_value - params->A_param_offset;
			do {
				g_new = g_base;
				for (size_t i = 0; i < vectors_size; i++)
				{
					g_new.A1_joint[i] += mu * (rnd01() - rnd01());
					g_new.A2_joint[i] += mu * (rnd01() - rnd01());
				}
				if(vectors_size == 10)
					g_new.A1_joint[10] += mu * (rnd01() - rnd01());
				for (size_t i = 0; i < vectors_size; i++)
				{
					g_new.fi1_joint[i] += mu * (rnd01() - rnd01());
					g_new.fi2_joint[i] += mu * (rnd01() - rnd01());
				}
				if(vectors_size == 10)
					g_new.fi1_joint[10] += mu * (rnd01() - rnd01());
				
				g_new.omega1 += mu * (rnd01() - rnd01());

				for (size_t i = 0; i < vectors_size; i++)
				{
					in_range_a = (g_new.A1_joint[i] >= A_low && g_new.A1_joint[i] < A_max && g_new.A2_joint[i] >= A_low && g_new.A2_joint[i] < A_max);
					in_range_fi = (g_new.fi1_joint[i] >= -fi1_range && g_new.fi1_joint[i] < fi1_range && g_new.fi2_joint[i] >= -fi2_range && g_new.fi2_joint[i] < fi2_range);
					if (!in_range_a || !in_range_fi)
						break;
				}
				if (vectors_size == 10)
				{
					in_range_a = (g_new.A1_joint[10] >= A_low && g_new.A1_joint[10] < A_max);
					in_range_fi = (g_new.fi1_joint[10] >= -fi1_range && g_new.fi1_joint[10] < fi1_range);
				}
				in_range_omega = (g_new.omega1 >= 0.0 && g_new.omega1 < omega1_max);

			} while (!in_range_a || !in_range_fi || !in_range_omega);

			return g_new;
		}

		MySolution_Type crossover(const MySolution_Type& G1,
			const MySolution_Type& G2,
			const std::function<double(void)>& rnd01)
		{
			MySolution_Type g_new;
			double r;

			for (size_t i = 0; i < vectors_size; i++)
			{
				r = rnd01();
				g_new.A1_joint[i] = r * G1.A1_joint[i] + (1.0 - r) * G2.A1_joint[i];
				r = rnd01();
				g_new.A2_joint[i] = r * G1.A2_joint[i] + (1.0 - r) * G2.A2_joint[i];
				r = rnd01();
				g_new.fi1_joint[i] = r * G1.fi1_joint[i] + (1.0 - r) * G2.fi1_joint[i];
				r = rnd01();
				g_new.fi2_joint[i] = r * G1.fi2_joint[i] + (1.0 - r) * G2.fi2_joint[i];
			}
			if (vectors_size == 10)
			{
				r = rnd01();
				g_new.A1_joint[10] = r * G1.A1_joint[10] + (1.0 - r) * G2.A1_joint[10];
				r = rnd01();
				g_new.fi1_joint[10] = r * G1.fi1_joint[10] + (1.0 - r) * G2.fi1_joint[10];
			}
			r = rnd01();
			g_new.omega1 = r * G1.omega1 + (1.0 - r) * G2.omega1;

			return g_new;
		}

		double calculate_SO_total_fitness(const ThisChromosomeType& Ch)
		{
			return Ch.middle_costs.Fitness_value;
		}

		void SO_report_generation(
			int generation_number,
			const EA::GenerationType<MySolution_Type, MyMiddleCost>& last_generation,
			const MySolution_Type& best_genes)
		{
			std::cout
				<< "Generation [" << generation_number << "], "
				<< "Best=" << last_generation.best_total_cost << ", "
				<< "Average=" << last_generation.average_cost << ", "
				<< "Exe_time=" << last_generation.exe_time
				//<< "Best genes= \n (" << best_genes.to_string() << ")" 
				<< std::endl;

			output_file
				<< generation_number << "\t"
				<< last_generation.average_cost << "\t"
				<< last_generation.best_total_cost << "\t"
				<< best_genes.to_string() << "\n";

			if (last_generation.chromosomes[last_generation.best_chromosome_index].total_cost < best_fitness)
				best_fitness = last_generation.chromosomes[last_generation.best_chromosome_index].total_cost;
			if (fit_file_open) {
				FitnessFile << generation_number << "\t"
					<< -last_generation.chromosomes[last_generation.best_chromosome_index].total_cost << "\n";
			}
			best_solution = best_genes;
			agent.training_window->generation += 1;
			agent.training_window->organism_num = 0;
			agent.training_window->best_fitness = best_fitness;
		}

		std::ofstream output_file;
		std::ofstream best_out_file;
		std::ofstream FitnessFile;
		bool fit_file_open;
		bool& user_request_stop;

	private:
		Model_Type& agent;
		trainParameters::GA_Params* params;
		const char* best_solution_filename;
		double best_fitness;
		size_t vectors_size;
		MySolution_Type best_solution;
		unsigned long num_evaluations;
		int stop_criteria_min_gen;	// need this because GA library is DUMB and tries to acces vector[n -2] 
									//where n is num of generations so when you exit before gen 2 it tries to do vector[-2]???
	};
}	//namespace end