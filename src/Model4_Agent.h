#pragma once
#ifndef MLPACK_METHODS_RL_ENVIRONMENT_MYMODEL4_HPP
#define MLPACK_METHODS_RL_ENVIRONMENT_MYMODEL4_HPP

#include <mlpack/prereqs.hpp>
#include <ctime>
#include "PhysicsEngine.h"
#include "ProgramWindow.h"

//thetatreshold * 10 - no falling over set

namespace mlpack {
namespace rl {

class Model4_Agent
{
public:
	class State
	{
	public:
        State() : data(dimension){}

        State(const arma::colvec& data) : data(data) {}

        arma::colvec Data() const { return data; }
        arma::colvec& Data() { return data; }

        //! Get the i-th part 0->4
        double LinearVelocity() const { return data[0]; }
        double& LinearVelocity() { return data[0]; }
        double verticalVelocity() const { return data[1]; }
        double& verticalVelocity() { return data[1]; }
        double Angle(const size_t i) const { return data[i * 2 + 2]; }
        double& Angle(const size_t i) { return data[i * 2 + 2]; }
        double AngularVelocity(const size_t i) const { return data[i * 2 + 3]; }
        double& AngularVelocity(const size_t i) { return data[i * 2 + 3]; }
        double MoveDirection() const { return data[12]; }
        double& MoveDirection() { return data[12]; }
        //*******2dir
        //@BIAS
        double Bias() const { return data[13]; }
        double& Bias() { return data[13]; }

        //! Encode the state to a vector..
        const arma::colvec& Encode() const { return data; }

        //! Dimension of the encoded state.
        static constexpr size_t dimension = 14;

    private:
        //! Locally-stored state data.
        arma::colvec data;
	};

	class Action
	{
	public:
		enum actions
		{
			rotate_backward_joint1 = 0,
            stop_joint1,
			rotate_forward_joint1,
			rotate_backward_joint2,
            stop_joint2,
			rotate_forward_joint2,
            rotate_backward_joint3,
            stop_joint3,
            rotate_forward_joint3,
            rotate_backward_joint4,
            stop_joint4,
            rotate_forward_joint4,
            no_action
		};
		// to store the action.
		Action::actions action;

		// track the size of the action space.
		static const size_t size = 13;

        // single joint action space
        static const int joint_actions = 3;
	};

    class ContiniousAction
    {
    public:
        std::vector<double> action;
        // Storing degree of freedom.
        static constexpr size_t size = 4;

        ContiniousAction() : action(Model4_Agent::ContiniousAction::size) {}
    };

    Model4_Agent(PhysicsWorld* training_world,
                SimWindow* training_window,
                int output_type,
                const size_t maxsteps = 100,
                bool dir_train = false,
                bool write_twoway = false,
                const double thetaThresholdRadians = 3.14,
                const double final_distance = 1.0,
                const double doneReward = 0.0) :
        training_world(training_world),
        training_window(training_window),
        output_type(output_type),
        maxSteps(maxsteps),
        perform_dir_train(dir_train),
        write_to_screen_twoway(write_twoway),
        thetaThresholdRadians(thetaThresholdRadians),
        final_distance(final_distance),
        doneReward(doneReward),
        stepsPerformed(0),
        previous(0),
        current(0),
        total_eval_time(0),
        last_eval_time(0) 
    {
        if (final_distance == 1.0)
            this->final_distance = maxsteps * 0.004;
    }

