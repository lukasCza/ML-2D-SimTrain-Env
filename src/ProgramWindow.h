#pragma once
#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include <iostream>
#include <fstream>
#include "imgui.h"
#include "imgui-SFML.h"
#include "PhysicsEngine.h"
#include "Parameters_class.h"

#define SCREEN_WIDTH 1280U																// window initial width
#define SCREEN_HEIGHT 720U																// window initial height
#define SCREEN_MIDDLE_POS_X ((float)SCREEN_WIDTH/2.f)									// center x position for box2d world center
#define SCREEN_MIDDLE_POS_Y (((float)SCREEN_HEIGHT/2.f)+((float)SCREEN_HEIGHT/2.f)/2.f)	// center y position for box2d world center
#define OBJECTS_SCALE 200.f																// initial scale of objects on screen
#define FONT_SIZE_PX 15.f
#define MIN_SCREEN_SCALE 0.6f
#define MAX_SCREEN_SCALE 1.5f
#define MYCOLOR sf::Color(245,110,10,196)

inline float radiansToDegrees(float radians) {return radians * (180.f / b2_pi);}

//for bonus function
struct model_draw_struct
{
	model_draw_struct(void* element, sf::Shape* shape, sf::Color* color, bool is_body) :
		element(element), shape(shape), color(color), is_body(is_body){}
	void* element;
	sf::Shape* shape;
	sf::Color* color;
	bool is_body;
};

class SimWindow
{
public:
	SimWindow();
	~SimWindow();
	void Init(sf::String window_title, bool training_window_mode = false);
	void Close();
	void HandleEvents(bool* ProgramRunFlag, sf::Clock* DeltaClock);
	void HandleBasicEvent();
	void DrawObjects(b2Body* body, sf::Shape* shape, sf::Color* color = nullptr);
	void DrawObjects(b2RevoluteJoint* joint, sf::Shape* shape, sf::Color* color);
	void SetOffset(b2Body* body);
	void ChangeTheme(bool update_custom_colors = false);
	void DrawWindow();
	void Display();
	//bonus function for lees code in drawing models (might increase draw time)
	void DrawModel(std::vector<model_draw_struct>& draw_struct);

	//color definitons
	sf::Color backgroundColor;
	sf::Color backElementsColor;
	sf::Color frontElementsColor;
	sf::Color outlinesColor;
	sf::Color transparentColor;
	sf::Color myColor = MYCOLOR;

	//ground obstacle
	sf::RectangleShape sf_obstacle;

	sf::RectangleShape sf_testbox;

	//-- model 1,2,6 elements--//
	sf::ConvexShape sf_ob1_up_leg;
	sf::ConvexShape sf_ob1_low_leg;
	sf::ConvexShape sf_ob1_torso;
	sf::CircleShape sf_ob1_joint1;
	sf::CircleShape sf_ob1_joint2;
	sf::CircleShape sf_ob1_circle;
	sf::ConvexShape sf_ob6_torso;
	sf::ConvexShape sf_ob6_head;

	//-- model 4,5 elements--//
	sf::ConvexShape sf_ob4_cellshape;
	sf::CircleShape sf_ob4_joints;
	sf::ConvexShape sf_ob5_cellshape;
	sf::CircleShape sf_ob5_joints;

	//-- model 3 elements--//
	sf::ConvexShape sf_ob3_feet;
	sf::ConvexShape sf_ob3_low_leg;
	sf::ConvexShape sf_ob3_up_leg;
	sf::ConvexShape sf_ob3_torso;
	sf::ConvexShape sf_ob3_low_arm;
	sf::ConvexShape sf_ob3_up_arm;
	sf::ConvexShape sf_ob3_head;
	sf::CircleShape sf_ob3_feet_joint;
	sf::CircleShape sf_ob3_knee_joint;
	sf::CircleShape sf_ob3_hip_joint;
	sf::CircleShape sf_ob3_arm_joint;
	sf::CircleShape sf_ob3_elbow_joint;
	
	//some container
	std::vector<model_draw_struct> current_OB_draw_struct;

	// -- ImGui --
	bool valueofbutton1 = false;
	bool valueofbutton2 = false;
	bool valueofbutton3 = false;
	bool valueofbutton4 = false;
	bool valueofbutton5 = false;	//load trained model
	bool valueofbutton6 = false;	//restart
	bool valueofbutton7 = false;	//stop -0
	bool valueofbutton8 = false;	//start testing
	bool valueofbutton9 = false;	//stop testing
	bool valueofbutton_direction_left = false;
	bool valueofbutton_direction_right = false;
	bool window_pause_flag = false;
	bool realtime_view_flag = false;
	bool exit_training_flag = false;
	ImGuiWindowFlags flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	ImVec2 window_size; // -unused
	SelectedModel selected_model = SELECTED_MODEL;
	int current_method = 1;
	int current_item = 1;
	bool draw_test_objects = false;
	bool add_obstacles = false;
	int currentTheme = 0;
	float colors_custom[16];

	trainParameters::GA_Params GA_parameters;
	trainParameters::NEAT_Params NEAT_parameters;
	trainParameters::QL_Params QL_parameters;
	unsigned int generation;
	int organism_num;
	double fitness;
	double best_fitness = 0.0;

private:
	sf::RenderWindow window;
	sf::Event event;
	sf::Font font;
	sf::Text text;
	float window_objects_scale = 1.0f;		//default scale 1:1
	float models_friction = MODELS_FRICTION;
	float window_offset = 0.0f;
	bool training_window_mode = false;
	//ground line vertex
	sf::Vertex line[2];
};
