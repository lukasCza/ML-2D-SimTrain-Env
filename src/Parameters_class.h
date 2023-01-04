#pragma once

namespace trainParameters
{
	class QL_Params
	{
	public:

		QL_Params() 
		{
			num_neurons_in_layer[0] = 128;	//input Layer
			num_neurons_in_layer[1] = 128;
			num_neurons_in_layer[2] = 96;
		}

		//basic parameters
		int single_eval_steps = 1500;
		int num_episodes = 1000;
		double target_reward = 1000;
		int num_hidden_layers = 1;
		int num_neurons_in_layer[11]; //maximum number of hideen layers 10
		float model_friction = 0.8f;
		bool two_direction_train = false;

		//greed policy 
		double initial_epsilon = 1.0;
		size_t anneal_interval = 1000;				//1000
		double min_epsilon = 0.01;
		double decay_rate = 0.99;					//0.9

		//config
		double optimizer_step_size = 0.01;			//0.01
		double discount_rate = 0.99;					//0.95
		size_t target_network_sync_interval = 1500;	//50?
		size_t exploration_steps = 3000;

		//replayMethod
		size_t batch_size = 64;						//10
		size_t capacity = 10000;
		size_t nsteps_into_future = 1;

		void check_values()
		{
			single_eval_steps = (single_eval_steps < 50) ? 50 : single_eval_steps;
			num_episodes = (num_episodes < 10) ? 10 : num_episodes;
			target_reward = (target_reward < (0.1 * single_eval_steps + 10.0)) ? (0.1 * single_eval_steps + 10.0) : target_reward;
			num_hidden_layers = (num_hidden_layers < 1) ? 1 : num_hidden_layers;
			num_hidden_layers = (num_hidden_layers > 10) ? 10 : num_hidden_layers;

			for (size_t i = 0; i < num_hidden_layers; i++)
			{
				num_neurons_in_layer[i] = (num_neurons_in_layer[i] < 1) ? 1 : num_neurons_in_layer[i];
			}
			initial_epsilon = (initial_epsilon > 1.0) ? 1.0 : initial_epsilon;
			initial_epsilon = (initial_epsilon < 0.01) ? 0.01 : initial_epsilon;
			anneal_interval = (anneal_interval < 100) ? 100 : anneal_interval;
			anneal_interval = (anneal_interval > 10000) ? 10000 : anneal_interval;
			min_epsilon = (min_epsilon < 0.001) ? 0.001 : min_epsilon;
			min_epsilon = (min_epsilon > 1.0) ? 1.0 : min_epsilon;
			decay_rate = (decay_rate < 0.01) ? 0.01 : decay_rate;
			decay_rate = (decay_rate > 1.0) ? 1.0 : decay_rate;
			optimizer_step_size = (optimizer_step_size < 0.001) ? 0.001 : optimizer_step_size;
			optimizer_step_size = (optimizer_step_size > 1.0) ? 1.0 : optimizer_step_size;
			discount_rate = (discount_rate < 0.01) ? 0.01 : discount_rate;
			discount_rate = (discount_rate > 1.0) ? 1.0 : discount_rate;
			target_network_sync_interval = (target_network_sync_interval < 10) ? 10 : target_network_sync_interval;
			exploration_steps = (exploration_steps < 10) ? 10 : exploration_steps;
			batch_size = (batch_size < 1) ? 1 : batch_size;
			batch_size = (batch_size > 500) ? 500 : batch_size;
			capacity = (capacity < 1000) ? 1000 : capacity;
			capacity = (capacity > 100000) ? 100000 : capacity;
			nsteps_into_future = (nsteps_into_future < 1) ? 1 : nsteps_into_future;
		}

	};

	class NEAT_Params
	{
	public:
		NEAT_Params(){}

		//basic params
		int single_eval_steps = 1200;
		int num_generations = 4000;		//1000
		int num_runs = 1;
		int population_size = 80;		//20
		double winner_fitness_value = 5000;
		float model_friction = 0.6f;
		bool two_direction_train = false;

		//visneat
		size_t num_elites = 6;
		float reproduce_ratio = 0.9f;
		float new_connection_mutation_rate = 0.2f;
		float new_node_mutation_rate = 0.1f;
		float max_perturbation = 0.75f;
		int draw_each = 100;

		//other parameters are in the NEAT_param.txt

