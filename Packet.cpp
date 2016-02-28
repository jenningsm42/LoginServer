#include "Packet.h"
#include <netinet/in.h>

Packet::Packet(short type) : type(type), ptr(0)
{
}

Packet::Packet(const char* buffer, size_t len)
{
    setBuffer(buffer, len);
}

Packet::Packet(Packet* other)
{
    memcpy(this, other, sizeof(Packet));
}

Packet::~Packet()
{
}

void Packet::pushChar(char val)
{
    if(ptr + sizeof(char) <= MAX_LEN)
        data[ptr++] = val;
}

void Packet::pushShort(short val)
{
    if(ptr + sizeof(short) <= MAX_LEN)
    {
        val = htons(val);
        data[ptr++] = val & 0xff;
        data[ptr++] = (val >> 8) & 0xff;
    }
}

void Packet::pushInt(int val)
{
    if(ptr + sizeof(int) <= MAX_LEN)
    {
        val = htonl(val);
        data[ptr++] = val & 0xff;
        data[ptr++] = (val >> 8) & 0xff;
        data[ptr++] = (val >> 16) & 0xff;
        data[ptr++] = (val >> 24) & 0xff;
    }
}

void Packet::pushCharArray(const char* pVal, size_t len)
{
    if(ptr + len <= MAX_LEN)
    {
		unsigned int i = 0;
        for(; i < len; i++, ptr++)
		{
			if(pVal[i] == 0) break;
			data[ptr] = pVal[i];
		}
		for(; i < len; i++, ptr++) // For extra padding
			data[ptr] = 0;
    }
}

char Packet::popChar()
{
	if(ptr - sizeof(char) >= 0)
		return data[--ptr];
    return 0;
}

short Packet::popShort()
{
    if(ptr - sizeof(short) >= 0)
    {
        char tmp[sizeof(short)];
        tmp[1] = data[--ptr];
        tmp[0] = data[--ptr];
        return ntohs(*(short*)tmp);
    }
    return 0;
}

int Packet::popInt()
{
    if(ptr - sizeof(int) >= 0)
    {
        char tmp[sizeof(int)];
        tmp[3] = data[--ptr];
        tmp[2] = data[--ptr];
        tmp[1] = data[--ptr];
        tmp[0] = data[--ptr];
        return ntohl(*(int*)tmp);
    }
    return 0;
}

char* Packet::popCharArray(size_t len)
{
    if(ptr - len >= 0)
    {
        char* tmp = new char[len];
        memcpy(tmp, data + ptr - len, len);
        ptr -= len;
        return tmp;
    }
    return (char*)"";
}

void Packet::setBuffer(const char* buffer, size_t len)
{
    memcpy(data, buffer + sizeof(short), len);
    ptr = len;
    char tmp[sizeof(short)];
    tmp[0] = buffer[0];
    tmp[1] = buffer[1];
    type = ntohs(*(short*)tmp);
}

char* Packet::getBuffer()
{
    short nlen = htons(ptr);
    short ntype = htons(type);
    char* buffer = new char[2*sizeof(short) + ptr];
    memcpy(buffer, &nlen, sizeof(short));
    memcpy(buffer + sizeof(short), &ntype, sizeof(short));
    if(ptr > 0) memcpy(buffer + 2*sizeof(short), data, ptr);
    return buffer;
}

void Packet::setType(short val)
{
    type = val;
}

short Packet::getType()
{
    return type;
}

size_t Packet::length()
{
    return ptr + 2*sizeof(short);
}
