#pragma once
class Message
{
private:
	int receiver; // ID
	int msgType;
	bool responded;

public:
	Message();
	Message(int r, int t);
	~Message();

	void Answered();

	int GetReceiver()	{ return receiver; }
	int GetType()		{ return msgType; }

	bool HasResponded() { return responded; }
};

/* Message type index */
/* Receiver = 1: Physics
	type 1 = nudge
/* Receiver = 2: Renderer
/* Receiver = 3: Networking
*/