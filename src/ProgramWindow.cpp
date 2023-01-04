#include "ProgramWindow.h"

SimWindow::SimWindow(){}
SimWindow::~SimWindow(){}

void SimWindow::Init(sf::String window_title, bool training_window_mode_set)
{
	// Initial config, creating render window
	window.create(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT),window_title);

	//sf::Image icon;
	//icon.loadFromFile("machinelearning.png");
	//window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

	training_window_mode = training_window_mode_set;
	if (training_window_mode_set == false)
	{
		//init
		ImGui::SFML::Init(window, false);
		//font
		ImGuiIO& io = ImGui::GetIO();
		io.Fonts->AddFontFromFileTTF("Cascadia.ttf", FONT_SIZE_PX);
		ImGui::SFML::UpdateFontTexture();
	}
	else
		font.loadFromFile("Cascadia.ttf");

	SimWindow::ChangeTheme();
	transparentColor = sf::Color::Transparent;

	//obstacle box
	sf_obstacle.setSize(sf::Vector2f(40.f*window_objects_scale, 40.f ));
	sf_obstacle.setOutlineColor(outlinesColor);
	sf_obstacle.setOutlineThickness(-1.f);
	sf_obstacle.setOrigin(sf::Vector2f(20.f , 20.f ));

	//ground line
	line[0] = sf::Vertex(sf::Vector2f(0.f, SCREEN_MIDDLE_POS_Y), outlinesColor);
	line[1] = sf::Vertex(sf::Vector2f((float)SCREEN_WIDTH, SCREEN_MIDDLE_POS_Y), outlinesColor);
	
	//test objects
	sf_testbox.setSize(sf::Vector2f(44.f , 44.f ));
	sf_testbox.setOutlineColor(outlinesColor);
	sf_testbox.setOutlineThickness(-1.f);
	sf_testbox.setOrigin(sf::Vector2f(22.f , 22.f ));


	//--------- OBJECT 1,2 ------------//
	//defining shapes

	//0.1 in box 2d  = 10 cm in SFML transition 1 cm = 2 px is used so 0.x in box2d = 200*0.x px in SFML

	// Values in vectors are alredy multiplied by OBJECTS_SCALE
	// mozna by³o zrobic na jednym zestawie wartosci i tworzac objekty w sfml wpisywac dane z wektorów box2d*200
	//joints
	sf_ob1_joint1.setRadius(20.f );
	sf_ob1_joint1.setOrigin(sf::Vector2f(20.f, 20.f));
	sf_ob1_joint2.setRadius(10.f );
	sf_ob1_joint2.setOrigin(sf::Vector2f(10.f, 10.f));
	sf_ob1_circle.setRadius(6.f );
	sf_ob1_circle.setOrigin(sf::Vector2f(6.f, -34.f));

	//body parts
	sf_ob1_up_leg.setPointCount(6);
	sf_ob1_up_leg.setPoint(0, sf::Vector2f(0.f, 0.f));
	sf_ob1_up_leg.setPoint(1, sf::Vector2f(0.f, 40.f));
	sf_ob1_up_leg.setPoint(2, sf::Vector2f(10.f, 80.f));
	sf_ob1_up_leg.setPoint(3, sf::Vector2f(30.f, 80.f));
	sf_ob1_up_leg.setPoint(4, sf::Vector2f(40.f, 40.f));
	sf_ob1_up_leg.setPoint(5, sf::Vector2f(40.f, 0.f));
	sf_ob1_up_leg.setOrigin(sf::Vector2f(20.f, 40.f));

	sf_ob1_low_leg.setPointCount(4);
	sf_ob1_low_leg.setPoint(0, sf::Vector2f(0.f, 0.f));
	sf_ob1_low_leg.setPoint(1, sf::Vector2f(4.f, 80.f));
	sf_ob1_low_leg.setPoint(2, sf::Vector2f(16.f, 80.f));
	sf_ob1_low_leg.setPoint(3, sf::Vector2f(20.f, 0.f));
	sf_ob1_low_leg.setOrigin(sf::Vector2f(10.f, 40.f));

	sf_ob1_torso.setPointCount(8);
	sf_ob1_torso.setPoint(0, sf::Vector2f(40.f, 0.f));
	sf_ob1_torso.setPoint(1, sf::Vector2f(0.f, 20.f));
	sf_ob1_torso.setPoint(2, sf::Vector2f(0.f, 60.f));
	sf_ob1_torso.setPoint(3, sf::Vector2f(20.f, 80.f));
	sf_ob1_torso.setPoint(4, sf::Vector2f(180.f, 80.f));
	sf_ob1_torso.setPoint(5, sf::Vector2f(200.f, 60.f));
	sf_ob1_torso.setPoint(6, sf::Vector2f(200.f, 20.f));
	sf_ob1_torso.setPoint(7, sf::Vector2f(160.f, 0.f));
	sf_ob1_torso.setOrigin(sf::Vector2f(100.f, 40.f));

	sf_ob1_up_leg.setOutlineThickness(-1.f);
	sf_ob1_low_leg.setOutlineThickness(-1.f);
	sf_ob1_torso.setOutlineThickness(-1.f);
	sf_ob1_joint1.setOutlineThickness(-1.f);
	sf_ob1_joint2.setOutlineThickness(-1.f);
	sf_ob1_circle.setOutlineThickness(-1.f);

	sf_ob1_up_leg.setOutlineColor(outlinesColor);
	sf_ob1_low_leg.setOutlineColor(outlinesColor);
	sf_ob1_torso.setOutlineColor(outlinesColor);
	sf_ob1_joint1.setOutlineColor(outlinesColor);
	sf_ob1_joint2.setOutlineColor(outlinesColor);
	sf_ob1_circle.setOutlineColor(outlinesColor);

	sf_ob6_torso.setPointCount(8);
	sf_ob6_torso.setPoint(0, sf::Vector2f(40.f, 0.f));
	sf_ob6_torso.setPoint(1, sf::Vector2f(0.f, 20.f));
	sf_ob6_torso.setPoint(2, sf::Vector2f(0.f, 60.f));
	sf_ob6_torso.setPoint(3, sf::Vector2f(20.f, 80.f));
	sf_ob6_torso.setPoint(4, sf::Vector2f(180.f, 80.f));
	sf_ob6_torso.setPoint(5, sf::Vector2f(200.f, 60.f));
	sf_ob6_torso.setPoint(6, sf::Vector2f(200.f, 15.f));
	sf_ob6_torso.setPoint(7, sf::Vector2f(185.f, 0.f));
	sf_ob6_torso.setOrigin(sf::Vector2f(100.f, 40.f));
	sf_ob6_torso.setOutlineThickness(-1.f);
	sf_ob6_torso.setOutlineColor(outlinesColor);

	sf_ob6_head.setPointCount(7);
	sf_ob6_head.setPoint(0, sf::Vector2f(10.f, 0.f));
	sf_ob6_head.setPoint(1, sf::Vector2f(0.f, 10.f));
	sf_ob6_head.setPoint(2, sf::Vector2f(0.f, 50.f));
	sf_ob6_head.setPoint(3, sf::Vector2f(70.f, 50.f));
	sf_ob6_head.setPoint(4, sf::Vector2f(80.f, 40.f));
	sf_ob6_head.setPoint(5, sf::Vector2f(80.f, 30.f));
	sf_ob6_head.setPoint(6, sf::Vector2f(40.f, 0.f));
	sf_ob6_head.setOrigin(sf::Vector2f(30.f, 30.f));
	sf_ob6_head.setOutlineThickness(-1.f);
	sf_ob6_head.setOutlineColor(outlinesColor);

	//--------- OBJECT 4 ------------//
	sf_ob4_joints.setRadius(10.f);
	sf_ob4_joints.setOrigin(sf::Vector2f(10.f, 10.f));

	sf_ob4_cellshape.setPointCount(8);
	sf_ob4_cellshape.setPoint(0, sf::Vector2f(10.f, 0.f));
	sf_ob4_cellshape.setPoint(1, sf::Vector2f(0.f, 8.f));
	sf_ob4_cellshape.setPoint(2, sf::Vector2f(0.f, 28.f));
	sf_ob4_cellshape.setPoint(3, sf::Vector2f(10.f, 36.f));
	sf_ob4_cellshape.setPoint(4, sf::Vector2f(54.f, 36.f));
	sf_ob4_cellshape.setPoint(5, sf::Vector2f(64.f, 28.f));
	sf_ob4_cellshape.setPoint(6, sf::Vector2f(64.f, 8.f));
	sf_ob4_cellshape.setPoint(7, sf::Vector2f(54.f, 0.f));

	sf_ob4_cellshape.setOrigin(sf::Vector2f(32.f, 18.f));

	sf_ob4_joints.setOutlineThickness(-1.f);
	sf_ob4_cellshape.setOutlineThickness(-1.f);

	sf_ob4_joints.setOutlineColor(outlinesColor);
	sf_ob4_cellshape.setOutlineColor(outlinesColor);

	//--------- OBJECT 5 ------------//
	sf_ob5_joints.setRadius(8.f);
	sf_ob5_joints.setOrigin(sf::Vector2f(8.f, 8.f));

	sf_ob5_cellshape.setPointCount(8);
	sf_ob5_cellshape.setPoint(0, sf::Vector2f(10.f, 0.f));
	sf_ob5_cellshape.setPoint(1, sf::Vector2f(0.f, 6.f));
	sf_ob5_cellshape.setPoint(2, sf::Vector2f(0.f, 22.f));
	sf_ob5_cellshape.setPoint(3, sf::Vector2f(10.f, 28.f));
	sf_ob5_cellshape.setPoint(4, sf::Vector2f(34.f, 28.f));
	sf_ob5_cellshape.setPoint(5, sf::Vector2f(44.f, 22.f));
	sf_ob5_cellshape.setPoint(6, sf::Vector2f(44.f, 6.f));
	sf_ob5_cellshape.setPoint(7, sf::Vector2f(34.f, 0.f));

	sf_ob5_cellshape.setOrigin(sf::Vector2f(22.f, 14.f));

	sf_ob5_joints.setOutlineThickness(-1.f);
	sf_ob5_cellshape.setOutlineThickness(-1.f);

	sf_ob5_joints.setOutlineColor(outlinesColor);
	sf_ob5_cellshape.setOutlineColor(outlinesColor);

	//--------- OBJECT 3 ------------//
	sf_ob3_feet_joint.setRadius(6.f );
	sf_ob3_feet_joint.setOrigin(sf::Vector2f(6.f, 6.f));
	sf_ob3_knee_joint.setRadius(12.f );
	sf_ob3_knee_joint.setOrigin(sf::Vector2f(12.f, 12.f));
	sf_ob3_hip_joint.setRadius(20.f );
	sf_ob3_hip_joint.setOrigin(sf::Vector2f(20.f, 20.f));
	sf_ob3_elbow_joint.setRadius(8.f );
	sf_ob3_elbow_joint.setOrigin(sf::Vector2f(8.f, 8.f));
	
	sf_ob3_feet_joint.setOutlineThickness(-1.f);
	sf_ob3_feet_joint.setOutlineColor(outlinesColor);
	sf_ob3_knee_joint.setOutlineThickness(-1.f);
	sf_ob3_knee_joint.setOutlineColor(outlinesColor);
	sf_ob3_hip_joint.setOutlineThickness(-1.f);
	sf_ob3_hip_joint.setOutlineColor(outlinesColor);
	sf_ob3_elbow_joint.setOutlineThickness(-1.f);
	sf_ob3_elbow_joint.setOutlineColor(outlinesColor);
	sf_ob3_arm_joint = sf_ob3_knee_joint;

	sf_ob3_feet.setPointCount(5);
	sf_ob3_feet.setPoint(0, sf::Vector2f(2.f, 0.f));
	sf_ob3_feet.setPoint(1, sf::Vector2f(0.f, 22.f));	//by³o 20->22
	sf_ob3_feet.setPoint(2, sf::Vector2f(48.f, 22.f));
	sf_ob3_feet.setPoint(3, sf::Vector2f(48.f, 14.f));
	sf_ob3_feet.setPoint(4, sf::Vector2f(14.f, 0.f));
	sf_ob3_feet.setOrigin(sf::Vector2f(8.f, 10.f));
	sf_ob3_feet.setOutlineThickness(-1.f);
	sf_ob3_feet.setOutlineColor(outlinesColor);

	sf_ob3_low_leg.setPointCount(6);
	sf_ob3_low_leg.setPoint(0, sf::Vector2f(0.f, 0.f));
	sf_ob3_low_leg.setPoint(1, sf::Vector2f(0.f, 38.f));
	sf_ob3_low_leg.setPoint(2, sf::Vector2f(6.f, 96.f));
	sf_ob3_low_leg.setPoint(3, sf::Vector2f(18.f, 96.f));
	sf_ob3_low_leg.setPoint(4, sf::Vector2f(24.f, 38.f));
	sf_ob3_low_leg.setPoint(5, sf::Vector2f(24.f, 0.f));
	sf_ob3_low_leg.setOrigin(sf::Vector2f(12.f, 48.f));
	sf_ob3_low_leg.setOutlineThickness(-1.f);
	sf_ob3_low_leg.setOutlineColor(outlinesColor);

	sf_ob3_up_leg.setPointCount(6);
	sf_ob3_up_leg.setPoint(0, sf::Vector2f(0.f, 0.f));
	sf_ob3_up_leg.setPoint(1, sf::Vector2f(0.f, 40.f));
	sf_ob3_up_leg.setPoint(2, sf::Vector2f(8.f, 80.f));
	sf_ob3_up_leg.setPoint(3, sf::Vector2f(32.f, 80.f));
	sf_ob3_up_leg.setPoint(4, sf::Vector2f(40.f, 40.f));
	sf_ob3_up_leg.setPoint(5, sf::Vector2f(40.f, 0.f));
	sf_ob3_up_leg.setOrigin(sf::Vector2f(20.f, 40.f));
	sf_ob3_up_leg.setOutlineThickness(-1.f);
	sf_ob3_up_leg.setOutlineColor(outlinesColor);

	sf_ob3_torso.setPointCount(8);
	sf_ob3_torso.setPoint(0, sf::Vector2f(10.f, 0.f));
	sf_ob3_torso.setPoint(1, sf::Vector2f(0.f, 10.f));
	sf_ob3_torso.setPoint(2, sf::Vector2f(0.f, 150.f));
	sf_ob3_torso.setPoint(3, sf::Vector2f(10.f, 160.f));
	sf_ob3_torso.setPoint(4, sf::Vector2f(50.f, 160.f));
	sf_ob3_torso.setPoint(5, sf::Vector2f(60.f, 150.f));
	sf_ob3_torso.setPoint(6, sf::Vector2f(60.f, 10.f));
	sf_ob3_torso.setPoint(7, sf::Vector2f(50.f, 0.f));
	sf_ob3_torso.setOrigin(sf::Vector2f(30.f, 80.f));
	sf_ob3_torso.setOutlineThickness(-1.f);
	sf_ob3_torso.setOutlineColor(outlinesColor);

	sf_ob3_up_arm.setPointCount(6);
	sf_ob3_up_arm.setPoint(0, sf::Vector2f(0.f, 0.f));
	sf_ob3_up_arm.setPoint(1, sf::Vector2f(0.f, 20.f));
	sf_ob3_up_arm.setPoint(2, sf::Vector2f(4.f, 60.f));
	sf_ob3_up_arm.setPoint(3, sf::Vector2f(20.f, 60.f));
	sf_ob3_up_arm.setPoint(4, sf::Vector2f(24.f, 20.f));
	sf_ob3_up_arm.setPoint(5, sf::Vector2f(24.f, 0.f));
	sf_ob3_up_arm.setOrigin(sf::Vector2f(12.f, 30.f));
	sf_ob3_up_arm.setOutlineThickness(-1.f);
	sf_ob3_up_arm.setOutlineColor(outlinesColor);
	
	sf_ob3_low_arm.setPointCount(4);
	sf_ob3_low_arm.setPoint(0, sf::Vector2f(0.f, 0.f));
	sf_ob3_low_arm.setPoint(1, sf::Vector2f(4.f, 80.f));
	sf_ob3_low_arm.setPoint(2, sf::Vector2f(12.f, 80.f));
	sf_ob3_low_arm.setPoint(3, sf::Vector2f(16.f, 0.f));
	sf_ob3_low_arm.setOrigin(sf::Vector2f(8.f, 40.f));
	sf_ob3_low_arm.setOutlineThickness(-1.f);
	sf_ob3_low_arm.setOutlineColor(outlinesColor);

	sf_ob3_head.setPointCount(8);
	sf_ob3_head.setPoint(0, sf::Vector2f(8.f, 0.f));
	sf_ob3_head.setPoint(1, sf::Vector2f(0.f, 6.f));
	sf_ob3_head.setPoint(2, sf::Vector2f(0.f, 38.f));
	sf_ob3_head.setPoint(3, sf::Vector2f(8.f, 44.f));
	sf_ob3_head.setPoint(4, sf::Vector2f(24.f, 44.f));
	sf_ob3_head.setPoint(5, sf::Vector2f(32.f, 38.f));
	sf_ob3_head.setPoint(6, sf::Vector2f(32.f, 6.f));
	sf_ob3_head.setPoint(7, sf::Vector2f(24.f, 0.f));
	sf_ob3_head.setOrigin(sf::Vector2f(16.f, 22.f));
	sf_ob3_head.setOutlineThickness(-1.f);
	sf_ob3_head.setOutlineColor(outlinesColor);
		// Init END
}

