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
		Model(const Model&) = delete;
		Model(Model&&) = delete;
		Model& operator = (const Model&) = delete;
		Model& operator = (Model&&) = delete;

		virtual ~Model();

		void setPos(const Vec3& pos_) override;
		Vec3 getPosition();
		void setOrientation(const Vec3& orienration_) override;

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