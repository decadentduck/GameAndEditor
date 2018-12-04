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

using namespace std;
using namespace GAME;
using namespace MATH;


EditorScene::EditorScene(Window& windowRef):Scene(windowRef) 
{

}

bool EditorScene::OnCreate() 
{
	
	camera = nullptr;
	
	/// Load Assets: as needed 
	//createlist of default objects

	//create list of scene objects


	/// Create a shader with attributes
	SceneEnvironment::getInstance()->setLight(Vec3(0.0f, 3.0f, 7.0f));

	OnResize(windowPtr->getWidth(), windowPtr->getHeight());
	return true;
}

bool EditorScene::addModel(const string tree, const Vec3 pos, const float rot)
{

	models.push_back(new Model(pos, Vec3(0.0f, 0.0f, 0.0f), rot, Vec3(0.05f, 0.05f, 0.05f)));
	models[models.size() - 1]->OnCreate();
	if (tree == "Tree1")
	{
		if (models[models.size() - 1]->LoadMesh("Tree1.obj") == false)
		{
			return false;
		}
	}
	else
	{
		if (models[models.size() - 1]->LoadMesh("Tree2.obj") == false)
		{
			return false;
		}
	}

	return true;
}

void EditorScene::OnResize(int w_, int h_){
	windowPtr->setWindowSize(w_,h_);
	glViewport(0,0,windowPtr->getWidth(),windowPtr->getHeight());
	if (camera) delete camera;
	camera = new Camera(w_, h_, Vec3(0.0f, 0.0f, 10.0f));
	Camera::currentCamera = camera;
	Trackball::getInstance()->setWindowDimensions(windowPtr->getWidth(), windowPtr->getHeight());
}

void EditorScene::Update(const float deltaTime) {
	for (Model* model : models) {
		model->Update(deltaTime);
	}
}

void EditorScene::Render() const{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);	
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	/// Draw your scene here
	for (Model* model : models) {
		model->Render();
	}
	SDL_GL_SwapWindow(windowPtr->getSDLWindow());
	
}

void EditorScene::HandleEvents(const SDL_Event& SDLEvent)
{

	
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
