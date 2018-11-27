#include "Model.h"
#include "Shader.h"
#include "ObjLoader.h"
#include "Camera.h"
#include "SceneEnvironment.h"
#include "Trackball.h"

namespace GAME 
{

	Model::Model(const Vec3 pos_, const Vec3 orientation_) 
	{
		pos = pos_;
		orientation = orientation_;
		rotation = 0.0f;
		scale = Vec3(1.0f, 1.0f, 1.0f);
		shader = nullptr;
	}

	Model::Model(const Vec3 pos_, const Vec3 orientation_, const float rotation_, const Vec3 scale_) 
	{
		pos = pos_;
		orientation = orientation_;
		rotation = rotation_;
		scale = scale_;
		shader = nullptr;
	}

	Model::~Model() 
	{
		OnDestroy();
	}

	void Model::setPos(const Vec3& pos_) 
	{
		Entity::setPos(pos_);
		updateModelMatrix();
	}

	void Model::setOrientation(const Vec3& orientation_) 
	{
		Entity::setOrientation(orientation_);
		updateModelMatrix();
	}

	void Model::updateModelMatrix() { modelMatrix = MMath::translate(pos); }

	bool Model::OnCreate() 
	{
		shader = new Shader("phongVert.glsl", "phongFrag.glsl", 3, 0, "vVertex", 1, "vNormal", 2, "texCoords");
		updateModelMatrix();
		return true;
	}



	bool Model::LoadMesh(const char* filename) 
	{
		if (ObjLoader::loadOBJ(filename) == false) 
		{
			return false;
		}
		/// Get the data out of the ObjLoader and into our own mesh
		meshes.push_back(new Mesh(GL_TRIANGLES, ObjLoader::vertices, ObjLoader::normals, ObjLoader::uvCoords));
		return true;
	}


	void Model::Update(const float deltaTime) 
	{
	}

	void Model::Render() const 
	{

		GLint projectionMatrixID = glGetUniformLocation(shader->getProgram(), "projectionMatrix");
		GLint viewMatrixID = glGetUniformLocation(shader->getProgram(), "viewMatrix");
		GLint modelMatrixID = glGetUniformLocation(shader->getProgram(), "modelMatrix");
		GLint normalMatrixID = glGetUniformLocation(shader->getProgram(), "normalMatrix");
		GLint lightPosID = glGetUniformLocation(shader->getProgram(), "lightPos");

		glUseProgram(shader->getProgram());

		glUniformMatrix4fv(projectionMatrixID, 1, GL_FALSE, Camera::currentCamera->getProjectionMatrix());
		glUniformMatrix4fv(viewMatrixID, 1, GL_FALSE, Camera::currentCamera->getViewMatrix());
		Matrix4 _modelMatrix = MMath::rotate(rotation, 0.0f, 1.0f, 0.0f) * MMath::scale(scale) *
			MMath::translate(-10.0f, -30.0f, 0.0f) * modelMatrix;
		glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, _modelMatrix);
		/*** If you want to use the trackball use this code instead
		glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, modelMatrix * Trackball::getInstance()->getMatrix4());
		***/

		/// Assigning the 4x4 modelMatrix to the 3x3 normalMatrix 
		/// copies just the upper 3x3 of the modelMatrix
		Matrix3 normalMatrix = Matrix3(_modelMatrix); /// Converts the 4x4 model matrix to a 3x3
		/*** If you want to use the trackball use this code instead
		glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, modelMatrix * Trackball::getInstance()->getMatrix4());
		***/
		glUniformMatrix3fv(normalMatrixID, 1, GL_FALSE, normalMatrix);

		glUniform3fv(lightPosID, 1, SceneEnvironment::getInstance()->getLight());

		for (Mesh* mesh : meshes)
		{
			mesh->Render();
		}


	}
	void Model::OnDestroy() 
	{
		if (shader) delete shader;
	}
}