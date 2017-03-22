#include "System.h"

#include "RigidBody.h"

#include <assimp\scene.h>
#include <assimp\postprocess.h>
#include <assimp\material.h>
#include <assimp\Importer.hpp>
#include <iostream>

#include "Entity.h"
#include "Util.h"

using namespace physx;

void RigidBody::RegisterHandlers()
{
	auto t = body_->getGlobalPose();
	entity_->GetTransform().position = vec3(t.p.x, t.p.y, t.p.z);
	entity_->GetTransform().rotation = quaternion(t.q.w, t.q.x, t.q.y, t.q.z);
	body_->userData = entity_;
}

RigidBody::RigidBody(Physics &physics, PxMaterial &material, PxGeometry &geometry, PxTransform &transform, bool dynamic) :
	physics_(physics)
{
	if (dynamic) body_ = physics.GetPhysics()->createRigidDynamic(transform);
	else body_ = physics.GetPhysics()->createRigidStatic(transform);

	auto shape = body_->createShape(geometry, material);

	shape->setQueryFilterData(PxFilterData { 0, 0, 0, Physics::DRIVABLE_SURFACE });
	shape->setSimulationFilterData(PxFilterData { Physics::GROUND, Physics::GROUND_COLLISIONS, 0, 0 });
	physics.GetScene()->addActor(*body_);
}

// TODO stolen from vehicle.cpp deduplicate this
static PxTriangleMesh *CreateMesh(const std::vector<vec3> &verts, PxPhysics *physics, PxCooking *cooking)
{
	PxTriangleMeshDesc desc;
	desc.points.count = (PxU32)verts.size();
	desc.points.stride = sizeof(vec3);
	desc.points.data = verts.data();
	/*
	// Create descriptor for convex mesh
	PxConvexMeshDesc convexDesc;
	convexDesc.points.count = (PxU32)verts.size();
	convexDesc.points.stride = sizeof(vec3);
	convexDesc.points.data = verts.data();
	convexDesc.flags = PxConvexFlag::eCOMPUTE_CONVEX;// | PxConvexFlag::eINFLATE_CONVEX;
	
	PxConvexMesh* convexMesh = NULL;
	
	PxDefaultMemoryOutputStream buf;
	if (cooking->cookConvexMesh(convexDesc, buf))
	{
		PxDefaultMemoryInputData id(buf.getData(), buf.getSize());
		convexMesh = physics->createConvexMesh(id);
	}
	*/
	PxDefaultMemoryOutputStream buf;
	if (cooking->cookTriangleMesh(desc, buf)) {
		PxDefaultMemoryInputData data(buf.getData(), buf.getSize());
		return physics->createTriangleMesh(data);
	}

	return nullptr;
}

static PxTriangleMeshGeometry CreateGeometry(const char *objfile, Physics &physics, float scale)
{
	// TODO stolen from Mesh.cpp make this not duplicated
	Assimp::Importer importer;
	auto fullfilename = Util::ModelDirectory + objfile;
	const aiScene* objFile = importer.ReadFile(fullfilename.c_str(), aiProcess_Triangulate | aiProcess_JoinIdenticalVertices);
	if (!objFile)
	{
		std::cerr << "could not load file " << objFile << ": " << importer.GetErrorString() << std::endl;
	}
	std::cout << objfile << std::endl;
	std::cout << "num of meshes: " << objFile->mNumMeshes << std::endl;
	std::vector<glm::vec3> vertices;
	//std::vector<glm::vec3> normals;
	//std::vector<glm::vec3> colours;
	aiMesh* objMesh = objFile->mMeshes[0];
	std::cout << "num of faces: " << objMesh->mNumFaces << std::endl;
	std::cout << "num of verts: " << objMesh->mNumVertices << std::endl;
	for (std::uint32_t i = 0u; i < objMesh->mNumFaces; i++)
	{
		const aiFace& face = objMesh->mFaces[i];
		for (int j = 0; j < 3; j++)
		{
			//aiVector3D norm = objMesh->mNormals[face.mIndices[j]];
			aiVector3D vert = objMesh->mVertices[face.mIndices[j]];
			if (objfile == "test_map_mesh.fbx")
			{
				//std::cout << vert.x << " " << vert.y << " " << vert.z << std::endl;
			}
			vec3 vertice(vert.x * scale, vert.y * scale, vert.z * scale);
			//vec3 normal(norm.x, norm.y, norm.z);
			vertices.push_back(vertice);
			//normals.push_back(normal);
			//colours.push_back(vec3();
		}
	}

	// no longer stolen
	auto mesh = CreateMesh(vertices, physics.GetPhysics(), physics.GetCooking());
	auto geom = PxTriangleMeshGeometry(mesh);

	//mesh->release();

	return geom;
}

RigidBody::RigidBody(Physics &physics, PxMaterial &material, const char *objfile, float scale, bool dynamic) :
	RigidBody(physics, material, CreateGeometry(objfile, physics, scale), PxTransform(PxIdentity), dynamic)
{}

RigidBody::~RigidBody()
{
	physics_.GetScene()->removeActor(*body_);
	body_->release();
}
