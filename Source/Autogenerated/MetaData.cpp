#include <Core/MetaSystem.h>
#include <Core/Application.h>
#include <Core/Transform.h>
#include <IO/Resource.h>
#include <Render/CameraComponent.h>
#include <Render/MeshComponent.h>

Variant* meta_CameraComponent_GetTransform(GigaObject* obj, int argc, Variant** argv) {
	CameraComponent* cobj = dynamic_cast<CameraComponent*>(obj);
	GIGA_ASSERT(cobj != 0, "Object is not of the correct type.");

	return(new Variant(cobj->GetTransform()));
}

Variant* meta_CameraComponent_SetViewport(GigaObject* obj, int argc, Variant** argv) {
	GIGA_ASSERT(argv[0]->IsInt(), "Incorrect type for argument 0.");

	GIGA_ASSERT(argv[1]->IsInt(), "Incorrect type for argument 1.");

	CameraComponent* cobj = dynamic_cast<CameraComponent*>(obj);
	GIGA_ASSERT(cobj != 0, "Object is not of the correct type.");

	cobj->SetViewport(argv[0]->AsInt(),argv[1]->AsInt());
	return(new Variant(0));
}

Variant* meta_MeshComponent_GetTransform(GigaObject* obj, int argc, Variant** argv) {
	MeshComponent* cobj = dynamic_cast<MeshComponent*>(obj);
	GIGA_ASSERT(cobj != 0, "Object is not of the correct type.");

	return(new Variant(cobj->GetTransform()));
}

Variant* meta_MeshComponent_Instantiate(GigaObject* obj, int argc, Variant** argv) {
	GIGA_ASSERT(argv[0]->IsObject(), "Incorrect type for argument 0.");

	MeshComponent* cobj = dynamic_cast<MeshComponent*>(obj);
	GIGA_ASSERT(cobj != 0, "Object is not of the correct type.");

	cobj->Instantiate(argv[0]->AsObject<Mesh *>());
	return(new Variant(0));
}

Variant* meta_Resource_GetData(GigaObject* obj, int argc, Variant** argv) {
	Resource* cobj = dynamic_cast<Resource*>(obj);
	GIGA_ASSERT(cobj != 0, "Object is not of the correct type.");

	return(new Variant(cobj->GetData()));
}

Variant* meta_Resource_Load(GigaObject* obj, int argc, Variant** argv) {
	Resource* cobj = dynamic_cast<Resource*>(obj);
	GIGA_ASSERT(cobj != 0, "Object is not of the correct type.");

	cobj->Load();
	return(new Variant(0));
}

Variant* meta_Resource_Unload(GigaObject* obj, int argc, Variant** argv) {
	Resource* cobj = dynamic_cast<Resource*>(obj);
	GIGA_ASSERT(cobj != 0, "Object is not of the correct type.");

	cobj->Unload();
	return(new Variant(0));
}

Variant* meta_ResourceSystem_AddSearchPath(GigaObject* obj, int argc, Variant** argv) {
	GIGA_ASSERT(argv[0]->IsString(), "Incorrect type for argument 0.");

	ResourceSystem* cobj = dynamic_cast<ResourceSystem*>(obj);
	GIGA_ASSERT(cobj != 0, "Object is not of the correct type.");

	cobj->AddSearchPath(argv[0]->AsString());
	return(new Variant(0));
}

Variant* meta_ResourceSystem_LoadResource(GigaObject* obj, int argc, Variant** argv) {
	GIGA_ASSERT(argv[0]->IsString(), "Incorrect type for argument 0.");

	GIGA_ASSERT(argv[1]->IsString(), "Incorrect type for argument 1.");

	ResourceSystem* cobj = dynamic_cast<ResourceSystem*>(obj);
	GIGA_ASSERT(cobj != 0, "Object is not of the correct type.");

	return(new Variant(cobj->LoadResource(argv[0]->AsString(),argv[1]->AsString())));
}

Variant* meta_Transform_GetForward(GigaObject* obj, int argc, Variant** argv) {
	Transform* cobj = dynamic_cast<Transform*>(obj);
	GIGA_ASSERT(cobj != 0, "Object is not of the correct type.");

	return(new Variant(cobj->GetForward()));
}

Variant* meta_Transform_GetLocalPosition(GigaObject* obj, int argc, Variant** argv) {
	Transform* cobj = dynamic_cast<Transform*>(obj);
	GIGA_ASSERT(cobj != 0, "Object is not of the correct type.");

	return(new Variant(cobj->GetLocalPosition()));
}

Variant* meta_Transform_GetLocalRotation(GigaObject* obj, int argc, Variant** argv) {
	Transform* cobj = dynamic_cast<Transform*>(obj);
	GIGA_ASSERT(cobj != 0, "Object is not of the correct type.");

	return(new Variant(cobj->GetLocalRotation()));
}

Variant* meta_Transform_GetLocalScaling(GigaObject* obj, int argc, Variant** argv) {
	Transform* cobj = dynamic_cast<Transform*>(obj);
	GIGA_ASSERT(cobj != 0, "Object is not of the correct type.");

	return(new Variant(cobj->GetLocalScaling()));
}

