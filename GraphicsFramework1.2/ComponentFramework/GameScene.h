#ifndef GAMESCENE_H
#define GAMESCENE_H

#include <string>
#include "Scene.h"
#include "Model.h"
#include "Camera.h"

namespace GAME
{
	class Window; /// Forward declaration

	class GameScene : public Scene
	{
	protected:

		bool addEnem(Vec3 pos, Vec3 vel);
		bool addMiss(Vec3 pos, Vec3 vel);
		bool makePlane();

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
		virtual void Fire();
		virtual bool StartFile(char *file_);
		virtual void StartBasic();

	private:
		std::vector<Model*> enems, miss, wl, temp, trees;
		Vec3 eye, at, up;
		Camera* camera;
		float shotDelay;
		bool gameStart, gameWin, gameLoss;
	};
}
#endif