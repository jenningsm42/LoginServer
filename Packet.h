#ifndef PACKET_H
#define PACKET_H
#define MAX_LEN 2048
#include <cstring>

class Packet
{
public:
    Packet(short type = -1);
    Packet(const char* buffer, size_t len);
    Packet(Packet* other);
    ~Packet();

    void pushChar(char val);
    void pushShort(short val);
    void pushInt(int val);
    void pushCharArray(const char* pVal, size_t len);

    char popChar();
    short popShort();
    int popInt();
    char* popCharArray(size_t len); // Creates new pointer, must be deleted

    void setBuffer(const char* buffer, size_t len); // type and data
    char* getBuffer(); // size, type, and data

    void setType(short val);
    short getType();

    size_t length(); // includes size and type (+4)

private:
    char data[MAX_LEN];
    short type;
    int ptr;
};

#endif // PACKET_H
