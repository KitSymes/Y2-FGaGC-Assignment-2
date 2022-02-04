#pragma once

#ifndef _DEBUG_H
#define _DEBUG_H

#include <string>
#include <Windows.h>

class Debug
{
public:
	static Debug& GetInstance();

	void Write(char* str);
	void Write(int num);
	void Write(float num);
	void Write(long num);

	void WriteLine(char* str);
};
#endif
