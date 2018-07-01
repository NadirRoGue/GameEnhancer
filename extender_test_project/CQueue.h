#ifndef CQUEUE_H_
#define CQUEUE_H_

#include "PacketHandler.h"

struct Node {
	TCPPacket *p;
	Node *next;
};

class CQueue {
private:
	Node *root, *last;
	INT size;
public:
	CQueue(VOID);
	~CQueue(VOID);
	VOID push(TCPPacket *p);
	TCPPacket * top(VOID);
	VOID pop(VOID);
	BOOL empty(VOID);
};

#endif