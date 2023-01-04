#pragma once
#include "Model1_Agent.h"
#include "Model2_Agent.h"
#include "Model3_Agent.h"
#include "Model4_Agent.h"
#include "Parameters_class.h"

#include "neat.h"
#include "network.h"
#include "population.h"
#include "organism.h"

using namespace mlpack::rl;
using namespace std;
using namespace NEAT;

namespace NEAT 
{

	template<typename Model_Type>
	double NEAT_action_learning_loop(Network* net, Model_Type* models_agent)
	{
		using State = typename Model_Type::State;
		using Action = typename Model_Type::ContiniousAction;

		State model_state;
		State model_next_state;
		Action cont_model_action;

		vector<NNode*>::iterator out_iter;

		model_state = models_agent->InitialSample();
		double sample_return = 1;
		double fitness_value = 0;

		while (models_agent->StepsPerformed() < models_agent->MaxSteps())
		{
			/* sent simulation output to NN input */
			net->load_sensors(model_state.Data().begin());

			if (!(net->activate())) return 1.0;	//break if network failed

			/* pick best action based on current NN outputs */
			out_iter = net->outputs.begin();
			for (int i = 0; i < cont_model_action.size; i++)
			{
				cont_model_action.action[i] = (*out_iter)->activation;
				++out_iter;
			}

			/* simulate world step for current best action */
			//sample_return = models_agnet->SampleContinious(model_state, cont_model_action);
			fitness_value = models_agent->SampleContinious(model_state, cont_model_action, model_next_state);
			model_state = model_next_state;
			models_agent->training_window->fitness = fitness_value;
			/* return fitness value if steps ended or model failed */
			//fitness_value += sample_return;
			//if (sample_return <= 0.0) return fitness_value;
			if (models_agent->IsTerminal(model_state)) return fitness_value;
		}
		return fitness_value;

	}

	template<typename Model_Type>
	bool NEAT_evaluate(Organism* org, Model_Type* models_agent, double& win_fitness)
	{
		Network* net;
		net = org->net;
		int numnodes;
		numnodes = ((org->gnome)->nodes).size();
		models_agent->training_window->organism_num = (org->gnome)->genome_id;
		models_agent->training_window->fitness = 0.0;
		//run simulation loop
		org->fitness = NEAT::NEAT_action_learning_loop<Model_Type>(net, models_agent);

		cout << "Org " << (org->gnome)->genome_id + 1 << " fitness: " << org->fitness << endl;

		//Decide if its a winner
		if (org->fitness >= win_fitness) {
			org->winner = true;
			win_fitness += 50.0;
			return true;
		}
		else {
			org->winner = false;
			return false;
		}
	}

	template<typename Model_Type>
	int NEAT_epoch(Population* pop, int generation, 
					char* filename, Model_Type* models_agent, double& win_fitness,
					int& max_gen, int& pop_size, char* filename2, double& gen_fitness)
	{
		vector<Organism*>::iterator curorg;
		vector<Species*>::iterator curspecies;
		bool win = false;
		int winnernum;

		//Evaluate each organism on a test
		for (curorg = (pop->organisms).begin(); curorg != (pop->organisms).end(); ++curorg)
			if (NEAT::NEAT_evaluate<Model_Type>(*curorg, models_agent, win_fitness)) 
				win = true;

		//Average and max their fitnesses for dumping to file and snapshot
		for (curspecies = (pop->species).begin(); curspecies != (pop->species).end(); ++curspecies)
		{
			(*curspecies)->compute_average_fitness();
			(*curspecies)->compute_max_fitness();
		}

		//sort organisms 
		std::sort(pop->organisms.begin(), pop->organisms.end(), order_orgs);
		gen_fitness = pop->organisms[0]->fitness;

		//Only print to file every print_every generations
		if (win || ((generation % (NEAT::print_every)) == 0) || generation == max_gen)
			pop->print_to_file_by_species(filename);

		if (win)
		{
			double max_fit = -999.9;
			for (curorg = (pop->organisms).begin(); curorg != (pop->organisms).end(); ++curorg) {
				if ((*curorg)->winner) {
					winnernum = ((*curorg)->gnome)->genome_id;
					cout << "WINNER IS #" << ((*curorg)->gnome)->genome_id << endl;
					if ((*curorg)->fitness > max_fit) {
						//write to file
						std::ofstream oFile(filename2, std::ios::trunc);
						if (oFile.is_open()) {
							(*curorg)->write_to_file(oFile);
							oFile.close();
							max_fit = (*curorg)->fitness;
						}
					}
				}
			}
		}
		//Create the next generation if exit not envoked
		if(models_agent->training_window->exit_training_flag == false && generation != max_gen)
			pop->epoch(generation);

		if (win) return ((generation - 1) * pop_size + winnernum);
		else return 0;
	}

