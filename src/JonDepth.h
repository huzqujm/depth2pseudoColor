#pragma once
#include <iostream>
#include <string>
#include <vector>

typedef unsigned short ushort;
typedef unsigned char uchar;

namespace Jon
{
	class depth
	{
	public:
		depth();
		~depth();

		/// constructor
		depth(const depth&);
		depth(const ushort* data, const int& height, const int& width);

		/// operator overloading
		depth operator=(const depth&);

		/// get pesudo-color image of depth image
		uchar* convert2Color();

		/// get height, width or data ptr of depth image
		const int getHeight() const;
		const int getWidth() const;
		const ushort* getDataPtr() const;

	private:
		ushort* m_uspDepthData;
		int m_height, m_width;
	};

}//end of namespace Jon