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
	
	camera = nullptr;
	
	FileReader f = FileReader();
	
	//createlist of default objects
	defaultModels = f.LoadFile("ObjectsDefault.xml");

	//create list of scene objects
	gameModels = f.LoadFile("WorldDefault.xml");

	/// Create a shader with attributes
	SceneEnvironment::getInstance()->setLight(Vec3(0.0f, 3.0f, 7.0f));

	OnResize(windowPtr->getWidth(), windowPtr->getHeight());
	return true;
}

bool EditorScene::addModel(const string tree, const Vec3 pos, const float rot)
{

	gameModels.push_back(new Model(pos, Vec3(0.0f, 0.0f, 0.0f), rot, Vec3(0.05f, 0.05f, 0.05f)));
	gameModels[gameModels.size() - 1]->OnCreate();
	if (tree == "Tree1.obj")
	{
		if (gameModels[gameModels.size() - 1]->LoadMesh("Tree1.obj") == false)
		{
			return false;
		}
	}
	else
	{
		if (gameModels[gameModels.size() - 1]->LoadMesh("Tree2.obj") == false)
		{
			return false;
		}
	}

	return true;
}

void EditorScene::OnResize(int w_, int h_)
{
	windowPtr->setWindowSize(w_,h_);
	glViewport(0,0,windowPtr->getWidth(),windowPtr->getHeight());
	if (camera) delete camera;
	camera = new Camera(w_, h_, Vec3(0.0f, 20.0f, 0.0f));
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
	if (SDLEvent.type == SDL_KEYDOWN)
	{
		switch (SDLEvent.key.keysym.sym)
		{
		case SDLK_1:
			//create tree one
			break;
		case SDLK_2:
			//create tree one
			break;
		case SDLK_3:
			//create tree one
			break;
		case SDLK_z:
			//object selection
			break;
		case SDLK_x:
			//object selection
			break;
		case SDLK_LEFT:
			//move object
			break;
		case SDLK_RIGHT:
			//move object
			break;
		case SDLK_UP:
			//move object
			break;
		case SDLK_DOWN:
			//move object
			break;
		default:
			break;
		}
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
