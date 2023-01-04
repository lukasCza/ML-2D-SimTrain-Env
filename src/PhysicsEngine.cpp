#include <box2d/box2d.h>
#include "PhysicsEngine.h"

PhysicsWorld::PhysicsWorld() {}
PhysicsWorld::~PhysicsWorld() {}

void PhysicsWorld::Init()
{
	// creates world object and defines gravity force

	b2Vec2 gravity(0.0f, GRAVITY);
	world = new b2World(gravity);

	//--- ground definition ----

	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(0.0f, -1.0f);
	groundBodyDef.type = b2_staticBody;
	groundBody = world->CreateBody(&groundBodyDef);

	b2PolygonShape ground;
	ground.SetAsBox(300.f, 1.0f);
	b2FixtureDef groundfixtureDef;
	groundfixtureDef.shape = &ground;
	groundfixtureDef.friction = 0.9;
	groundfixtureDef.filter.categoryBits = 0x0020;
	groundBody->CreateFixture(&groundfixtureDef);

	//set vectors size
	obj1_parts.resize(obj1_num_parts);
	obj1_joints.resize(obj1_num_joints);
	obj2_parts.resize(obj2_num_parts);
	obj2_joints.resize(obj2_num_joints);
	obj3_parts.resize(obj3_num_parts);
	obj3_joints.resize(obj3_num_joints);
	obj4_parts.resize(obj4_num_parts);
	obj4_joints.resize(obj4_num_joints);

	obj5_parts.resize(obj5_num_parts);
	obj5_joints.resize(obj5_num_joints);
	obj6_parts.resize(obj6_num_parts);
	obj6_joints.resize(obj6_num_joints);

	//define shapes - vertex arrays
	//model 1
	upper_leg[0].Set(-0.1f, 0.2f);
	upper_leg[1].Set(-0.1f, 0.0f);
	upper_leg[2].Set(-0.05f, -0.2f);
	upper_leg[3].Set(0.05f, -0.2f);
	upper_leg[4].Set(0.1f, 0.0f);
	upper_leg[5].Set(0.1f, 0.2f);

	lower_leg[0].Set(-0.05f, 0.2f);
	lower_leg[1].Set(-0.03f, -0.2f);
	lower_leg[2].Set(0.03f, -0.2f);
	lower_leg[3].Set(0.05f, 0.2f);

	torso[0].Set(-0.3f, 0.2f);
	torso[1].Set(-0.5f, 0.1f);
	torso[2].Set(-0.5f, -0.1f);
	torso[3].Set(-0.4f, -0.2f);
	torso[4].Set(0.4f, -0.2f);
	torso[5].Set(0.5f, -0.1f);
	torso[6].Set(0.5f, 0.1f);
	torso[7].Set(0.3f, 0.2f);

	torso_v2[0].Set(-0.3f, 0.2f);
	torso_v2[1].Set(-0.5f, 0.1f);
	torso_v2[2].Set(-0.5f, -0.1f);
	torso_v2[3].Set(-0.4f, -0.2f);
	torso_v2[4].Set(0.4f, -0.2f);
	torso_v2[5].Set(0.5f, -0.1f);
	torso_v2[6].Set(0.5f, 0.1f);
	torso_v2[7].Set(0.4f, 0.2f);

	head[0].Set(-0.1f, 0.15f);
	head[1].Set(-0.15f,0.1f);
	head[2].Set(-0.15f, -0.1f);
	head[3].Set(0.2f, -0.1f);
	head[4].Set(0.25f,-0.05f);
	head[5].Set(0.25f, 0.0f);
	head[6].Set(0.05f, 0.15f);

	//model 4,5

	obj4_cellshape[0].Set(-0.1f, 0.08f);
	obj4_cellshape[1].Set(-0.15f, 0.04f);
	obj4_cellshape[2].Set(-0.15f, -0.04f);
	obj4_cellshape[3].Set(-0.1f, -0.08f);
	obj4_cellshape[4].Set(0.1f, -0.08f);
	obj4_cellshape[5].Set(0.15f, -0.04f);
	obj4_cellshape[6].Set(0.15f, 0.04f);
	obj4_cellshape[7].Set(0.1f, 0.08f);

	obj5_cellshape[0].Set(-0.05f, 0.06f);
	obj5_cellshape[1].Set(-0.1f, 0.03f);
	obj5_cellshape[2].Set(-0.1f, -0.03f);
	obj5_cellshape[3].Set(-0.05f, -0.06f);
	obj5_cellshape[4].Set(0.05f, -0.06f);
	obj5_cellshape[5].Set(0.1f, -0.03f);
	obj5_cellshape[6].Set(0.1f, 0.03f);
	obj5_cellshape[7].Set(0.05f, 0.06f);


	// model 3

	obj3_feet[0].Set(-0.03f, 0.05f);
	obj3_feet[1].Set(-0.04f, -0.05f);
	obj3_feet[2].Set(0.2f, -0.05f);
	obj3_feet[3].Set(0.2f, -0.02f);
	obj3_feet[4].Set(0.03f, 0.05f);

	obj3_lower_leg[0].Set(-0.06f, 0.24f);
	obj3_lower_leg[1].Set(-0.06f, 0.05f);
	obj3_lower_leg[2].Set(-0.03f, -0.24f);
	obj3_lower_leg[3].Set(0.03f, -0.24f);
	obj3_lower_leg[4].Set(0.06f, 0.05f);
	obj3_lower_leg[5].Set(0.06f, 0.24f);

	obj3_upper_leg[0].Set(-0.1f, 0.2f);
	obj3_upper_leg[1].Set(-0.1f, 0.0f);
	obj3_upper_leg[2].Set(-0.06f, -0.2f);
	obj3_upper_leg[3].Set(0.06f, -0.2f);
	obj3_upper_leg[4].Set(0.1f, 0.0f);
	obj3_upper_leg[5].Set(0.1f, 0.2f);

	obj3_torso[0].Set(-0.1f, 0.4f);
	obj3_torso[1].Set(-0.15f, 0.35f);
	obj3_torso[2].Set(-0.15f, -0.35f);
	obj3_torso[3].Set(-0.1f, -0.4f);
	obj3_torso[4].Set(0.1f, -0.4f);
	obj3_torso[5].Set(0.15f, -0.35f);
	obj3_torso[6].Set(0.15f, 0.35f);
	obj3_torso[7].Set(0.1f, 0.4f);

	obj3_upper_arm[0].Set(-0.06f, 0.15f);
	obj3_upper_arm[1].Set(-0.06f, 0.05f);
	obj3_upper_arm[2].Set(-0.04f, -0.15f);
	obj3_upper_arm[3].Set(0.04f, -0.15f);
	obj3_upper_arm[4].Set(0.06f, 0.05f);
	obj3_upper_arm[5].Set(0.06f, 0.15f);

	obj3_lower_arm[0].Set(-0.04f, 0.2f);
	obj3_lower_arm[1].Set(-0.02f, -0.2f);
	obj3_lower_arm[2].Set(0.02f, -0.2f);
	obj3_lower_arm[3].Set(0.04f, 0.2f);

	obj3_head[0].Set(-0.04f, 0.11f);
	obj3_head[1].Set(-0.08f, 0.08f);
	obj3_head[2].Set(-0.08f, -0.08f);
	obj3_head[3].Set(-0.04f, -0.11f);
	obj3_head[4].Set(0.04f, -0.11f);
	obj3_head[5].Set(0.08f, -0.08f);
	obj3_head[6].Set(0.08f, 0.08f);
	obj3_head[7].Set(0.04f, 0.11f);

}

