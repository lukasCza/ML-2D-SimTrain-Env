#pragma once

#include "Training_methods.h"
#include <iostream>

enum Method_selected
{
	QL_network = 0,
	NEAT_network,
	Vis_NEAT_network,
	GA_algorithm
};

class Test_Models
{
public:

	Test_Models(PhysicsWorld& test_world, SelectedModel _model) : test_world(test_world), model(_model)
	{
		for (size_t i = 0; i < 6; i++)
			for (size_t j = 0; j < 4; j++)
				loaded_data[i][j] = false;
		method_selected = NEAT_network;

		all_joints.push_back(&test_world.obj1_joints);
		all_joints.push_back(&test_world.obj2_joints);
		all_joints.push_back(&test_world.obj3_joints);
		all_joints.push_back(&test_world.obj4_joints);
		all_joints.push_back(&test_world.obj5_joints);
		all_joints.push_back(&test_world.obj6_joints);

		all_parts.push_back(&test_world.obj1_parts);
		all_parts.push_back(&test_world.obj2_parts);
		all_parts.push_back(&test_world.obj3_parts);
		all_parts.push_back(&test_world.obj4_parts);
		all_parts.push_back(&test_world.obj5_parts);
		all_parts.push_back(&test_world.obj6_parts);

		joint_count.push_back(test_world.obj1_num_joints);
		joint_count.push_back(test_world.obj2_num_joints);
		joint_count.push_back(test_world.obj3_num_joints);
		joint_count.push_back(test_world.obj4_num_joints);
		joint_count.push_back(test_world.obj5_num_joints);
		joint_count.push_back(test_world.obj6_num_joints);

		parts_count.push_back(test_world.obj1_num_parts);
		parts_count.push_back(test_world.obj2_num_parts);
		parts_count.push_back(test_world.obj3_num_parts);
		parts_count.push_back(test_world.obj4_num_parts);
		parts_count.push_back(test_world.obj5_num_parts);
		parts_count.push_back(test_world.obj6_num_parts);

		QL_networks.resize(6);
		NEAT_networks.resize(6);
		for (int i = 0; i < 6; i++)
			NEAT_networks[i] = nullptr;
		Vis_NEAT_network_phenotype.resize(6);
	}
	~Test_Models()
	{
		for (int i = 0; i < 6; i++)
			if (NEAT_networks[i] != nullptr)
				delete NEAT_networks[i];
	}
	
