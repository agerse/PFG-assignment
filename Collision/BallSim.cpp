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

void BallSim::Input(char key)
{
	Mode mode = mLastMode;
	switch (key)
	{
	//case 'x':
	//	break;
	//case '1':
	//	mode = WAITING;
	//	break;
	//case '2':
	//	mode = LINEAR;
	//	break;
	//case '3':
	//	mode = ACCEL;
	//	break;
	//case '4':
	//	mode = BOUNCE;
	//	break;
	//case '5':
	//	mode = BOUNCE2;
	//	break;
	//	case '6':
	//	mode = CUBE;
	//	break;
		case '65':
		case '97':
			mode = LEFT;
			break;
		case '68':
		case '100':
			mode = RIGHT;
			break;
		case '87':
		case '119':
			mode = UP;
			break;
		case '83':
		case '115':
			mode = DOWN;
			break;
	default:
		return;
	}
	mLastMode = mMode;
	mMode = mode;
	Start();
}

void BallSim::Start()
{
	switch (mMode)
	{
	//case WAITING:
	//	mBall.GetPosition() = Vector3(-2, 0.5f, 0);
	//	mVel = Vector3(0, 0, 0);
	//	mGrav = Vector3(0, -9.81f, 0) * 0.5f;
	//	mAccel = Vector3(0, 0, 0);
	//	mCOR = 1;
	//	break;
	//case LINEAR:
	//	mVel = Vector3(1, 0, 0);
	//	mGrav = Vector3(0, 0, 0);
	//	mAccel = Vector3(0, 0, 0);
	//	mCOR = 1;
	//	break;
	//case ACCEL:
	//	mVel = Vector3(1, 0, 0);
	//	mGrav = Vector3(0, 0, 0);
	//	mAccel = Vector3(10, 0, 0);
	//	break;
	//case BOUNCE:
	//	mBall.GetPosition() = Vector3(-2, 0.5f, 0);
	//	mVel = Vector3(0.25f, 0.9f, 0) * 3;
	//	mGrav = Vector3(0, -9.81f, 0) * 0.5f;
	//	mAccel = Vector3(0, 0, 0);
	//	mCOR = 1;
	//	break;
	//case BOUNCE2:
	//	mBall.GetPosition() = Vector3(-2, 0.5f, 0);
	//	mVel = Vector3(0.25f, 0.9f, 0) * 3;
	//	mGrav = Vector3(0, -9.81f, 0) * 0.5f;
	//	mAccel = Vector3(0, 0, 0);
	//	mCOR = 0.8f;
	//	break;

	case UP:
		mBall.GetPosition() = Vector3(-2, 0.5f, 0);
		mVel
	case LEFT:

	case RIGHT:

	case DOWN:

	}
}

void BallSim::Update(float dTime, const Vector3& camPos, MouseAndKeys& input, Model& rock)
{
	mAngle += dTime * 0.5f;
	Vector3 pos = mBall.GetPosition();
	pos += mVel * dTime;
	mBall.GetPosition() = pos;
	if (pos.y < mRadius){
		//bounce up 
		mVel.y *= -1;
		//lose energy
		mVel *= mCOR;
		//ensure we arent left under the floor
		mBall.GetPosition().y = mRadius;
		pos = mBall.GetPosition();
	}
	//apply accelerations unless we've come to a halt
	if (mVel.LengthSquared() < 0.001f && fabsf(pos.y - mRadius) < 0.001f)
		mVel = Vector3(0, 0, 0);
	else
	{
		mVel += mGrav * dTime;
		mVel += mAccel * dTime;
	}
}

void BallSim::Render(FX::MyFX& fx, float dTime)
{
	float alpha = 0.5f + sinf(mAngle * 2) * 0.5f;
	Vector3 mBallColour = Vector3(0.f, 0.f, 10.f);
	FX::SetupPointLight(1, true, mBall.GetPosition(), mBallColour, Vector3(0.5f, 0.5f, 0.5f), Vector3(0, 0, 0), 15.f, 1.f);
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