void SimWindow::Close()
{

	window.close();
	if(training_window_mode == false)
		ImGui::SFML::Shutdown();
}

void SimWindow::HandleEvents(bool* ProgramRunFlag, sf::Clock* DeltaClock){
	
	while (window.pollEvent(event))
	{
		ImGui::SFML::ProcessEvent(event);
		
		//all events are handled by ImGui window
		if (event.type == sf::Event::Closed) {
			*ProgramRunFlag = false;
			break;
		}
	}
	ImGui::SFML::Update(window, DeltaClock->restart());
}

void SimWindow::HandleBasicEvent()
{
	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::KeyReleased) {
			if (event.key.code == sf::Keyboard::P)
			{
				window_pause_flag = !window_pause_flag;
				break;
			}
			if (event.key.code == sf::Keyboard::R) 
			{
				realtime_view_flag = !realtime_view_flag;
				break;
			}
			if (event.key.code == sf::Keyboard::Escape)
			{
				exit_training_flag = true;
				break;
			}
		}
		if (event.type == sf::Event::Closed) 
		{
			exit_training_flag = true;
			break;
		}
	}
}

void SimWindow::Display()
{
	if(training_window_mode == false)
		ImGui::SFML::Render(window);

	window.display();
	window.clear(backgroundColor);

	//draw starting line for next frame = allways at the back
	sf::Vertex line_start[] =
	{
		sf::Vertex(sf::Vector2f((SCREEN_MIDDLE_POS_X - (window_offset * window_objects_scale * OBJECTS_SCALE)), 0.f), outlinesColor),
		sf::Vertex(sf::Vector2f((SCREEN_MIDDLE_POS_X - (window_offset * window_objects_scale * OBJECTS_SCALE)), SCREEN_MIDDLE_POS_Y), outlinesColor)
	};
	window.draw(line_start, 2, sf::Lines);
}
void SimWindow::DrawObjects(b2Body* body, sf::Shape* shape, sf::Color* color)
{ 
	sf::Vector2f vect1;
	b2Vec2 position = body->GetPosition();
	float angle = body->GetAngle();
	angle = radiansToDegrees(angle);
	vect1.x = SCREEN_MIDDLE_POS_X + (position.x - window_offset) * (OBJECTS_SCALE * window_objects_scale);
	vect1.y = SCREEN_MIDDLE_POS_Y - (position.y) * (OBJECTS_SCALE * window_objects_scale);
	shape->setPosition(vect1);
	shape->setRotation(-angle);
	shape->setFillColor(*color);
	window.draw(*shape);
}
void SimWindow::DrawObjects(b2RevoluteJoint* joint, sf::Shape* shape, sf::Color* color)
{
	sf::Vector2f vect1;
	b2Vec2 position = joint->GetAnchorA();
	vect1.x = SCREEN_MIDDLE_POS_X + (position.x - window_offset) * (OBJECTS_SCALE * window_objects_scale);
	vect1.y = SCREEN_MIDDLE_POS_Y - (position.y) * (OBJECTS_SCALE * window_objects_scale);
	shape->setPosition(vect1);
	shape->setRotation(0.f);
	shape->setFillColor(*color);
	window.draw(*shape);
}

