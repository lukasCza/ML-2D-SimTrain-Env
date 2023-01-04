#pragma once

#define DISCRETE_SPPED_GAIN 0.4f

#include "PhysicsEngine.h"
#include "ProgramWindow.h"
#include "Parameters_class.h"
#include "fast_draw.h"

//mlpack
#include <mlpack/core.hpp>
#include <mlpack/methods/ann/ffn.hpp>
#include <mlpack/methods/ann/init_rules/gaussian_init.hpp>
#include <mlpack/methods/ann/layer/layer.hpp>
#include <mlpack/methods/ann/loss_functions/mean_squared_error.hpp>
#include <mlpack/methods/reinforcement_learning/q_learning.hpp>
#include <mlpack/methods/reinforcement_learning/async_learning.hpp>
#include <mlpack/methods/reinforcement_learning/q_networks/simple_dqn.hpp>
#include <mlpack/methods/reinforcement_learning/policy/greedy_policy.hpp>
#include <mlpack/methods/reinforcement_learning/policy/aggregated_policy.hpp>
#include <mlpack/methods/reinforcement_learning/training_config.hpp>
#include <ensmallen.hpp>

//NEAT_method
#include "NEAT_fun.h"

//visualized neat
#include "Vis_NEAT_func.h"

//GA
#include "../Libraries/include/openGA.hpp"
#include "GA_fun.h"

//agents:

#include "Model1_Agent.h"
#include "Model2_Agent.h"
#include "Model3_Agent.h"
#include "Model4_Agent.h"
#include "Model5_Agent.h"
#include "Model6_Agent.h"

using namespace mlpack;
using namespace mlpack::ann;
using namespace ens;
using namespace mlpack::rl;
using namespace std;

template<typename Model_Type>
void neuralnetworkQL_train(SelectedModel model, trainParameters::QL_Params params, int window_theme)
{
	using Inputs = typename Model_Type::State;
	using Output = typename Model_Type::Action;

	PhysicsWorld training_world;
	SimWindow training_window;
	clock_t SimTime;

	char filename1[50];
	char filename2[50];
	sprintf_s(filename1, "models/DQN/FFNN_output_model%d.xml", ((int)model + 1));
	sprintf_s(filename2, "models/DQN/FFNN_output_txt_model%d.txt", ((int)model + 1));
	char filename3[50];
	sprintf_s(filename3, "models/DQN/FFNN_test_network_model%d.xml", ((int)model + 1));
	char filename4[50];
	sprintf_s(filename4, "models/DQN/model%d_fitness_report.txt", ((int)model + 1));

	training_world.Init();
	training_world.selected_model = model;
	training_window.selected_model = model;
	training_world.objects_friction = params.model_friction;
	training_world.createMechanism();
	training_window.currentTheme = window_theme;
	training_window.Init("Q-Learn method", true);
	training_window.current_OB_draw_struct = ilikeitthatway(training_world, training_window, model);
	training_window.organism_num = -1;
	training_window.generation = 1;

	Model_Type modelagent(&training_world, &training_window, 0, params.single_eval_steps, params.two_direction_train, params.two_direction_train);

	//Feed forward neural network setup																	
	int i = 0;
	FFN<MeanSquaredError<>, GaussianInitialization> fnn_model(MeanSquaredError<>(), GaussianInitialization(0, 0.001));
	fnn_model.Add<Linear<>>(Inputs::dimension, params.num_neurons_in_layer[i]);
	fnn_model.Add<ReLULayer<>>();
	for (i = 0; i < params.num_hidden_layers; i++)
	{
		fnn_model.Add<Linear<>>(params.num_neurons_in_layer[i], params.num_neurons_in_layer[i+1]);
		fnn_model.Add<ReLULayer<>>();
	}
	fnn_model.Add<Linear<>>(params.num_neurons_in_layer[i], Output::size);

	// Set up the network
	SimpleDQN<> DQNmodel(fnn_model, false);

	// Set up the policy and replay method.
	GreedyPolicy<Model_Type> policy(params.initial_epsilon, params.anneal_interval, params.min_epsilon, params.decay_rate);
	RandomReplay<Model_Type> replayMethod(params.batch_size, params.capacity, params.nsteps_into_future);
	TrainingConfig config;
	config.StepSize() = params.optimizer_step_size;
	config.Discount() = params.discount_rate;
	config.TargetNetworkSyncInterval() = params.target_network_sync_interval;
	config.ExplorationSteps() = params.exploration_steps;
	config.DoubleQLearning() = false;
	config.NoisyQLearning() = false;

	QLearning<Model_Type, decltype(DQNmodel), AdamUpdate, decltype(policy)>
	agent(config, DQNmodel, policy, replayMethod, ens::AdamUpdate(), move(modelagent));

	arma::running_stat<double> averageReturn;
	size_t episodes = 0;
	bool converged = true;
	double best_fitness = -1000.0;
	ofstream outputFile(filename4, ios::trunc);
	bool file_is_open = outputFile.is_open();

	cout << "\n\tQ-Learning starts!\n" << endl;
	SimTime = clock();
	while (true)
	{
		training_window.generation = episodes + 1;
		training_window.best_fitness = best_fitness;
		double episodeReturn = agent.Episode();
		if (episodeReturn > best_fitness)
			best_fitness = episodeReturn;
		if (file_is_open)
			outputFile << episodes + 1 << "\t" << episodeReturn << "\n";
		averageReturn(episodeReturn);
		episodes += 1;

		if (episodes > params.num_episodes)
		{
			cout << "Maximum episode number reached. Training finished." << endl;
			converged = false;
			break;
		}
		cout << "Episode No.: " << episodes;
		cout << " Average return: " << averageReturn.mean()
			<< " Best return: " << best_fitness
			<< " Episode return: " << episodeReturn << endl;
		if (episodeReturn > params.target_reward)
			break;
		//if (averageReturn.mean() > params.target_reward)
		//	break;
		if (training_window.exit_training_flag == true)
		{
			converged = false;
			break;
		}
	}
	double elapsedTime = (double)((clock() - SimTime) / CLOCKS_PER_SEC);
	cout << "---- Total time elapsed: " << elapsedTime << "s ----" << endl;

	if (converged)
	cout << "Hooray! Q-Learning agent successfully trained" << endl;
	else
	cout << "****  Training end by user request.  ****\n**** Achieved results saved to file. ****" << endl;

	if (file_is_open)
		outputFile.close();
	training_window.Close();
	training_world.End();
	//save network
	fnn_model.Parameters() = agent.Network().Parameters();
	mlpack::data::Save(filename1, "FNN", fnn_model, false);
	mlpack::data::Save(filename2, "FNN", fnn_model, false);
	mlpack::data::Save(filename3, "FNN", fnn_model, false);
}