Variant* meta_Transform_GetRight(GigaObject* obj, int argc, Variant** argv) {
	Transform* cobj = dynamic_cast<Transform*>(obj);
	GIGA_ASSERT(cobj != 0, "Object is not of the correct type.");

	return(new Variant(cobj->GetRight()));
}

Variant* meta_Transform_GetTransform(GigaObject* obj, int argc, Variant** argv) {
	Transform* cobj = dynamic_cast<Transform*>(obj);
	GIGA_ASSERT(cobj != 0, "Object is not of the correct type.");

	return(new Variant(cobj->GetTransform()));
}

Variant* meta_Transform_GetUp(GigaObject* obj, int argc, Variant** argv) {
	Transform* cobj = dynamic_cast<Transform*>(obj);
	GIGA_ASSERT(cobj != 0, "Object is not of the correct type.");

	return(new Variant(cobj->GetUp()));
}

Variant* meta_Transform_GetWorldPosition(GigaObject* obj, int argc, Variant** argv) {
	Transform* cobj = dynamic_cast<Transform*>(obj);
	GIGA_ASSERT(cobj != 0, "Object is not of the correct type.");

	return(new Variant(cobj->GetWorldPosition()));
}

Variant* meta_Transform_GetWorldRotation(GigaObject* obj, int argc, Variant** argv) {
	Transform* cobj = dynamic_cast<Transform*>(obj);
	GIGA_ASSERT(cobj != 0, "Object is not of the correct type.");

	return(new Variant(cobj->GetWorldRotation()));
}

Variant* meta_Transform_GetWorldScaling(GigaObject* obj, int argc, Variant** argv) {
	Transform* cobj = dynamic_cast<Transform*>(obj);
	GIGA_ASSERT(cobj != 0, "Object is not of the correct type.");

	return(new Variant(cobj->GetWorldScaling()));
}

Variant* meta_Transform_Move(GigaObject* obj, int argc, Variant** argv) {
	GIGA_ASSERT(argv[0]->IsVector3(), "Incorrect type for argument 0.");

	Transform* cobj = dynamic_cast<Transform*>(obj);
	GIGA_ASSERT(cobj != 0, "Object is not of the correct type.");

	cobj->Move(argv[0]->AsVector3());
	return(new Variant(0));
}

Variant* meta_Transform_Rotate(GigaObject* obj, int argc, Variant** argv) {
	GIGA_ASSERT(argv[0]->IsVector3(), "Incorrect type for argument 0.");

	GIGA_ASSERT(argv[1]->IsFloat(), "Incorrect type for argument 1.");

	Transform* cobj = dynamic_cast<Transform*>(obj);
	GIGA_ASSERT(cobj != 0, "Object is not of the correct type.");

	cobj->Rotate(argv[0]->AsVector3(),argv[1]->AsFloat());
	return(new Variant(0));
}

Variant* meta_Transform_Scale(GigaObject* obj, int argc, Variant** argv) {
	GIGA_ASSERT(argv[0]->IsVector3(), "Incorrect type for argument 0.");

	Transform* cobj = dynamic_cast<Transform*>(obj);
	GIGA_ASSERT(cobj != 0, "Object is not of the correct type.");

	cobj->Scale(argv[0]->AsVector3());
	return(new Variant(0));
}

Variant* meta_Transform_SetLocalPosition(GigaObject* obj, int argc, Variant** argv) {
	GIGA_ASSERT(argv[0]->IsVector3(), "Incorrect type for argument 0.");

	Transform* cobj = dynamic_cast<Transform*>(obj);
	GIGA_ASSERT(cobj != 0, "Object is not of the correct type.");

	cobj->SetLocalPosition(argv[0]->AsVector3());
	return(new Variant(0));
}

Variant* meta_Transform_SetLocalRotation(GigaObject* obj, int argc, Variant** argv) {
	GIGA_ASSERT(argv[0]->IsQuaternion(), "Incorrect type for argument 0.");

	Transform* cobj = dynamic_cast<Transform*>(obj);
	GIGA_ASSERT(cobj != 0, "Object is not of the correct type.");

	cobj->SetLocalRotation(argv[0]->AsQuaternion());
	return(new Variant(0));
}

Variant* meta_Transform_SetLocalScaling(GigaObject* obj, int argc, Variant** argv) {
	GIGA_ASSERT(argv[0]->IsVector3(), "Incorrect type for argument 0.");

	Transform* cobj = dynamic_cast<Transform*>(obj);
	GIGA_ASSERT(cobj != 0, "Object is not of the correct type.");

	cobj->SetLocalScaling(argv[0]->AsVector3());
	return(new Variant(0));
}

Variant* meta_Transform_SetParent(GigaObject* obj, int argc, Variant** argv) {
	GIGA_ASSERT(argv[0]->IsObject(), "Incorrect type for argument 0.");

	Transform* cobj = dynamic_cast<Transform*>(obj);
	GIGA_ASSERT(cobj != 0, "Object is not of the correct type.");

	cobj->SetParent(argv[0]->AsObject<Transform *>());
	return(new Variant(0));
}

