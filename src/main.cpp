/**********
 * @file main.cpp
 * @author lukasCza
 ***********/

//change default stack size cuz NEAT is very unbalanced
//#pragma comment(linker, "/STACK:67108864")	//default is 1MB changed to 64MB
//#pragma comment(linker, "/STACK:134217728")		//128MB
#pragma comment(linker, "/STACK:268435456")		//256MB

#include <time.h>
#include <cstdio>
#include <cmath>
#include "PhysicsEngine.h"
#include "ProgramWindow.h"
#include "Training_methods.h"
#include "Load_results.h"
#include "fast_draw.h"
#include "Model1_Agent.h"
#include "Model2_Agent.h"
#include "Model3_Agent.h"
#include "Model4_Agent.h"
#include "Model5_Agent.h"
#include "Model6_Agent.h"

#define FPS30 33.f	//refresh rate

double clockToMilliseconds(clock_t ticks) {return (ticks / (double)CLOCKS_PER_SEC) * 1000.0;}

int main() 
{
	PhysicsWorld PhysicEngine;
	SimWindow SimulationWindow;
	Test_Models Testing_Env(PhysicEngine, PhysicEngine.selected_model);

	SimulationWindow.Init("Machine Learning for Bioinspired Mechanisms");
	PhysicEngine.add_test_objects = false;
	PhysicEngine.Init();
	PhysicEngine.createMechanism();

	SimulationWindow.current_OB_draw_struct = ilikeitthatway(PhysicEngine, SimulationWindow, PhysicEngine.selected_model);

	//loop variables
	bool Program_running = true;
	sf::Clock DeltaClock;
	clock_t previous = clock();
	double lag = 0.0;
	double lag2 = 0.0;
	//std::vector<std::thread> threads;

	while (Program_running)
	{
		//timers
		clock_t current = clock();
		clock_t elapsed = current - previous;
		previous = current;
		lag += clockToMilliseconds(elapsed);
		lag2 += clockToMilliseconds(elapsed);

		while (lag >= MS_PER_UPDATE)
		{
			if (Testing_Env.GetRunFlag())
				Testing_Env.Sample();
			//world simulation
			PhysicEngine.CheckAngleLimits();
			PhysicEngine.TimeStep();

			lag -= MS_PER_UPDATE;
		}

		while (lag2 >= FPS30)
		{
			lag2 -=FPS30;

			//handle window events
			SimulationWindow.HandleEvents(&Program_running, &DeltaClock);
			
			//change displayed model
			if (SimulationWindow.selected_model != PhysicEngine.selected_model)
			{
				PhysicEngine.destroyMechanism(); //destroy old
				PhysicEngine.selected_model = SimulationWindow.selected_model;
				PhysicEngine.createMechanism();
				SimulationWindow.current_OB_draw_struct = ilikeitthatway(PhysicEngine, SimulationWindow, 
					PhysicEngine.selected_model);
				Testing_Env.SelectNewModel(PhysicEngine.selected_model);
				Testing_Env.check_model_data();
				Testing_Env.reset_dt_for_GA();
				Testing_Env.flushbackNEAT();
			}

			//select training method
			//QL
			if (SimulationWindow.valueofbutton1)
			{
				switch (PhysicEngine.selected_model)
				{
				case MODEL_1:
					neuralnetworkQL_train<mlpack::rl::Model1_Agent>
					(PhysicEngine.selected_model, SimulationWindow.QL_parameters, SimulationWindow.currentTheme);
					break;
				case MODEL_2:
					neuralnetworkQL_train<mlpack::rl::Model2_Agent>
					(PhysicEngine.selected_model, SimulationWindow.QL_parameters, SimulationWindow.currentTheme);
					break;
				case MODEL_3:
					neuralnetworkQL_train<mlpack::rl::Model3_Agent>
					(PhysicEngine.selected_model, SimulationWindow.QL_parameters, SimulationWindow.currentTheme);
					break;
				case MODEL_4:
					neuralnetworkQL_train<mlpack::rl::Model4_Agent>
					(PhysicEngine.selected_model, SimulationWindow.QL_parameters, SimulationWindow.currentTheme);
					break;
				case MODEL_5:
					neuralnetworkQL_train<mlpack::rl::Model5_Agent>
					(PhysicEngine.selected_model, SimulationWindow.QL_parameters, SimulationWindow.currentTheme);
					break;
				case MODEL_6:
					neuralnetworkQL_train<mlpack::rl::Model6_Agent>
					(PhysicEngine.selected_model, SimulationWindow.QL_parameters, SimulationWindow.currentTheme);
					break;
				default:
					break;
				}
				previous = clock();
				SimulationWindow.valueofbutton1 = false;
			}
			//NEAT
			if (SimulationWindow.valueofbutton2)
			{
				//threads.push_back(std::thread(NEATmethod_train));
				switch (PhysicEngine.selected_model)
				{
				case MODEL_1:
					NEATmethod_train<mlpack::rl::Model1_Agent>
					(PhysicEngine.selected_model, SimulationWindow.NEAT_parameters, SimulationWindow.currentTheme);
					break;
				case MODEL_2:
					NEATmethod_train<mlpack::rl::Model2_Agent>
					(PhysicEngine.selected_model, SimulationWindow.NEAT_parameters, SimulationWindow.currentTheme);
					break;
				case MODEL_3:
					NEATmethod_train<mlpack::rl::Model3_Agent>
					(PhysicEngine.selected_model, SimulationWindow.NEAT_parameters, SimulationWindow.currentTheme);
					break;
				case MODEL_4:
					NEATmethod_train<mlpack::rl::Model4_Agent>
					(PhysicEngine.selected_model, SimulationWindow.NEAT_parameters, SimulationWindow.currentTheme);
					break;
				case MODEL_5:
					NEATmethod_train<mlpack::rl::Model5_Agent>
					(PhysicEngine.selected_model, SimulationWindow.NEAT_parameters, SimulationWindow.currentTheme);
					break;
				case MODEL_6:
					NEATmethod_train<mlpack::rl::Model6_Agent>
					(PhysicEngine.selected_model, SimulationWindow.NEAT_parameters, SimulationWindow.currentTheme);
					break;
				default:
					break;
				}
				previous = clock();
				SimulationWindow.valueofbutton2 = false;
			}
			//visNEAT
			if (SimulationWindow.valueofbutton3)
			{
				switch (PhysicEngine.selected_model)
				{
				case MODEL_1:
					Visualizer_NEATmethod_train<mlpack::rl::Model1_Agent>
					(PhysicEngine.selected_model, SimulationWindow.NEAT_parameters, SimulationWindow.currentTheme);
					break;
				case MODEL_2:
					Visualizer_NEATmethod_train<mlpack::rl::Model2_Agent>
					(PhysicEngine.selected_model, SimulationWindow.NEAT_parameters, SimulationWindow.currentTheme);
					break;
				case MODEL_3:
					Visualizer_NEATmethod_train<mlpack::rl::Model3_Agent>
					(PhysicEngine.selected_model, SimulationWindow.NEAT_parameters, SimulationWindow.currentTheme);
					break;
				case MODEL_4:
					Visualizer_NEATmethod_train<mlpack::rl::Model4_Agent>
					(PhysicEngine.selected_model, SimulationWindow.NEAT_parameters, SimulationWindow.currentTheme);
					break;
				case MODEL_5:
					Visualizer_NEATmethod_train<mlpack::rl::Model5_Agent>
					(PhysicEngine.selected_model, SimulationWindow.NEAT_parameters, SimulationWindow.currentTheme);
					break;
				case MODEL_6:
					Visualizer_NEATmethod_train<mlpack::rl::Model6_Agent>
					(PhysicEngine.selected_model, SimulationWindow.NEAT_parameters, SimulationWindow.currentTheme);
					break;
				default:
					break;
				}
				previous = clock();
				SimulationWindow.valueofbutton3 = false;
			}
			//GA
			if (SimulationWindow.valueofbutton4)
			{
				switch (PhysicEngine.selected_model)
				{
				case MODEL_1:
					GA_train<mlpack::rl::Model1_Agent, GeneticAlgorithm::MySolution_Model1>
					(PhysicEngine.selected_model, SimulationWindow.GA_parameters, SimulationWindow.currentTheme);
					break;
				case MODEL_2:
					GA_train<mlpack::rl::Model2_Agent, GeneticAlgorithm::MySolution_Model2or4>
					(PhysicEngine.selected_model, SimulationWindow.GA_parameters, SimulationWindow.currentTheme);
					break;
				case MODEL_3:
					GA_train<mlpack::rl::Model3_Agent, GeneticAlgorithm::MySolution_Model3>
					(PhysicEngine.selected_model, SimulationWindow.GA_parameters, SimulationWindow.currentTheme);
					break;
				case MODEL_4:
					GA_train<mlpack::rl::Model4_Agent, GeneticAlgorithm::MySolution_Model2or4>
					(PhysicEngine.selected_model, SimulationWindow.GA_parameters, SimulationWindow.currentTheme);
					break;
				case MODEL_5:
					GA_train<mlpack::rl::Model5_Agent, GeneticAlgorithm::MySolution_Model5>
					(PhysicEngine.selected_model, SimulationWindow.GA_parameters, SimulationWindow.currentTheme);
					break;
				case MODEL_6:
					GA_train<mlpack::rl::Model6_Agent, GeneticAlgorithm::MySolution_Model6>
					(PhysicEngine.selected_model, SimulationWindow.GA_parameters, SimulationWindow.currentTheme);
					break;
				default:
					break;
				}
				previous = clock();
				SimulationWindow.valueofbutton4 = false;
			}
			//end of training method select

			// test options
			//reset
			if (SimulationWindow.valueofbutton6)
			{	
				PhysicEngine.obstacles = SimulationWindow.add_obstacles;
				PhysicEngine.add_test_objects = SimulationWindow.draw_test_objects;
				PhysicEngine.resetMechanism();
				PhysicEngine.AddRemoveObstacleTestobj();
				Testing_Env.reset_dt_for_GA();
				Testing_Env.flushbackNEAT();
				SimulationWindow.valueofbutton6 = false;
			}
			//select method
			if (SimulationWindow.current_method != Testing_Env.GetSelectedMethodInt())
			{
				Testing_Env.SelectNewMethod((Method_selected)SimulationWindow.current_method);
				Testing_Env.check_model_data();
			}
			// load model
			if (SimulationWindow.valueofbutton5)
			{
				Testing_Env.Load_data();
				Testing_Env.check_model_data();
				SimulationWindow.valueofbutton5 = false;
			}
			//start
			if (SimulationWindow.valueofbutton8)
			{
				Testing_Env.StartTesting();
				SimulationWindow.valueofbutton8 = false;
			}
			// stop
			if (SimulationWindow.valueofbutton9)
			{
				Testing_Env.StopTesting();
				SimulationWindow.valueofbutton9 = false;
			}
			//directions
			if (SimulationWindow.valueofbutton7)
			{
				Testing_Env.ChangeDirectionStop();
				SimulationWindow.valueofbutton7 = false;
			}
			if (SimulationWindow.valueofbutton_direction_left)
			{
				Testing_Env.ChangeDirectionLeft();
				SimulationWindow.valueofbutton_direction_left = false;
			}
			if (SimulationWindow.valueofbutton_direction_right)
			{
				Testing_Env.ChangeDirectionRight();
				SimulationWindow.valueofbutton_direction_right = false;
			}
			//end of test options
			
			//set offset
			switch (PhysicEngine.selected_model)
			{
			case MODEL_1:
				SimulationWindow.SetOffset(PhysicEngine.obj1_parts[4]);
				break;
			case MODEL_2:
				SimulationWindow.SetOffset(PhysicEngine.obj2_parts[4]);
				break;
			case MODEL_3:
				SimulationWindow.SetOffset(PhysicEngine.obj3_parts[6]);
				break;
			case MODEL_4:
				SimulationWindow.SetOffset(PhysicEngine.obj4_parts[2]);
				break;
			case MODEL_5:
				SimulationWindow.SetOffset(PhysicEngine.obj5_parts[6]);
				break;
			case MODEL_6:
				SimulationWindow.SetOffset(PhysicEngine.obj6_parts[4]);
				break;
			default:
				break;
			}
			//draw window and objects
			if(PhysicEngine.obstacles == true)
				SimulationWindow.DrawObjects(PhysicEngine.groundBody2, 
					&SimulationWindow.sf_obstacle, &SimulationWindow.transparentColor);
			
			if (PhysicEngine.add_test_objects == true)
			{
				SimulationWindow.DrawObjects(PhysicEngine.testbox, 
					&SimulationWindow.sf_testbox, &SimulationWindow.frontElementsColor);
				SimulationWindow.DrawObjects(PhysicEngine.testball, 
					&SimulationWindow.sf_ob1_joint1, &SimulationWindow.frontElementsColor);
			}
			//debuging:
			//
			// 
			
			//draw menu and display frame
			SimulationWindow.DrawModel(SimulationWindow.current_OB_draw_struct);
			SimulationWindow.DrawWindow();
			SimulationWindow.Display();
		}
	}//end of program loop

	//for (auto& th : threads)
	//th.join();
	PhysicEngine.End();
	SimulationWindow.Close();

	return 0;
}