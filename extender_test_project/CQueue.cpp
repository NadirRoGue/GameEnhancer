#include "stdafx.h"

CQueue::CQueue(VOID) {
	size = 0;
	root = new Node;
	root->next = 0;
	root->p = 0;
	last = root;
}

CQueue::~CQueue(VOID) {
	Node *del = root;
	Node *temp = 0;
	do {
		temp = del->next;
		delete del;
	} while(temp != 0);
}

VOID CQueue::push(TCPPacket *packet) {
	last->p = packet;
	last->next = new Node;
	last->next->next = 0;
	last->next->p = 0;
}

TCPPacket * CQueue::top(VOID) {
	return root->p;
}

VOID CQueue::pop(VOID) {
	Node *temp = root;
	root = root->next;
	delete temp;
}

BOOL CQueue::empty(VOID) {
	return root == 0 || root->p == 0;
}