template<typename Model_Type, typename MY_Solution_Type>
void GA_train(SelectedModel model, trainParameters::GA_Params params, int window_theme)
{
	PhysicsWorld training_world;
	SimWindow training_window;

	char filename[50];
	sprintf_s(filename, "models/GA/GA_output_model%d.txt", ((int)model + 1));
	char filename2[50];
	sprintf_s(filename2, "models/GA/GA_test_param_model%d.txt", ((int)model + 1));
	char filename3[50];
	sprintf_s(filename3, "models/GA/model%d_fitness_report.txt", ((int)model + 1));

	training_world.Init();
	training_world.selected_model = model;
	training_window.selected_model = model;
	training_world.objects_friction = params.model_friction;
	training_world.createMechanism();
	training_window.currentTheme = window_theme;
	training_window.Init("Genetic Algorithm evaluation", true);
	training_window.current_OB_draw_struct = ilikeitthatway(training_world, training_window, model);

	Model_Type modelagent(&training_world, &training_window, 2, params.single_eval_steps, false, false);

	EA::Genetic<MY_Solution_Type, GeneticAlgorithm::MyMiddleCost> ga_obj;
	GeneticAlgorithm::GA_operations<Model_Type, MY_Solution_Type> GA_evaluate(modelagent, ga_obj.user_request_stop, filename, filename3, filename2, &params);

	EA::Chronometer timer;
	timer.tic();

	using std::bind;
	using std::placeholders::_1;
	using std::placeholders::_2;
	using std::placeholders::_3;

	ga_obj.problem_mode = EA::GA_MODE::SOGA;
	ga_obj.multi_threading = false;
	ga_obj.verbose = false;		//display all
	ga_obj.population = params.population_size;
	ga_obj.generation_max = params.num_generations;
	ga_obj.calculate_SO_total_fitness = bind(&GeneticAlgorithm::GA_operations<Model_Type, MY_Solution_Type>::calculate_SO_total_fitness, &GA_evaluate, _1);
	ga_obj.init_genes = bind(&GeneticAlgorithm::GA_operations<Model_Type, MY_Solution_Type>::init_genes, &GA_evaluate, _1, _2);
	ga_obj.eval_solution = bind(&GeneticAlgorithm::GA_operations<Model_Type, MY_Solution_Type>::eval_solution, &GA_evaluate, _1, _2);
	ga_obj.mutate = bind(&GeneticAlgorithm::GA_operations<Model_Type, MY_Solution_Type>::mutate, &GA_evaluate, _1, _2, _3);
	ga_obj.crossover = bind(&GeneticAlgorithm::GA_operations<Model_Type, MY_Solution_Type>::crossover, &GA_evaluate, _1, _2, _3);
	ga_obj.SO_report_generation = bind(&GeneticAlgorithm::GA_operations<Model_Type, MY_Solution_Type>::SO_report_generation, &GA_evaluate, _1, _2, _3);
	ga_obj.tol_stall_best = params.tol_stall_best;
	ga_obj.best_stall_max = params.best_stall_max;
	ga_obj.average_stall_max = params.average_stall_max;
	ga_obj.tol_stall_average = params.tol_stall_average;
	ga_obj.elite_count = params.elite_count;
	ga_obj.crossover_fraction = params.crossover_fraction;
	ga_obj.mutation_rate = params.mutation_rate;
	ga_obj.solve();

	std::cout << "The problem is optimized in " << timer.toc() << " seconds." << std::endl;

	if (training_window.exit_training_flag == true)
		cout << "****  Training end by user request.  ****\n**** Achieved results saved to file. ****" << endl;
	training_window.Close();
	training_world.End();
}

