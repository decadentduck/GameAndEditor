#include "Camera.h"

namespace GAME {

	Camera* Camera::currentCamera = nullptr;

	Camera::Camera(const int viewportWidth, const int viewportHeight)
	{
		pos = Vec3(0.0f, -30.0f, 10.0f);
		at = Vec3(0.0f, 0.0f, -1.0f);
		up = Vec3(0.0f, 1.0f, 0.0f);
		float aspect = float(viewportWidth) / float(viewportHeight);
		projectionMatrix = MMath::perspective(45.0f, aspect, 0.75f, 100.0f);
		updateViewMatrix();
	}

	Camera::~Camera() {}

	void Camera::updateViewMatrix(){
		viewMatrix = MMath::lookAt(pos, at, up);
	}

	Matrix4&  Camera::getProjectionMatrix() {
		return projectionMatrix;
	}


	Matrix4&  Camera::getViewMatrix(){
		return viewMatrix;
	}

	void Camera::setPos(const Vec3& pos_){
		Entity::setPos(pos_);
		updateViewMatrix();
	}

	void Camera::setOrientation(const Vec3& orientation_){
		Entity::setOrientation(orientation_);
		updateViewMatrix();
	}

	void Camera::SetCamera(const Vec3& eye_, const Vec3& at_, const Vec3& up_)
	{
		pos = eye_;
		at = at_;
		up = up_;
		updateViewMatrix();
	}

}