#include "FileReader.h"

using namespace GAME;

FileReader::FileReader() { }

FileReader::~FileReader() { }

std::vector<Model*> FileReader::LoadFile(char file[])
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
		pugi::xml_node parent = doc.child("Objects");

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


			if (!addModel(name, pos, rot)) return models;
			//empty temp values
			name = "";
			pos = Vec3(0.0f, 0.0f, 0.0f);
			rot = 0.0f;

		}
	}
	else { return models; }

	return models;
}

void FileReader::SaveFile(char file[], std::vector<Model*> models_)
{
	pugi::xml_document doc;
	pugi::xml_parse_result result;
	float index = 0;

	result = doc.load_file(file);

	if (result)
	{
		pugi::xml_node parent = doc.child("Objects");
		doc.remove_child(parent);

		parent = doc.append_child("Objects");
		for (Model *m : models_)
		{
			//add node with some name
			pugi::xml_node node = parent.append_child("object");
			//node.set_name(m->name.c_str());
			node.append_attribute("name").set_value(m->name.c_str());

			// add description node with text child
			pugi::xml_node pos = node.append_child("pos");
			pos.append_attribute("X").set_value(m->getPosition().x);
			pos.append_attribute("Y").set_value(m->getPosition().y);
			pos.append_attribute("Z").set_value(m->getPosition().z);
			printf("child added ");
		}

		doc.save_file(file);
	}
}

bool FileReader::addModel(string file_, const Vec3 pos, const float rot)
{

	models.push_back(new Model(pos, Vec3(0.0f, 0.0f, 0.0f), rot, Vec3(0.05f, 0.05f, 0.05f), Vec3(0.0f, 0.0f, 0.0f), file_.c_str()));
	models[models.size() - 1]->OnCreate();
	
	if (models[models.size() - 1]->LoadMesh(file_.c_str()) == false)
	{
		return false;
	}

	return true;
}
