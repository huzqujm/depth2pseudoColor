#include "JonDepth.h"

namespace Jon
{
	depth::depth():
		m_uspDepthData(nullptr), m_height(0), m_width(0)
	{}

	depth::depth(const ushort* depthData, const int& height, const int& width):
		m_height(height), m_width(width)
	{
		/*
		-* parameter depthData cannot be a nullptr
		*/
		try
		{
			if (depthData == nullptr)
			{
				std::string errorMessage("parameter of depth constructor is null\n");
				throw(errorMessage);
			}
		}
		catch (const std::string& e)
		{
			std::cout << e << "in: " << __FILE__ << "line: " << __LINE__ << std::endl;
		}
		m_uspDepthData = new ushort[m_height * m_width];
		std::copy(depthData, depthData + height * width, m_uspDepthData);
	}

	depth::depth(const depth& init)
	{
		*this = init;
	}

	depth depth::operator=(const depth& leftOptr)
	{
		m_height = leftOptr.getHeight();
		m_width = leftOptr.getWidth();
		const ushort* dataPtr = leftOptr.getDataPtr();
		m_uspDepthData = new ushort[m_height * m_width];
		std::copy(dataPtr, dataPtr + m_height * m_width, m_uspDepthData);
		return leftOptr;
	}

	const int depth::getHeight() const
	{
		return m_height;
	}

	const int depth::getWidth() const
	{
		return m_width;
	}

	const ushort* depth::getDataPtr() const
	{
		return m_uspDepthData;
	}

	uchar* depth::convert2Color()
	{
		/// color image to be returned
		uchar* rgbImage(nullptr);

		const uchar farColor[] = { 255, 0, 0 }, nearColor[] = { 20, 40, 255 };
		const int N = 256 * 256;
		int histogram[N] = { 1 };

		/// histogram
		for (int row_i = 0; row_i < m_height; row_i++)
		{
			for (int col_i = 0; col_i < m_width; col_i++)
			{
				int index = static_cast<int>(m_uspDepthData[row_i * m_width + col_i]);
				histogram[index]++;
			}
		}

		/// integral arry
		for (int hist_i = 1; hist_i < N; hist_i++)
		{
			histogram[hist_i] += histogram[hist_i - 1];
		}

		/// remap the integral histogram to range [0...256];
		int sum = histogram[N - 1];
		for (int hist_i = 0; hist_i < N; hist_i++)
		{
			histogram[hist_i] = (histogram[hist_i] << 8) / sum;
		}

		/// init color image
		rgbImage = new uchar[m_height * m_width * 3];
		memset(rgbImage, 0, sizeof(rgbImage));

		/// generate color image by using the histogram to interpolate between two colors
		for (int row_i = 0; row_i < m_height; row_i++)
		{
			for (int col_i = 0; col_i < m_width; col_i++)
			{
				int curPos = row_i * m_width + col_i;

				ushort curDepthVal = m_uspDepthData[curPos];
				int curHist = histogram[curDepthVal];

				/// get the corresponding rgb postions in the two dimension color image
				uchar &curRVal(rgbImage[curPos * 3]), &curGVal(rgbImage[curPos * 3 + 1]), 
					&curBVal(rgbImage[curPos * 3 + 2]);
				if (curDepthVal > 0)
				{
					curRVal = ((std::numeric_limits<uchar>::max() - curHist) * nearColor[0] + curHist * farColor[0]) >> 8;
					curGVal = ((std::numeric_limits<uchar>::max() - curHist) * nearColor[1] + curHist * farColor[1]) >> 8;
					curBVal = ((std::numeric_limits<uchar>::max() - curHist) * nearColor[2] + curHist * farColor[2]) >> 8;
				}
				else
				{
					curRVal = static_cast<uchar>(0);
					curGVal = static_cast<uchar>(0);
					curBVal = static_cast<uchar>(0);
				}
			}
		}
		return rgbImage;
	}

	depth::~depth()
	{
		if (m_uspDepthData)
			delete[] m_uspDepthData;
		m_height = 0;
		m_width = 0;
	}

}/// end of namespace Jon