	template<typename Model_Type>
	Population* NEAT_run(Model_Type* models_agent, trainParameters::NEAT_Params* params, SelectedModel model)
	{
		using State = typename Model_Type::State;
		using Action = typename Model_Type::ContiniousAction;

		Population* pop = 0;
		Genome* start_genome;
		double winner_fitness = params->winner_fitness_value;
		double gen_best_fitness = 0.0;
		int gen;
		int expcount;
		int status;
		int* runs;
		runs = new int[params->num_runs];
		int totalevals;
		int samples;
		NEAT::pop_size = params->population_size;
		NEAT::num_runs = params->num_runs;

		char filename[50];
		sprintf_s(filename, "models/NEAT/test/model%d_total_best_genes.txt", ((int)model + 1));
		char filename3[50];
		sprintf_s(filename3, "models/NEAT/test/model%d_fitness_report.txt", ((int)model + 1));
		ofstream FitnessFile(filename3, std::ios::trunc);
		bool fit_file_open = FitnessFile.is_open();

		memset(runs, 0, params->num_runs * sizeof(int));
		cout << "START SINGLE EVOLUTION" << endl;
		cout << "Generating the start genome" << endl;
		start_genome = new Genome(State::dimension, Action::size, 0, 0);

		//Run multiple experiments
		for (expcount = 0; expcount < params->num_runs; expcount++) 
		{
			cout << "EXPERIMENT #" << expcount << endl;
			cout << "Start Genome: " << start_genome << endl;
			//Spawn the Population
			cout << "Spawning Population off Genome" << endl;

			pop = new Population(start_genome, params->population_size);
			cout << "Verifying Spawned Pop" << endl;
			pop->verify();

			for (gen = 1; gen <= params->num_generations ; gen++)
			{
				cout << "Generation " << gen << endl;
				char temp[50];
				sprintf_s(temp, "models/NEAT/output/Model%d_run%d_gen_%d.txt",((int)model+1), (expcount+1), gen);

				models_agent->training_window->generation = gen;
				models_agent->training_window->best_fitness = pop->highest_fitness;

				status = NEAT::NEAT_epoch<Model_Type>(pop, gen, temp, models_agent,
													winner_fitness,
													params->num_generations,
													params->population_size, 
													filename, gen_best_fitness);
				if (status) {
					runs[expcount] = status;
					//gen = params->num_generations + 1;
				}
				if (fit_file_open)
					FitnessFile << gen << "\t" << gen_best_fitness << "\n";

				if (models_agent->training_window->exit_training_flag == true)
					break;
			}
			if (models_agent->training_window->exit_training_flag == true)
				break;
			if (expcount < params->num_runs - 1)
				delete pop;
		}
		int range;
		if (models_agent->training_window->exit_training_flag == false)
			range = params->num_runs;
		else
			range = expcount;

		totalevals = 0;
		samples = 0;
		for (expcount = 0; expcount < range; expcount++) {
			cout << runs[expcount] << endl;
			if (runs[expcount] > 0)
			{
				totalevals += runs[expcount];
				samples++;
			}
		}
		delete runs;
		if (fit_file_open)
			FitnessFile.close();

		cout << "Failures: " << (params->num_runs - samples) << " out of " << params->num_runs << " runs" << endl;
		cout << "Average evals: " << (samples > 0 ? (double)totalevals / samples : 0) << endl;
		cout << "Total best fitness:" << pop->highest_fitness << endl;

		//print best up to 5 organism to test file
		char filename2[50];
		std::sort(pop->organisms.begin(), pop->organisms.end(), order_orgs);
		vector<Organism*>::iterator best_orgs;
		int i = 1;
		for (best_orgs = pop->organisms.begin(); best_orgs != pop->organisms.end(); ++best_orgs)
		{
			sprintf_s(filename2, "models/NEAT/test/model%d_best_genes%d.txt", ((int)model + 1), i);
			(*best_orgs)->print_to_file(filename2);
			std::cout << (*best_orgs)->fitness << std::endl;
			i++;
			if (i > 4)
				break;
		}
		return pop;
	}
}