void PhysicsWorld::End()
{
	//deletes world object

	delete world;
}

void PhysicsWorld::TimeStep(float timeStep, 
							int32 velocityIterations, 
							int32 positionIterations)
{
	//Time step for single iteration of world

	world->Step(timeStep, velocityIterations, positionIterations);
}
void PhysicsWorld::createMechanism()
{
	// selected model needs to be model you want to create
		PhysicsWorld::AddRemoveObstacleTestobj();

	if(selected_model == MODEL_1)
	{
		//---- body factory ----

		b2BodyDef bodyDef;
		bodyDef.type = b2_dynamicBody;
		bodyDef.position.Set(0.0f, 0.84f);
		obj1_parts[4] = world->CreateBody(&bodyDef);
		bodyDef.position.Set(-0.4f, 0.64f);
		obj1_parts[5] = world->CreateBody(&bodyDef);
		bodyDef.position.Set(-0.4f, 0.24f);
		obj1_parts[6] = world->CreateBody(&bodyDef);
		bodyDef.position.Set(-0.4f, 0.64f);
		obj1_parts[0] = world->CreateBody(&bodyDef);
		bodyDef.position.Set(-0.4f, 0.24f);
		obj1_parts[1] = world->CreateBody(&bodyDef);
		bodyDef.position.Set(0.4f, 0.64f);
		obj1_parts[7] = world->CreateBody(&bodyDef);
		bodyDef.position.Set(0.4f, 0.24f);
		obj1_parts[8] = world->CreateBody(&bodyDef);
		bodyDef.position.Set(0.4f, 0.64f);
		obj1_parts[2] = world->CreateBody(&bodyDef);
		bodyDef.position.Set(0.4f, 0.24f);
		//bodyDef.angle = degreesToRadians(-30.f);
		obj1_parts[3] = world->CreateBody(&bodyDef);

		b2CircleShape b2_circle1;
		b2PolygonShape b2_up_leg;
		b2PolygonShape b2_low_leg;
		b2PolygonShape b2_torso;

		b2_circle1.m_radius = 0.03f;
		b2_circle1.m_p.Set(0.0f, -0.2f);
		b2_up_leg.Set(upper_leg, 6);
		b2_low_leg.Set(lower_leg, 4);
		b2_torso.Set(torso, 8);

		//legs numeration 0	 	2 front: 5	4	7
		//			back: 1		3		 6		8
		//
		//----- creating fixtures-------

		b2FixtureDef fixtureDef;
		fixtureDef.shape = &b2_torso;
		fixtureDef.density = 30.f;
		fixtureDef.friction = 0.3f;
		fixtureDef.filter.categoryBits = 0x0002;
		fixtureDef.filter.maskBits = 0x0022;
		obj1_parts[4]->CreateFixture(&fixtureDef);
		fixtureDef.shape = &b2_up_leg;
		fixtureDef.density = 20.f;
		fixtureDef.friction = objects_friction;
		fixtureDef.filter.categoryBits = 0x0004;
		fixtureDef.filter.maskBits = 0x0020;	//24 to colide
		obj1_parts[5]->CreateFixture(&fixtureDef);
		obj1_parts[7]->CreateFixture(&fixtureDef);
		fixtureDef.shape = &b2_low_leg;
		obj1_parts[6]->CreateFixture(&fixtureDef);
		obj1_parts[8]->CreateFixture(&fixtureDef);
		fixtureDef.shape = &b2_circle1;
		obj1_parts[6]->CreateFixture(&fixtureDef);
		obj1_parts[8]->CreateFixture(&fixtureDef);
		fixtureDef.filter.categoryBits = 0x0200;
		fixtureDef.filter.maskBits = 0x0020;	//0220 to colide
		fixtureDef.shape = &b2_low_leg;
		obj1_parts[1]->CreateFixture(&fixtureDef);
		obj1_parts[3]->CreateFixture(&fixtureDef);
		fixtureDef.shape = &b2_circle1;
		obj1_parts[1]->CreateFixture(&fixtureDef);
		obj1_parts[3]->CreateFixture(&fixtureDef);
		fixtureDef.shape = &b2_up_leg;
		obj1_parts[0]->CreateFixture(&fixtureDef);
		obj1_parts[2]->CreateFixture(&fixtureDef);

		//--- creating joints -------
		//		joints front:	0	2	back	1	3
		//						4	7			5	6

		b2RevoluteJointDef jointDef;
		jointDef.bodyA = obj1_parts[4];
		jointDef.bodyB = obj1_parts[5];
		jointDef.localAnchorA = b2Vec2(-0.4f, 0.0f);
		jointDef.localAnchorB = b2Vec2(0.0f, 0.2f);
		jointDef.lowerAngle = degreesToRadians(-90.f);		//upper joints
		jointDef.upperAngle = degreesToRadians(45.f);
		jointDef.enableLimit = true;
		jointDef.maxMotorTorque = 100.0f;
		jointDef.motorSpeed = 0.0f;
		jointDef.enableMotor = true;
		obj1_joints[0] = (b2RevoluteJoint*)world->CreateJoint(&jointDef);
		jointDef.bodyB = obj1_parts[0];
		obj1_joints[1] = (b2RevoluteJoint*)world->CreateJoint(&jointDef);
		jointDef.bodyB = obj1_parts[7];
		jointDef.localAnchorA = b2Vec2(0.4f, 0.0f);
		obj1_joints[2] = (b2RevoluteJoint*)world->CreateJoint(&jointDef);
		jointDef.bodyB = obj1_parts[2];
		obj1_joints[3] = (b2RevoluteJoint*)world->CreateJoint(&jointDef);
		jointDef.bodyA = obj1_parts[5];
		jointDef.bodyB = obj1_parts[6];
		jointDef.localAnchorA = b2Vec2(0.0f, -0.2f);
		jointDef.localAnchorB = b2Vec2(0.0f, 0.2f);
		jointDef.lowerAngle = degreesToRadians(-0.f);				//knee joints rotation
		jointDef.upperAngle = degreesToRadians(140.f);
		jointDef.maxMotorTorque = 100.0f;
		jointDef.motorSpeed = 0.0f;
		jointDef.enableMotor = true;
		obj1_joints[4] = (b2RevoluteJoint*)world->CreateJoint(&jointDef);
		jointDef.bodyA = obj1_parts[0];
		jointDef.bodyB = obj1_parts[1];
		obj1_joints[5] = (b2RevoluteJoint*)world->CreateJoint(&jointDef);
		jointDef.bodyA = obj1_parts[2];
		jointDef.bodyB = obj1_parts[3];
		obj1_joints[6] = (b2RevoluteJoint*)world->CreateJoint(&jointDef);
		jointDef.bodyA = obj1_parts[7];
		jointDef.bodyB = obj1_parts[8];
		obj1_joints[7] = (b2RevoluteJoint*)world->CreateJoint(&jointDef);
		//---- end of model 1 declaration -----
	}

	if (selected_model == MODEL_2)
	{
		b2BodyDef bodyDef;
		bodyDef.type = b2_dynamicBody;
		bodyDef.position.Set(0.0f, 0.84f);
		obj2_parts[4] = world->CreateBody(&bodyDef);
		bodyDef.position.Set(-0.4f, 0.64f);
		obj2_parts[2] = world->CreateBody(&bodyDef);
		bodyDef.position.Set(-0.4f, 0.24f);
		obj2_parts[0] = world->CreateBody(&bodyDef);
		bodyDef.position.Set(0.4f, 0.64f);
		obj2_parts[3] = world->CreateBody(&bodyDef);
		bodyDef.position.Set(0.4f, 0.24f);
		obj2_parts[1] = world->CreateBody(&bodyDef);

		//	2	4	3
		//	0		1

		b2CircleShape b2_circle1;
		b2PolygonShape b2_up_leg;
		b2PolygonShape b2_low_leg;
		b2PolygonShape b2_torso;

		b2_circle1.m_radius = 0.03f;
		b2_circle1.m_p.Set(0.0f, -0.2f);
		b2_up_leg.Set(upper_leg, 6);
		b2_low_leg.Set(lower_leg, 4);
		b2_torso.Set(torso, 8);

		b2FixtureDef fixtureDef;
		fixtureDef.shape = &b2_torso;
		fixtureDef.density = 30.f;
		fixtureDef.friction = 0.3f;
		fixtureDef.filter.categoryBits = 0x0002;
		fixtureDef.filter.maskBits = 0x0022;
		obj2_parts[4]->CreateFixture(&fixtureDef);
		fixtureDef.shape = &b2_up_leg;
		fixtureDef.density = 20.f;
		fixtureDef.friction = objects_friction;
		fixtureDef.filter.categoryBits = 0x0004;
		fixtureDef.filter.maskBits = 0x0024;
		obj2_parts[2]->CreateFixture(&fixtureDef);
		obj2_parts[3]->CreateFixture(&fixtureDef);
		fixtureDef.shape = &b2_low_leg;
		obj2_parts[0]->CreateFixture(&fixtureDef);
		obj2_parts[1]->CreateFixture(&fixtureDef);
		fixtureDef.shape = &b2_circle1;
		obj2_parts[0]->CreateFixture(&fixtureDef);
		obj2_parts[1]->CreateFixture(&fixtureDef);

		b2RevoluteJointDef jointDef;
		jointDef.bodyA = obj2_parts[4];
		jointDef.bodyB = obj2_parts[2];
		jointDef.localAnchorA = b2Vec2(-0.4f, 0.0f);
		jointDef.localAnchorB = b2Vec2(0.0f, 0.2f);
		jointDef.lowerAngle = degreesToRadians(-90.f);		//upper joints
		jointDef.upperAngle = degreesToRadians(90.f);
		jointDef.enableLimit = true;
		jointDef.maxMotorTorque = 100.0f;
		jointDef.motorSpeed = 0.0f;
		jointDef.enableMotor = true;
		obj2_joints[0] = (b2RevoluteJoint*)world->CreateJoint(&jointDef);
		jointDef.bodyB = obj2_parts[3];
		jointDef.localAnchorA = b2Vec2(0.4f, 0.0f);
		obj2_joints[1] = (b2RevoluteJoint*)world->CreateJoint(&jointDef);
		jointDef.bodyA = obj2_parts[2];
		jointDef.bodyB = obj2_parts[0];
		jointDef.localAnchorA = b2Vec2(0.0f, -0.2f);
		jointDef.localAnchorB = b2Vec2(0.0f, 0.2f);
		jointDef.lowerAngle = degreesToRadians(-0.f);				//knee joints rotation
		jointDef.upperAngle = degreesToRadians(140.f);
		jointDef.maxMotorTorque = 100.0f;
		jointDef.motorSpeed = 0.0f;
		jointDef.enableMotor = true;		
		obj2_joints[2] = (b2RevoluteJoint*)world->CreateJoint(&jointDef);
		jointDef.bodyA = obj2_parts[3];
		jointDef.bodyB = obj2_parts[1];
		obj2_joints[3] = (b2RevoluteJoint*)world->CreateJoint(&jointDef);
		//end
	}

	if (selected_model == MODEL_3)
	{
		b2BodyDef bodyDef;
		bodyDef.type = b2_dynamicBody;
		bodyDef.position.Set(0.0f, 0.05f);
		obj3_parts[0] = world->CreateBody(&bodyDef);			//back feet
		obj3_parts[1] = world->CreateBody(&bodyDef);			//front feet
		bodyDef.position.Set(0.0f, 0.34f);
		obj3_parts[2] = world->CreateBody(&bodyDef);			//back low leg
		obj3_parts[3] = world->CreateBody(&bodyDef);			//front  low leg
		bodyDef.position.Set(0.0f, 0.78f);
		obj3_parts[4] = world->CreateBody(&bodyDef);			//back upper leg
		obj3_parts[5] = world->CreateBody(&bodyDef);			//front  upper leg
		bodyDef.position.Set(0.0f, 1.33f);
		obj3_parts[6] = world->CreateBody(&bodyDef);			//torso
		bodyDef.position.Set(0.0f, 1.48f);
		obj3_parts[7] = world->CreateBody(&bodyDef);			//front upper arm
		obj3_parts[8] = world->CreateBody(&bodyDef);			//back
		bodyDef.position.Set(0.0f, 1.13f);
		obj3_parts[9] = world->CreateBody(&bodyDef);			//front lower arm
		obj3_parts[10] = world->CreateBody(&bodyDef);
		bodyDef.position.Set(0.0f, 1.84f);					//head
		obj3_parts[11] = world->CreateBody(&bodyDef);

		b2PolygonShape up_leg;
		b2PolygonShape low_leg;
		b2PolygonShape torso;
		b2PolygonShape up_arm;
		b2PolygonShape low_arm;
		b2PolygonShape head;
		b2PolygonShape feet;

		feet.Set(obj3_feet, 5);
		low_leg.Set(obj3_lower_leg, 6);
		up_leg.Set(obj3_upper_leg, 6);
		torso.Set(obj3_torso, 8);
		low_arm.Set(obj3_lower_arm, 4);
		up_arm.Set(obj3_upper_arm, 6);
		head.Set(obj3_head, 8);

		b2FixtureDef fixtureDef;
		fixtureDef.density = 160.f;
		fixtureDef.friction = objects_friction;
		fixtureDef.filter.categoryBits = 0x0002;
		fixtureDef.filter.maskBits = 0x0020;
		fixtureDef.shape = &feet;
		obj3_parts[0]->CreateFixture(&fixtureDef);
		obj3_parts[1]->CreateFixture(&fixtureDef);
		fixtureDef.shape = &low_leg;
		obj3_parts[2]->CreateFixture(&fixtureDef);
		obj3_parts[3]->CreateFixture(&fixtureDef);
		fixtureDef.shape = &up_leg;
		obj3_parts[4]->CreateFixture(&fixtureDef);
		obj3_parts[5]->CreateFixture(&fixtureDef);
		fixtureDef.shape = &torso;
		obj3_parts[6]->CreateFixture(&fixtureDef);
		fixtureDef.shape = &up_arm;
		obj3_parts[7]->CreateFixture(&fixtureDef);
		obj3_parts[8]->CreateFixture(&fixtureDef);
		fixtureDef.shape = &low_arm;
		obj3_parts[9]->CreateFixture(&fixtureDef);
		obj3_parts[10]->CreateFixture(&fixtureDef);
		fixtureDef.shape = &head;
		obj3_parts[11]->CreateFixture(&fixtureDef);
		
		b2RevoluteJointDef jointDef;
		jointDef.bodyA = obj3_parts[2];
		jointDef.bodyB = obj3_parts[0];
		jointDef.localAnchorB = b2Vec2(0.0f, 0.05f);
		jointDef.localAnchorA = b2Vec2(0.0f, -0.24f);
		jointDef.lowerAngle = degreesToRadians(-40.f);			//feet rotation
		jointDef.upperAngle = degreesToRadians(30.f);
		jointDef.enableLimit = true;
		jointDef.maxMotorTorque = 600.0f;
		jointDef.motorSpeed = 0.0f;
		jointDef.enableMotor = true;
		obj3_joints[0] = (b2RevoluteJoint*)world->CreateJoint(&jointDef);
		jointDef.bodyA = obj3_parts[3];
		jointDef.bodyB = obj3_parts[1];
		obj3_joints[1] = (b2RevoluteJoint*)world->CreateJoint(&jointDef);
		jointDef.lowerAngle = degreesToRadians(-140.f);			//knee rotation
		jointDef.upperAngle = degreesToRadians(2.f);
		jointDef.bodyA = obj3_parts[4];
		jointDef.bodyB = obj3_parts[2];
		jointDef.localAnchorB = b2Vec2(0.0f, 0.24f);
		jointDef.localAnchorA = b2Vec2(0.0f, -0.2f);
		obj3_joints[2] = (b2RevoluteJoint*)world->CreateJoint(&jointDef);
		jointDef.bodyA = obj3_parts[5];
		jointDef.bodyB = obj3_parts[3];
		obj3_joints[3] = (b2RevoluteJoint*)world->CreateJoint(&jointDef);
		jointDef.lowerAngle = degreesToRadians(-60.f);			//hip rotation
		jointDef.upperAngle = degreesToRadians(130.f);
		jointDef.bodyA = obj3_parts[6];
		jointDef.bodyB = obj3_parts[4];
		jointDef.localAnchorB = b2Vec2(0.0f, 0.2f);
		jointDef.localAnchorA = b2Vec2(0.0f, -0.35f);
		obj3_joints[4] = (b2RevoluteJoint*)world->CreateJoint(&jointDef);
		jointDef.bodyA = obj3_parts[6];
		jointDef.bodyB = obj3_parts[5];
		obj3_joints[5] = (b2RevoluteJoint*)world->CreateJoint(&jointDef);
		jointDef.lowerAngle = degreesToRadians(-100.f);			//arm rotation
		jointDef.upperAngle = degreesToRadians(170.f);
		jointDef.bodyA = obj3_parts[6];
		jointDef.bodyB = obj3_parts[8];
		jointDef.localAnchorB = b2Vec2(0.0f, 0.15f);
		jointDef.localAnchorA = b2Vec2(0.0f, 0.3f);
		obj3_joints[6] = (b2RevoluteJoint*)world->CreateJoint(&jointDef);
		jointDef.bodyA = obj3_parts[6];
		jointDef.bodyB = obj3_parts[7];
		obj3_joints[7] = (b2RevoluteJoint*)world->CreateJoint(&jointDef);
		jointDef.lowerAngle = degreesToRadians(-0.f);			//elbow rotation
		jointDef.upperAngle = degreesToRadians(130.f);
		jointDef.bodyA = obj3_parts[8];
		jointDef.bodyB = obj3_parts[10];
		jointDef.localAnchorA = b2Vec2(0.0f, -0.15f);
		jointDef.localAnchorB = b2Vec2(0.0f, 0.2f);
		obj3_joints[8] = (b2RevoluteJoint*)world->CreateJoint(&jointDef);
		jointDef.bodyA = obj3_parts[7];
		jointDef.bodyB = obj3_parts[9];
		obj3_joints[9] = (b2RevoluteJoint*)world->CreateJoint(&jointDef);
		jointDef.lowerAngle = degreesToRadians(-60.f);			//head rotation
		jointDef.upperAngle = degreesToRadians(60.f);
		jointDef.bodyA = obj3_parts[11];
		jointDef.bodyB = obj3_parts[6];
		jointDef.localAnchorB = b2Vec2(0.0f, 0.4f);
		jointDef.localAnchorA = b2Vec2(0.0f, -0.11f);
		obj3_joints[10] = (b2RevoluteJoint*)world->CreateJoint(&jointDef);
	}

	if (selected_model == MODEL_4)
	{
		b2BodyDef bodyDef;
		bodyDef.type = b2_dynamicBody;
		bodyDef.allowSleep = false;
		bodyDef.position.Set(-0.6f, 0.11f);
		obj4_parts[0] = world->CreateBody(&bodyDef);
		bodyDef.position.Set(-0.3f, 0.11f);
		obj4_parts[1] = world->CreateBody(&bodyDef);
		bodyDef.position.Set(0.0f, 0.11f);
		obj4_parts[2] = world->CreateBody(&bodyDef);
		bodyDef.position.Set(0.3f, 0.11f);
		obj4_parts[3] = world->CreateBody(&bodyDef);
		bodyDef.position.Set(0.6f, 0.11f);
		obj4_parts[4] = world->CreateBody(&bodyDef);

		b2PolygonShape ob4_cell;
		ob4_cell.Set(obj4_cellshape, 8);

		b2FixtureDef fixtureDef;
		fixtureDef.shape = &ob4_cell;
		fixtureDef.density = 10.0f;
		fixtureDef.friction = objects_friction;
		fixtureDef.filter.categoryBits = 0x0002;
		fixtureDef.filter.maskBits = 0x0020;
		obj4_parts[0]->CreateFixture(&fixtureDef);
		obj4_parts[1]->CreateFixture(&fixtureDef);
		obj4_parts[2]->CreateFixture(&fixtureDef);
		obj4_parts[3]->CreateFixture(&fixtureDef);
		obj4_parts[4]->CreateFixture(&fixtureDef);

		b2RevoluteJointDef jointDef2;
		jointDef2.bodyA = obj4_parts[0];
		jointDef2.bodyB = obj4_parts[1];
		jointDef2.localAnchorA = b2Vec2(0.15f, 0.0f);
		jointDef2.localAnchorB = b2Vec2(-0.15f, 0.0f);
		jointDef2.lowerAngle = degreesToRadians(-130.f);	//rotation limit
		jointDef2.upperAngle = degreesToRadians(130.f);
		jointDef2.enableLimit = true;
		jointDef2.enableMotor = true;
		jointDef2.maxMotorTorque = 20.f;
		jointDef2.motorSpeed = 0.0f;
		obj4_joints[0] = (b2RevoluteJoint*)world->CreateJoint(&jointDef2);
		jointDef2.bodyA = obj4_parts[1];
		jointDef2.bodyB = obj4_parts[2];
		obj4_joints[1] = (b2RevoluteJoint*)world->CreateJoint(&jointDef2);
		jointDef2.bodyA = obj4_parts[2];
		jointDef2.bodyB = obj4_parts[3];
		obj4_joints[2] = (b2RevoluteJoint*)world->CreateJoint(&jointDef2);
		jointDef2.bodyA = obj4_parts[3];
		jointDef2.bodyB = obj4_parts[4];
		obj4_joints[3] = (b2RevoluteJoint*)world->CreateJoint(&jointDef2);
		//end
	}

	if (selected_model == MODEL_5)
	{
		b2BodyDef bodyDef;
		bodyDef.type = b2_dynamicBody;
		bodyDef.allowSleep = false;
		b2Vec2 pos( -1.2f, 0.09f);
		for (int i = 0; i < obj5_num_parts; i++)
		{
			bodyDef.position.Set(pos.x, pos.y);
			obj5_parts[i] = world->CreateBody(&bodyDef);
			pos.x += 0.2f;
		}
		b2PolygonShape ob5_cell;
		ob5_cell.Set(obj5_cellshape, 8);

		b2FixtureDef fixtureDef;
		fixtureDef.shape = &ob5_cell;
		fixtureDef.density = 10.0f;
		fixtureDef.friction = objects_friction;
		fixtureDef.filter.categoryBits = 0x0002;
		fixtureDef.filter.maskBits = 0x0020;
		for (int i = 0; i < obj5_num_parts; i++)
		{
			obj5_parts[i]->CreateFixture(&fixtureDef);
		}

		b2RevoluteJointDef jointDef2;
		jointDef2.localAnchorA = b2Vec2(0.1f, 0.0f);
		jointDef2.localAnchorB = b2Vec2(-0.1f, 0.0f);
		jointDef2.lowerAngle = degreesToRadians(-130.f);	//rotation limit
		jointDef2.upperAngle = degreesToRadians(130.f);
		jointDef2.enableLimit = true;
		jointDef2.enableMotor = true;
		jointDef2.maxMotorTorque = 20.f;
		jointDef2.motorSpeed = 0.0f;
		for (int i = 0; i < obj5_num_joints; i++)
		{
			jointDef2.bodyA = obj5_parts[i];
			jointDef2.bodyB = obj5_parts[i+1];
			obj5_joints[i] = (b2RevoluteJoint*)world->CreateJoint(&jointDef2);
		}
	}

	if (selected_model == MODEL_6)
	{
		b2BodyDef bodyDef;
		bodyDef.type = b2_dynamicBody;
		bodyDef.position.Set(0.0f, 0.84f);
		obj6_parts[4] = world->CreateBody(&bodyDef);
		bodyDef.position.Set(-0.4f, 0.64f);
		obj6_parts[2] = world->CreateBody(&bodyDef);
		bodyDef.position.Set(-0.4f, 0.24f);
		obj6_parts[0] = world->CreateBody(&bodyDef);
		bodyDef.position.Set(0.4f, 0.64f);
		obj6_parts[3] = world->CreateBody(&bodyDef);
		bodyDef.position.Set(0.4f, 0.24f);
		obj6_parts[1] = world->CreateBody(&bodyDef);
		bodyDef.position.Set(0.58f, 1.07f);
		obj6_parts[5] = world->CreateBody(&bodyDef);
		//	2	4	3
		//	0		1

		b2CircleShape b2_circle1;
		b2PolygonShape b2_up_leg;
		b2PolygonShape b2_low_leg;
		b2PolygonShape b2_torso;
		b2PolygonShape b2_head;

		b2_circle1.m_radius = 0.03f;
		b2_circle1.m_p.Set(0.0f, -0.2f);
		b2_up_leg.Set(upper_leg, 6);
		b2_low_leg.Set(lower_leg, 4);
		b2_torso.Set(torso_v2, 8);
		b2_head.Set(head, 7);

		b2FixtureDef fixtureDef;
		fixtureDef.shape = &b2_torso;
		fixtureDef.density = 30.f;
		fixtureDef.friction = 0.3f;
		fixtureDef.filter.categoryBits = 0x0002;
		fixtureDef.filter.maskBits = 0x0020;
		obj6_parts[4]->CreateFixture(&fixtureDef);
		fixtureDef.shape = &b2_head;
		obj6_parts[5]->CreateFixture(&fixtureDef);
		fixtureDef.shape = &b2_up_leg;
		fixtureDef.density = 20.f;
		fixtureDef.friction = objects_friction;
		fixtureDef.filter.categoryBits = 0x0004;
		fixtureDef.filter.maskBits = 0x0024;
		obj6_parts[2]->CreateFixture(&fixtureDef);
		obj6_parts[3]->CreateFixture(&fixtureDef);
		fixtureDef.shape = &b2_low_leg;
		obj6_parts[0]->CreateFixture(&fixtureDef);
		obj6_parts[1]->CreateFixture(&fixtureDef);
		fixtureDef.shape = &b2_circle1;
		obj6_parts[0]->CreateFixture(&fixtureDef);
		obj6_parts[1]->CreateFixture(&fixtureDef);

		b2RevoluteJointDef jointDef;
		jointDef.bodyA = obj6_parts[4];
		jointDef.bodyB = obj6_parts[2];
		jointDef.localAnchorA = b2Vec2(-0.4f, 0.0f);
		jointDef.localAnchorB = b2Vec2(0.0f, 0.2f);
		jointDef.lowerAngle = degreesToRadians(-90.f);		//upper joints
		jointDef.upperAngle = degreesToRadians(90.f);
		jointDef.enableLimit = true;
		jointDef.maxMotorTorque = 100.0f;
		jointDef.motorSpeed = 0.0f;
		jointDef.enableMotor = true;
		obj6_joints[0] = (b2RevoluteJoint*)world->CreateJoint(&jointDef);
		jointDef.bodyB = obj6_parts[3];
		jointDef.localAnchorA = b2Vec2(0.4f, 0.0f);
		obj6_joints[1] = (b2RevoluteJoint*)world->CreateJoint(&jointDef);
		jointDef.bodyA = obj6_parts[2];
		jointDef.bodyB = obj6_parts[0];
		jointDef.localAnchorA = b2Vec2(0.0f, -0.2f);
		jointDef.localAnchorB = b2Vec2(0.0f, 0.2f);
		jointDef.lowerAngle = degreesToRadians(-140.f);				//knee joints rotation
		jointDef.upperAngle = degreesToRadians(0.f);
		jointDef.maxMotorTorque = 100.0f;
		jointDef.motorSpeed = 0.0f;
		jointDef.enableMotor = true;
		obj6_joints[2] = (b2RevoluteJoint*)world->CreateJoint(&jointDef);
		jointDef.bodyA = obj6_parts[3];
		jointDef.bodyB = obj6_parts[1];
		obj6_joints[3] = (b2RevoluteJoint*)world->CreateJoint(&jointDef);
		jointDef.bodyA = obj6_parts[4];
		jointDef.bodyB = obj6_parts[5];
		jointDef.localAnchorA = b2Vec2(0.45f, 0.15f);
		jointDef.localAnchorB = b2Vec2(-0.13f, -0.08f);
		jointDef.lowerAngle = degreesToRadians(-50.f);				//head
		jointDef.upperAngle = degreesToRadians(50.f);
		obj6_joints[4] = (b2RevoluteJoint*)world->CreateJoint(&jointDef);
		//end
	}
	// ...
	// ...
}

