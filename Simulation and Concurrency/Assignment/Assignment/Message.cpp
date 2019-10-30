#include "Message.h"


Message::Message()
{
}

Message::Message(int r, int t)
{
	receiver = r;
	msgType = t;
	responded = false;
}

Message::~Message()
{
}

void Message::Answered()
{
	responded = true;
	delete this;
}