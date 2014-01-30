#ifndef VEC_TYPES_H_
#define VEC_TYPES_H_


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

	bool operator==(const vec2& parRhs) const
	{
		return (x == parRhs.x && y == parRhs.y);
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