void PhysicsWorld::destroyMechanism() 
{
	//only selected model can be destroyed

	if (selected_model == MODEL_1) 
	{
		for (size_t i = 0; i < obj1_num_joints; i++)
		{
			world->DestroyJoint(obj1_joints[i]);
			obj1_joints[i] = nullptr;
		}
		for (size_t i = 0; i < obj1_num_parts; i++)
		{
			world->DestroyBody(obj1_parts[i]);
			obj1_parts[i] = nullptr;
		}
	}

	if (selected_model == MODEL_2)
	{
		for (size_t i = 0; i < obj2_num_joints; i++)
		{
			world->DestroyJoint(obj2_joints[i]);
			obj2_joints[i] = nullptr;
		}
		for (size_t i = 0; i < obj2_num_parts; i++)
		{
			world->DestroyBody(obj2_parts[i]);
			obj2_parts[i] = nullptr;
		}
	}

	if (selected_model == MODEL_3)
	{
		for (size_t i = 0; i < obj3_num_joints; i++)
		{
			world->DestroyJoint(obj3_joints[i]);
			obj3_joints[i] = nullptr;
		}
		for (size_t i = 0; i < obj3_num_parts; i++)
		{
			world->DestroyBody(obj3_parts[i]);
			obj3_parts[i] = nullptr;
		}
	}

	if (selected_model == MODEL_4)
	{
		for (size_t i = 0; i < obj4_num_joints; i++)
		{
			world->DestroyJoint(obj4_joints[i]);
			obj4_joints[i] = nullptr;
		}
		for (size_t i = 0; i < obj4_num_parts; i++)
		{
			world->DestroyBody(obj4_parts[i]);
			obj4_parts[i] = nullptr;
		}
	}

	if (selected_model == MODEL_5)
	{
		for (size_t i = 0; i < obj5_num_joints; i++)
		{
			world->DestroyJoint(obj5_joints[i]);
			obj5_joints[i] = nullptr;
		}
		for (size_t i = 0; i < obj5_num_parts; i++)
		{
			world->DestroyBody(obj5_parts[i]);
			obj5_parts[i] = nullptr;
		}
	}

	if (selected_model == MODEL_6)
	{
		for (size_t i = 0; i < obj6_num_joints; i++)
		{
			world->DestroyJoint(obj6_joints[i]);
			obj6_joints[i] = nullptr;
		}
		for (size_t i = 0; i < obj6_num_parts; i++)
		{
			world->DestroyBody(obj6_parts[i]);
			obj6_parts[i] = nullptr;
		}
	}
	//...
	//...
}

