#include "Entity.h"

Entity::Entity(std::shared_ptr<Mesh> inputMesh, std::shared_ptr<Material> material)
{
	this->material = material;
	mesh = inputMesh;
	transform = std::shared_ptr<Transformation>(new Transformation());
}

Entity::~Entity()
{

}

std::shared_ptr<Material> Entity::GetMaterial()
{
	return material;
}

std::shared_ptr<Mesh> Entity::GetMesh()
{
	return mesh;
}

std::shared_ptr<Transformation> Entity::GetTransformation()
{
	return transform;
}

void Entity::SetTransformation(std::shared_ptr<Transformation> transformation)
{
	transform = transformation;
}
