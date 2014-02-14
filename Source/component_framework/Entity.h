#ifndef __ENTITY_H__
#define __ENTITY_H__

#include <string>
#include <vector>
#include <utils/math/cxform.h>


namespace SGF {
	
typedef unsigned int EntityID;

// Define the null entity id
const EntityID NULL_ENTITY = 0;

//-----------------------------------------------------------------------------

class Entity {
	friend class EntityManager;
	friend class EntitySystem;
	friend class EntitySerializer;
public:
	~Entity();

	// Get the this entity's ID
	// ID is a unique indentifier to this Entity
	EntityID GetID() const { return _id; }

	// name property, doesn't have to be unique
	const std::string&	GetName() const		{ return mName; }
	std::string&		GetNameRef()		{ return mName; }
	void SetName( const std::string& name ) { mName = name; }

	// transform...
	types::xform*	GetTransform();
	bool			HasTransform() const	{ return ( mTransform != NULL ); }

	// tags
	std::vector< std::string>* GetTags();
	bool HasTag( const std::string& tag );

	// death
	bool IsDead() const						{ return _dead; }
	void Kill()								{ _dead = true; }

private:

	// Only the parent class should be able to instantiate a new entity.
	Entity( EntityID id );
	void RemoveTag( const std::string& tag );

	const EntityID	_id;
	unsigned int	_index;
	unsigned int	_typeBits;
	bool			_dead;	
	std::string		mName;
	std::vector< std::string >* _tags;

	types::xform*	mTransform;
};

//-----------------------------------------------------------------------------

} // end of namespace SGF

#endif