void PhysicsWorld::resetMechanism()
{	
	// reset model position

	if (selected_model == MODEL_1)
	{
		for (size_t i = 0; i < obj1_num_joints; i++)
		{
			obj1_joints[i]->SetMotorSpeed(0.0f);
			obj1_joints[i]->SetMaxMotorTorque(0.0f);
		}
		world->Step(TIMESTEP, VELOCITYITER, POSITIONITER);
		for (size_t i = 0; i < obj1_num_joints; i++)
		{
			obj1_joints[i]->SetMaxMotorTorque(100.f);
		}
		b2Vec2 zeros(0.0f, 0.0f);
		for (size_t i = 0; i < obj1_num_parts; i++)
		{
			obj1_parts[i]->SetAngularVelocity(0.0f);
			obj1_parts[i]->SetLinearVelocity(zeros);
		}
		obj1_parts[4]->SetTransform(b2Vec2(0.0f, 0.84f), 0.0f);
		obj1_parts[5]->SetTransform(b2Vec2(-0.4f, 0.64f), 0.0f);
		obj1_parts[6]->SetTransform(b2Vec2(-0.4f, 0.24f), 0.0f);
		obj1_parts[0]->SetTransform(b2Vec2(-0.4f, 0.64f), 0.0f);
		obj1_parts[1]->SetTransform(b2Vec2(-0.4f, 0.24f), 0.0f);
		obj1_parts[7]->SetTransform(b2Vec2(0.4f, 0.64f), 0.0f);
		obj1_parts[8]->SetTransform(b2Vec2(0.4f, 0.24f), 0.0f);
		obj1_parts[2]->SetTransform(b2Vec2(0.4f, 0.64f), 0.0f);
		obj1_parts[3]->SetTransform(b2Vec2(0.4f, 0.24f), 0.0f);
	}

	if (selected_model == MODEL_2)
	{
		for (size_t i = 0; i < obj2_num_joints; i++)
		{
			obj2_joints[i]->SetMotorSpeed(0.0f);
			obj2_joints[i]->SetMaxMotorTorque(0.0f);
		}
		world->Step(TIMESTEP, VELOCITYITER, POSITIONITER);
		for (size_t i = 0; i < obj2_num_joints; i++)
		{
			obj2_joints[i]->SetMaxMotorTorque(100.f);
		}
		b2Vec2 zeros(0.0f, 0.0f);
		for (size_t i = 0; i < obj2_num_parts; i++)
		{
			obj2_parts[i]->SetAngularVelocity(0.0f);
			obj2_parts[i]->SetLinearVelocity(zeros);
		}
		obj2_parts[4]->SetTransform(b2Vec2(0.0f, 0.84f), 0.0f);
		obj2_parts[2]->SetTransform(b2Vec2(-0.4f, 0.64f), 0.0f);
		obj2_parts[0]->SetTransform(b2Vec2(-0.4f, 0.24f), 0.0f);
		obj2_parts[3]->SetTransform(b2Vec2(0.4f, 0.64f), 0.0f);
		obj2_parts[1]->SetTransform(b2Vec2(0.4f, 0.24f), 0.0f);
	}

	if (selected_model == MODEL_3)
	{
		for (size_t i = 0; i < obj3_num_joints; i++)
		{
			obj3_joints[i]->SetMotorSpeed(0.0f);
			obj3_joints[i]->SetMaxMotorTorque(0.0f);
		}
		world->Step(TIMESTEP, VELOCITYITER, POSITIONITER);
		for (size_t i = 0; i < obj3_num_joints; i++)
		{
			obj3_joints[i]->SetMaxMotorTorque(600.f);
		}
		b2Vec2 zeros(0.0f, 0.0f);
		for (size_t i = 0; i < obj3_num_parts; i++)
		{
			obj3_parts[i]->SetAngularVelocity(0.0f);
			obj3_parts[i]->SetLinearVelocity(zeros);
		}
		obj3_parts[0]->SetTransform(b2Vec2(0.0f, 0.05f), 0.0f);
		obj3_parts[1]->SetTransform(b2Vec2(0.0f, 0.05f), 0.0f);
		obj3_parts[2]->SetTransform(b2Vec2(0.0f, 0.34f), 0.0f);
		obj3_parts[3]->SetTransform(b2Vec2(0.0f, 0.34f), 0.0f);
		obj3_parts[4]->SetTransform(b2Vec2(0.0f, 0.78f), 0.0f);
		obj3_parts[5]->SetTransform(b2Vec2(0.0f, 0.78f), 0.0f);
		obj3_parts[6]->SetTransform(b2Vec2(0.0f, 1.33f), 0.0f);
		obj3_parts[7]->SetTransform(b2Vec2(0.0f, 1.48f), 0.0f);
		obj3_parts[8]->SetTransform(b2Vec2(0.0f, 1.48f), 0.0f);
		obj3_parts[9]->SetTransform(b2Vec2(0.0f, 1.13f), 0.0f);
		obj3_parts[10]->SetTransform(b2Vec2(0.0f, 1.13f), 0.0f);
		obj3_parts[11]->SetTransform(b2Vec2(0.0f, 1.84f), 0.0f);
	}

	if (selected_model == MODEL_4)
	{
		for (size_t i = 0; i < obj4_num_joints; i++)
		{
			obj4_joints[i]->SetMotorSpeed(0.0f);
			obj4_joints[i]->SetMaxMotorTorque(0.0f);
		}
		world->Step(TIMESTEP, VELOCITYITER, POSITIONITER);
		for (size_t i = 0; i < obj4_num_joints; i++)
		{
			obj4_joints[i]->SetMaxMotorTorque(20.0f);
		}
		b2Vec2 zeros(0.0f, 0.0f);
		for (size_t i = 0; i < obj4_num_parts; i++)
		{
			obj4_parts[i]->SetAngularVelocity(0.0f);
			obj4_parts[i]->SetLinearVelocity(zeros);
		}
		obj4_parts[0]->SetTransform(b2Vec2(-0.6f, 0.11f), 0.0f);
		obj4_parts[1]->SetTransform(b2Vec2(-0.3f, 0.11f), 0.0f);
		obj4_parts[2]->SetTransform(b2Vec2(0.0f, 0.11f), 0.0f);
		obj4_parts[3]->SetTransform(b2Vec2(0.3f, 0.11f), 0.0f);
		obj4_parts[4]->SetTransform(b2Vec2(0.6f, 0.11f), 0.0f);
	}

	if (selected_model == MODEL_5)
	{
		for (size_t i = 0; i < obj5_num_joints; i++)
		{
			obj5_joints[i]->SetMotorSpeed(0.0f);
			//obj5_joints[i]->SetMaxMotorTorque(0.0f);
		}
		b2Vec2 zeros(0.0f, 0.0f);
		for (size_t i = 0; i < obj5_num_parts; i++)
		{
			obj5_parts[i]->SetAngularVelocity(0.0f);
			obj5_parts[i]->SetLinearVelocity(zeros);
		}
		//world->Step(TIMESTEP, VELOCITYITER, POSITIONITER);
		/*for (size_t i = 0; i < obj5_num_joints; i++)
		{
			obj5_joints[i]->SetMaxMotorTorque(20.0f);
		}*/
		b2Vec2 pos(-1.2f, 0.09f);
		for (int i = 0; i < obj5_num_parts; i++)
		{
			obj5_parts[i]->SetTransform(pos,0.0f);
			pos.x += 0.2f;
		}
	}

	if (selected_model == MODEL_6)
	{
		for (size_t i = 0; i < obj6_num_joints; i++)
		{
			obj6_joints[i]->SetMotorSpeed(0.0f);
			//obj6_joints[i]->SetMaxMotorTorque(0.0f);
		}
		b2Vec2 zeros(0.0f, 0.0f);
		for (size_t i = 0; i < obj6_num_parts; i++)
		{
			obj6_parts[i]->SetAngularVelocity(0.0f);
			obj6_parts[i]->SetLinearVelocity(zeros);
		}
		//world->Step(TIMESTEP, VELOCITYITER, POSITIONITER);
		//for (size_t i = 0; i < obj6_num_joints; i++)
		//{
		//	obj6_joints[i]->SetMaxMotorTorque(100.f);
		//}
		obj6_parts[4]->SetTransform(b2Vec2(0.0f, 0.84f), 0.0f);
		obj6_parts[2]->SetTransform(b2Vec2(-0.4f, 0.64f), 0.0f);
		obj6_parts[0]->SetTransform(b2Vec2(-0.4f, 0.24f), 0.0f);
		obj6_parts[3]->SetTransform(b2Vec2(0.4f, 0.64f), 0.0f);
		obj6_parts[1]->SetTransform(b2Vec2(0.4f, 0.24f), 0.0f);
		obj6_parts[5]->SetTransform(b2Vec2(0.58f, 1.07f), 0.0f);
	}

	//float t = world->GetTreeQuality();
}