	void Sample()
	{	
		if (loaded_data[(int)model][(int)method_selected] == true && testing_run_flag)
		{
			if (method_selected == QL_network)
			{
				int model_id = (int)model;
				std::vector<double> input_data;
				input_data = Get_input_Data();
				arma::colvec input_arma_data(input_data);
				arma::colvec actionValues;
				QL_networks[model_id].Predict(input_arma_data, actionValues);
				int action = 0;
				double max_value = -999.0;
				for (int i = 0; i < actionValues.size(); i++) {
					if (actionValues[i] > max_value)
					{
						max_value = actionValues[i];
						action = i;
					}
					//cout << actionValues[i] << " ";
				}
				//cout << "\n";
				/*action = static_cast<decltype(action)>(
					arma::as_scalar(arma::find(actionValues == actionValues.max(), 1)));*/

				int joint_num = action / 3;
				int action_type = action % 3;
				if (action != actionValues.size()-1)			//-1
				{
					if (action_type == 0)
						if (all_joints[model_id]->data()[joint_num]->GetMotorSpeed() > -4.0)
							all_joints[model_id]->data()[joint_num]->SetMotorSpeed(-2.0f);
							//all_joints[model_id]->data()[joint_num]->SetMotorSpeed(all_joints[model_id]->data()[joint_num]->GetMotorSpeed() - DISCRETE_SPPED_GAIN);
					if (action_type == 1)
						all_joints[model_id]->data()[joint_num]->SetMotorSpeed(0.0f);
					if (action_type == 2)
						if (all_joints[model_id]->data()[joint_num]->GetMotorSpeed() < 4.0)
							all_joints[model_id]->data()[joint_num]->SetMotorSpeed(2.0f);
							//all_joints[model_id]->data()[joint_num]->SetMotorSpeed(all_joints[model_id]->data()[joint_num]->GetMotorSpeed() + DISCRETE_SPPED_GAIN);
				}
			}

			if (method_selected == GA_algorithm)
			{
				std::vector<double> input_data;
				input_data = Get_input_Data();
				int model_id = (int)model;
				int i;
				for (i = 0; i < joint_count[model_id]; i++)
				{
					if (model == MODEL_3 && i == 10)
						break;
					all_joints[model_id]->data()[i]->SetMotorSpeed((float)(GA_parameters[model_id].A1_joint[i] * sin(GA_parameters[model_id].omega1 *
						ga_dt - GA_parameters[model_id].fi1_joint[i]) + GA_parameters[model_id].A2_joint[i] *
						cos(GA_parameters[model_id].omega1 * ga_dt - GA_parameters[model_id].fi2_joint[i])) * (float)direction);
				}
				if (model == MODEL_3)
				{
					all_joints[model_id]->data()[i]->SetMotorSpeed((float)(input_data[7] * GA_parameters[model_id].A1_joint[10] *
						sin(GA_parameters[model_id].fi1_joint[10])));
				}
				if(direction != 0.0)
					ga_dt += 0.01;
			}

			if (method_selected == NEAT_network)
			{
				float val_torque;
				int model_id = (int)model;
				if (model_id == 0 || model_id == 1 || model_id == 5)
					val_torque = 100.f;
				else if (model_id == 2)
					val_torque = 600.f;
				else if(model_id == 3 || model_id == 4)
					val_torque = 40.f;

				double action = 0.0;
				std::vector<double> input_data;
				input_data = Get_input_Data();
				NEAT_networks[model_id]->load_sensors(input_data.data());			// if armadilo colvec sorts data in difrent way this wont work
				NEAT_networks[model_id]->activate();
				out_iter = NEAT_networks[model_id]->outputs.begin();
				for (int i = 0; i < joint_count[model_id]; i++)
				{
					action = (*out_iter)->activation;
					if ((((action - 0.5) > 0.0) && all_joints[model_id]->data()[i]->GetMotorSpeed() < 4.0f) ||
						(((action - 0.5) < 0.0) && all_joints[model_id]->data()[i]->GetMotorSpeed() > -4.0f))
						all_joints[model_id]->data()[i]->SetMotorSpeed(all_joints[model_id]->data()[i]->GetMotorSpeed() + ((float)action - 0.5f));
					all_joints[model_id]->data()[i]->SetMaxMotorTorque(std::abs((float)action - 0.5f) * val_torque + (val_torque/2.f));
					++out_iter;
				}
			}

			if (method_selected == Vis_NEAT_network)
			{
				float val_torque;
				int model_id = (int)model;
				if (model_id == 0 || model_id == 1 || model_id == 5)
					val_torque = 100.f;
				else if (model_id == 2)
					val_torque = 600.f;
				else if (model_id == 3 || model_id == 4)
					val_torque = 40.f;

				float action = 0.0;
				std::vector<double> input_data;
				input_data = Get_input_Data();
				for (int i = 0; i < ((parts_count[model_id] * 2) + 4); i++)
				{
					Vis_NEAT_network_phenotype[model_id].getInput(i)._output = (float)input_data[i];
				}
				Vis_NEAT_network_phenotype[model_id].update();
				for (int i = 0; i < joint_count[model_id]; i++)
				{
					action = Vis_NEAT_network_phenotype[model_id].getOutput(i)._output;
					if ((((action - 0.5) > 0.0) && all_joints[model_id]->data()[i]->GetMotorSpeed() < 4.0f) ||
						(((action - 0.5) < 0.0) && all_joints[model_id]->data()[i]->GetMotorSpeed() > -4.0f))
						all_joints[model_id]->data()[i]->SetMotorSpeed(all_joints[model_id]->data()[i]->GetMotorSpeed() + ((float)action - 0.5f));
					all_joints[model_id]->data()[i]->SetMaxMotorTorque(std::abs((float)action - 0.5f) * val_torque + (val_torque / 2.f));
				}
			}
		}
	}

