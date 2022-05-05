#pragma once

#ifndef _DEBUG_H
#define _DEBUG_H

#include <string>
#include <Windows.h>
#include "Vector3.h"

class Debug
{
public:
	static Debug& GetInstance();

	void Write(char* str);
	void Write(const char* str);
	void Write(int num);
	void Write(float num);
	void Write(long num);
	void Write(Vector3 vector);

	void WriteLine(char* str);
	void WriteLine(const char* str);
	void WriteLine(Vector3 vector);
};
#endif