void PhysicsWorld::AddRemoveObstacleTestobj()
{
	// --- obstacle ----
	if (obstacles == true && groundBody2 == nullptr)
	{
		b2BodyDef groundBodyDef2;
		groundBodyDef2.position.Set(3.0f, 0.1f);
		groundBodyDef2.type = b2_staticBody;
		groundBody2 = world->CreateBody(&groundBodyDef2);

		b2PolygonShape groundBox2;
		groundBox2.SetAsBox(0.1f, 0.1f);

		b2FixtureDef groundfixtureDef2;
		groundfixtureDef2.shape = &groundBox2;
		groundfixtureDef2.filter.categoryBits = 0x0020;
		groundBody2->CreateFixture(&groundfixtureDef2);
	}
	if (obstacles == false && groundBody2 != nullptr)
	{ 
		world->DestroyBody(groundBody2);
		groundBody2 = nullptr;
	}
	//test objects
	if (add_test_objects == true && testball == nullptr && testbox == nullptr)
	{
		b2BodyDef bodyDef;
		bodyDef.type = b2_dynamicBody;
		bodyDef.allowSleep = false;
		bodyDef.position.Set(1.f, 1.f);
		testball = world->CreateBody(&bodyDef);
		bodyDef.position.Set(-1.f, 1.f);
		testbox = world->CreateBody(&bodyDef);
		b2PolygonShape ob2_box;
		b2CircleShape circle;
		circle.m_radius = 0.1f;
		ob2_box.SetAsBox(0.1f, 0.1f);
		b2FixtureDef fixtureDef;
		fixtureDef.density = 1.0f;
		fixtureDef.friction = 0.8f;
		fixtureDef.filter.categoryBits = 0x0020;
		fixtureDef.shape = &ob2_box;
		testbox->CreateFixture(&fixtureDef);
		fixtureDef.shape = &circle;
		testball->CreateFixture(&fixtureDef);
	}
	if (add_test_objects == false && testball != nullptr && testbox != nullptr)
	{
		world->DestroyBody(testball);
		world->DestroyBody(testbox);
		testball = nullptr;
		testbox = nullptr;
	}
}

void PhysicsWorld::CheckAngleLimits(void)
{
	for (b2RevoluteJoint* jn = (b2RevoluteJoint*)world->GetJointList(); jn; jn = (b2RevoluteJoint*)jn->GetNext())
	{
		if (jn->IsLimitEnabled())
		{
			if (jn->GetJointAngle() <= (jn->GetLowerLimit() + margin) && jn->GetMotorSpeed() < 0.0f)
				jn->SetMotorSpeed(0.0f);
			if (jn->GetJointAngle() >= (jn->GetUpperLimit() - margin) && jn->GetMotorSpeed() > 0.0f)
				jn->SetMotorSpeed(0.0f);
		}
	}
	
}