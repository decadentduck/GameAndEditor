#include <GL/glew.h>
#include <SDL.h>
#include <iostream>
#include "MMath.h"
#include "Window.h"
#include "Shader.h"
#include "EditorScene.h"
#include "SceneEnvironment.h"
#include "ObjLoader.h"
#include <string>
#include "FileReader.h"

using namespace std;
using namespace GAME;
using namespace MATH;


EditorScene::EditorScene(Window& windowRef):Scene(windowRef) 
{

}

bool EditorScene::OnCreate() 
{
	eye = Vec3(0.0f, -30.0f, 10.0f);
	at = Vec3(0.0f, 0.0f, -1.0f);
	up = Vec3(0.0f, 1.0f, 0.0f);
	camera = nullptr;
	
	FileReader f = FileReader();
	
	selectedObjectIndex = 0;
	//createlist of default objects
	defaultModels = f.LoadFile("ObjectsDefault.xml");

	//create list of scene objects
	gameModels = f.LoadFile("WorldDefault.xml");

	/// Create a shader with attributes
	SceneEnvironment::getInstance()->setLight(Vec3(0.0f, 3.0f, 7.0f));

	OnResize(windowPtr->getWidth(), windowPtr->getHeight());
	return true;
}

bool EditorScene::addModel(char file[], const Vec3 pos, const float rot)
{

	gameModels.push_back(new Model(pos, Vec3(0.0f, 0.0f, 0.0f), rot, Vec3(0.05f, 0.05f, 0.05f), Vec3(0.0f, 0.0f, 0.0f), file));
	gameModels[gameModels.size() - 1]->OnCreate();

	if (gameModels[gameModels.size() - 1]->LoadMesh(file) == false) { return false; }

	return true;
}

void EditorScene::OnResize(int w_, int h_)
{
	windowPtr->setWindowSize(w_,h_);
	glViewport(0,0,windowPtr->getWidth(),windowPtr->getHeight());
	if (camera) delete camera;
	camera = new Camera(w_, h_);
	camera->SetCamera(eye, at, up);
	Camera::currentCamera = camera;
}

void EditorScene::Update(const float deltaTime) 
{
	
}

void EditorScene::Render() const
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);	
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	/// Draw your scene here
	for (Model* model : gameModels) { model->Render(); }
	SDL_GL_SwapWindow(windowPtr->getSDLWindow());
	
}

void EditorScene::HandleEvents(const SDL_Event& SDLEvent)
{
	FileReader FR = FileReader();
	if (SDLEvent.type == SDL_KEYDOWN)
	{
		switch (SDLEvent.key.keysym.sym)
		{
		case SDLK_e:
			FR.SaveFile("SaveFile.xml", gameModels);
			break;
		case SDLK_1:
			//create tree one
			break;
		case SDLK_2:
			//create tree one
			break;
		case SDLK_3:
			//create tree one
			break;
		case SDLK_w:
			//camera moverment;
			eye = MMath::translate(0.0f, 0.0f, 1.0f) * eye;
			at = MMath::translate(0.0f, 0.0f, 1.0f) * at;
			break;
		case SDLK_a:
			//camera moverment;
			eye = MMath::translate(1.0f, 0.0f, 0.0f) * eye;
			at = MMath::translate(1.0f, 0.0f, 0.0f) * at;
			break;
		case SDLK_s:
			//camera moverment;
			eye = MMath::translate(0.0f, 0.0f, -1.0f) * eye;
			at = MMath::translate(0.0f, 0.0f, -1.0f) * at;
			break;
		case SDLK_d:
			//camera moverment;
			eye = MMath::translate(-1.0f, 0.0f, 0.0f) * eye;
			at = MMath::translate(-1.0f, 0.0f, 0.0f) * at;
			break;
		case SDLK_z:
			//object selection
			selectedObjectIndex--;
			if (selectedObjectIndex < 0) selectedObjectIndex = gameModels.size() - 1;
			break;
		case SDLK_x:
			//object selection
			selectedObjectIndex++;
			if (selectedObjectIndex == gameModels.size()) selectedObjectIndex = 0;
			break;
		case SDLK_LEFT:
			//move object
			gameModels[selectedObjectIndex]->setPos( MMath::translate( Vec3(1, 0, 0)) * gameModels[selectedObjectIndex]->getPosition());
			break;
		case SDLK_RIGHT:
			//move object
			gameModels[selectedObjectIndex]->setPos(MMath::translate(Vec3(-1, 0, 0)) * gameModels[selectedObjectIndex]->getPosition());
			break;
		case SDLK_UP:
			//move object
			gameModels[selectedObjectIndex]->setPos(MMath::translate(Vec3(0, 0, 1)) * gameModels[selectedObjectIndex]->getPosition());
			break;
		case SDLK_DOWN:
			//move object
			gameModels[selectedObjectIndex]->setPos(MMath::translate(Vec3(0, 0, -1)) * gameModels[selectedObjectIndex]->getPosition());
			break;
		default:
			break;
		}
		camera->SetCamera(eye, at, up);
		Camera::currentCamera = camera;
	}
}

EditorScene::~EditorScene() {
	OnDestroy();
}

void EditorScene::OnDestroy() {
	/// Cleanup Assets
	if (camera) delete camera;
	for (Model* model : defaultModels) { if (model) delete model; }
	for (Model* model : gameModels) { if (model) delete model; }
}
