#ifndef GAMESCENE_H
#define GAMESCENE_H

#include "Scene.h"
#include "Model.h"
#include "Camera.h"

namespace GAME
{
	class Window; /// Forward declaration

	class GameScene : public Scene
	{
	protected:

		bool addModel(const char* filename);

	public:
		explicit GameScene(Window& windowRef);
		virtual ~GameScene();


		/// Delete these possible default constructors and operators  
		GameScene(const GameScene&) = delete;
		GameScene(GameScene &&) = delete;
		GameScene& operator=(const GameScene &) = delete;
		GameScene& operator=(GameScene &&) = delete;

		virtual bool OnCreate();
		virtual void OnDestroy();
		virtual void Update(const float deltaTime);
		virtual void Render() const;
		virtual void HandleEvents(const SDL_Event &SDLEvent);
		virtual void OnResize(const int, const int);

	private:
		std::vector<Model*> models;
		Vec3 eye, at, up;
		Camera* camera;
	};
}
#endif