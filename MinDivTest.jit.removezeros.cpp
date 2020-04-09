/// @file
///	@ingroup 	minexamples
///	@copyright	Copyright 2018 The Min-DevKit Authors. All rights reserved.
///	@license	Use of this source code is governed by the MIT License found in the License.md file.

#include "c74_min.h"

using namespace c74::min;

class MinDivTest_jit_removezeros : public object<MinDivTest_jit_removezeros>, public matrix_operator<> {
public:
	MIN_DESCRIPTION {"Post to the Max Console."};
	MIN_TAGS {"matrix operator"};
	MIN_AUTHOR {"Federico Foderaro"};
	MIN_RELATED {""};

	inlet<>  input {this, "(matrix) Input", "matrix"};
	outlet<> output {this, "(matrix) Output", "matrix"};

	attribute<number> distance_threshold{ this, "distance_threshold", 0.2, description {"..."} };
			

	long index = -1;
	long counter = 0;

	template<class matrix_type, size_t plane_count>
	cell<matrix_type, plane_count> calc_cell(cell<matrix_type, plane_count> input, const matrix_info& info, matrix_coord& position) {
		cell<matrix_type, plane_count> output;

		for (auto plane = 0; plane < info.plane_count(); ++plane)
		{
			output[plane] = input[plane];
		}

		long currentPos = position.x();

		if ((counter % 2) == 0)
		{	
			for (auto cellPos = currentPos; cellPos < info.width() - 1; cellPos++)
			{	
				if (cellPos != position.x())
				{
					auto otherCell = get_cell<matrix_type, plane_count>(info, cellPos, 0);

					float sum = 0.0f;
					for (auto plane = 0; plane < info.plane_count(); ++plane)
					{
						float val = otherCell[plane];
						float val1 = input[plane];
						float sub = otherCell[plane] - input[plane];
						sum += (float)((otherCell[plane] - input[plane])*(otherCell[plane] - input[plane]));
					}

					if (sum <= distance_threshold)
					{
						index = cellPos;
						break;
					}
					else
					{
						index = -1;
					}
				}
			}
		}
		else
		{	
			if (index != -1)
			{	
				auto otherCell = get_cell<matrix_type, plane_count>(info, index, 0);
				for (auto plane = 0; plane < info.plane_count(); ++plane)
				{
					output[plane] = otherCell[plane];
				}
			}
		}

		counter++;
		if (counter > 1000) counter = 0;

		return output;
	}

private:
	template<class matrix_type, size_t plane_count>
	cell<matrix_type, plane_count> get_cell(const matrix_info& info, int x, int y) {
		auto x1 = clamp<decltype(x)>(x, 0, info.width() - 1);
		auto y1 = clamp<decltype(y)>(y, 0, info.height() - 1);
		auto c = info.in_cell<matrix_type, plane_count>(x1, y1);
		return c;
	}
};


MIN_EXTERNAL(MinDivTest_jit_removezeros);
