#pragma once
#include <iostream>
#include <time.h>
#include "Model1_Agent.h"
#include "Model2_Agent.h"
#include "Model3_Agent.h"
#include "Model4_Agent.h"
#include "Parameters_class.h"

#include <Visualized_NEAT/neat/Evolver.h>
#include <Visualized_NEAT/neat//NetworkPhenotype.h>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <Visualized_NEAT/visualization/PhenotypeImage.h>
#include <Visualized_NEAT/neat//ParentSelectorProportional.h>

using namespace mlpack::rl;

namespace VIS_NEAT
{
	template<typename Model_Type>
	float fitness_Evaluate(neat::NetworkPhenotype& phenotype, Model_Type& model_agent)
	{
		using State = typename Model_Type::State;
		using Action = typename Model_Type::ContiniousAction;

		State model_state;
		State model_next_state;
		Action model_action;

		model_state = model_agent.InitialSample();

		double sample_return = 1;
		double fitness_value = 0;
		double* data;

		while (model_agent.StepsPerformed() < model_agent.MaxSteps())
		{
			/* sent simulation output to NN input */
			data = model_state.Data().begin();
			for (int i = 0; i < model_state.dimension; i++)
			{
				phenotype.getInput(i)._output = (float)data[i];
			}

			phenotype.update();
			/* pick best action based on current NN outputs */
			for (int i = 0; i < model_action.size; i++)
			{
				model_action.action[i] = (double)phenotype.getOutput(i)._output;
			}
			/* simulate world step for current best action */
			//sample_return = model_agent.SampleContinious(model_state, model_action);
			fitness_value = model_agent.SampleContinious(model_state, model_action, model_next_state);
			model_state = model_next_state;
			model_agent.training_window->fitness = fitness_value;
			/* return fitness value if steps ended or model failed */
			//fitness_value += sample_return;

			if (model_agent.IsTerminal(model_state)) return (float)fitness_value;
		}
		return (float)fitness_value;
	}

	template<typename Model_Type>
	float fitnessUpdate(neat::Evolver& evolver, Model_Type& model_agent)
	{
		float best_fitness = -999.0;
		for (size_t i = 0, populationSize = evolver.getPopulationSize(); i < populationSize; i++) {
			neat::NetworkPhenotype phenotype;

			assert(evolver._population[i]._genotype != nullptr);

			phenotype.create(*evolver._population[i]._genotype);
			model_agent.training_window->organism_num = i;
			model_agent.training_window->fitness = 0.0;
			float fitness = VIS_NEAT::fitness_Evaluate<Model_Type>(phenotype, model_agent);
			if (fitness > best_fitness)
				best_fitness = fitness;

			std::cout << "Organism no.:  " << (i + 1) << "  evaluated. Fitness: " << fitness << std::endl;
			evolver._population[i]._fitness = fitness; //std::pow(fitness, 2.0f);	//allways positive?
		}
		return best_fitness;
	}

	void drawNetwork(PhenotypeImage& image,
					sf::RenderWindow& window,
					int population,
					int num_generations,
					int max_generations,
					sf::Rect<float>& region,
					sf::Font& font,
					std::shared_ptr<neat::NetworkGenotype>& fittest,
					std::mt19937& generator,
					bool save_img)
	{
		// Draw
		image.updateShowStats(population, num_generations,
							window, region, *fittest, "evaluated neural network",
							font, 15, generator);
		sf::Sprite s(image.getTexture());
		sf::Event event;
		while (window.pollEvent(event));

		window.clear(sf::Color::White);
		window.draw(s);
		window.display();

		if (save_img == true)
			image.saveToFile("./models/VisualizedNEAT/Visualized_NEAT_Network.png");
	}

