#ifndef EDITORSCENE_H
#define EDITORSCENE_H
#include "Scene.h"
#include "Model.h"
#include "Camera.h"

using namespace std;

namespace GAME 
{
	class Window; /// Forward declaration

	class EditorScene : public Scene 
	{
	protected:
		bool addModel(const string file, const Vec3 pos, const float rot);
		
	public:
		explicit EditorScene(Window& windowRef);
		virtual ~EditorScene();

		/// Delete these possible default constructors and operators  
		EditorScene(const EditorScene&) = delete;
		EditorScene(EditorScene &&) = delete;
		EditorScene& operator=(const EditorScene &) = delete;
		EditorScene& operator=(EditorScene &&) = delete;

		virtual bool OnCreate();
		virtual void OnDestroy();
		virtual void Update(const float deltaTime);
		virtual void Render() const;
		virtual void HandleEvents(const SDL_Event &SDLEvent);
		virtual void OnResize(const int, const int);
	
	private:
		std::vector<Model*> models;
		Camera* camera;
		bool LoadFile(char file[]);
	};
}
#endif