		void check_values()
		{
			single_eval_steps = (single_eval_steps < 10) ? 10 : single_eval_steps;
			num_generations = (num_generations < 10) ? 10 : num_generations;
			num_generations = (num_generations > 1000000) ? 1000000 : num_generations;
			num_runs = (num_runs < 1) ? 1 : num_runs;
			population_size = (population_size < 2) ? 2 : population_size;
			population_size = (population_size > 5000) ? 5000 : population_size;
			winner_fitness_value = (winner_fitness_value < 10) ? 10 : winner_fitness_value;
			num_elites = (num_elites < 1) ? 1 : num_elites;
			reproduce_ratio = (reproduce_ratio < 0.1f) ? 0.1f : reproduce_ratio;
			reproduce_ratio = (reproduce_ratio > 1.0f) ? 1.0f : reproduce_ratio;
			new_connection_mutation_rate = (new_connection_mutation_rate < 0.1f) ? 0.1f : new_connection_mutation_rate;
			new_connection_mutation_rate = (new_connection_mutation_rate > 1.0f) ? 1.0f : new_connection_mutation_rate;
			new_node_mutation_rate = (new_node_mutation_rate < 0.01f) ? 0.01f : new_node_mutation_rate;
			new_node_mutation_rate = (new_node_mutation_rate > 1.0f) ? 1.0f : new_node_mutation_rate;
			max_perturbation = (max_perturbation < 0.1f) ? 0.1f : max_perturbation;
			max_perturbation = (max_perturbation > 1.0f) ? 1.0f : max_perturbation;
			draw_each = (draw_each < 10) ? 10 : draw_each;
		}
	};

	class GA_Params
	{
	public:
		GA_Params(){}

		int single_eval_steps = 1200;
		int num_generations = 2000;
		int population_size = 80;
		double crossover_fraction = 1.0;
		double mutation_rate = 0.4;
		int elite_count = 8;
		float model_friction = 0.6f;

		double tol_stall_best = 1;
		int best_stall_max = 100;
		double tol_stall_average = 1;
		int average_stall_max = 20;

		double pi_multiplier_for_fi1_range = 1.0;
		double pi_multiplier_for_fi2_range = 1.0;
		double max_omega1_freq = 2.0;
		double max_omega2_freq = 2.0;
		double max_A_param_value = 2.0;
		double A_param_offset = -1.0;

		void check_values()
		{
			single_eval_steps = (single_eval_steps < 10) ? 10 : single_eval_steps;
			num_generations = (num_generations < 10) ? 10 : num_generations;
			population_size = (population_size < 2) ? 2 : population_size;
			num_generations = (num_generations > 1000000) ? 1000000 : num_generations;
			population_size = (population_size > 5000) ? 5000 : population_size;
			tol_stall_best = (tol_stall_best < 0.1) ? 0.1 : tol_stall_best;
			best_stall_max = (best_stall_max < 2) ? 2 : best_stall_max;
			tol_stall_average = (tol_stall_average < 0.1) ? 0.1 : tol_stall_average;
			average_stall_max = (average_stall_max < 2) ? 2 : average_stall_max;
			crossover_fraction = (crossover_fraction < 0.01) ? 0.01 : crossover_fraction;
			crossover_fraction = (crossover_fraction > 1.0) ? 1.0 : crossover_fraction;
			mutation_rate = (mutation_rate > 1.0) ? 1.0 : mutation_rate;
			mutation_rate = (mutation_rate < 0.01) ? 0.01 : mutation_rate;
			pi_multiplier_for_fi1_range = (pi_multiplier_for_fi1_range < 0.1) ? 0.1 : pi_multiplier_for_fi1_range;
			pi_multiplier_for_fi2_range = (pi_multiplier_for_fi2_range < 0.1) ? 0.1 : pi_multiplier_for_fi2_range;
			max_omega1_freq = (max_omega1_freq < 0.01) ? 0.01 : max_omega1_freq;
			max_omega1_freq = (max_omega1_freq > 20) ? 20 : max_omega1_freq;
			max_omega2_freq = (max_omega2_freq < 0.01) ? 0.01 : max_omega2_freq;
			max_omega2_freq = (max_omega2_freq > 20) ? 20 : max_omega2_freq;
			max_A_param_value = (max_A_param_value < 0.1) ? 0.1 : max_A_param_value;
			A_param_offset = (A_param_offset > 0.0) ? 0.0 : A_param_offset;
			A_param_offset = (A_param_offset < -10.0) ? -10.0 : A_param_offset;
			elite_count = (elite_count < 0) ? 1 : elite_count;
			elite_count = (elite_count > population_size) ? population_size : elite_count;
		}
	};
}