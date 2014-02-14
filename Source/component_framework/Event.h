#ifndef __EVENT_H__
#define __EVENT_H__

namespace SGF
{
	typedef unsigned int EventType;

	class IEventData {
	public:
		IEventData(EventType type) : _type(type) {}

		EventType GetType() const { return _type; }

	private:
		EventType _type;
	};

	class IEventListener {
	public:
		virtual ~IEventListener() {}

		virtual bool HandleEvent( IEventData const &evt ) = 0;
	};
};

#endif