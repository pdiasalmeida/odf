#ifndef COMMON_HPP_
#define COMMON_HPP_

namespace PluginEngine {

	typedef unsigned char byte;

	// Struct used to define a detected roi
	struct Rect {
		int x;
		int y;
		int width;
		int height;
	};

	struct Size {
		int width;
		int height;
	};

}

#endif
