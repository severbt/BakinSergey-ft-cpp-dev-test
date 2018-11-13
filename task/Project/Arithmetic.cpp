#include "Arithmetic.h"

#include <stack>
#include <sstream>
#include <stdexcept>

using namespace std;
using namespace math;

/////////////////////////////////////////////////////
namespace
{
	bool is_equal(const double num1, const double num2)
	{
		return (num1 - num2) < 0.000001 ? true : false;
	}

	bool is_digit(const char c)
	{
		return (c >= '0' && c <= '9' ? true : false);
	}

	bool is_space(const char c)
	{
		return c == ' ';
	}

	int priority(const char symbol)
	{
		switch (symbol)
		{
		case '^': return 3;
		case '*':
		case '/': return 2;
		case '+':
		case '-': return 1;
		case '(':
		case ')': return 0;
		default: break;
		}

		return -1;
	}

	bool action(stack< double >& st, const string& op)
	{
		double left, right, result;
		left = right = result = 0.0;

		switch (op[0])
		{
		case '+':
		{
			if (st.size() < 2) return false;

			right = st.top(); st.pop();
			left = st.top(); st.pop();

			result = left + right;
			break;
		}
		case '-':
		{
			if (st.size() >= 2)
			{
				right = st.top(); st.pop();
				left = st.top(); st.pop();
			}
			else if (st.size() == 1)
			{
				right = st.top(); st.pop();
			}
			else return false;

			result = left - right;
			break;
		}
		case '*':
		{
			if (st.size() < 2) return false;

			right = st.top(); st.pop();
			left = st.top(); st.pop();

			result = left * right;
			break;
		}
		case '/':
		{
			if (st.size() < 2) return false;

			right = st.top(); st.pop();
			if (is_equal(right, 0.0)) return false;
			left = st.top(); st.pop();

			result = left / right;
			break;
		}
		case '^':
		{
			if (st.size() < 2) return false;

			right = st.top(); st.pop();
			left = st.top(); st.pop();

			result = pow(left, right);
			break;
		}
		/*
		op - can be any operations cos , ctg, ...
		*/
		default: return false;
		}

		st.push(result);
		return true;
	}
}
/////////////////////////////////////////////////////

Arithmetic::Arithmetic() : m_result(0.0)
{
}

Arithmetic::~Arithmetic()
{
}

bool Arithmetic::transform(const string& str)
{
	size_t len = str.size();
	stack< char > st;
	char   temp = 0;
	string out;
	out.reserve(len*2 + 1);


	for (size_t i = 0; i < len; i++)
	{
		if (isspace(str[i]))
		{
			continue;
		}

		if (isdigit(str[i]))
		{
			size_t old = i;

			while (i < len)
			{
				if (isdigit(str[i]) || str[i] == '.')
					++i;
				else
					break;
			}

			out.append(str, old, i - old);
			out += ' ';
			--i;
			continue;
		}

		switch (str[i])
		{
		case '^':
		case '*':
		case '/':
		case '+':
		case '-':
		{
			if (!st.empty())
			{
				if (str[i] == '-' && str[i - 1] == '(')
				{
					out += '0';
					out += ' ';
				}
				else if (priority(str[i]) <= priority(st.top()))
				{
					out += st.top();
					out += ' ';
					st.pop();
				}
			}
			st.push(str[i]);
			break;
		}
		case '(':
		{
			st.push('(');
			break;
		}
		case ')':
		{
			while (1)
			{
				if (st.empty()) return false;

				temp = st.top(); st.pop();
				if (temp == '(') break;

				out += temp;
				out += ' ';
			}
			break;
		}
		/*
		str[i] - can be any operations cos , ctg, ...
		*/
		default: return false;
		}
	}

	while (!st.empty())
	{
		out += st.top(); st.pop();
		out += ' ';	
	}

	m_formula.swap(out);
	return true;
}

bool Arithmetic::calculate()
{
	size_t len = m_formula.size();
	size_t pos = 0;
	string temp_tok;
	stack < double > st;
	double tmp_numb;

	for (size_t i = 0; i < len; i++)
	{
		pos = m_formula.find(' ', i);
		if (pos == string::npos) return false;

		temp_tok = m_formula.substr(i, pos - i);
		i = pos;

		if (isdigit(temp_tok[0]))
		{
			stringstream(temp_tok) >> tmp_numb;
			st.push(tmp_numb);
		}
		else
		{
			if (!action(st, temp_tok)) return false;
		}		
	}

	if (st.empty()) return false;

	m_result = st.top(); st.pop();
	return true;
}

double Arithmetic::calculate(const string& str)
{
	Arithmetic a;
	if (!a.transform(str))
	{
		runtime_error("Error transform.");
	}

	if (!a.calculate())
	{
		runtime_error("Error transform.");
	}

	return a.answer();
}

double Arithmetic::answer() const
{
	return m_result;
}