    /* ---- discrete method for QLearn ---- */
    double Sample(const State& state,
                  const Action& action,
                  State& nextState)
    {
        stepsPerformed++;
        //perform actions
        int joint_num = (int)action.action / action.joint_actions;
        int action_type = (int)action.action % action.joint_actions;

        if (action.action != Action::actions::no_action)
        {
            if(action_type == 0)
                if (training_world->obj4_joints[joint_num]->GetMotorSpeed() > -4.0)
                    training_world->obj4_joints[joint_num]->SetMotorSpeed(-2.0f);
                    //training_world->obj4_joints[joint_num]->SetMotorSpeed(training_world->obj4_joints[joint_num]->GetMotorSpeed() - DISCRETE_SPPED_GAIN);
            if (action_type == 1)
                training_world->obj4_joints[joint_num]->SetMotorSpeed(0.0f);
            if (action_type == 2)
                if (training_world->obj4_joints[joint_num]->GetMotorSpeed() < 4.0)
                    training_world->obj4_joints[joint_num]->SetMotorSpeed(2.0f);
                    //training_world->obj4_joints[joint_num]->SetMotorSpeed(training_world->obj4_joints[joint_num]->GetMotorSpeed() + DISCRETE_SPPED_GAIN);
        }
        //simlate one training_world step
        training_world->CheckAngleLimits();
        training_world->TimeStep();
        training_window->HandleBasicEvent();
        
        //fill next step data
        nextState.LinearVelocity() = training_world->obj4_parts[2]->GetLinearVelocity().x;
        nextState.verticalVelocity() = training_world->obj4_parts[2]->GetLinearVelocity().y;
        for (int i = 0; i < training_world->obj4_num_parts; i++)
        {
            nextState.Angle(i) = (double)training_world->obj4_parts[i]->GetAngle();
            nextState.AngularVelocity(i) = (double)training_world->obj4_parts[i]->GetAngularVelocity();
        }
        nextState.MoveDirection() = state.MoveDirection();
        //*******2dir
        //@BIAS
        nextState.Bias() = state.Bias();

        if (training_window->window_pause_flag == false)
        {
            training_window->fitness = fitness;
            training_window->SetOffset(training_world->obj4_parts[2]);
            training_window->DrawModel(training_window->current_OB_draw_struct);
            training_window->DrawWindow();
            training_window->Display();
        }

        if (training_window->realtime_view_flag == true && training_window->window_pause_flag == false)
        {
            current = std::clock();
            last_eval_time = current - previous;
            previous = current;
            while ((current - previous) < ((std::clock_t)MS_PER_UPDATE - last_eval_time))
            {
                current = std::clock();
            }
            previous = std::clock();
        }

        // Check if the episode has terminated.
        bool terminal = IsTerminal(nextState);
        if (terminal && maxSteps != 0 && stepsPerformed >= maxSteps) //all steps performed
        {
            if (training_window->realtime_view_flag == true && training_window->window_pause_flag == false)
                std::cout << "total real time elapsed: " << (std::clock() - total_eval_time) / 1000.0 << " sec." << std::endl;
            return 0;
        }  
        else if (terminal) //penalty for falling over
            return -10;

        //**********2dir 
        //------ reward system with training in two directions -------------
        if (perform_dir_train) {
            if (randomness[0] == (int)stepsPerformed) {
                nextState.MoveDirection() = (double)randomness[2];
                dir_change_pos = training_world->obj4_parts[2]->GetPosition().x;
            }
            if (randomness[1] == (int)stepsPerformed) {
                nextState.MoveDirection() = (double)randomness[3];
                dir_change_pos = training_world->obj4_parts[2]->GetPosition().x;
            }
            if (write_to_screen_twoway && (randomness[1] == (int)stepsPerformed || randomness[0] == (int)stepsPerformed))
                std::cout << "prev_dir:" << ((state.MoveDirection() < 0.0) ? " " : "  ") << state.MoveDirection()
                << " next_dir:" << ((nextState.MoveDirection() < 0.0) ? " " : "  ") << nextState.MoveDirection() << std::endl;
        }
        // ----------------------------------------------------------------
        double reward = 0.0;
        if (state.MoveDirection() == 0.0) {
            if (std::abs(nextState.LinearVelocity()) < 0.05)
                reward = 1.0;
            if (std::abs(nextState.LinearVelocity()) < 0.005)
                reward = 5.0;
        }
        else {
            if ((nextState.LinearVelocity() > 0.05 && nextState.MoveDirection() == 1.0) ||
                (nextState.LinearVelocity() < -0.05 && nextState.MoveDirection() == -1.0))
                reward = 1.0;
            if (((training_world->obj4_parts[2]->GetPosition().x - dir_change_pos) > final_distance && nextState.MoveDirection() == 1.0) ||
                ((training_world->obj4_parts[2]->GetPosition().x - dir_change_pos) < -final_distance && nextState.MoveDirection() == -1.0))
                reward = 5.0;
        }
        //reward = training_world->obj4_parts[2]->GetPosition().x;
        fitness += reward;
        return reward;
    }

