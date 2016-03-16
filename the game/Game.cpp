#include <sstream>
#include <thread>
#include <iomanip>

#include "WindowUtils.h"
#include "D3D.h"
#include "Game.h"
#include "GeometryBuilder.h"
#include "CommonStates.h"

using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;

void Game::OnResize(int screenWidth, int screenHeight)
{
	
	OnResize_Default(screenWidth, screenHeight);
}


void Game::Load()
{

	//wood floor
	mQuad.Initialise(BuildQuad(mMeshMgr));
	MaterialExt *pMat = &mQuad.GetMesh().GetSubMesh(0).material;
	pMat->gfxData.Set(Vector4(0.9f, 0.8f, 0.8f, 0), Vector4(0.9f, 0.8f, 0.8f, 0), Vector4(0, 0, 0, 1));
	pMat->pTextureRV = mFX.mCache.LoadTexture("floor.dds", true, gd3dDevice);
	pMat->texture = "floor";
	mLoadData.loadedSoFar++;

	//torch
	mTorch.Initialise(BuildSphere(mMeshMgr, 16,16));
	pMat = &mTorch.GetMesh().GetSubMesh(0).material;
	pMat->gfxData.Set(Vector4(1, 0, 0, 1), Vector4(1, 0, 0, 1), Vector4(1, 0, 0, 1));
	mTorch.GetScale() = Vector3(0.1f, 0.1f, 0.1f);
	mTorch.GetPosition() = Vector3(0, 0.2f, 0);
	mLoadData.loadedSoFar++;


	mCube.Initialise(BuildCube(mMeshMgr));
	mLoadData.loadedSoFar++;

	//The rock
	Mesh& ms = mMeshMgr.CreateMesh("rock");
	ms.CreateFrom("../bin/data/the_rock/TheRock2.obj", gd3dDevice, mFX.mCache);
	mRock.Initialise(ms);
	mRock.GetScale() = Vector3(0.005f, 0.005f, 0.005f);
	mRock.GetPosition() = Vector3(1.5f, 1, 0);
	mRock.GetRotation() = Vector3(0, 0, 0);
	mLoadData.loadedSoFar++;

	mBallSim.Initialise(mMeshMgr);
	mEnemy.Initialise(mMeshMgr);
}

void Game::LoadDisplay(float dTime)
{
	BeginRender(Colours::Blue);

	mpSpriteBatch->Begin();

	static int pips = 0;
	static float elapsed = 0;
	elapsed += dTime;
	if (elapsed > 0.25f){
		pips++;
		elapsed = 0;
	}
	if (pips > 10)
		pips = 0;
	wstringstream ss;
	ss << L"Loading meshes(" << (int)(((float)mLoadData.loadedSoFar / (float)mLoadData.totalToLoad)*100.f) << L"%) ";
	for (int i = 0; i < pips; ++i)
		ss << L'.';
	mpFont2->DrawString(mpSpriteBatch, ss.str().c_str(), Vector2(100, 200), Colours::White, 0, Vector2(0, 0), Vector2(1.f, 1.f));

	ss.str(L"");
	ss << L"FPS:" << (int)(1.f / dTime);
	mpFont->DrawString(mpSpriteBatch, ss.str().c_str(), Vector2(10, 550), Colours::White, 0, Vector2(0, 0), Vector2(0.5f, 0.5f));

	mpSpriteBatch->End();

	EndRender();
}

void Game::Initialise()
{
	mFX.Init(gd3dDevice);


	FX::SetupDirectionalLight(0, true, Vector3(-0.7f, -0.7f, 0.7f), Vector3(0.8f,0.8f,0.8f), Vector3(0.15f,0.15f,0.15f), Vector3(0.25f,0.25f,0.25f));

	mpSpriteBatch = new SpriteBatch(gd3dImmediateContext);
	assert(mpSpriteBatch);
	mpFont = new SpriteFont(gd3dDevice, L"../bin/data/comicSansMS.spritefont");
	assert(mpFont);

	mpFont2 = new SpriteFont(gd3dDevice, L"../bin/data/algerian.spritefont");
	assert(mpFont2);

	mLoadData.totalToLoad = 4;
	mLoadData.loadedSoFar = 0;
	mLoadData.running = true;
	mLoadData.loader = std::async(launch::async, &Game::Load, this);

	mMKInput.Initialise(GetMainWnd());
	mGamepad.Initialise();

}

void Game::Release()
{
	mFX.Release();
	mMeshMgr.Release();
	delete mpFont;
	mpFont = nullptr;
	delete mpSpriteBatch;
	mpSpriteBatch = nullptr;
	delete mpFont2;
	mpFont2 = nullptr;
}

void Game::Update(float dTime)
{
	mGamepad.Update();

	const float camInc = 10.f * dTime;

	/*if (mMKInput.IsPressed(VK_A))
			mCamPos.y += camInc;
	else if(mMKInput.IsPressed(VK_Z))
			mCamPos.y -= camInc;
	else if (mMKInput.IsPressed(VK_D))
			mCamPos.x -= camInc;
	else if (mMKInput.IsPressed(VK_F))
			mCamPos.x += camInc;
	else if (mMKInput.IsPressed(VK_W))
			mCamPos.z += camInc;
	else if (mMKInput.IsPressed(VK_S))
			mCamPos.z -= camInc;*/

	mCamPos.x += mGamepad.GetState(0).leftStickX * dTime;
	mCamPos.z += mGamepad.GetState(0).leftStickY * dTime;
	mCamPos.y += mGamepad.GetState(0).rightStickY * dTime;

	mBallSim.Update(dTime, mCamPos,mMKInput, mRock);
}