	void Load_data()
	{
		if (method_selected == QL_network)
		{
			char filename[50];
			sprintf_s(filename, "models/DQN/FFNN_test_network_model%d.xml", ((int)model + 1));
			if (mlpack::data::Load(filename, "FNN", QL_networks[(int)model]))
				loaded_data[(int)model][(int)method_selected] = true;
			else
				std::cout << "\nFailed to open:" << filename << " check file name and place it in correct folder" << std::endl;
		}

		if(method_selected == GA_algorithm)
		{
			char filename[50];
			char curword[128];
			sprintf_s(filename, "models/GA/GA_test_param_model%d.txt", ((int)model + 1));
			std::ifstream file(filename, ios::in);
			if (file.is_open())
			{
				file >> curword;	//ignore opening bracket
				for (int i = 0; i < joint_count[(int)model]; i++)
				{
					if (model == MODEL_3 && i == 10)
						break;

					file >> curword;
					file >> GA_parameters[(int)model].A1_joint[i];
					file >> curword;
					file >> GA_parameters[(int)model].A2_joint[i];
					file >> curword;
					file >> GA_parameters[(int)model].fi1_joint[i];
					file >> curword;
					file >> GA_parameters[(int)model].fi2_joint[i];
				}
				if (model == MODEL_3)
				{
					file >> curword;
					file >> GA_parameters[(int)model].A1_joint[10];
					file >> curword;
					file >> GA_parameters[(int)model].fi1_joint[10];
				}
				file >> curword;
				file >> GA_parameters[(int)model].omega1;			//omega
				file.close();
				loaded_data[(int)model][(int)method_selected] = true;
			}
			else
				std::cout << "\nFailed to open:"<< filename << " check file name and place it in correct folder" << std::endl;
		}

		if (method_selected == NEAT_network)
		{
			NEAT::Genome* net_genome;
			char filename[50];
			sprintf_s(filename, "models/NEAT/test/model%d_total_best_genes.txt", ((int)model + 1));
			net_genome = NEAT::Genome::new_Genome_load(filename);
			if (net_genome != 0)
			{
				if (loaded_data[(int)model][(int)method_selected] == true)
					delete NEAT_networks[(int)model];
				NEAT_networks[(int)model] = net_genome->genesis((int)model);
				delete net_genome;
				loaded_data[(int)model][(int)method_selected] = true;
			}
			else
				std::cout << "\nFailed to open:" << filename << " check file name and place it in correct folder" << std::endl;
		}

		if (method_selected == Vis_NEAT_network)
		{
			neat::NetworkGenotype genotype;
			char filename[50];
			sprintf_s(filename, "models/VisualizedNEAT/model%d_test_net.txt", ((int)model + 1));
			std::ifstream iFile(filename, std::ios::in);
			if (iFile.is_open())
			{
				genotype.readFromStream(iFile);
				iFile.close();
				Vis_NEAT_network_phenotype[(int)model].create(genotype);
				loaded_data[(int)model][(int)method_selected] = true;
			}
			else
				std::cout << "\nFailed to open:" << filename << " check file name and place it in correct folder" << std::endl;
		}
	}

	void check_model_data()
	{
		if (loaded_data[(int)model][(int)method_selected] == false)
			std::cout << "\nNo data loaded for this model and selected ML method" << std::endl;
		else
			std::cout << "\nData selected" << std::endl;
	}