void SimWindow::SetOffset(b2Body* body)
{
	// Set body based on which background will move when object moves close to window border

	float position_ov_x = body->GetPosition().x;
	if (position_ov_x > (2.0f * (1.f / window_objects_scale)))
		window_offset = position_ov_x - (2.0f * (1.f / window_objects_scale));
	else if (position_ov_x < (-2.0f * (1.f / window_objects_scale)))
		window_offset = position_ov_x + (2.0f * (1.f / window_objects_scale));
	else
		window_offset = 0.0f;
}

void SimWindow::DrawWindow()
{
	//draw all background objects and ImGui
	//draw ground line
	window.draw(line, 2, sf::Lines);

	//draw ground angled lines
	float window_ground_offset = fmod(window_offset, 1.0f);
	for (float i = -200.f ; i < 1500.f; i += 50.f * window_objects_scale)
	{
		sf::Vertex line2[] =
		{
			sf::Vertex(sf::Vector2f((0.f + i - window_ground_offset * OBJECTS_SCALE), SCREEN_MIDDLE_POS_Y), outlinesColor),
			sf::Vertex(sf::Vector2f((50.f + i - window_ground_offset * OBJECTS_SCALE), SCREEN_MIDDLE_POS_Y + 40.f), outlinesColor)
		};
		window.draw(line2, 2, sf::Lines);
	}

	//display help text if training window mode is on
	if (training_window_mode == true)
	{
		text.setFont(font);
		text.setString("ESC/Close window - exit training \nP - disable/enable simulation window\nR - disable/enable simulation in near real time");
		text.setCharacterSize(14);
		text.setPosition(20.f, 20.f);
		window.draw(text);
		char display_state[100];
		sprintf_s(display_state, "Generation no.: %d\nOrganism no.:   %d\nFitness:        %.2f\nBest Fitness:   %.2f", generation, organism_num + 1, fitness, best_fitness);
		text.setString(display_state);
		text.setPosition(20.f, 80.f);
		window.draw(text);
	}
	
	if (training_window_mode == false)
	{
		//----------------------------------------- ImGui ----------------------------------------------------
		//window settings
		ImGui::Begin("Settings Window",(bool*)0, flags);
		ImGui::SetWindowPos(ImVec2(30.f, 30.f));
		ImGui::SetWindowSize(ImVec2(344.f, 435.f));
		window_size = ImGui::GetWindowSize();

		//environment settings
		const char* items[] = { "Model 1 (4 legged mechanism)",
								"Model 2 (2 legged mechanism)" ,
								"Model 3 (human mechanism)" ,
								"Model 4 (snake mechanism)",
								"Model 5 (long snake mech.)",
								"Model 6 (model 2 version B)"};
		const char* Themes[] = { "Dark mode", "Light mode", "Custom mode" };

		ImGui::Text("Environment settings:");
		//theme settings
		ImGui::SameLine(291.f);
		if (ImGui::Button("Theme", ImVec2(45.f, 20.f)))
			ImGui::OpenPopup("Theme");
		bool Custom_change = false;
		if (ImGui::BeginPopup("Theme", flags))
		{
			if (ImGui::ListBox("\n\n", &currentTheme, Themes, IM_ARRAYSIZE(Themes), IM_ARRAYSIZE(Themes)))
			{
				//change colors
				SimWindow::ChangeTheme();

				if (currentTheme == 2)
					ImGui::OpenPopup("colors");
				else
					ImGui::CloseCurrentPopup();
			}
			if (ImGui::BeginPopup("colors", flags))
			{
				ImGui::ColorEdit3("background color", &colors_custom[0]);
				ImGui::ColorEdit3("back elements color", &colors_custom[3]);
				ImGui::ColorEdit3("front elements color", &colors_custom[6]);
				ImGui::ColorEdit3("custom color", &colors_custom[13]);
				ImGui::ColorEdit4("outlines color", &colors_custom[9]);
				if (ImGui::Button("Apply", ImVec2(60.f, 25.f))) {
					Custom_change = true;
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}
			if(Custom_change)
				ImGui::CloseCurrentPopup();
			ImGui::EndPopup();
		}
		if (Custom_change)
			SimWindow::ChangeTheme(Custom_change);
		//end of theme settings

		ImGui::Separator();
		ImGui::Combo(" Model", &current_item, items, IM_ARRAYSIZE(items));		//"Select Model"
		selected_model = (SelectedModel)(current_item);
		ImGui::SliderFloat(" friction", &models_friction, 0.01, 1.0, "%.2f");
		GA_parameters.model_friction = models_friction;
		QL_parameters.model_friction = models_friction;
		NEAT_parameters.model_friction = models_friction;
		ImGui::SliderFloat(" scale", &window_objects_scale, MIN_SCREEN_SCALE, MAX_SCREEN_SCALE, "%.2f x");
		if(ImGui::Checkbox("test_objects", &draw_test_objects))
			ImGui::OpenPopup("reset to apply");
		ImGui::SameLine(150.f);
		if(ImGui::Checkbox("obstacles", &add_obstacles))
			ImGui::OpenPopup("reset to apply");
		if (ImGui::BeginPopup("reset to apply"))
		{
			ImGui::Text("Reset to apply");
			ImGui::EndPopup();
		}

		//learning settings
		ImGui::Separator();
		ImGui::Text("Training settings:");
		ImGui::Separator();
		//QLearn
		if(ImGui::Button("Start Q-Learn", ImVec2(160.f, 25.f)))
			ImGui::OpenPopup("Qlearn_settings");
		if (ImGui::BeginPopup("Qlearn_settings"))
		{
			ImGui::Text("Q-learn settings:");
			ImGui::SameLine(375.f);
			if (ImGui::Button("help", ImVec2(80.f, 19.f)))
				ImGui::OpenPopup("Qlearn_help1");
			if (ImGui::BeginPopup("Qlearn_help1"))
			{
				ImGui::Text("single_eval_steps - single simulation number of steps 100 step = 1sec");
				ImGui::Text("num_episodes - Number of episodes played");
				ImGui::Text("target_reward - the target reward required to complete the algorithm ");
				ImGui::Text("num_hidden_layers - Neural Network settings\n num_neurons_in_layer - each layer num of neurons.\nFirst value is input layer connections");
				ImGui::Text("Final hidden layer shouldn\'t be less than:\n - 25 for Model 1\n - 13 for Model 2/4\n - 34 for Model 3 ");

				if (ImGui::Button("back", ImVec2(80.f, 19.f)))
					ImGui::CloseCurrentPopup();
				ImGui::EndPopup();
			}
			ImGui::Separator();
			ImGui::InputInt("single_eval_steps", &QL_parameters.single_eval_steps);
			ImGui::InputInt("num_episodes", &QL_parameters.num_episodes, 100, 1000);
			ImGui::InputDouble("target_reward", &QL_parameters.target_reward, 10.0, 100.0, "%.1f");
			ImGui::InputInt("num_hidden_layers", &QL_parameters.num_hidden_layers);
			//check now to avoid flashing of diffrent setup
			QL_parameters.num_hidden_layers = (QL_parameters.num_hidden_layers < 1) ? 1 : QL_parameters.num_hidden_layers;
			QL_parameters.num_hidden_layers = (QL_parameters.num_hidden_layers > 10) ? 10 : QL_parameters.num_hidden_layers;
			//--
			ImGui::InputInt("input_layer", &QL_parameters.num_neurons_in_layer[0]);
			if (QL_parameters.num_hidden_layers < 6)
			{
				ImGui::InputScalarN("num_neurons_in_layer", ImGuiDataType_S32, &QL_parameters.num_neurons_in_layer[1], QL_parameters.num_hidden_layers);
				ImGui::Text("-");
			}
			else
			{
				ImGui::InputScalarN("num_neurons_in_layer[1-5]", ImGuiDataType_S32, &QL_parameters.num_neurons_in_layer[1], 5);
				ImGui::InputScalarN("num_neurons_in_layer[6-10]", ImGuiDataType_S32, &QL_parameters.num_neurons_in_layer[6], (QL_parameters.num_hidden_layers - 5));
			}
			ImGui::Separator();
			ImGui::Text("Greed policy settings:");
			ImGui::SameLine(375.f);
			if (ImGui::Button("help\n1", ImVec2(80.f, 19.f)))
				ImGui::OpenPopup("Qlearn_help2");
			if (ImGui::BeginPopup("Qlearn_help2"))
			{
				ImGui::Text("initial_epsilon - The initial probability to \n explore(select a random action).");
				ImGui::Text("anneal_interval - The steps during which the \nprobability to explore will anneal.");
				ImGui::Text("min_epsilon - Epsilon will never be less than\n this value.");
				ImGui::Text("decay_rate - How much to change the model in \nresponse to the estimated error each time the model \nweights are updated.");

				if (ImGui::Button("back\n1", ImVec2(80.f, 19.f)))
					ImGui::CloseCurrentPopup();
				ImGui::EndPopup();
			}
			ImGui::Separator();
			ImGui::InputDouble("initial_epsilon", &QL_parameters.initial_epsilon, 0.01, 0.1, "%.2f");
			ImGui::InputInt("anneal_interval", (int*)&QL_parameters.anneal_interval, 10, 100);
			ImGui::InputDouble("min_epsilon", &QL_parameters.min_epsilon, 0.01, 0.1, "%.2f");
			ImGui::InputDouble("decay_rate", &QL_parameters.decay_rate, 0.01, 0.1, "%.2f");
			ImGui::Separator();
			ImGui::Text("Replay method settings:");
			ImGui::SameLine(375.f);
			if (ImGui::Button("help\n2", ImVec2(80.f, 19.f)))
				ImGui::OpenPopup("Qlearn_help3");
			if (ImGui::BeginPopup("Qlearn_help3"))
			{
				ImGui::Text("batchSize - Number of examples returned at each\nsample.");
				ImGui::Text("capacity - Total memory size in terms of number\nof examples.");
				ImGui::Text("nSteps - Number of steps to look in the future.");

				if (ImGui::Button("back\n2", ImVec2(80.f, 19.f)))
					ImGui::CloseCurrentPopup();
				ImGui::EndPopup();
			}			
			ImGui::Separator();
			ImGui::InputInt("batch_size", (int*)&QL_parameters.batch_size, 10);
			ImGui::InputInt("capacity", (int*)&QL_parameters.capacity, 100, 1000);
			ImGui::InputInt("nsteps_into_future", (int*)&QL_parameters.nsteps_into_future);
			ImGui::Separator();
			ImGui::Text("Training config:");
			ImGui::SameLine(375.f);
			if (ImGui::Button("help\n3", ImVec2(80.f, 19.f)))
				ImGui::OpenPopup("Qlearn_help4");
			if (ImGui::BeginPopup("Qlearn_help4"))
			{
				ImGui::Text("optimizer_step_size - Step size of optimizer.");
				ImGui::Text("discount_rate - Discount rate for future \nreward.");
				ImGui::Text("target_network_sync_interval - Interval for \nsyncing target network.");
				ImGui::Text("exploration_steps - Exploration steps before \nlearning. The agent won't start learn until those \nsteps have passed");

				if (ImGui::Button("back\n3", ImVec2(80.f, 19.f)))
					ImGui::CloseCurrentPopup();
				ImGui::EndPopup();
			}
			ImGui::Separator();
			ImGui::InputDouble("optimizer_step_size", &QL_parameters.optimizer_step_size, 0.01, 0.1, "%.3f");
			ImGui::InputDouble("discount_rate", &QL_parameters.discount_rate, 0.01, 0.1, "%.3f");
			ImGui::InputInt("target_network_sync_interval", (int*)&QL_parameters.target_network_sync_interval);
			ImGui::InputInt("exploration_steps", (int*)&QL_parameters.exploration_steps);
			QL_parameters.check_values();
			ImGui::Separator();
			ImGui::Checkbox(" two_dir_QLtrain", &QL_parameters.two_direction_train);
			ImGui::Separator();

			valueofbutton1 = ImGui::Button("Begin QLearn", ImVec2(120.f, 25.f));
			ImGui::SameLine();
			if (ImGui::Button("Cancel", ImVec2(120.f, 25.f)))
				ImGui::CloseCurrentPopup();
			ImGui::EndPopup();
		}

		//classicNEAT
		ImGui::SameLine();
		if (ImGui::Button("Start Classic NEAT", ImVec2(160.f, 25.f)))
			ImGui::OpenPopup("CL_NEAT");
		if (ImGui::BeginPopup("CL_NEAT"))
		{
			ImGui::Text("NEAT settings:");
			ImGui::SameLine(340.f);
			if (ImGui::Button("help\nNEAT", ImVec2(80.f, 19.f)))
				ImGui::OpenPopup("NEAT_help1");
			if (ImGui::BeginPopup("NEAT_help1"))
			{
				ImGui::Text("single_eval_steps - single simulation number of steps 100 step = 1sec");
				ImGui::Text("num_generations - Number of generations run");
				ImGui::Text("num_runs - Number of runs performed. Each run have -num_generations-");
				ImGui::Text("population_size - Size of population generated in each generation");
				ImGui::Text("winner_fitness_value - desired fitness value");
				ImGui::Text("Output files format:\n");
				ImGui::Text("Trait is a group of parameters that can be expressed more than one time.");
				ImGui::Text("Nodes are single nodes of neural network. In file format is:\nnode X1 X2 X3 X4 where:\nX1 - node number, X2 - aditional parameter,\nX3 - node type(0 - neuron, 1 - sensor),\nX4 - nodeplace(0 - hidden, 1 - input, 2 - output, 3 - bias)");
				ImGui::Text("Genes are connections between two nodes with weights assign to them.");

				if (ImGui::Button("back\nNEAT", ImVec2(80.f, 19.f)))
					ImGui::CloseCurrentPopup();
				ImGui::EndPopup();
			}
			ImGui::Separator();
			ImGui::InputInt("single_eval_steps", &NEAT_parameters.single_eval_steps, 10, 100);
			ImGui::InputInt("num_generations", &NEAT_parameters.num_generations, 100, 1000);
			ImGui::InputInt("num_runs", &NEAT_parameters.num_runs, 1, 10);
			ImGui::InputInt("population_size", &NEAT_parameters.population_size, 1, 10);
			ImGui::InputDouble("winner_fitness_value", &NEAT_parameters.winner_fitness_value, 10, 100, "%.1f");
			ImGui::Separator();
			ImGui::Text("All other configuration parameters can be found in:\n ../models/NEATconfig/NEAT_params.txt");
			ImGui::Text("To change parameters modify file before starting NEAT");
			NEAT_parameters.check_values();
			ImGui::Separator();
			ImGui::Checkbox(" two_dir_NEATtrain", &NEAT_parameters.two_direction_train);
			ImGui::Separator();

			valueofbutton2 = ImGui::Button("Begin NEAT", ImVec2(120.f, 25.f));
			ImGui::SameLine();
			if (ImGui::Button("Cancel", ImVec2(120.f, 25.f)))
				ImGui::CloseCurrentPopup();
			ImGui::EndPopup();
		}
		
		//GA eval
		if (ImGui::Button("Start GA evaluation", ImVec2(160.f, 25.f)))
			ImGui::OpenPopup("GA_eval");
		if (ImGui::BeginPopup("GA_eval"))
		{
			ImGui::Text("Genetic Algorithm settings:");
			ImGui::SameLine(370.f);
			if (ImGui::Button("help\nGA", ImVec2(80.f, 19.f)))
				ImGui::OpenPopup("GA_help1");
			if (ImGui::BeginPopup("GA_help1"))
			{
				ImGui::Text("single_eval_steps - single simulation number\n of steps 100 step = 1sec");
				ImGui::Text("num_generations - Number of generations run");
				ImGui::Text("population_size - Size of population generated\n in each generation");
				if (ImGui::Button("back\nGA", ImVec2(80.f, 19.f)))
					ImGui::CloseCurrentPopup();
				ImGui::EndPopup();
			}
			ImGui::Separator();
			ImGui::InputInt("single_eval_step", &GA_parameters.single_eval_steps, 10, 100);
			ImGui::InputInt("num_generation", &GA_parameters.num_generations, 100, 1000);
			ImGui::InputInt("population_size", &GA_parameters.population_size, 1, 10);
			ImGui::InputDouble("crossover_fraction", &GA_parameters.crossover_fraction, 0.01, 0.1, "%.2f");
			ImGui::InputDouble("mutation_rate", &GA_parameters.mutation_rate, 0.01, 0.1, "%.2f");
			ImGui::InputInt("elite_count", &GA_parameters.elite_count, 1, 10);

			ImGui::Separator();
			ImGui::Text("Stop reason settings:");
			ImGui::SameLine(370.f);
			if (ImGui::Button("help\nGA2", ImVec2(80.f, 19.f)))
				ImGui::OpenPopup("GA_help2");
			if (ImGui::BeginPopup("GA_help2"))
			{
				ImGui::Text("Genetic Algorithm may stop because of one of the following reasons:");
				ImGui::Text("* MaxGenerations - Reaching the maximum generation number.");
				ImGui::Text("* StallAverage - The average cost has not changed more than\n\'tol_stall_average\' for \'average_stall_max\' generation steps");
				ImGui::Text("* StallBest - The average cost has not changed more than\n \'tol_stall_best\' for \'best_stall_max\' generation steps");
				if (ImGui::Button("back\nGA2", ImVec2(80.f, 19.f)))
					ImGui::CloseCurrentPopup();
				ImGui::EndPopup();
			}
			ImGui::Separator();
			ImGui::InputDouble("tol_stall_best", &GA_parameters.tol_stall_best, 0.01, 0.1, "%.2f");
			ImGui::InputInt("best_stall_max", &GA_parameters.best_stall_max, 10, 100);
			ImGui::InputDouble("tol_stall_average", &GA_parameters.tol_stall_average, 0.01, 0.1, "%.2f");
			ImGui::InputInt("average_stall_max", &GA_parameters.average_stall_max, 10, 100);
			ImGui::Separator();
			ImGui::Text("fitness function settings:");
			ImGui::SameLine(370.f);
			if (ImGui::Button("help\nGA3", ImVec2(80.f, 19.f)))
				ImGui::OpenPopup("GA_help3");
			if (ImGui::BeginPopup("GA_help3"))
			{
				ImGui::Text("pi_multiplier... - scale parameter used to set max value of fi parameters.\n fi < (pi * pi_multiplier_for_fi_range) and fi > (-pi * pi_multiplier_for_fi_range)");
				ImGui::Text("max_omega1_freq - sets max value for frequency f. In w = 2*pi*f");
				ImGui::Text("max_A_param_value/A_param_offset - set max value variation of parameter A:\nA =(max_A_param_value * rnd(0->1) - A_param_offset)");


				if (ImGui::Button("back\nGA3", ImVec2(80.f, 19.f)))
					ImGui::CloseCurrentPopup();
				ImGui::EndPopup();
			}
			ImGui::Separator();
			ImGui::InputDouble("pi_multiplier_for_fi1_range", &GA_parameters.pi_multiplier_for_fi1_range, 0.01, 0.1, "%.2f");
			ImGui::InputDouble("pi_multiplier_for_fi2_range", &GA_parameters.pi_multiplier_for_fi2_range, 0.01, 0.1, "%.2f");
			ImGui::InputDouble("max_omega1_freq", &GA_parameters.max_omega1_freq, 0.01, 0.1, "%.2f");
			ImGui::InputDouble("max_omega2_freq - not used", &GA_parameters.max_omega2_freq, 0.01, 0.1, "%.2f");
			ImGui::InputDouble("max_A_param_value", &GA_parameters.max_A_param_value, 0.01, 0.1, "%.2f");
			ImGui::InputDouble("A_param_offset", &GA_parameters.A_param_offset, 0.01, 0.1, "%.2f");
			GA_parameters.check_values();
			ImGui::Separator();

			valueofbutton4 = ImGui::Button("Begin GA", ImVec2(120.f, 25.f));
			ImGui::SameLine();
			if (ImGui::Button("Cancel", ImVec2(120.f, 25.f)))
				ImGui::CloseCurrentPopup();
			ImGui::EndPopup();
		}

		ImGui::SameLine();
		//Vis_neat
		if (ImGui::Button("Start VisualizedNEAT", ImVec2(160.f, 25.f)))
			ImGui::OpenPopup("Vis_NEAT");
		if (ImGui::BeginPopup("Vis_NEAT"))
		{
			ImGui::Text("Visualized NEAT settings:");
			ImGui::SameLine(340.f);
			if (ImGui::Button("help\nVis", ImVec2(80.f, 19.f)))
				ImGui::OpenPopup("VisNEAT_help1");
			if (ImGui::BeginPopup("VisNEAT_help1"))
			{
				ImGui::Text("single_eval_steps - single simulation number\n of steps 100 step = 1sec");
				ImGui::Text("num_generations - Number of generations run");
				ImGui::Text("population_size - Size of population generated\n in each generation");
				if (ImGui::Button("back\nVis", ImVec2(80.f, 19.f)))
					ImGui::CloseCurrentPopup();
				ImGui::EndPopup();
			}
			ImGui::Separator();
			ImGui::InputInt("single_eval_step", &NEAT_parameters.single_eval_steps, 10, 100);
			ImGui::InputInt("num_generation", &NEAT_parameters.num_generations, 100, 1000);
			ImGui::InputInt("population_size", &NEAT_parameters.population_size, 1, 10);
			ImGui::InputInt("num_elites", (int*)&NEAT_parameters.num_elites);
			ImGui::InputFloat("reproduce_ratio", &NEAT_parameters.reproduce_ratio, 0.05f, 0.1, "%.2f");
			ImGui::InputFloat("new_connection_mut_rate", &NEAT_parameters.new_connection_mutation_rate, 0.05f, 0.1, "%.2f");
			ImGui::InputFloat("new_node_mutation_rate", &NEAT_parameters.new_node_mutation_rate, 0.05f, 0.1, "%.2f");
			ImGui::InputFloat("max_perturbation", &NEAT_parameters.max_perturbation, 0.05f, 0.1, "%.2f");
			ImGui::InputInt("draw_each_x_generations", &NEAT_parameters.draw_each, 10, 20);
			NEAT_parameters.check_values();
			ImGui::Separator();
			ImGui::Checkbox("two_dir_VisNEATtrain", &NEAT_parameters.two_direction_train);
			ImGui::Separator();

			valueofbutton3 = ImGui::Button("Begin VisNEAT", ImVec2(120.f, 25.f));
			ImGui::SameLine();
			if (ImGui::Button("Cancel", ImVec2(120.f, 25.f)))
				ImGui::CloseCurrentPopup();
			ImGui::EndPopup();
		}

		//testing settings
		ImGui::Separator();
		ImGui::Text("Test settings:");
		ImGui::Separator();
		ImGui::Text("Select tested method:");
		const char* items2[] = { "DQL Neural Network", "NEAT Neural Network" , "Visualized NEAT NN" , "Genetic Algorithm Function" };
		ImGui::Combo(" Method", &current_method, items2, IM_ARRAYSIZE(items2));
		ImGui::Separator();
		valueofbutton8 = ImGui::Button("Start", ImVec2(160.f, 25.f));
		ImGui::SameLine();
		valueofbutton9 = ImGui::Button("Stop", ImVec2(160.f, 25.f));
		valueofbutton5 = ImGui::Button("Load model", ImVec2(160.f, 25.f));
		ImGui::SameLine();
		valueofbutton6 = ImGui::Button("Reset simulation", ImVec2(160.f, 25.f));
		ImGui::Separator();
		ImGui::Text("Control:");
		ImGui::Separator();
		valueofbutton_direction_left = ImGui::Button("<-", ImVec2(30.f, 25.f));
		ImGui::SameLine();
		valueofbutton7 = ImGui::Button("Stop\n\n", ImVec2(45.f, 25.f));
		ImGui::SameLine();
		valueofbutton_direction_right = ImGui::Button("->", ImVec2(30.f, 25.f));
		// testing end
		ImGui::End();

		//------------------------------------------- end of imgui --------------------------------------------------
		//end of draw


		//REST OF CODE IS JUST FOR SCALING
		// 
		//desnt metter that it's done after drawn frame who cares..

		sf_obstacle.setScale(window_objects_scale, window_objects_scale);
		sf_testbox.setScale(window_objects_scale, window_objects_scale);

		if (selected_model == MODEL_1 || selected_model == MODEL_2 || selected_model == MODEL_6)
		{
			sf_ob1_joint1.setScale(window_objects_scale, window_objects_scale); 
			sf_ob1_joint2.setScale(window_objects_scale, window_objects_scale);
			sf_ob1_circle.setScale(window_objects_scale, window_objects_scale);
			sf_ob1_up_leg.setScale(window_objects_scale, window_objects_scale);
			sf_ob1_low_leg.setScale(window_objects_scale, window_objects_scale);
			sf_ob1_torso.setScale(window_objects_scale, window_objects_scale);
			sf_ob6_torso.setScale(window_objects_scale, window_objects_scale);
			sf_ob6_head.setScale(window_objects_scale, window_objects_scale);
		}
		if (selected_model == MODEL_3)
		{
			sf_ob3_feet_joint.setScale(window_objects_scale, window_objects_scale);
			sf_ob3_knee_joint.setScale(window_objects_scale, window_objects_scale);
			sf_ob3_hip_joint.setScale(window_objects_scale, window_objects_scale);
			sf_ob3_elbow_joint.setScale(window_objects_scale, window_objects_scale);
			sf_ob3_arm_joint.setScale(window_objects_scale, window_objects_scale);

			sf_ob3_feet.setScale(window_objects_scale, window_objects_scale);
			sf_ob3_low_leg.setScale(window_objects_scale, window_objects_scale);
			sf_ob3_up_leg.setScale(window_objects_scale, window_objects_scale);
			sf_ob3_torso.setScale(window_objects_scale, window_objects_scale);
			sf_ob3_up_arm.setScale(window_objects_scale, window_objects_scale);
			sf_ob3_low_arm.setScale(window_objects_scale, window_objects_scale);
			sf_ob3_head.setScale(window_objects_scale, window_objects_scale);
		}
		if (selected_model == MODEL_4)
		{
			sf_ob4_joints.setScale(window_objects_scale, window_objects_scale);
			sf_ob4_cellshape.setScale(window_objects_scale, window_objects_scale);
		}
		if(selected_model == MODEL_5)
		{
			sf_ob5_joints.setScale(window_objects_scale, window_objects_scale);
			sf_ob5_cellshape.setScale(window_objects_scale, window_objects_scale);
		}
	} //dont do if menu == false
}

void SimWindow::DrawModel(std::vector<model_draw_struct>& draw_struct)
{
	//function to draw entire model form array of pointers
	for (auto& x : draw_struct)
	{
		if (x.is_body == true)
			SimWindow::DrawObjects((b2Body*)x.element, x.shape, x.color);
		else
			SimWindow::DrawObjects((b2RevoluteJoint*)x.element, x.shape, x.color);
	}
}

void SimWindow::ChangeTheme(bool update_custom_colors)
{
	if (currentTheme == 0)	//dark
	{
		//40 57 83 imgui blue 38 38 38 gui grey
		backgroundColor = sf::Color(30, 35, 45);
		backElementsColor = sf::Color(54, 65, 79);
		frontElementsColor = sf::Color(92, 101, 113);
		outlinesColor = sf::Color(180, 180, 180, 235);
		myColor = sf::Color(41, 73, 54);
		text.setFillColor(sf::Color::White);

		if (training_window_mode == false)
		{
			ImGui::StyleColorsDark();
			ImVec4* colors = ImGui::GetStyle().Colors;
			colors[ImGuiCol_WindowBg] = ImColor(21,25, 29);
			colors[ImGuiCol_PopupBg] = ImColor(21, 25, 29);
			colors[ImGuiCol_Border] = ImColor(21, 25, 29);
			colors[ImGuiCol_Separator] = ImColor(73, 80, 87,128);
			colors[ImGuiCol_Text] = ImColor(248, 249, 250);
			colors[ImGuiCol_TitleBg] = ImColor(1, 1, 1);
			colors[ImGuiCol_TitleBgActive] = ImColor(1, 1, 1);
			colors[ImGuiCol_TitleBgCollapsed] = ImColor(5, 5, 5);
			colors[ImGuiCol_Button] = ImColor(73, 80, 87);
			colors[ImGuiCol_ButtonActive] = ImColor(173, 181, 189);
			colors[ImGuiCol_ButtonHovered] = ImColor(108, 117, 125);
			colors[ImGuiCol_FrameBg] = ImColor(73, 80, 87);
			colors[ImGuiCol_FrameBgActive] = ImColor(108, 117, 125);
			colors[ImGuiCol_FrameBgHovered] = ImColor(173, 181, 189);
			colors[ImGuiCol_SliderGrab] = ImColor(108, 117, 125);
			colors[ImGuiCol_SliderGrabActive] = ImColor(73, 80, 87);
			colors[ImGuiCol_Header] = ImColor(73, 80, 87);
			colors[ImGuiCol_HeaderActive] = ImColor(108, 117, 125);
			colors[ImGuiCol_HeaderHovered] = ImColor(173, 181, 189);
			colors[ImGuiCol_CheckMark] = ImColor(173, 181, 189);
		}
	}
	if (currentTheme == 1)//light
	{
		backgroundColor = sf::Color(255, 255, 255, 255);
		backElementsColor = sf::Color(113, 165, 222, 255);
		frontElementsColor = sf::Color(174, 203, 235, 255);
		outlinesColor = sf::Color(5, 5, 5, 255);
		myColor = sf::Color(246, 142, 95);
		text.setFillColor(sf::Color::Black);

		if (training_window_mode == false) 
		{
			ImGui::StyleColorsLight();
			ImVec4* colors = ImGui::GetStyle().Colors; 
			colors[ImGuiCol_PopupBg] = colors[ImGuiCol_WindowBg];
		}
	}
	if (currentTheme == 2)
	{
		if (update_custom_colors == false)
		{
			backgroundColor = sf::Color(2, 2, 2);
			backElementsColor = sf::Color(33, 33, 33);
			frontElementsColor = sf::Color(22, 22, 22);
			outlinesColor = sf::Color(100, 245, 110, 128);
			myColor = sf::Color(22, 22, 22);
			text.setFillColor(sf::Color::White);

			colors_custom[0] = backgroundColor.r / 255.f;
			colors_custom[1] = backgroundColor.g / 255.f;
			colors_custom[2] = backgroundColor.b / 255.f;
			colors_custom[3] = backElementsColor.r / 255.f;
			colors_custom[4] = backElementsColor.g / 255.f;
			colors_custom[5] = backElementsColor.b / 255.f;
			colors_custom[6] = frontElementsColor.r / 255.f;
			colors_custom[7] = frontElementsColor.g / 255.f;
			colors_custom[8] = frontElementsColor.b / 255.f;
			colors_custom[9] = outlinesColor.r / 255.f;
			colors_custom[10] = outlinesColor.g / 255.f;
			colors_custom[11] = outlinesColor.b / 255.f;
			colors_custom[12] = outlinesColor.a / 255.f;
			colors_custom[13] = myColor.r / 255.f;
			colors_custom[14] = myColor.g / 255.f;
			colors_custom[15] = myColor.b / 255.f;

			if (training_window_mode == false)
			{
				ImGui::StyleColorsDark();
				//Custom colors palette of all visible menu elements:
				/*ImVec4* colors = ImGui::GetStyle().Colors;
				colors[ImGuiCol_WindowBg] = ImColor(21, 25, 29);
				colors[ImGuiCol_PopupBg] = ImColor(21, 25, 29);
				colors[ImGuiCol_Border] = ImColor(21, 25, 29);
				colors[ImGuiCol_Separator] = ImColor(73, 80, 87, 128);
				colors[ImGuiCol_Text] = ImColor(248, 249, 250);
				colors[ImGuiCol_TitleBg] = ImColor(1, 1, 1);
				colors[ImGuiCol_TitleBgActive] = ImColor(1, 1, 1);
				colors[ImGuiCol_TitleBgCollapsed] = ImColor(5, 5, 5);
				colors[ImGuiCol_Button] = ImColor(73, 80, 87);
				colors[ImGuiCol_ButtonActive] = ImColor(173, 181, 189);
				colors[ImGuiCol_ButtonHovered] = ImColor(108, 117, 125);
				colors[ImGuiCol_FrameBg] = ImColor(73, 80, 87);
				colors[ImGuiCol_FrameBgActive] = ImColor(108, 117, 125);
				colors[ImGuiCol_FrameBgHovered] = ImColor(173, 181, 189);
				colors[ImGuiCol_SliderGrab] = ImColor(108, 117, 125);
				colors[ImGuiCol_SliderGrabActive] = ImColor(73, 80, 87);
				colors[ImGuiCol_Header] = ImColor(73, 80, 87);
				colors[ImGuiCol_HeaderActive] = ImColor(108, 117, 125);
				colors[ImGuiCol_HeaderHovered] = ImColor(173, 181, 189);
				colors[ImGuiCol_CheckMark] = ImColor(173, 181, 189);*/
			}
		}
		else
		{
			backgroundColor.r = (sf::Uint8)(colors_custom[0] * 255.f);
			backgroundColor.g = (sf::Uint8)(colors_custom[1] * 255.f);
			backgroundColor.b = (sf::Uint8)(colors_custom[2] * 255.f);
			backElementsColor.r = (sf::Uint8)(colors_custom[3] * 255.f);
			backElementsColor.g = (sf::Uint8)(colors_custom[4] * 255.f);
			backElementsColor.b = (sf::Uint8)(colors_custom[5] * 255.f);
			frontElementsColor.r = (sf::Uint8)(colors_custom[6] * 255.f);
			frontElementsColor.g = (sf::Uint8)(colors_custom[7] * 255.f);
			frontElementsColor.b = (sf::Uint8)(colors_custom[8] * 255.f);
			outlinesColor.r = (sf::Uint8)(colors_custom[9] * 255.f);
			outlinesColor.g = (sf::Uint8)(colors_custom[10] * 255.f);
			outlinesColor.b = (sf::Uint8)(colors_custom[11] * 255.f);
			outlinesColor.a = (sf::Uint8)(colors_custom[12] * 255.f);
			myColor.r = (sf::Uint8)(colors_custom[13] * 255.f);
			myColor.g = (sf::Uint8)(colors_custom[14] * 255.f);
			myColor.b = (sf::Uint8)(colors_custom[15] * 255.f);
		}
	}
	sf_obstacle.setOutlineColor(outlinesColor);
	sf_testbox.setOutlineColor(outlinesColor);
	sf_ob1_up_leg.setOutlineColor(outlinesColor);
	sf_ob1_low_leg.setOutlineColor(outlinesColor);
	sf_ob1_torso.setOutlineColor(outlinesColor);
	sf_ob1_joint1.setOutlineColor(outlinesColor);
	sf_ob1_joint2.setOutlineColor(outlinesColor);
	sf_ob1_circle.setOutlineColor(outlinesColor);
	sf_ob6_torso.setOutlineColor(outlinesColor);
	sf_ob6_head.setOutlineColor(outlinesColor);
	sf_ob4_cellshape.setOutlineColor(outlinesColor);
	sf_ob4_joints.setOutlineColor(outlinesColor);
	sf_ob5_cellshape.setOutlineColor(outlinesColor);
	sf_ob5_joints.setOutlineColor(outlinesColor);
	sf_ob3_feet.setOutlineColor(outlinesColor);
	sf_ob3_low_leg.setOutlineColor(outlinesColor);
	sf_ob3_up_leg.setOutlineColor(outlinesColor);
	sf_ob3_torso.setOutlineColor(outlinesColor);
	sf_ob3_low_arm.setOutlineColor(outlinesColor);
	sf_ob3_up_arm.setOutlineColor(outlinesColor);
	sf_ob3_head.setOutlineColor(outlinesColor);
	sf_ob3_feet_joint.setOutlineColor(outlinesColor);
	sf_ob3_knee_joint.setOutlineColor(outlinesColor);
	sf_ob3_hip_joint.setOutlineColor(outlinesColor);
	sf_ob3_arm_joint.setOutlineColor(outlinesColor);
	sf_ob3_elbow_joint.setOutlineColor(outlinesColor);
	line[0].color = outlinesColor;
	line[1].color = outlinesColor;
}

// greetings  traveler you have come soo far....
