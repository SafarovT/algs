#include "../../../../external/catch2/catch.hpp"
#include "../task28/task28.h"
#include <sstream>

SCENARIO("Хорошие программы")
{
	bool result = false;
	std::stringstream ss;

	ss  << "VAR\n"
		<< "RECORD\n"
		<< "TEXT: STRING;\n"
		<< "NUMBER: INTEGER;\n"
		<< "END;\n"
		<< "\n"
		<< "\n"
		<< "\n"
		<< "\n"
		<< "\n"
		<< "\n";
	result = CheckIsProgramValid(ss);
	CHECK(result);
	ss.clear();

	ss << "VAR\n"
		<< "RECORD\n"
		<< "TEXT: STRING;\n"
		<< "ENDD: INTEGER;\n"
		<< "END;\n"
		<< "\n"
		<< "\n"
		<< "\n"
		<< "\n"
		<< "\n"
		<< "\n";
	result = CheckIsProgramValid(ss);
	CHECK(result);
	ss.clear();

	ss << "VAR\n"
		<< "IF ()\n"
		<< "THEN;\n"
		<< " BEGIN;\n"
		<< "ad;\n"
		<< "END\n"
		<< "\n"
		<< "\n"
		<< "\n"
		<< "\n"
		<< "\n";
	result = CheckIsProgramValid(ss);
	CHECK(result);
	ss.clear();

	ss << "VAR\n"
		<< "IF ()\n"
		<< "THEN;\n"
		<< " BEGIN;\n"
		<< " END\n"
		<< "ELSE\n"
		<< "BEGIN\n"
		<< "END\n"
		<< "\n"
		<< "\n"
		<< "\n";
	result = CheckIsProgramValid(ss);
	CHECK(result);
	ss.clear();

	ss
		<< "RECORD\n"
		<< "TEST: RECORD\n"
		<< "TEXT: STRING\n"
		<< " END\n"
		<< " END\n"
		<< "IF\n"
		<< "THEN\n"
		<< "IF\n"
		<< "THEN\n"
		<< "BEGIN\n"
		<< "END\n"
		<< "ELSE\n"
		<< "REPEAT\n"
		<< "UNTIL\n"
		<< "\n"
		<< "\n"
		<< "\n"
		<< "\n";
	result = CheckIsProgramValid(ss);
	CHECK(result);
	ss.clear();

	ss
		<< "IF\n"
		<< "THEN\n"
		<< "IF\n"
		<< "THEN\n"
		<< "BEGIN\n"
		<< "END;\n"
		<< "ELSE\n"
		<< "\n"
		<< "\n"
		<< "\n";
	result = CheckIsProgramValid(ss);
	CHECK(result);
	ss.clear();

	ss
		<< "REPEAT\n"
		<< "REPEAT\n"
		<< "UNTIL\n"
		<< "UNTIL\n"
		<< "\n"
		<< "\n"
		<< "\n";
	result = CheckIsProgramValid(ss);
	CHECK(result);
	ss.clear();
}

SCENARIO("Плохие программы")
{
	bool result = false;
	std::stringstream ss;

	ss << "BEGIN\n"
		<< "RECORD\n"
		<< "END\n"
		<< "END\n"
		<< "END;\n"
		<< "\n"
		<< "\n"
		<< "\n"
		<< "\n"
		<< "\n"
		<< "\n";
	result = CheckIsProgramValid(ss);
	CHECK(!result);
	ss.clear();

	ss << "VAR\n"
		<< "IF ()\n"
		<< " BEGIN;\n"
		<< " END\n"
		<< "ELSE\n"
		<< "\n"
		<< "\n"
		<< "\n"
		<< "\n"
		<< "\n";
	result = CheckIsProgramValid(ss);
	CHECK(!result);
	ss.clear();

	ss << "VAR\n"
		<< "IF ()\n"
		<< " BEGIN;\n"
		<< " END\n"
		<< "\n"
		<< "\n"
		<< "\n"
		<< "\n"
		<< "\n";
	result = CheckIsProgramValid(ss);
	CHECK(!result);
	ss.clear();

	ss << "VAR\n"
		<< "IF\n"
		<< "THEN\n"
		<< " BEGIN END;\n"
		<< "ELSE\n"
		<< "\n"
		<< "\n"
		<< "\n"
		<< "\n";
	result = CheckIsProgramValid(ss);
	CHECK(!result);
	ss.clear();
}