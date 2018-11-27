#ifndef OPENINGSCENE_H
#define OPENINGSCENE_H

#include "Scene.h"
#include "Model.h"
#include "Camera.h"

namespace GAME
{
	class Window; /// Forward declaration

	class OpeningScene : public Scene
	{
	protected:

		bool addModel(const char* filename);

	public:
		explicit OpeningScene(Window& windowRef);
		virtual ~OpeningScene();


		/// Delete these possible default constructors and operators  
		OpeningScene(const OpeningScene&) = delete;
		OpeningScene(OpeningScene &&) = delete;
		OpeningScene& operator=(const OpeningScene &) = delete;
		OpeningScene& operator=(OpeningScene &&) = delete;

		virtual bool OnCreate();
		virtual void OnDestroy();
		virtual void Update(const float deltaTime);
		virtual void Render() const;
		virtual void HandleEvents(const SDL_Event &SDLEvent);
		virtual void OnResize(const int, const int);

	private:
		std::vector<Model*> models;
		Camera* camera;
	};
}
#endif