template<typename Model_Type>
void NEATmethod_train(SelectedModel model, trainParameters::NEAT_Params params, int window_theme)
{
	//reminder:
	//nodetype
	//	NEURON = 0,
	//	SENSOR = 1
	//enum nodeplace
	//	HIDDEN = 0,
	//	INPUT = 1,
	//	OUTPUT = 2,
	//	BIAS = 3
	PhysicsWorld training_world;
	SimWindow training_window;
	clock_t SimTime;

	training_world.Init();
	training_world.selected_model = model;
	training_window.selected_model = model;
	training_world.objects_friction = params.model_friction;
	training_world.createMechanism();
	training_window.currentTheme = window_theme;
	training_window.Init("Classic NEAT [NeuroEvolution of Augmenting Topologies] method", true);
	training_window.current_OB_draw_struct = ilikeitthatway(training_world, training_window, model);

	NEAT::Population* p = 0;
	srand((unsigned)time(NULL));
	NEAT::load_neat_params("models/NEATconfig/NEAT_params.txt", true);
														//test
	Model_Type agent(&training_world, &training_window, 1, params.single_eval_steps, params.two_direction_train, params.two_direction_train);
	SimTime = clock();

	p = NEAT::NEAT_run<Model_Type>(&agent, &params, model);
	if (p)
		delete p;

	double elapsedTime = (double)((clock() - SimTime) / CLOCKS_PER_SEC);
	cout << "---- Total elapsed time: " << elapsedTime << "s ----" << endl;

	if (training_window.exit_training_flag == true)
		cout << "****  Training end by user request.  ****\n**** Achieved results saved to file. ****" << endl;

	training_window.Close();
	training_world.End();
}

template<typename Model_Type>
void Visualizer_NEATmethod_train(SelectedModel model, trainParameters::NEAT_Params params, int window_theme)
{
	PhysicsWorld training_world;
	SimWindow training_window;
	clock_t SimTime;

	char filename[50];
	sprintf_s(filename, "models/VisualizedNEAT/model%d_output.txt", ((int)model + 1));
	char filename2[50];
	sprintf_s(filename2, "models/VisualizedNEAT/model%d_test_net.txt", ((int)model + 1));
	char filename3[50];
	sprintf_s(filename3, "models/VisualizedNEAT/model%d_fitness_report.txt", ((int)model + 1));

	training_world.Init();
	training_world.selected_model = model;
	training_window.selected_model = model;
	training_world.objects_friction = params.model_friction;
	training_world.createMechanism();
	training_window.currentTheme = window_theme;
	training_window.Init("Visualized NEAT [NeuroEvolution of Augmenting Topologies] method", true);
	training_window.current_OB_draw_struct = ilikeitthatway(training_world, training_window, model);

	Model_Type modelagent(&training_world, &training_window, 1, params.single_eval_steps, params.two_direction_train, params.two_direction_train);
	SimTime = clock();

	VIS_NEAT::train_VIS_NEAT_network<Model_Type>(modelagent, params, filename, filename2, filename3);

	double elapsedTime = (double)((clock() - SimTime) / CLOCKS_PER_SEC);
	cout << "---- Total elapsed time: " << elapsedTime << "s ----" << endl;

	if (training_window.exit_training_flag == true)
		cout << "****  Training end by user request.  ****\n**** Achieved results saved to file. ****" << endl;
	training_window.Close();
	training_world.End();
}