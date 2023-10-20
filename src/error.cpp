#include "error.hpp"

string FormatString(const char* format ...)
{
	va_list args;
	va_start(args, format);

	string result;
	auto iter = format;
	while (*iter != '\0') {
		if (*iter == '%') {
			// Check next value
			if (*(iter + 1) == '\0') {
				throw SyntaxError("Format string escape is missing specifier: %?. Available options are %f, %d, %c, %s, and %%.\n", SyntaxError::FORMAT_STRING_ERROR);
			}
			char next = *(iter + 1);
			string s;
			switch (next) {
			case 'f':
				s = to_string(va_arg(args, double));
				break;
			case 'c':
				result.push_back(va_arg(args, char));
				break;
			case 'd':
				s = to_string(va_arg(args, int));
				break;
			case 's':
				s = va_arg(args, char*);
				break;
			case '%':
				result.push_back('%');
				break;
			default:
				throw SyntaxError("Format string escape specifier is not supported. Supported operations are: %f, %d, %c, %s, and %%.\n", SyntaxError::FORMAT_STRING_ERROR);
			}
			auto s_it = s.begin();
			while (s_it != s.end()) {
				result.push_back(*s_it);
				s_it++;
			}
			iter++;
		} else {
			result.push_back(*iter);
		}
		iter++;
	}

	return result;
}
