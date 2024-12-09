#ifndef ENTITY_HPP
#define ENTITY_HPP

class c_entity
{
public:
	char pad_0000[ 4 ]; //0x0000
	Vector3 head_pos; //0x0004
	char pad_0010[ 36 ]; //0x0010
	Vector2 view_angle; //0x0034
	char pad_003C[ 58 ]; //0x003C
	uint8_t state; //0x0076
	char pad_0077[ 117 ]; //0x0077
	int32_t health; //0x00EC
	char pad_00F0[ 540 ]; //0x00F0
	int32_t team; //0x030C
};

#endif