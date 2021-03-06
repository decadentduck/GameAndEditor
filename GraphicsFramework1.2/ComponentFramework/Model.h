#ifndef MODEL_H
#define MODEL_H
#include <vector>
#include "MMath.h"
#include "Mesh.h"
#include "Entity.h"

namespace GAME {

	using namespace MATH;

	class Model : public Entity {
	protected:
		float rotation;
		Vec3 scale;
		Vec3 velocity;

		class Shader *shader;

		Matrix4 modelMatrix;
		std::vector<Mesh*> meshes;

	public:
		Model(const Vec3 _pos, const Vec3 _orientation, Vec3 _velocity);
		Model(const Vec3 _pos, const Vec3 _orientation, const float rotation_, const Vec3 scale_, Vec3 _velocity);
		Model(const Vec3 _pos, const Vec3 _orientation, const float rotation_, const Vec3 scale_, Vec3 _velocity, char file_[]);
		Model(const Vec3 _pos, const Vec3 _orientation, const float rotation_, const Vec3 scale_, Vec3 _velocity, std::string file_);
		Model(const Model&) = delete;
		Model(Model&&) = delete;
		Model& operator = (const Model&) = delete;
		Model& operator = (Model&&) = delete;

		virtual ~Model();

		void setPos(const Vec3& pos_) override;
		void setVel(const Vec3 vel_);
		void setScale(const Vec3 scale_);
		Vec3 getPosition();
		std::string getFileName();
		void setOrientation(const Vec3& orienration_) override;
		char *fileName;
		std::string name;

		virtual bool OnCreate();
		virtual void OnDestroy();
		virtual void Render() const;
		virtual void Update(const float deltaTime);
		virtual void Update(const float deltaTime, Vec3 velocity);

		virtual bool LoadMesh(const char* filename);

	protected:

		void updateModelMatrix();
	};
} /// end of namespace

#endif