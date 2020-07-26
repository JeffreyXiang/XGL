#ifndef XGL_TOOL_H
#define XGL_TOOL_H

namespace XGL
{
	const double PI = 3.14159265358979323846;

	template <typename T>
	T degToRad(T deg) { return (T)(deg * PI / 180); }

	template <typename T>
	T radToDeg(T rad) { return (T)(rad * 180 / PI); }
}

#endif // !XGL_TOOL_H