	std::vector<double> Get_input_Data()
	{
		std::vector<double> input_data;
		int model_id = (int)model;
		if (model == MODEL_1)
		{
			input_data.push_back((double)all_parts[model_id]->data()[4]->GetLinearVelocity().x);
			input_data.push_back((double)all_parts[model_id]->data()[4]->GetLinearVelocity().y);
		}
		if (model == MODEL_2)
		{
			input_data.push_back((double)all_parts[model_id]->data()[4]->GetLinearVelocity().x);
			input_data.push_back((double)all_parts[model_id]->data()[4]->GetLinearVelocity().y);
		}
		if (model == MODEL_3)
		{
			input_data.push_back((double)all_parts[model_id]->data()[6]->GetLinearVelocity().x);
			input_data.push_back((double)all_parts[model_id]->data()[6]->GetLinearVelocity().y);
		}
		if (model == MODEL_4)
		{
			input_data.push_back((double)all_parts[model_id]->data()[2]->GetLinearVelocity().x);
			input_data.push_back((double)all_parts[model_id]->data()[2]->GetLinearVelocity().y);
		}
		if (model == MODEL_5)
		{
			input_data.push_back((double)all_parts[model_id]->data()[6]->GetLinearVelocity().x);
			input_data.push_back((double)all_parts[model_id]->data()[6]->GetLinearVelocity().y);
		}
		if (model == MODEL_6)
		{
			input_data.push_back((double)all_parts[model_id]->data()[4]->GetLinearVelocity().x);
			input_data.push_back((double)all_parts[model_id]->data()[4]->GetLinearVelocity().y);
		}
		int i;
		for (i = 0; i < parts_count[model_id]; i++)
		{
			input_data.push_back((double)all_parts[model_id]->data()[i]->GetAngle());
			input_data.push_back((double)all_parts[model_id]->data()[i]->GetAngularVelocity());
		}
		input_data.push_back(direction);
		
		//@BIAS
		//if (model == MODEL_5 || model == MODEL_6)
		input_data.push_back(1.0);

		return input_data;
	}
	void flushbackNEAT() {
		if (loaded_data[(int)model][(int)method_selected] == true){
			if (method_selected == NEAT_network)
				NEAT_networks[(int)model]->flush();
			if (method_selected == Vis_NEAT_network)
				Vis_NEAT_network_phenotype[(int)model].resetOutputs();
		}
	}

	void SelectNewModel(SelectedModel _model) { model = _model; }
	void SelectNewMethod(Method_selected method) { method_selected = method; }
	int GetSelectedMethodInt() { return (int)method_selected; }
	void ChangeDirectionLeft() { direction = -1.0; }
	void ChangeDirectionRight() { direction = 1.0; }
	void ChangeDirectionStop() { direction = 0.0; }
	void StopTesting() { testing_run_flag = false; }
	void StartTesting() { testing_run_flag = true; }
	void reset_dt_for_GA() {ga_dt = 0.0;}
	bool GetRunFlag() const { return testing_run_flag; }
	
private:

	PhysicsWorld& test_world;
	SelectedModel model;
	Method_selected method_selected;
	bool testing_run_flag = false;
	bool loaded_data[6][4];	//model[] , method[]
	double direction = 1.0;

	std::vector<int> joint_count;
	std::vector<int> parts_count;
	std::vector<std::vector<b2RevoluteJoint*>*> all_joints;
	std::vector<std::vector<b2Body*>*> all_parts;

	//methods containers
	std::vector<mlpack::ann::FFN<mlpack::ann::MeanSquaredError<>, mlpack::ann::GaussianInitialization>> QL_networks;
	GeneticAlgorithm::MySolution_Model5 GA_parameters[6]; //biggest arrays so can be used for all cases;
	double ga_dt = 0.0;
	std::vector<NEAT::Network*> NEAT_networks;
	std::vector<NEAT::NNode*>::iterator out_iter;
	std::vector<neat::NetworkPhenotype> Vis_NEAT_network_phenotype;
};
