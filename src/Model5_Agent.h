#pragma once

/***********************************
commented version of agent is in Model4_Agent
************************************/

#ifndef MLPACK_METHODS_RL_ENVIRONMENT_MYMODEL5_HPP
#define MLPACK_METHODS_RL_ENVIRONMENT_MYMODEL5_HPP

#include <mlpack/prereqs.hpp>
#include <iostream>
#include <ctime>
#include "PhysicsEngine.h"
#include "ProgramWindow.h"

namespace mlpack {
    namespace rl {

        class Model5_Agent
        {
        public:
            class State
            {
            public:
                State() : data(dimension) {}
                State(const arma::colvec& data) : data(data) {}

                arma::colvec Data() const { return data; }
                arma::colvec& Data() { return data; }

                //! i range: 0->12
                double LinearVelocity() const { return data[0]; }
                double& LinearVelocity() { return data[0]; }
                double verticalVelocity() const { return data[1]; }
                double& verticalVelocity() { return data[1]; }
                double Angle(const size_t i) const { return data[i * 2 + 2]; }
                double& Angle(const size_t i) { return data[i * 2 + 2]; }
                double AngularVelocity(const size_t i) const { return data[i * 2 + 3]; }
                double& AngularVelocity(const size_t i) { return data[i * 2 + 3]; }
                double MoveDirection() const { return data[28]; }
                double& MoveDirection() { return data[28]; }

                //@BIAS
                double Bias() const { return data[29]; }
                double& Bias() { return data[29]; }

                const arma::colvec& Encode() const { return data; }
                static constexpr size_t dimension = 30;
            private:
                arma::colvec data;
            };

            class Action
            {
            public:
                enum actions
                {
                    rotate_backward_joint1 = 0, stop_joint1, rotate_forward_joint1,
                    rotate_backward_joint2, stop_joint2, rotate_forward_joint2,
                    rotate_backward_joint3, stop_joint3, rotate_forward_joint3,
                    rotate_backward_joint4, stop_joint4, rotate_forward_joint4,
                    rotate_backward_joint5, stop_joint5, rotate_forward_joint5,
                    rotate_backward_joint6, stop_joint6, rotate_forward_joint6,
                    rotate_backward_joint7, stop_joint7, rotate_forward_joint7,
                    rotate_backward_joint8, stop_joint8, rotate_forward_joint8,
                    rotate_backward_joint9, stop_joint9, rotate_forward_joint9,
                    rotate_backward_joint10, stop_joint10, rotate_forward_joint10,
                    rotate_backward_joint11, stop_joint11, rotate_forward_joint11,
                    rotate_backward_joint12, stop_joint12, rotate_forward_joint12,
                    no_action
                };
                Action::actions action;
                static const size_t size = 37;
                static const int joint_actions = 3;
            };

            class ContiniousAction
            {
            public:
                std::vector<double> action;
                static constexpr size_t size = 12;

                ContiniousAction() : action(Model5_Agent::ContiniousAction::size) {}
            };
            Model5_Agent(PhysicsWorld* training_world,
                SimWindow* training_window,
                int output_type,
                const size_t maxsteps = 100,
                bool dir_train = false,
                bool write_twoway = false,
                const double thetaThresholdRadians = 10,
                const double final_distance = 1.0,
                const double doneReward = 0.0) :
                training_world(training_world),
                training_window(training_window),
                output_type(output_type),
                maxSteps(maxsteps),
                thetaThresholdRadians(thetaThresholdRadians),
                final_distance(final_distance),
                doneReward(doneReward),
                perform_dir_train(dir_train),
                write_to_screen_twoway(write_twoway),
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
                    if (action_type == 0)
                        if (training_world->obj5_joints[joint_num]->GetMotorSpeed() > -4.0)
                            training_world->obj5_joints[joint_num]->SetMotorSpeed(-2.0f);
                    //training_world->obj5_joints[joint_num]->SetMotorSpeed(training_world->obj5_joints[joint_num]->GetMotorSpeed() - DISCRETE_SPPED_GAIN);
                    if (action_type == 1)
                        training_world->obj5_joints[joint_num]->SetMotorSpeed(0.0f);
                    if (action_type == 2)
                        if (training_world->obj5_joints[joint_num]->GetMotorSpeed() < 4.0)
                            training_world->obj5_joints[joint_num]->SetMotorSpeed(2.0f);
                    //training_world->obj5_joints[joint_num]->SetMotorSpeed(training_world->obj5_joints[joint_num]->GetMotorSpeed() + DISCRETE_SPPED_GAIN);
                }
                //simlate one training_world step
                training_world->CheckAngleLimits();
                training_world->TimeStep();
                training_window->HandleBasicEvent();

                //fill next step data
                nextState.LinearVelocity() = (double)training_world->obj5_parts[6]->GetLinearVelocity().x;
                nextState.verticalVelocity() = (double)training_world->obj5_parts[6]->GetLinearVelocity().y;
                for (int i = 0; i < training_world->obj5_num_parts; i++)
                {
                    nextState.Angle(i) = (double)training_world->obj5_parts[i]->GetAngle();
                    nextState.AngularVelocity(i) = (double)training_world->obj5_parts[i]->GetAngularVelocity();
                }
                nextState.MoveDirection() = state.MoveDirection();
                //@BIAS
                nextState.Bias() = state.Bias();

