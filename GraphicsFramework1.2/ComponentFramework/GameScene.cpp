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
#include "FileReader.h"

using namespace GAME;
using namespace MATH;

GameScene::GameScene(Window& windowRef) :Scene(windowRef) { }

bool GameScene::OnCreate() 
{
	gameStart = false;
	gameWin = false;
	gameLoss = false;
	eye = Vec3(0.0f, 0.0f, 90.0f);
	at = Vec3(0.0f, 0.0f, -1.0f);
	up = Vec3(0.0f, 1.0f, 0.0f);
	camera = nullptr;

	SceneEnvironment::getInstance()->setLight(Vec3(0.0f, 3.0f, 7.0f));

	shotDelay = 0.0f;

	OnResize(windowPtr->getWidth(), windowPtr->getHeight());
	return true;
}

bool GAME::GameScene::addEnem(Vec3 pos, Vec3 vel)
{
	enems.push_back(new Model(pos, Vec3(), vel));
	enems[enems.size() - 1]->OnCreate();

	if (enems[enems.size() - 1]->LoadMesh("Jellyfish.obj") == false) { return false; }
	return true;
}

bool GAME::GameScene::addMiss(Vec3 pos, Vec3 vel)
{
	miss.push_back(new Model(pos, Vec3(), vel));
	miss[miss.size() - 1]->OnCreate();

	if (miss[miss.size() - 1]->LoadMesh("Missile.obj") == false) { return false; }
	return true;
}

void GameScene::OnResize(int w_, int h_) 
{
	windowPtr->setWindowSize(w_, h_);
	glViewport(0, 0, windowPtr->getWidth(), windowPtr->getHeight());
	if (camera) delete camera;
	camera = new Camera(w_, h_);
	camera->SetCamera(eye, at, up);
	Trackball::getInstance()->setWindowDimensions(windowPtr->getWidth(), windowPtr->getHeight());
}

void GameScene::Update(const float deltaTime) 
{
	if (!gameLoss && gameStart) {
		int i = 0;
		int j = 0;

		for (Model* model : miss)
		{
			model->Update(deltaTime);

			if (model->getPosition().z < -10) { miss.erase(miss.begin() + i); printf("destroyed\n"); }

			for (Model* enem : enems)
			{
				float dist = abs(sqrt(pow(model->getPosition().x - enem->getPosition().x, 2) + pow(model->getPosition().z - enem->getPosition().z, 2) + 0));
				if (dist <= 5) { miss.erase(miss.begin() + i); enems.erase(enems.begin() + j); }
				j++;
			}
			i++;
		}

		for (Model* enem : enems)
		{
			enem->Update(deltaTime);
			if (enem->getPosition().z >= 90)
			{
				gameLoss = true;
				printf("lose\n");
			}
		}

		shotDelay -= deltaTime;

		if (enems.size() <= 0)
		{
			gameWin = true;
			printf("win\n");
		}
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
			switch (SDLEvent.key.keysym.sym)
			{
				if (!gameStart) 
				{ //select file
			case SDLK_1:
				StartBasic();
				break;
			case SDLK_2:
				StartFile("WorldDefault.xml");
				break;
				}
				else if (!gameWin && !gameLoss && gameStart)
				{ //controls
			case SDLK_RIGHT:
				if (eye.x < 100) {
					eye = MMath::translate(2.0f, 0.0f, 0.0f) * eye;
					at = MMath::translate(2.0f, 0.0f, 0.0f) * at;
					}
				else { printf("left limit\n"); }
				break;
			case SDLK_LEFT:
				if (eye.x > -100) {
					eye = MMath::translate(-2.0f, 0.0f, 0.0f) * eye;
					at = MMath::translate(-2.0f, 0.0f, 0.0f) * at;
					}
				else { printf("right limit\n"); }
				break;
			case SDLK_SPACE:
				if (shotDelay <= 0) { printf("fired\n"); Fire(); }
				break;
				}
				else 
				{ //end menu
			case SDLK_RETURN:
				gameStart, gameWin, gameLoss = false;
				eye = Vec3(0.0f, 0.0f, 90.0f);
				at = Vec3(0.0f, 0.0f, -1.0f);
				up = Vec3(0.0f, 1.0f, 0.0f);
				break;

				}
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
	addMiss(Vec3((eye.x) + 10, 20, eye.z - 10), Vec3(0, 0, -80));
}

void GameScene::StartBasic() 
{
	addEnem(Vec3(0.0f, 15.0f, -20.0f), Vec3(0.0f, 0.0f, 3.0f));
	addEnem(Vec3(60.0f, 15.0f, 0.0f), Vec3(0.0f, 0.0f, 3.0f));
	addEnem(Vec3(-30.0f, 15.0f, -40.0f), Vec3(0.0f, 0.0f, 3.0f));
	addEnem(Vec3(-50.0f, 15.0f, 0.0f), Vec3(0.0f, 0.0f, 3.0f));

	gameStart = true;
}

bool GameScene::StartFile(char *file_) 
{
	FileReader fR = FileReader();

	fR.LoadFile(file_);

	gameStart = true;
}