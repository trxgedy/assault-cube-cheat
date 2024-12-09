#ifndef VEC_HPP
#define VEC_HPP

class Vector3
{
public:
	float x, y, z;

	Vector3 operator-( const Vector3 &arg )
	{
		return Vector3 { x - arg.x, y - arg.y, z - arg.z };
	}

	float get_hypotenuse( )
	{
		return std::sqrt( ( x * x ) + ( y * y ) + ( z * z ) );
	}
};

class Vector2
{
public:
	float x, y;
};

#endif