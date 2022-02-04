#include "Debug.h"

Debug& Debug::GetInstance()
{
	static Debug _instance;
	return _instance;
}

void Debug::Write(char* str)
{
	OutputDebugStringA(str);
}

void Debug::Write(int num)
{
	char str[1024] = { 0 };
	sprintf_s(str, "%d", num);
	OutputDebugStringA(str);
}

void Debug::Write(float num)
{
	char str[1024] = { 0 };
	sprintf_s(str, "%.5f", num);
	OutputDebugStringA(str);
}

void Debug::Write(long num)
{
	char str[1024] = { 0 };
	sprintf_s(str, "%lu", num);
	OutputDebugStringA(str);
}

void Debug::WriteLine(char* str)
{
	OutputDebugStringA(str);
	OutputDebugStringA("\n");
}
