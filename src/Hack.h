#pragma once


class Hack
{
public:
	static void Initialize();

	static void GetModuleHandles();
	static void GetInterfaces();
	static void GetOffsets();

	static void MainLoop();
};