void Game::Render(float dTime)
{
	if (mLoadData.running)
	{
		if (!mLoadData.loader._Is_ready())
		{
			LoadDisplay(dTime);
			return;
		}
		mLoadData.loader.get();
		mLoadData.running = false;
		return;
	}

	BeginRender(Colours::Black);

	FX::SetPerFrameConsts(gd3dImmediateContext, mCamPos);

	CreateViewMatrix(FX::GetViewMatrix(), mCamPos, Vector3(0, 0, 0), Vector3(0, 1, 0));
	CreateProjectionMatrix(FX::GetProjectionMatrix(), 0.25f*PI, GetAspectRatio(), 1, 1000.f);


	MaterialExt mat = MaterialExt::default;

	mBallSim.Render(mFX, dTime);
	mEnemy.Render(mFX, dTime);

	//floor
	mQuad.GetPosition() = Vector3(0, 0, 0);
	mQuad.GetRotation() = Vector3(0, 0, 0);
	mQuad.GetScale() = Vector3(3, 1, 3);
	//mFX.Render(mQuad, gd3dImmediateContext);


	//mFX.Render(mRock, gd3dImmediateContext);


	wstring mssg, mssg2;
	
	/*if (mMKInput.GetMouseButton(MouseAndKeys::LBUTTON))
	{
		Ray ray;
		mMKInput.MousePosToWorldRay(FX::GetViewMatrix(), FX::GetProjectionMatrix(), mCamPos, ray);
		Plane p(Vector3(0, 0, 0), Vector3(0, 0, -1));
		float dist;
		ray.Intersects(p, dist);
		Vector3 pos(ray.position + ray.direction*dist);

		wstringstream sstr;
		sstr.precision(1);
		sstr << L"Mouse move 3D x=" << pos.x << L" y=" << pos.y << L" z=" << pos.z;
		mssg = sstr.str();

		mat = MaterialExt::default;
		mat.pTextureRV = mFX.mCache.LoadTexture("saw2.dds", true, gd3dDevice);
		mat.flags |= MaterialExt::TFlags::ALPHA_TRANSPARENCY;
		mQuad.GetPosition() = pos;
		mQuad.GetRotation() = Vector3(-PI / 2, 0, GetClock() * 4);
		mQuad.GetScale() = Vector3(0.2f, 0.2f, 0.2f);
		mFX.Render(mQuad, gd3dImmediateContext,&mat);

	}*/


	CommonStates state(gd3dDevice);
	mpSpriteBatch->Begin(SpriteSortMode_Deferred, state.NonPremultiplied());

	Ray ray;
	mMKInput.MousePosToWorldRay(FX::GetViewMatrix(), FX::GetProjectionMatrix(), mCamPos, ray);

	//if (mMKInput.GetMouseButton(MouseAndKeys::LBUTTON))
	//{
	//	float dist;
	//	//if (ray.Intersects(BoundingSphere(mTorch.GetPosition(), 1.f * mTorch.GetScale().x), dist))
	//	if (RayToSphere(mTorch.GetPosition(), 1.f * mTorch.GetScale().x, ray, dist))
	//	{
	//		mssg = L"HIT!!!!";
	//		if (RayToPlane(Plane(mTorch.GetPosition(), FX::GetViewMatrix().Forward()), ray, dist))
	//		{
	//			Vector3 pos(ray.position + ray.direction * dist);
	//			mTorch.GetPosition() = pos;
	//		}
	//	}
	//	else
	//		mssg = L"MISS";
	//	mpFont->DrawString(mpSpriteBatch, mssg.c_str(), Vector2(100, 100), DirectX::Colors::White, 0, Vector2(0, 0), Vector2(2, 2));
	//}



	//general messages
	wstringstream ss;
	if (dTime > 0)
		ss << L"FPS: " << (int)(1.f / dTime);
	else
		ss << L"FPS: 0";
	mpFont->DrawString(mpSpriteBatch, ss.str().c_str(), Vector2(10, 10), Colours::White, 0, Vector2(0, 0), Vector2(0.5f, 0.5f));

	mBallSim.RenderText(mpFont, mpSpriteBatch);

	mpSpriteBatch->End();


	EndRender();

	mMKInput.PostProcess();
}

LRESULT Game::WindowsMssgHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	const float camInc = 20.f * GetElapsedSec();
	//do something game specific here
	switch (msg)
	{
		// Respond to a keyboard event.
	case WM_CHAR:
		mBallSim.Input(wParam);
		switch (wParam)
		{
		case 27:
		case 'q':
		case 'Q':
			PostQuitMessage(0);
			return 0;
		case ' ':
			mCamPos = mDefCamPos;
			break;
		}
	case WM_INPUT:
			mMKInput.MessageEvent((HRAWINPUT)lParam);
			break;
	}
	//default message handling (resize window, full screen, etc)
	return DefaultMssgHandler(hwnd, msg, wParam, lParam);
}

