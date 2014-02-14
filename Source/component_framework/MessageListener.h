#ifndef INC_CF_MESSAGE_LISTENER_H
#define INC_CF_MESSAGE_LISTENER_H

// #include <vector>

// classes
// * IMessageHandlers
// * IMessageHandler
// * MessageListeners
// * MessageListener
// * MessageSender
// ---

// Includes the MessageListener interface, you have to inherit the 
// MessageListener< CompType, MessageType > class
// and implement the 
// void HandleMessage( SGF::Entity* entity, CompType* comp, MessageType* message )
// function
// 

/* e.g. of how to use

	class MessageTestSystem : 
		public ComponentUpdator< BuildingComponent, false >, 
		public MessageListener< BuildingComponent, Message_PhysicsCollision >,
		public MessageListener< BuildingComponent, Message_PhysicsCollision2 >
	{
	public:
		MessageTestSystem( SGF::EventManager* eventManager, SGF::EntityManager* entityManager ) : ComponentUpdator( eventManager, entityManager ) { }
		void RefreshComponent( SGF::Entity* e, BuildingComponent* comp )  { }
		void UpdateComponent( SGF::Entity* e, BuildingComponent* comp )  { }

		void HandleMessage( SGF::Entity* entity, BuildingComponent* scomp, Message_PhysicsCollision* message )
		{
			message->_done = true;
			// message implementation
		}

		void HandleMessage( SGF::Entity* entity, BuildingComponent* scomp, Message_PhysicsCollision2* message )
		{
			// message implementation
		}
	};

*/

// --------------------------------------------------------------

namespace SGF { class Entity; }

// --------------------------------------------------------------

template< class TMessageType >
class IMessageHandlers
{
public:
	IMessageHandlers() { }
	~IMessageHandlers() { }

	std::vector< void* > mListeners;
};

//---------------------

template< class TMessageType >
class IMessageHandler
{
public:
	IMessageHandler() 
	{
		ceng::GetSingletonPtr< IMessageHandlers< TMessageType > >()->mListeners.push_back( (void*)this );
	}

	~IMessageHandler()
	{
		// remove this listener
		// ceng::VectorRemove(...)
		std::vector< void* >& container = ceng::GetSingletonPtr< IMessageHandlers< TMessageType > >()->mListeners;
		void* element = (void*)this;
		for( std::size_t i = 0; i < container.size(); ++i ) 
		{
			if( container[ i ] == element ) {
				container[ i ] = container[ container.size() - 1 ];
				container.pop_back();
				break;
			}
		}
		// ceng::GetSingletonPtr< IMessageHandlers< TMessageType > >()->mListeners.erase( (void*)this );
	}

	virtual void HandleMessage( TMessageType* message ) { }
	virtual void HandleMessage( SGF::Entity* entity, TMessageType* message ) { }


};

// --------------------------------------------------------------

template< class TMessageType >
class MessageListeners : public ceng::CStaticSingleton< MessageListeners< TMessageType > >
{
public:
	
	void SendMessage( const TMessageType& message )
	{
		for( std::size_t i = 0; i < mListeners.size(); ++i )
		{
			mListeners[i]->HandleMessage( message );
		}
	}

	void SendMessage( SGF::Entity* entity, const TMessageType& message )
	{
		for( std::size_t i = 0; i < mListeners.size(); ++i ) 
		{
			mListeners[i]->HandleMessage( entity, message );
		}
	}

	std::vector< IMessageHandler< TMessageType >* > mListeners;

private:
	MessageListeners() { }
	friend class ceng::CStaticSingleton< MessageListeners< TMessageType > >;

};

// --------------------------------------------------------------

template< class CompType, class MessageType >
class MessageListener : public IMessageHandler< MessageType >
{
public:
	MessageListener() :	IMessageHandler< MessageType >() { }
	
	// Send the message to everyone who likes the type MessageType
	virtual void HandleMessage( MessageType* message ) 
	{ 
		// we assume it's multiple inheritance from SGF::EntitySystem
		SGF::EntitySystem* entity_system = dynamic_cast< SGF::EntitySystem* >( this );
		if( entity_system == NULL )
		{
			std::cout << "HandleMessage() - error happened, we're not of type SGF::EntitySystem" << std::endl;
			return;
		}

		const std::set< SGF::Entity* >& entities = entity_system->GetActivesConst();
		std::set< SGF::Entity* >::const_iterator i;
		for( i = entities.begin(); i != entities.end(); ++i )
		{
			SGF::Entity* entity = *i;
			SGF::ComponentMapper< CompType > cmap = entity_system->GetEntityManager()->GetComponentList< CompType >( entity );
			
			for( int j = 0; j < cmap.size(); ++j ) 
			{
				HandleMessage( entity, cmap[j], message );
				if( message->_done ) 
					return;
			}
		}
	}

	virtual void HandleMessage( SGF::Entity* entity, MessageType* message )
	{

		SGF::EntitySystem* entity_system = dynamic_cast< SGF::EntitySystem* >( this );
		if( entity_system == NULL )
		{
			std::cout << "HandleMessage() - error happened, we're not of type SGF::EntitySystem" << std::endl;
			return;
		}
		
		SGF::ComponentMapper< CompType > cmap = entity_system->GetEntityManager()->GetComponentList< CompType >( entity );
			
		for( int j = 0; j < cmap.size(); ++j ) 
		{
			HandleMessage( entity, cmap[j], message );
			if( message->_done ) 
				return;
		}
	}

	virtual void HandleMessage( SGF::Entity* entity, CompType* scomp, MessageType* message ) = 0;
};

//-----------------------------------------------------------------------------

class MessageSender
{
public:
	virtual ~MessageSender() { }

	template< class TMessageType >
	void SendMessage( TMessageType& message )
	{
		SendMessage( &message );
	}

	// handle the pointer case
	template< class TMessageType >
	void SendMessage( TMessageType* message )
	{
		if( message == NULL ) return;

		std::vector< void* >& listeners = ceng::GetSingletonPtr< IMessageHandlers< TMessageType > >()->mListeners;
		IMessageHandler< TMessageType >* current = NULL; 
		for( std::size_t i = 0; i < listeners.size(); ++i )
		{
			current = (IMessageHandler< TMessageType >*)listeners[i];
			current->HandleMessage( message );
			if( message->_done ) 
				return;
		}
	}

	template< class TMessageType >
	void SendMessage( SGF::Entity* entity, TMessageType& message )
	{
		SendMessage( entity, &message );
	}

	// pointer safe guard
	template< class TMessageType >
	void SendMessage( SGF::Entity* entity, TMessageType* message )
	{
		if( message == NULL ) return;
		
		std::vector< void* >& listeners = ceng::GetSingletonPtr< IMessageHandlers< TMessageType > >()->mListeners;
		IMessageHandler< TMessageType >* current = NULL; 
		for( std::size_t i = 0; i < listeners.size(); ++i )
		{
			current = (IMessageHandler< TMessageType >*)listeners[i];
			current->HandleMessage( entity, message );
			if( message->_done )
				return;
		}
	}
};

//-----------------------------------------------------------------------------

#endif
