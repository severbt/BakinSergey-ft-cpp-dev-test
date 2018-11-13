#pragma once

#include <string>

namespace math
{
	class Arithmetic
	{
	public:
		explicit Arithmetic();
		~Arithmetic();

		bool transform(const std::string& str);
		bool calculate();
		double answer() const;

		static double calculate(const std::string& str);

	private:
		std::string m_formula;
		double      m_result;
	};
}



