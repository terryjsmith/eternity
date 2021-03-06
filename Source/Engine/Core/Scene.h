
#ifndef scene_h
#define scene_h

#include <eternity.h>
#include <Core/GigaObject.h>
#include <Core/Entity.h>

GIGA_CLASS()
class GIGA_API Scene : public GigaObject {
public:
	Scene() = default;
	~Scene() = default;

    GIGA_CLASS_NAME("Scene");

	int GetSceneID();

	/**
	 * Get/set name
	 */
	void SetName(std::string name) { m_name = name; }
	std::string GetName() { return m_name; }

	/**
	 * Add entities
	 */
	void AddEntity(Entity* entity);
	std::vector<Entity*> GetEntities();

protected:
	 int m_sceneID;
	 std::string m_name;

	std::vector<Entity*> m_entities;
};

#endif
