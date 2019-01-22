/*
 * This is a personal academic project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
 */

#include <QtCore/QCoreApplication>

#include "program.h"
int main() {
	char** argv = nullptr;
	auto argc = 0;
	[[maybe_unused]] QCoreApplication a{argc, argv};
	// chay program
	Program program;
	// start program
	program.Start();

	return QCoreApplication::exec();
}
