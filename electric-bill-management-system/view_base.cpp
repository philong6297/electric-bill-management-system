/*
 * This is a personal academic project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
 */

#include "view_base.h"
#include "program.h"

using view::ViewBase;
ViewBase::ViewBase(Program *parent) : QObject{parent}, program_{parent} {
	connect(this, &ViewBase::Request, program_, &Program::OnReceiveRequest);
}

ViewBase::~ViewBase() {
	disconnect(this, &ViewBase::Request, program_, &Program::OnReceiveRequest);
	program_ = nullptr;
}