	//main function
	template<typename Model_Type>
	void train_VIS_NEAT_network(Model_Type& model_agent, trainParameters::NEAT_Params params, char* _filename, char* _filename2, char* _filename3)
	{
		using Inputs = typename Model_Type::State;
		using Outputs = typename Model_Type::ContiniousAction;

		ofstream FitnessFile(_filename3, std::ios::trunc);
		bool fit_file_open = FitnessFile.is_open();

		//vis window
		sf::RenderWindow window(sf::VideoMode(1000, 800), "Network View");
		sf::Rect<float> region(60.0f, 60.0f, 880.0f, 680.0f);
		PhenotypeImage image(1000, 800);
		sf::Font font;
		if (!font.loadFromFile("Cascadia.ttf"))
			abort();

		std::mt19937 generator(time(nullptr));
		neat::Evolver evolver;
		evolver._settings._populationSize = (size_t)params.population_size;
		evolver._settings._numElites = params.num_elites;
		evolver._settings._reproduceRatio = params.reproduce_ratio;
		evolver._settings._newConnectionMutationRate = params.new_connection_mutation_rate;
		evolver._settings._newNodeMutationRate = params.new_node_mutation_rate;
		evolver._settings._maxPerturbation = params.max_perturbation;

		std::cout << "Evolving network..." << std::endl;
		evolver.initialize(Inputs::dimension, Outputs::size,
			std::shared_ptr<neat::ParentSelectorProportional>(new neat::ParentSelectorProportional()), generator);
		
		const size_t numGenerations = params.num_generations;
		size_t g;
		float best_fitness = -999.0;

		for (g = 0; g < numGenerations; g++) 
		{
			//************ main loop *********
			model_agent.training_window->generation = g + 1;
			best_fitness = VIS_NEAT::fitnessUpdate<Model_Type>(evolver, model_agent);
			if ((double)best_fitness > model_agent.training_window->best_fitness)
				model_agent.training_window->best_fitness = (double)best_fitness;

			std::cout << "Generation no.:  " << g << "  evaluated\n" 
					<< "Generation best fitness:" << best_fitness << std::endl;
			if (fit_file_open)
				FitnessFile << g + 1 << "\t" << best_fitness << "\n";
			evolver.epoch(generator);
			//************** end ************

			if (g % params.draw_each == 0)
			{
				std::shared_ptr<neat::NetworkGenotype> curr_gen_fittest;
				float curr_gen_maxFitness = -999999.0f;

				for (size_t i = 0, populationSize = evolver.getPopulationSize(); i < populationSize; i++) {
					float curr_gen_fitness = evolver._population[i]._fitness;

					if (curr_gen_fitness > curr_gen_maxFitness) {
						curr_gen_maxFitness = curr_gen_fitness;
						curr_gen_fittest = evolver._population[i]._genotype;
					}
				}
				VIS_NEAT::drawNetwork(image, window, evolver.getPopulationSize(), g, numGenerations, region, font, curr_gen_fittest, generator, false);
			}
			if (model_agent.training_window->exit_training_flag == true)
				break;
		}

		// Update fitnesses for final generation
		best_fitness = VIS_NEAT::fitnessUpdate<Model_Type>(evolver, model_agent);
		std::cout << "Evolved network done." << std::endl;
		if (fit_file_open)
		{
			FitnessFile << g + 1 << "\t" << best_fitness << "\n";
			FitnessFile.close();
		}
		// Find fittest
		std::shared_ptr<neat::NetworkGenotype> fittest;
		float maxFitness = -999999.0f;

		for (size_t i = 0, populationSize = evolver.getPopulationSize(); i < populationSize; i++) {
			float fitness = evolver._population[i]._fitness;

			if (fitness > maxFitness) {
				maxFitness = fitness;
				fittest = evolver._population[i]._genotype;
			}
		}

		std::ofstream outFile(_filename, std::ios::trunc);
		fittest.get()->writeToStream(outFile);
		outFile.close();
		outFile.open(_filename2, std::ios::trunc);
		fittest.get()->writeToStream(outFile);
		outFile.close();

		//neat::NetworkPhenotype bestPhenotype;
		//bestPhenotype.create(*fittest);
		//std::cout << "Fitness: " << VIS_NEAT::fitness_Evaluate(bestPhenotype, model_agent) << std::endl;
		//std::cout << "Num Connections: " << fittest->getConnectionSet().size() << std::endl;
		//std::cout << "Num Hidden Units: " << fittest->getNumHidden() << std::endl;

		std::cout << "Generating image..." << std::endl;
		VIS_NEAT::drawNetwork(image, window, evolver.getPopulationSize(), g, numGenerations, region, font, fittest, generator, true);

		window.close();
	}
}
