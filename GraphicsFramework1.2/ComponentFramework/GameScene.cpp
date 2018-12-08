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
	gameWin = false;
	eye = Vec3(0.0f, 0.0f, 90.0f);
	at = Vec3(0.0f, 0.0f, -1.0f);
	up = Vec3(0.0f, 1.0f, 0.0f);
	camera = nullptr;

	//call file reader and populate with enemies

	/// Load Assets: as needed 
	if (addEnem(Vec3(0.0f, 15.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f)) == false) { return false; }

	/// Create a shader with attributes
	SceneEnvironment::getInstance()->setLight(Vec3(0.0f, 3.0f, 7.0f));

	shotDelay = 0.0f;

	OnResize(windowPtr->getWidth(), windowPtr->getHeight());
	return true;
}


bool GAME::GameScene::addEnem(Vec3 pos, Vec3 rot, Vec3 vel)
{
	enems.push_back(new Model(Vec3(0.0f, 15.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f)));
	enems[enems.size() - 1]->OnCreate();

	if (enems[enems.size() - 1]->LoadMesh("Jellyfish.obj") == false) { return false; }
	return true;
}


bool GAME::GameScene::addMiss(Vec3 pos, Vec3 rot, Vec3 vel)
{
	miss.push_back(new Model(pos, rot, vel));
	miss[miss.size() - 1]->OnCreate();

	if (miss[miss.size() - 1]->LoadMesh("Missile.obj") == false) { return false; }
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
	int i = 0;
	int j = 0;

	for (Model* model : miss) 
	{ 
		model->Update(deltaTime); 

		if (model->getPosition().z < -10) { miss.erase(miss.begin() + i); printf("destroyed\n"); }

		for (Model* enem : enems) 
		{
			float dist = abs(sqrt(pow(model->getPosition().x - enem->getPosition().x, 2) + pow(model->getPosition().z - enem->getPosition().z, 2) + 0));
			if (dist <= 5){ miss.erase(miss.begin() + i); enems.erase(enems.begin() + j);}
			j++;
		}
		i++;
	}
	shotDelay -= deltaTime;

	if (enems.size() <= 0) 
	{
		gameWin = true;
	}
}

void GameScene::Render() const 
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/// Draw your scene here
	for (Model* model : miss) { model->Render(); }
	for (Model* model : enems) { model->Render(); }
	SDL_GL_SwapWindow(windowPtr->getSDLWindow());

}

void GameScene::HandleEvents(const SDL_Event& SDLEvent) 
{
	
		if (SDLEvent.type == SDL_KEYDOWN)
		{
			//printf("KEYPRESS\n");
			switch (SDLEvent.key.keysym.sym)
			{
				if (!gameWin)
				{
			case SDLK_LEFT:
				eye = MMath::translate(1.0f, 0.0f, 0.0f) * eye;
				at = MMath::translate(1.0f, 0.0f, 0.0f) * at;
				break;
			case SDLK_RIGHT:
				eye = MMath::translate(-1.0f, 0.0f, 0.0f) * eye;
				at = MMath::translate(-1.0f, 0.0f, 0.0f) * at;
				break;

			case SDLK_SPACE:
				if (shotDelay <= 0) { printf("fired\n"); Fire(); }
				break;
			default:
				break;
				}
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
	for (Model* model : enems) {
		if (model) delete model;
	}
	for (Model* model : miss) {
		if (model) delete model;
	}
}

void GameScene::Fire() 
{
	shotDelay = 1.5f;
	addMiss(Vec3((-1 * eye.x) + 10, 20, eye.z - 40), Vec3(0, 0, 0), Vec3(0, 0, -50));
}
