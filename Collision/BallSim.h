#ifndef BALLSIM_H
#define BALLSIM_H

#include "Mesh.h"
#include "Model.h"
#include "Input.h"
#include "FX.h"
#include "SpriteBatch.h"
#include "SpriteFont.h"

class BallSim
{
public:
	//setup once
	void Initialise(MeshManager& mgr);
	//process input
	void Input(char key);
	//start or restart a behaviour
	void Start();
	//update the physics every frame
	void Update(float dTime, const DirectX::SimpleMath::Vector3& camPos, MouseAndKeys& input, Model& rock);
	//render the scene
	void Render(FX::MyFX& fx, float dTime);
	//render text on screen
	void RenderText(DirectX::SpriteFont *pF, DirectX::SpriteBatch *pBatch);

private:

	Model mBall;	//bouncing ball
	Model mCube;	//cube to hit
	//velocity
	DirectX::SimpleMath::Vector3 mVel = DirectX::SimpleMath::Vector3(0, 0, 0);
	//gravity acceleration
	DirectX::SimpleMath::Vector3 mGrav = DirectX::SimpleMath::Vector3(0, 0, 0);
	//linear acceleration
	DirectX::SimpleMath::Vector3 mAccel = DirectX::SimpleMath::Vector3(0, 0, 0);
	//coefficient of restitution or friction - a breaking or energy sapping constant
	float mCOR = 1;
	//radius of the ball
	float mRadius = 0.1f;
	//what behaviour are we simulating?
	typedef enum { WAITING, LINEAR, ACCEL, BOUNCE, BOUNCE2, CUBE } Mode;
	//the mode we are in now
	Mode mMode, mLastMode;
};

#endif
