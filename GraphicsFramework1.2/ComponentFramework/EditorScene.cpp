#include <GL/glew.h>
#include <SDL.h>
#include <iostream>
#include "MMath.h"
#include "Window.h"
#include "Shader.h"
#include "EditorScene.h"
#include "SceneEnvironment.h"
#include "Trackball.h"
#include "pugixml.hpp"
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
	if (addModel("Tree1.obj", Vec3(), 0.0f) == false) { return false; }

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


bool EditorScene::LoadFile(char file[])
{
	models.clear();
	pugi::xml_document doc;
	pugi::xml_parse_result result;


	string name = string("");
	Vec3 pos = Vec3(0, 0, 0);
	int rot = 0;

	result = doc.load_file(file); 

	if (result)
	{
		pugi::xml_node parent = doc.child("objects");

		for (pugi::xml_node child : parent.children())
		{
			if (child.attribute("name"))
			{
				name = child.attribute("name").value();
			}

			for (pugi::xml_node grandChild : child.children("pos"))
			{
				float x, y, z;
				x = y = z = 0.0f;

				if (grandChild.attribute("X"))
				{
					x = atof(grandChild.attribute("X").value());
				}
				if (grandChild.attribute("Y"))
				{
					y = atof(grandChild.attribute("Y").value());
				}
				if (grandChild.attribute("Z"))
				{
					z = atof(grandChild.attribute("Z").value());
				}
				pos = Vec3(x, y, z);

			}

			for (pugi::xml_node grandChild : child.children("rot"))
			{
				if (grandChild.attribute("value"))
				{
					rot = atof(grandChild.attribute("value").value());
				}
			}

			//instantiate the object here
			if (!addModel(name, pos, rot)) return false;
			//empty temp values
			name = "";
			pos = Vec3(0.0f, 0.0f, 0.0f);
			rot = 0.0f;			

		}
	}
	else { return false; }

	return true;
}

void EditorScene::HandleEvents(const SDL_Event& SDLEvent){
	if (SDLEvent.type == SDL_EventType::SDL_MOUSEBUTTONDOWN) {
		Trackball::getInstance()->onLeftMouseDown(SDLEvent.button.x, SDLEvent.button.y);
	}else if(SDLEvent.type == SDL_EventType::SDL_MOUSEBUTTONUP){
		Trackball::getInstance()->onLeftMouseUp(SDLEvent.button.x, SDLEvent.button.y);
	}else if (SDLEvent.type == SDL_EventType::SDL_MOUSEMOTION && 
		SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT)) {
		Trackball::getInstance()->onMouseMove(SDLEvent.button.x, SDLEvent.button.y);
	}
		
}

EditorScene::~EditorScene() {
	OnDestroy();
}

void EditorScene::OnDestroy() {
	/// Cleanup Assets
	if (camera) delete camera;
	for (Model* model : models) {
		if (model) delete model;
	}
}
