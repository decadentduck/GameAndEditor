#include <GL/glew.h>
#include <SDL.h>
#include <iostream>
#include "MMath.h"
#include "Window.h"
#include "Shader.h"
#include "GameScene.h"
#include "SceneEnvironment.h"
#include "Trackball.h"
#include "ObjLoader.h"

using namespace GAME;
using namespace MATH;

GameScene::GameScene(Window& windowRef) :Scene(windowRef) { }

bool GameScene::OnCreate() 
{
	eye = Vec3(0.0f, 0.0f, 10.0f);
	at = Vec3(0.0f, 0.0f, -1.0f);
	up = Vec3(0.0f, 1.0f, 0.0f);
	camera = nullptr;

	/// Load Assets: as needed 
	if (addModel("Tree1.obj") == false) { return false; }

	/// Create a shader with attributes
	SceneEnvironment::getInstance()->setLight(Vec3(0.0f, 3.0f, 7.0f));

	OnResize(windowPtr->getWidth(), windowPtr->getHeight());
	return true;
}


bool GAME::GameScene::addModel(const char* filename)
{
	models.push_back(new Model(Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f)));
	models[models.size() - 1]->OnCreate();

	if (models[models.size() - 1]->LoadMesh(filename) == false) { return false; }
	return true;
}

void GameScene::OnResize(int w_, int h_) 
{
	windowPtr->setWindowSize(w_, h_);
	glViewport(0, 0, windowPtr->getWidth(), windowPtr->getHeight());
	if (camera) delete camera;
	camera = new Camera(w_, h_, Vec3(0.0f, 0.0f, 10.0f));
	camera->SetCamera(eye, at, up);
	Trackball::getInstance()->setWindowDimensions(windowPtr->getWidth(), windowPtr->getHeight());
}

void GameScene::Update(const float deltaTime) 
{
	for (Model* model : models) { model->Update(deltaTime); }
}

void GameScene::Render() const 
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/// Draw your scene here
	for (Model* model : models) { model->Render(); }
	SDL_GL_SwapWindow(windowPtr->getSDLWindow());

}

void GameScene::HandleEvents(const SDL_Event& SDLEvent) 
{
	//Handle camera via keyboard input
	if (SDLEvent.type == SDL_KEYDOWN)
	{
		printf("KEYPRESS\n");
		switch (SDLEvent.key.keysym.sym)
		{
		case SDLK_UP:
			eye = MMath::translate(0.0f, 0.0f, -1.0f) * eye;
			at = MMath::translate(0.0f, 0.0f, -1.0f) * at;
			break;
		case SDLK_DOWN:
			eye = MMath::translate(0.0f, 0.0f, 1.0f) * eye;
			at = MMath::translate(0.0f, 0.0f, 1.0f) * at;
			break;
		case SDLK_LEFT:
			at = at - eye;
			at = MMath::rotate(5, 0.0f, 1.0f, 0.0f) * at;
			at = at + eye;
			break;
		case SDLK_RIGHT:
			at = at - eye;
			at = MMath::rotate(-5, 0.0f, 1.0f, 0.0f) * at;
			at = at + eye;
			break;
		default:
			break;
		}
		camera->SetCamera(eye, at, up);
		Camera::currentCamera = camera;
	}
}

GameScene::~GameScene() { OnDestroy(); }

void GameScene::OnDestroy() 
{
	/// Cleanup Assets
	if (camera)
	{
		camera = nullptr;
		delete camera;
	}
	for (Model* model : models) {
		if (model) delete model;
	}
}