    double Sample(const State& state, const Action& action)
    {
        State nextState;
        return Sample(state, action, nextState);
    }

    /* ---- continious method for NEAT and GA ---- */
    double SampleContinious(const State& state,
        const ContiniousAction& action,
        State& nextState)
    {
        // Update the number of steps performed.
        stepsPerformed++;

        //perform actions
        for (int i = 0; i < action.size; i++)
        {
            if (output_type == 2) //ga
                training_world->obj4_joints[i]->SetMotorSpeed((float)action.action[i]);
            if (output_type == 1) //neat
            {
                if ((((action.action[i] - 0.5) > 0.0) && training_world->obj4_joints[i]->GetMotorSpeed() < 4.0f) ||
                    (((action.action[i] - 0.5) < 0.0) && training_world->obj4_joints[i]->GetMotorSpeed() > -4.0f))
                    training_world->obj4_joints[i]->SetMotorSpeed(training_world->obj4_joints[i]->GetMotorSpeed() + ((float)action.action[i] - 0.5f));
                training_world->obj4_joints[i]->SetMaxMotorTorque(std::abs((float)action.action[i] - 0.5f) * 40.f + 20.f);
            }
        }
         //simlate one training_world step
        training_world->CheckAngleLimits();
        training_world->TimeStep();
        training_window->HandleBasicEvent();

        //fill next step data
        nextState.LinearVelocity() = training_world->obj4_parts[2]->GetLinearVelocity().x;
        nextState.verticalVelocity() = training_world->obj4_parts[2]->GetLinearVelocity().y;
        for (int i = 0; i < training_world->obj4_num_parts; i++)
        {
            nextState.Angle(i) = (double)training_world->obj4_parts[i]->GetAngle();
            nextState.AngularVelocity(i) = (double)training_world->obj4_parts[i]->GetAngularVelocity();
        }
        nextState.MoveDirection() = state.MoveDirection();
        //***************2dir
        //@BIAS
        nextState.Bias() = state.Bias();

        if (training_window->window_pause_flag == false)
        {
            training_window->SetOffset(training_world->obj4_parts[2]);
            training_window->DrawModel(training_window->current_OB_draw_struct);
            training_window->DrawWindow();
            training_window->Display();
        }

        if (training_window->realtime_view_flag == true && training_window->window_pause_flag == false)
        {
            current = std::clock();
            last_eval_time = current - previous;
            previous = current;
            while ((current - previous) < ((std::clock_t)MS_PER_UPDATE - last_eval_time))
            {
                current = std::clock();
            }
            previous = std::clock();
        }

        // Check if the episode has terminated.
        bool terminal = IsTerminal(nextState);
        if (terminal && maxSteps != 0 && stepsPerformed >= maxSteps)
        {
            if (training_window->realtime_view_flag == true && training_window->window_pause_flag == false)
                std::cout << "total real time elapsed: " << (std::clock() - total_eval_time) / 1000.0 << " sec." << std::endl;
        }
        else if (terminal)
            return -1000.0 * (1.0 - ((double)stepsPerformed/(double)maxSteps)); //penalty
        //return training_world->obj4_parts[2]->GetPosition().x * 500.0 + 50.0;      //reward bsed on final destination reached
    
        //*********2dir
        double return_val = 0.0;
        if (output_type == 1)
        {
            //------ reward system with training in two directions -------------
            if (perform_dir_train) {
                if (randomness[0] == (int)stepsPerformed || randomness[1] == (int)stepsPerformed) {
                    if (randomness[0] == (int)stepsPerformed)
                        nextState.MoveDirection() = (double)randomness[2];
                    else
                        nextState.MoveDirection() = (double)randomness[3];
                    if (state.MoveDirection() != 0.0)
                        fit_in_dir_change += state.MoveDirection() * ((training_world->obj4_parts[2]->GetPosition().x - dir_change_pos) * 500.0);
                    else
                        fit_in_dir_change += -std::abs((training_world->obj4_parts[2]->GetPosition().x - dir_change_pos) * 500.0);
                    dir_change_pos = training_world->obj4_parts[2]->GetPosition().x;
                    if (write_to_screen_twoway)
                        std::cout << "prev_dir:" << ((state.MoveDirection() < 0.0) ? " " : "  ") << state.MoveDirection()
                        << " next_dir:" << ((nextState.MoveDirection() < 0.0) ? " " : "  ") << nextState.MoveDirection()
                        << " fit_in_change: " << fit_in_dir_change << std::endl;
                }
            }
            if (nextState.MoveDirection() != 0.0)
                return_val = fit_in_dir_change + (nextState.MoveDirection()
                    * ((training_world->obj4_parts[2]->GetPosition().x - dir_change_pos) * 500.0));
            else
                return_val = fit_in_dir_change + (-std::abs((training_world->obj4_parts[2]->GetPosition().x - dir_change_pos) * 500.0));
            // ----------------------------------------------------------------
        }
        else
            return_val = (double)(training_world->obj4_parts[2]->GetPosition().x * 500.f + 50.f);

        return return_val;      //reward based on final destination reached
    }

