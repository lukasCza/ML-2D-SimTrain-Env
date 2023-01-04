#pragma once
#include "ProgramWindow.h"
#include "PhysicsEngine.h"

std::vector<model_draw_struct> ilikeitthatway(PhysicsWorld& world, SimWindow& window, SelectedModel model) 
{
	std::vector<model_draw_struct> output;

	switch (model)
	{
	case MODEL_1:
		output.push_back(model_draw_struct(world.obj1_parts[1], &window.sf_ob1_low_leg, &window.backElementsColor, true));
		output.push_back(model_draw_struct(world.obj1_parts[3], &window.sf_ob1_low_leg, &window.backElementsColor, true));
		output.push_back(model_draw_struct(world.obj1_parts[0], &window.sf_ob1_up_leg, &window.backElementsColor, true));
		output.push_back(model_draw_struct(world.obj1_parts[2], &window.sf_ob1_up_leg, &window.backElementsColor, true));
		output.push_back(model_draw_struct(world.obj1_parts[1], &window.sf_ob1_circle, &window.backElementsColor, true));
		output.push_back(model_draw_struct(world.obj1_parts[3], &window.sf_ob1_circle, &window.backElementsColor, true));
		output.push_back(model_draw_struct(world.obj1_joints[5], &window.sf_ob1_joint2, &window.backElementsColor, false));
		output.push_back(model_draw_struct(world.obj1_joints[6], &window.sf_ob1_joint2, &window.backElementsColor, false));
		output.push_back(model_draw_struct(world.obj1_parts[4], &window.sf_ob1_torso, &window.frontElementsColor, true));
		output.push_back(model_draw_struct(world.obj1_parts[6], &window.sf_ob1_low_leg, &window.frontElementsColor, true));
		output.push_back(model_draw_struct(world.obj1_parts[6], &window.sf_ob1_circle, &window.frontElementsColor, true));
		output.push_back(model_draw_struct(world.obj1_parts[8], &window.sf_ob1_low_leg, &window.frontElementsColor, true));
		output.push_back(model_draw_struct(world.obj1_parts[8], &window.sf_ob1_circle, &window.frontElementsColor, true));
		output.push_back(model_draw_struct(world.obj1_parts[5], &window.sf_ob1_up_leg, &window.frontElementsColor, true));
		output.push_back(model_draw_struct(world.obj1_parts[7], &window.sf_ob1_up_leg, &window.frontElementsColor, true));
		output.push_back(model_draw_struct(world.obj1_joints[4], &window.sf_ob1_joint2, &window.frontElementsColor, false));
		output.push_back(model_draw_struct(world.obj1_joints[7], &window.sf_ob1_joint2, &window.frontElementsColor, false));
		output.push_back(model_draw_struct(world.obj1_joints[0], &window.sf_ob1_joint1, &window.frontElementsColor, false));
		output.push_back(model_draw_struct(world.obj1_joints[2], &window.sf_ob1_joint1, &window.frontElementsColor, false));
		break;
	case MODEL_2:
		output.push_back(model_draw_struct(world.obj2_parts[4], &window.sf_ob1_torso, &window.frontElementsColor, true));
		output.push_back(model_draw_struct(world.obj2_parts[0], &window.sf_ob1_low_leg, &window.frontElementsColor, true));
		output.push_back(model_draw_struct(world.obj2_parts[0], &window.sf_ob1_circle, &window.frontElementsColor, true));
		output.push_back(model_draw_struct(world.obj2_parts[1], &window.sf_ob1_low_leg, &window.frontElementsColor, true));
		output.push_back(model_draw_struct(world.obj2_parts[1], &window.sf_ob1_circle, &window.frontElementsColor, true));
		output.push_back(model_draw_struct(world.obj2_parts[2], &window.sf_ob1_up_leg, &window.frontElementsColor, true));
		output.push_back(model_draw_struct(world.obj2_parts[3], &window.sf_ob1_up_leg, &window.frontElementsColor, true));
		output.push_back(model_draw_struct(world.obj2_joints[2], &window.sf_ob1_joint2, &window.frontElementsColor, false));
		output.push_back(model_draw_struct(world.obj2_joints[3], &window.sf_ob1_joint2, &window.frontElementsColor, false));
		output.push_back(model_draw_struct(world.obj2_joints[0], &window.sf_ob1_joint1, &window.frontElementsColor, false));
		output.push_back(model_draw_struct(world.obj2_joints[1], &window.sf_ob1_joint1, &window.frontElementsColor, false));
		break;
	case MODEL_3:
		output.push_back(model_draw_struct(world.obj3_parts[0], &window.sf_ob3_feet, &window.backElementsColor, true));
		output.push_back(model_draw_struct(world.obj3_parts[2], &window.sf_ob3_low_leg, &window.backElementsColor, true));
		output.push_back(model_draw_struct(world.obj3_parts[4], &window.sf_ob3_up_leg, &window.backElementsColor, true));
		output.push_back(model_draw_struct(world.obj3_parts[8], &window.sf_ob3_up_arm, &window.backElementsColor, true));
		output.push_back(model_draw_struct(world.obj3_parts[10], &window.sf_ob3_low_arm, &window.backElementsColor, true));
		output.push_back(model_draw_struct(world.obj3_joints[0], &window.sf_ob3_feet_joint, &window.backElementsColor, false));
		output.push_back(model_draw_struct(world.obj3_joints[2], &window.sf_ob3_knee_joint, &window.backElementsColor, false));
		output.push_back(model_draw_struct(world.obj3_joints[8], &window.sf_ob3_elbow_joint, &window.backElementsColor, false));
		output.push_back(model_draw_struct(world.obj3_parts[6], &window.sf_ob3_torso, &window.frontElementsColor, true));
		output.push_back(model_draw_struct(world.obj3_parts[11], &window.sf_ob3_head, &window.frontElementsColor, true));
		output.push_back(model_draw_struct(world.obj3_parts[1], &window.sf_ob3_feet, &window.frontElementsColor, true));
		output.push_back(model_draw_struct(world.obj3_parts[3], &window.sf_ob3_low_leg, &window.frontElementsColor, true));
		output.push_back(model_draw_struct(world.obj3_parts[5], &window.sf_ob3_up_leg, &window.frontElementsColor, true));
		output.push_back(model_draw_struct(world.obj3_parts[7], &window.sf_ob3_up_arm, &window.frontElementsColor, true));
		output.push_back(model_draw_struct(world.obj3_joints[1], &window.sf_ob3_feet_joint, &window.frontElementsColor, false));
		output.push_back(model_draw_struct(world.obj3_joints[3], &window.sf_ob3_knee_joint, &window.frontElementsColor, false));
		output.push_back(model_draw_struct(world.obj3_joints[5], &window.sf_ob3_hip_joint, &window.frontElementsColor, false));
		output.push_back(model_draw_struct(world.obj3_parts[9], &window.sf_ob3_low_arm, &window.frontElementsColor, true));
		output.push_back(model_draw_struct(world.obj3_joints[7], &window.sf_ob3_arm_joint, &window.frontElementsColor, false));
		output.push_back(model_draw_struct(world.obj3_joints[9], &window.sf_ob3_elbow_joint, &window.frontElementsColor, false));
		break;
	case MODEL_4:
		//for (int i = 0; i < world.obj4_num_parts; i++)
		//	output.push_back(model_draw_struct(world.obj4_parts[i], &window.sf_ob4_cellshape, &window.myColor, true));
		//for (int i = 0; i < world.obj4_num_joints; i++)
		//	output.push_back(model_draw_struct(world.obj4_joints[i], &window.sf_ob4_joints, &window.myColor, false));
		output.push_back(model_draw_struct(world.obj4_parts[0], &window.sf_ob4_cellshape, &window.myColor, true));
		output.push_back(model_draw_struct(world.obj4_parts[1], &window.sf_ob4_cellshape, &window.myColor, true));
		output.push_back(model_draw_struct(world.obj4_joints[0], &window.sf_ob4_joints, &window.myColor, false));
		output.push_back(model_draw_struct(world.obj4_parts[2], &window.sf_ob4_cellshape, &window.myColor, true));
		output.push_back(model_draw_struct(world.obj4_joints[1], &window.sf_ob4_joints, &window.myColor, false));
		output.push_back(model_draw_struct(world.obj4_parts[3], &window.sf_ob4_cellshape, &window.myColor, true));
		output.push_back(model_draw_struct(world.obj4_joints[2], &window.sf_ob4_joints, &window.myColor, false));
		output.push_back(model_draw_struct(world.obj4_parts[4], &window.sf_ob4_cellshape, &window.myColor, true));
		output.push_back(model_draw_struct(world.obj4_joints[3], &window.sf_ob4_joints, &window.myColor, false));
		break;
	case MODEL_5:
		for (int i = 0; i < world.obj5_num_parts; i++)
			output.push_back(model_draw_struct(world.obj5_parts[i], &window.sf_ob5_cellshape, &window.myColor, true));
		for (int i = 0; i < world.obj5_num_joints; i++)
			output.push_back(model_draw_struct(world.obj5_joints[i], &window.sf_ob5_joints, &window.myColor, false));
		break;
	case MODEL_6:
		output.push_back(model_draw_struct(world.obj6_parts[4], &window.sf_ob6_torso, &window.frontElementsColor, true));
		output.push_back(model_draw_struct(world.obj6_parts[5], &window.sf_ob6_head, &window.frontElementsColor, true));
		output.push_back(model_draw_struct(world.obj6_joints[4], &window.sf_ob1_joint2, &window.frontElementsColor, false));
		output.push_back(model_draw_struct(world.obj6_parts[0], &window.sf_ob1_low_leg, &window.frontElementsColor, true));
		output.push_back(model_draw_struct(world.obj6_parts[0], &window.sf_ob1_circle, &window.frontElementsColor, true));
		output.push_back(model_draw_struct(world.obj6_parts[1], &window.sf_ob1_low_leg, &window.frontElementsColor, true));
		output.push_back(model_draw_struct(world.obj6_parts[1], &window.sf_ob1_circle, &window.frontElementsColor, true));
		output.push_back(model_draw_struct(world.obj6_parts[2], &window.sf_ob1_up_leg, &window.frontElementsColor, true));
		output.push_back(model_draw_struct(world.obj6_parts[3], &window.sf_ob1_up_leg, &window.frontElementsColor, true));
		output.push_back(model_draw_struct(world.obj6_joints[2], &window.sf_ob1_joint2, &window.frontElementsColor, false));
		output.push_back(model_draw_struct(world.obj6_joints[3], &window.sf_ob1_joint2, &window.frontElementsColor, false));
		output.push_back(model_draw_struct(world.obj6_joints[0], &window.sf_ob1_joint1, &window.frontElementsColor, false));
		output.push_back(model_draw_struct(world.obj6_joints[1], &window.sf_ob1_joint1, &window.frontElementsColor, false));
		break;
	default:
		break;
	}
	return output;
}