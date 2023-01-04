#pragma once
#include <box2d/box2d.h>
#include <vector>

#define MS_PER_UPDATE 10.f						// time step for simulation		 
#define TIMESTEP  (MS_PER_UPDATE / 1000.f)		// delta time for simulation
#define VELOCITYITER 12							// velocity iterations per step; default: 8
#define POSITIONITER 8							// position iterations per step; default: 3
#define GRAVITY -9.81f							//gravity force
#define MODELS_FRICTION 0.8f
#define SELECTED_MODEL MODEL_2				// select between models: (it's just starting model)
												// 1 - multi-membered Mechanism 4-legged dog type 
												// 2 - multi-membered Mechanism 2-legged with vertical torso
												// 3 - multi-membered Mechanism	multi-legged snake-like
												//additional can be created by modifying coments with HERE quote

												// function to translate degrres to radians
inline float degreesToRadians(float degree) {
	return degree * (b2_pi / 180.f);
}

enum SelectedModel {
	MODEL_1,
	MODEL_2,
	MODEL_3,
	MODEL_4,
	MODEL_5,
	MODEL_6
};

class PhysicsWorld
{
public:
	PhysicsWorld();
	~PhysicsWorld();
	void Init();
	void End();
	void TimeStep(float timeStep = TIMESTEP, int32 velocityIterations = VELOCITYITER, int32 positionIterations = POSITIONITER);
	void AddRemoveObstacleTestobj();
	void createMechanism();
	void destroyMechanism();
	void resetMechanism();
	void CheckAngleLimits(void);

	SelectedModel selected_model = SELECTED_MODEL;
	bool add_test_objects = false;
	bool obstacles = false;
	float objects_friction = MODELS_FRICTION;
	b2Body* testball = nullptr;
	b2Body* testbox = nullptr;
	b2Body* groundBody2 = nullptr;

	//--- object 1 elements ---//
	
	//elements draw order: 1, 3, 0, 2, 4, 5, 7, 6, 8
	// joints draw order: 5, 6, 0, 2, 4, 7
	const int obj1_num_parts = 9;
	const int obj1_num_joints = 8;
	std::vector<b2Body*> obj1_parts;
	std::vector<b2RevoluteJoint*> obj1_joints;

	//--- object 2 elements ---//

	//elements draw order: 1->5
	const int obj2_num_parts = 5;
	const int obj2_num_joints = 4;
	std::vector<b2Body*> obj2_parts;
	std::vector<b2RevoluteJoint*> obj2_joints;

	//--- object 3 elements ---//

	//elements draw order: 0,2,4,8,10,6,11,1,3,5,7,9
	//joints draw order: 0,2,8,1,3,5,7,9	
	const int obj3_num_parts = 12;
	const int obj3_num_joints = 11;
	std::vector<b2Body*> obj3_parts;
	std::vector<b2RevoluteJoint*> obj3_joints;

	//--- object 4 elements ---//

	//elements draw order 1->5
	const int obj4_num_parts = 5;
	const int obj4_num_joints = 4;
	std::vector<b2Body*> obj4_parts;
	std::vector<b2RevoluteJoint*> obj4_joints;

	//--- object 5 elements ---//

	//elements draw order 
	const int obj5_num_parts = 13;
	const int obj5_num_joints = 12;
	std::vector<b2Body*> obj5_parts;
	std::vector<b2RevoluteJoint*> obj5_joints;

	//--- object 6 elements ---//

	//elements draw order 
	const int obj6_num_parts = 6;
	const int obj6_num_joints = 5;
	std::vector<b2Body*> obj6_parts;
	std::vector<b2RevoluteJoint*> obj6_joints;

protected:
	b2World* world = nullptr;
	b2Body* groundBody = nullptr;

	float margin = b2_pi / 180.f;

	//--- object 1,2,6 elements ---
	b2Vec2 upper_leg[6];
	b2Vec2 lower_leg[4];
	b2Vec2 torso[8];
	b2Vec2 torso_v2[8];
	b2Vec2 head[7];

	//--- object 3 elements ---
	b2Vec2 obj3_feet[5];
	b2Vec2 obj3_lower_leg[6];
	b2Vec2 obj3_upper_leg[6];
	b2Vec2 obj3_lower_arm[4];
	b2Vec2 obj3_upper_arm[6];
	b2Vec2 obj3_torso[8];
	b2Vec2 obj3_head[8];

	//--- object 4,5 elements ---
	b2Vec2 obj4_cellshape[8];
	b2Vec2 obj5_cellshape[8];
};