    double SampleContinious(const State& state, const ContiniousAction& action)
    {
        State nextState;
        return SampleContinious(state, action, nextState);
    }

    /* ---- Initial sample common for all cases ---- */
    State InitialSample()
    {
        State initial_data;
        stepsPerformed = 0;
        fitness = 0.0;
        dir_change_pos = 0.0f;
        fit_in_dir_change = 0.0;

        training_world->resetMechanism();

        initial_data.LinearVelocity() = training_world->obj4_parts[2]->GetLinearVelocity().x;
        initial_data.verticalVelocity() = training_world->obj4_parts[2]->GetLinearVelocity().y;
        for (int i = 0; i < training_world->obj4_num_parts; i++)
        {
            initial_data.Angle(i) = (double)training_world->obj4_parts[i]->GetAngle();
            initial_data.AngularVelocity(i) = (double)training_world->obj4_parts[i]->GetAngularVelocity();
        }
        //initial_data.MoveDirection() = 1.0;
        //******2dir
        if (perform_dir_train)
        {
            randomness[0] = (int)maxSteps / 3;
            randomness[1] = ((int)maxSteps * 2) / 3;
            randomness[2] = 0;
            randomness[3] = -1;
            initial_data.MoveDirection() = 1.0;   //początkowy kierunek
        }
        else
            initial_data.MoveDirection() = 1.0;
        //@BIAS
        initial_data.Bias() = 1.0;

        total_eval_time = std::clock();
        previous = std::clock();

        return initial_data;
    }

    /* ---- Termination check ---- */
    bool IsTerminal(const State& state) const
    {
        if (maxSteps != 0 && stepsPerformed >= maxSteps)
            return true;

        if (std::abs(state.Angle(2)) > (thetaThresholdRadians*10.0))
            return true;

        return false;
    }

    //! Get the number of steps performed.
    size_t StepsPerformed() const { return stepsPerformed; }

    size_t MaxSteps() const { return maxSteps; }
    size_t& MaxSteps() { return maxSteps; }

    //box2d training_world
    PhysicsWorld* training_world = nullptr;

    //aditional window
    SimWindow* training_window = nullptr;

private:
    size_t maxSteps;
    double doneReward;
    size_t stepsPerformed;

    bool perform_dir_train;
    bool write_to_screen_twoway;
    int randomness[4];
    double fit_in_dir_change;
    float dir_change_pos;

    int output_type;
    double fitness;
    double thetaThresholdRadians;
    double final_distance;
    std::clock_t previous;
    std::clock_t current;
    std::clock_t total_eval_time;
    std::clock_t last_eval_time;
};
}
}

#endif