Variant* meta_Transform_SetRoot(GigaObject* obj, int argc, Variant** argv) {
	GIGA_ASSERT(argv[0]->IsBool(), "Incorrect type for argument 0.");

	Transform* cobj = dynamic_cast<Transform*>(obj);
	GIGA_ASSERT(cobj != 0, "Object is not of the correct type.");

	cobj->SetRoot(argv[0]->AsBool());
	return(new Variant(0));
}

Variant* meta_Transform_SetWorldPosition(GigaObject* obj, int argc, Variant** argv) {
	GIGA_ASSERT(argv[0]->IsVector3(), "Incorrect type for argument 0.");

	Transform* cobj = dynamic_cast<Transform*>(obj);
	GIGA_ASSERT(cobj != 0, "Object is not of the correct type.");

	cobj->SetWorldPosition(argv[0]->AsVector3());
	return(new Variant(0));
}

Variant* meta_Transform_SetWorldRotation(GigaObject* obj, int argc, Variant** argv) {
	GIGA_ASSERT(argv[0]->IsQuaternion(), "Incorrect type for argument 0.");

	Transform* cobj = dynamic_cast<Transform*>(obj);
	GIGA_ASSERT(cobj != 0, "Object is not of the correct type.");

	cobj->SetWorldRotation(argv[0]->AsQuaternion());
	return(new Variant(0));
}

Variant* meta_Transform_SetWorldScaling(GigaObject* obj, int argc, Variant** argv) {
	GIGA_ASSERT(argv[0]->IsVector3(), "Incorrect type for argument 0.");

	Transform* cobj = dynamic_cast<Transform*>(obj);
	GIGA_ASSERT(cobj != 0, "Object is not of the correct type.");

	cobj->SetWorldScaling(argv[0]->AsVector3());
	return(new Variant(0));
}

void RegisterMetaFunctions() {
	MetaSystem* metaSystem = GetSystem<MetaSystem>();

	metaSystem->RegisterFunction("CameraComponent", "GetTransform", meta_CameraComponent_GetTransform);
	metaSystem->RegisterFunction("CameraComponent", "SetViewport", meta_CameraComponent_SetViewport);

	metaSystem->RegisterFunction("MeshComponent", "GetTransform", meta_MeshComponent_GetTransform);
	metaSystem->RegisterFunction("MeshComponent", "Instantiate", meta_MeshComponent_Instantiate);

	metaSystem->RegisterFunction("Resource", "GetData", meta_Resource_GetData);
	metaSystem->RegisterFunction("Resource", "Load", meta_Resource_Load);
	metaSystem->RegisterFunction("Resource", "Unload", meta_Resource_Unload);

	metaSystem->RegisterFunction("ResourceSystem", "AddSearchPath", meta_ResourceSystem_AddSearchPath);
	metaSystem->RegisterFunction("ResourceSystem", "LoadResource", meta_ResourceSystem_LoadResource);

	metaSystem->RegisterFunction("Transform", "GetForward", meta_Transform_GetForward);
	metaSystem->RegisterFunction("Transform", "GetLocalPosition", meta_Transform_GetLocalPosition);
	metaSystem->RegisterFunction("Transform", "GetLocalRotation", meta_Transform_GetLocalRotation);
	metaSystem->RegisterFunction("Transform", "GetLocalScaling", meta_Transform_GetLocalScaling);
	metaSystem->RegisterFunction("Transform", "GetRight", meta_Transform_GetRight);
	metaSystem->RegisterFunction("Transform", "GetTransform", meta_Transform_GetTransform);
	metaSystem->RegisterFunction("Transform", "GetUp", meta_Transform_GetUp);
	metaSystem->RegisterFunction("Transform", "GetWorldPosition", meta_Transform_GetWorldPosition);
	metaSystem->RegisterFunction("Transform", "GetWorldRotation", meta_Transform_GetWorldRotation);
	metaSystem->RegisterFunction("Transform", "GetWorldScaling", meta_Transform_GetWorldScaling);
	metaSystem->RegisterFunction("Transform", "Move", meta_Transform_Move);
	metaSystem->RegisterFunction("Transform", "Rotate", meta_Transform_Rotate);
	metaSystem->RegisterFunction("Transform", "Scale", meta_Transform_Scale);
	metaSystem->RegisterFunction("Transform", "SetLocalPosition", meta_Transform_SetLocalPosition);
	metaSystem->RegisterFunction("Transform", "SetLocalRotation", meta_Transform_SetLocalRotation);
	metaSystem->RegisterFunction("Transform", "SetLocalScaling", meta_Transform_SetLocalScaling);
	metaSystem->RegisterFunction("Transform", "SetParent", meta_Transform_SetParent);
	metaSystem->RegisterFunction("Transform", "SetRoot", meta_Transform_SetRoot);
	metaSystem->RegisterFunction("Transform", "SetWorldPosition", meta_Transform_SetWorldPosition);
	metaSystem->RegisterFunction("Transform", "SetWorldRotation", meta_Transform_SetWorldRotation);
	metaSystem->RegisterFunction("Transform", "SetWorldScaling", meta_Transform_SetWorldScaling);

}

