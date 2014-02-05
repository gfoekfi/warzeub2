#ifndef VEC_TYPES_H_
#define VEC_TYPES_H_


// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

const float FLOAT_PRECISION = 0.000001f;

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

template <typename T>
union vec2
{
	vec2(T parX, T parY) : x(parX), y(parY) { }
	vec2(const vec2& parRhs) : x(parRhs.x), y (parRhs.y) { }
	vec2() : x(0), y(0) { }

	vec2& operator=(const vec2& parRhs)
	{
		x = parRhs.x;
		y = parRhs.y;

		return *this;
	}

	vec2& operator+=(const vec2& parRhs)
	{
		x += parRhs.x;
		y += parRhs.y;

		return *this;
	}

	vec2 operator+(const vec2& parRhs)
	{
		vec2 result(x + parRhs.x, y + parRhs.y);
		return result;
	}

	bool operator==(const vec2& parRhs) const // FIXME: Dangerous for floating type
	{
		return (x == parRhs.x && y == parRhs.y);
	}

	bool operator!=(const vec2& parRhs) const // FIXME: Dangerous for floating type
	{
		return !(*this == parRhs);
	}

	bool operator<(const vec2& parRhs) const
	{
		if (x < parRhs.x)
			return true;
		else if (x > parRhs.x)
			return false;

		if (y < parRhs.y)
			return true;
		else if (y > parRhs.y)
			return false;

		return false;
	}

   struct { T x, y; };
   struct { T u, v; };
   struct { T w, h; };
   struct { T values[2]; };
};

// ============================================================================

typedef vec2<int> int2;
typedef vec2<float> float2;

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================


#endif