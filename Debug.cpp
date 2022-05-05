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

void Debug::Write(const char* str)
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

void Debug::Write(Vector3 vector)
{
	Write("(");
	Write(vector.x);
	Write(", ");
	Write(vector.y);
	Write(", ");
	Write(vector.z);
	Write(")");
}

void Debug::WriteLine(char* str)
{
	Write(str);
	OutputDebugStringA("\n");
}

void Debug::WriteLine(const char* str)
{
	Write(str);
	OutputDebugStringA("\n");
}

void Debug::WriteLine(Vector3 vector)
{
	Write(vector);
	OutputDebugStringA("\n");
}
