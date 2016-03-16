#ifndef ENEMY_H
#define ENEMY_H

#include "SpriteBatch.h"
#include "SpriteFont.h"
#include "Mesh.h"
#include "Model.h"
#include "FX.h"

class Enemy{
public:
	//setup 
	void Initialise(MeshManager & mgr);
	//start or restart a behaviour
	void Start();
	//update the physics every frame
	void Update(float dTime, const DirectX::SimpleMath::Vector3& camPos, Model& rock);
	//render the scene
	void Render(FX::MyFX& fx, float dTime);
private:
	Model mEnemy;	//enemy 
	//velocity 
	DirectX::SimpleMath::Vector3 mVel = DirectX::SimpleMath::Vector3(0, 0, 0);
	//linear acceleration
	DirectX::SimpleMath::Vector3 mAccel = DirectX::SimpleMath::Vector3(0, 0, 0);
	//cooefficient of restitution or friction  a breaking or energy sapping constant
	float mCOR = 1;
	//radius of the ship
	float mRadius = 0.5f;
	//what behaviour are we simulating?
	typedef enum {FIRE, DEAD, FLYING} Mode;
	//the mode we are in now
	Mode mMode, mLastMode;
};

#endif