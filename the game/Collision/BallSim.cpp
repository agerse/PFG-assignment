#include <string>
#include <iomanip>

#include "BallSim.h"
#include "D3D.h"

using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;
float mAngle = 5.f;

void BallSim::Initialise(MeshManager& mgr)
{
	Mesh *p = mgr.GetMesh("sphere");
	assert(p);
	mBall.Initialise(*p);
	mBall.GetScale() = Vector3(0.1f, 0.1f, 0.1f);
	MaterialExt mat;
	mat.gfxData.Set(Vector4(0, 0, 1, 1), Vector4(0, 0, 1, 1), Vector4(0, 0, 1, 1));
	mat.flags &= MaterialExt::TFlags::LIT;
	mat.flags &= ~MaterialExt::TFlags::CCW_WINDING;
	mBall.SetOverrideMat(&mat);
	mLastMode = mMode = WAITING;
	p = mgr.GetMesh("cube");
	assert(p);
	mCube.Initialise(*p);
	Start();
}

void BallSim::Input(MouseAndKeys input)
{
	Mode movementMode = mLastMode;
	//switch (key)
	//{
	//case '1':
	//	movementMode = WAITING;
	//	break;
	//case 'a':
	//case 'A':
	//	movementMode = LEFT;
	//	break;
	//case 'd':
	//case 'D':
	//	movementMode = RIGHT;
	//	break;
	//case 'w':
	//case 'W':
	//	movementMode = UP;
	//	break;
	//case 's':
	//case 'S':
	//	movementMode = DOWN;
	//	break;
	//default:
	//	//movementMode = NONE;
	//	return;
	//}
	if (input.IsPressed(VK_A))
		movementMode = LEFT;
	else if (input.IsPressed(VK_W))
		movementMode = UP;
	else if (input.IsPressed(VK_D))
		movementMode = RIGHT;
	else if (input.IsPressed(VK_S))
		movementMode = DOWN;	
	else movementMode = WAITING;
	if (input.IsPressed(VK_W) && input.IsPressed(VK_A))
		movementMode = UPLEFT;
	if (input.IsPressed(VK_W) && input.IsPressed(VK_D))
		movementMode = UPRIGHT;
	if (input.IsPressed(VK_S) && input.IsPressed(VK_A))
		movementMode = DOWNLEFT;
	if (input.IsPressed(VK_S) && input.IsPressed(VK_D))
		movementMode = DOWNRIGHT;

	mLastMode = mMode;
	mMode = movementMode;
	Start();
}

void BallSim::Start()
{
	switch (mMode)
	{

		//When the player hits the up arrow
	case UP:
		mVel = Vector3(0, 2, 0);
		break;
		//When the player hits the left arrow
	case LEFT:
		mVel = Vector3(-2, 0, 0);
		break;
		//When the player hits the right arrow
	case RIGHT:
		mVel = Vector3(2, 0, 0);
		break;
		//When the player hits the down arrow
	case DOWN:
		mVel = Vector3(0, -2, 0);
		break;
	case UPLEFT:
		mVel = Vector3(-2, 2, 0);
		break;
	case UPRIGHT:
		mVel = Vector3(2, 2, 0);
		break;
	case DOWNLEFT:
		mVel = Vector3(-2, -2, 0);
		break;
	case DOWNRIGHT:
		mVel = Vector3(2, -2, 0);
		break;
	}

	mCOR = 0.9965f;
}

void BallSim::Update(float dTime, const Vector3& camPos, MouseAndKeys& input, Model& rock)
{
	mAngle += dTime * 0.5f;
	Vector3 pos = mBall.GetPosition();
	pos += mVel * dTime;
	mBall.GetPosition() = pos;

	if (mVel.LengthSquared() < 0.001f && fabsf(pos.y - mRadius) < 0.001f)
		mVel = Vector3(0, 0, 0);
	else
	{
		mVel += mAccel * dTime;
	}
	mVel *= mCOR;
}

void BallSim::Render(FX::MyFX& fx, float dTime)
{
	float alpha = 0.5f + sinf(mAngle * 2) * 0.5f;
	Vector3 mBallColour = Vector3(10.f, 10.f, 10.f);
	FX::SetupPointLight(1, true, mBall.GetPosition(), mBallColour, Vector3(0.f, 0.5f, 0.5f), Vector3(0, 0, 0), 15.f, 1.f);
	fx.Render(mBall, gd3dImmediateContext);
}

void BallSim::RenderText(SpriteFont *pF, SpriteBatch *pBatch)
{
	wstring mssg;
	/*switch (mMode)
	{
	case WAITING:
	mssg = L"mode = Waiting";
	break;
	case LINEAR:
	mssg = L"mode = Linear, no gravity";
	break;
	case ACCEL:
	mssg = L"mode = linear, acceleration";
	break;
	case BOUNCE:
	mssg = L"mode = bounce";
	break;
	case BOUNCE2:
	mssg = L"mode = bounce with friction";
	break;
	}*/

	pF->DrawString(pBatch, mssg.c_str(), Vector2(10, 10), Colours::White, 0, Vector2(0, 0));

	/*mssg = L"Keys: x=restart, 1=waiting 2=linear, 3=acceleration, 4=bounce, 5=bounce with friction";
	pF->DrawString(pBatch, mssg.c_str(), Vector2(10, 30), Colours::White, 0, Vector2(0, 0), Vector2(0.75f, 0.75f));*/

	/*wstringstream ss;
	ss << std::setprecision(3);
	ss << L"Velocity=" << mVel.x << L", " << mVel.y;
	pF->DrawString(pBatch, ss.str().c_str(), Vector2(10, 45), Colours::White, 0, Vector2(0, 0), Vector2(0.75f, 0.75f));*/
}