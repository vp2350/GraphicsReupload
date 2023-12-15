#pragma 

#include "Mesh.h"
#include "Transformation.h"
#include "Material.h"
#include <memory>

class Entity
{
public:
	Entity(std::shared_ptr<Mesh> inputMesh, std::shared_ptr<Material> material);
	~Entity();
	std::shared_ptr<Material> GetMaterial();
	std::shared_ptr<Mesh> GetMesh();
	std::shared_ptr<Transformation> GetTransformation();
	void SetTransformation(std::shared_ptr<Transformation> transformation);

private: 
	std::shared_ptr<Material> material;
	std::shared_ptr<Mesh> mesh;
	std::shared_ptr<Transformation> transform;
};

