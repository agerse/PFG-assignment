#include <iomanip>

#include "Enemy.h"
#include "D3D.h"

using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;

void Enemy::Initialise(MeshManager& mgr){
	Mesh *p = mgr.GetMesh("enemy");
	assert(p);

}

void Enemy::Start(){

}

void Enemy::Update(float dTime, const Vector3& camPos, Model& rock){

}

void Enemy::Render(){

}
