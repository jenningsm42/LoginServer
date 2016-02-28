#ifndef PACKETTYPES_H
#define PACKETTYPES_H

enum PacketTypesRecv
{
    E_PT_RECV_LOGIN = 0x00  // username(24), password(24)
};

enum PacketTypesSend
{
    E_PT_SEND_LOGIN_SUCCESS     	= 0x00,  // -
    E_PT_SEND_INVALID_USERNAME  	= 0x01,  // -
    E_PT_SEND_INVALID_PASSWORD 		= 0x02,  // -
    E_PT_SEND_ALREADY_AUTHENTICATED	= 0x03,	// -
    E_PT_SEND_REALM_LIST        	= 0x04   // {IP(16), name(32)}^2
};

#endif // PACKETTYPES_H