                if (training_window->window_pause_flag == false)
                {
                    training_window->fitness = fitness;
                    training_window->SetOffset(training_world->obj5_parts[6]);
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

                //reward system
                //------ reward system with training in two directions -------------
                if (perform_dir_train) {
                    if (randomness[0] == (int)stepsPerformed) {
                        nextState.MoveDirection() = (double)randomness[2];
                        dir_change_pos = training_world->obj5_parts[6]->GetPosition().x;
                    }
                    if (randomness[1] == (int)stepsPerformed) {
                        nextState.MoveDirection() = (double)randomness[3];
                        dir_change_pos = training_world->obj5_parts[6]->GetPosition().x;
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
                    if (((training_world->obj5_parts[6]->GetPosition().x - dir_change_pos) > final_distance && nextState.MoveDirection() == 1.0) ||
                        ((training_world->obj5_parts[6]->GetPosition().x - dir_change_pos) < -final_distance && nextState.MoveDirection() == -1.0))
                        reward = 5.0;
                }

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
                        training_world->obj5_joints[i]->SetMotorSpeed((float)action.action[i]);
                    if (output_type == 1) //neat
                    {
                        if ((((action.action[i] - 0.5) > 0.0) && training_world->obj5_joints[i]->GetMotorSpeed() < 4.0f) ||
                            (((action.action[i] - 0.5) < 0.0) && training_world->obj5_joints[i]->GetMotorSpeed() > -4.0f))
                            training_world->obj5_joints[i]->SetMotorSpeed(training_world->obj5_joints[i]->GetMotorSpeed() + ((float)action.action[i] - 0.5f));
                        training_world->obj5_joints[i]->SetMaxMotorTorque(std::abs((float)action.action[i] - 0.5f) * 100.f + 50.f);
                    }
                }
                //simlate one training_world step
                training_world->CheckAngleLimits();
                training_world->TimeStep();
                training_window->HandleBasicEvent();

                //fill next step data
                nextState.LinearVelocity() = (double)training_world->obj5_parts[6]->GetLinearVelocity().x;
                nextState.verticalVelocity() = (double)training_world->obj5_parts[6]->GetLinearVelocity().y;
                for (int i = 0; i < training_world->obj5_num_parts; i++)
                {
                    nextState.Angle(i) = (double)training_world->obj5_parts[i]->GetAngle();
                    nextState.AngularVelocity(i) = (double)training_world->obj5_parts[i]->GetAngularVelocity();
                }
                nextState.MoveDirection() = state.MoveDirection();
                //@BIAS
                nextState.Bias() = state.Bias();

                if (training_window->window_pause_flag == false)
                {
                    training_window->SetOffset(training_world->obj5_parts[6]);
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

                double return_val = 0.0;
                if (output_type == 1)
                {
                    //------ reward system with training in two directions -------------
                    if (perform_dir_train) {
                        if (randomness[0] == (int)stepsPerformed || randomness[1] == (int)stepsPerformed) {
                            if(randomness[0] == (int)stepsPerformed)
                                nextState.MoveDirection() = (double)randomness[2];
                            else
                                nextState.MoveDirection() = (double)randomness[3];
                            if (state.MoveDirection() != 0.0)
                                fit_in_dir_change += state.MoveDirection() * ((training_world->obj5_parts[6]->GetPosition().x - dir_change_pos) * 500.0);
                            else
                                fit_in_dir_change += -std::abs((training_world->obj5_parts[6]->GetPosition().x - dir_change_pos) * 500.0);
                            dir_change_pos = training_world->obj5_parts[6]->GetPosition().x;
                            if (write_to_screen_twoway)
                                std::cout << "prev_dir:" << ((state.MoveDirection() < 0.0) ? " " : "  ") << state.MoveDirection()
                                << " next_dir:" << ((nextState.MoveDirection() < 0.0) ? " " : "  ") << nextState.MoveDirection()
                                << " fit_in_change: " << fit_in_dir_change << std::endl;
                        }
                    }
                    if (nextState.MoveDirection() != 0.0)
                        return_val = fit_in_dir_change + (nextState.MoveDirection()
                            * ((training_world->obj5_parts[6]->GetPosition().x - dir_change_pos) * 500.0));
                    else
                        return_val = fit_in_dir_change + (-std::abs((training_world->obj5_parts[6]->GetPosition().x - dir_change_pos) * 500.0));
                    // ----------------------------------------------------------------
                }
                else
                    return_val = (double)(training_world->obj5_parts[6]->GetPosition().x * 500.f + 50.f);

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

                initial_data.LinearVelocity() = (double)training_world->obj5_parts[6]->GetLinearVelocity().x;
                initial_data.verticalVelocity() = (double)training_world->obj5_parts[6]->GetLinearVelocity().y;
                for (int i = 0; i < training_world->obj5_num_parts; i++)
                {
                    initial_data.Angle(i) = (double)training_world->obj5_parts[i]->GetAngle();
                    initial_data.AngularVelocity(i) = (double)training_world->obj5_parts[i]->GetAngularVelocity();
                }
                if (perform_dir_train)
                {
                    randomness[0] = (int)maxSteps / 3;
                    randomness[1] = ((int)maxSteps * 2) / 3;
                    randomness[2] = 0;
                    randomness[3] = -1;
                    initial_data.MoveDirection() = 1.0;   //pocz¹tkowy kierunek
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
                if (std::abs(state.Angle(6)) > thetaThresholdRadians)
                    return true;

                return false;
            }

            size_t StepsPerformed() const { return stepsPerformed; }
            size_t MaxSteps() const { return maxSteps; }
            size_t& MaxSteps() { return maxSteps; }

            PhysicsWorld* training_world = nullptr;
            SimWindow* training_window = nullptr;

        private:
            size_t maxSteps;
            double doneReward;
            size_t stepsPerformed;

            bool perform_dir_train;
            bool write_to_screen_twoway;

            int randomness[4];  
            int output_type;
            double fitness;
            double fit_in_dir_change;
            float dir_change_pos;
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