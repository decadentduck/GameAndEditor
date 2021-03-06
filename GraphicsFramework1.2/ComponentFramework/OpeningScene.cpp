#include <GL/glew.h>
#include <SDL.h>
#include <iostream>
#include "MMath.h"
#include "Window.h"
#include "Shader.h"
#include "OpeningScene.h"
#include "SceneEnvironment.h"
#include "Trackball.h"

#include "ObjLoader.h"

using namespace GAME;
using namespace MATH;


OpeningScene::OpeningScene(Window& windowRef) :Scene(windowRef) {

}


bool OpeningScene::OnCreate() {

	camera = nullptr;


	/// Load Assets: as needed 
	/*if (addModel("Tree1.obj") == false) {
		return false;
	}*/

	/// Create a shader with attributes
	SceneEnvironment::getInstance()->setLight(Vec3(0.0f, 3.0f, 7.0f));

	OnResize(windowPtr->getWidth(), windowPtr->getHeight());
	return true;
}


bool GAME::OpeningScene::addModel(const char* filename)
{
	models.push_back(new Model(Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0, 0, 0)));
	models[models.size() - 1]->OnCreate();

	if (models[models.size() - 1]->LoadMesh(filename) == false) {
		return false;
	}
	return true;
}

void OpeningScene::OnResize(int w_, int h_) {
	windowPtr->setWindowSize(w_, h_);
	glViewport(0, 0, windowPtr->getWidth(), windowPtr->getHeight());
	if (camera) delete camera;
	camera = new Camera(w_, h_);
	Camera::currentCamera = camera;
	Trackball::getInstance()->setWindowDimensions(windowPtr->getWidth(), windowPtr->getHeight());
}

void OpeningScene::Update(const float deltaTime) {
	for (Model* model : models) {
		model->Update(deltaTime);
	}
}

void OpeningScene::Render() const {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/// Draw your scene here
	for (Model* model : models) {
		model->Render();
	}
	SDL_GL_SwapWindow(windowPtr->getSDLWindow());

}

void OpeningScene::HandleEvents(const SDL_Event& SDLEvent) {
	if (SDLEvent.type == SDL_EventType::SDL_MOUSEBUTTONDOWN) {
		Trackball::getInstance()->onLeftMouseDown(SDLEvent.button.x, SDLEvent.button.y);
	}
	else if (SDLEvent.type == SDL_EventType::SDL_MOUSEBUTTONUP) {
		Trackball::getInstance()->onLeftMouseUp(SDLEvent.button.x, SDLEvent.button.y);
	}
	else if (SDLEvent.type == SDL_EventType::SDL_MOUSEMOTION &&
		SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT)) {
		Trackball::getInstance()->onMouseMove(SDLEvent.button.x, SDLEvent.button.y);
	}

}

OpeningScene::~OpeningScene() {
	OnDestroy();
}

void OpeningScene::OnDestroy() {
	/// Cleanup Assets
	if (camera) delete camera;
	for (Model* model : models) {
		if (model) delete model;
	}
}
