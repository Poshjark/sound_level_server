#include "Server.h"

namespace
{
	static constexpr ::std::uint16_t DefaultPort = 3000;
}

Server& GetServer()
{
	static Server server{ ::DefaultPort };
	return server;
}

// TODO::crossplatform
BOOL WINAPI CtrlHandler(DWORD fdwCtrlType)
{
	switch (fdwCtrlType)
	{
		// Handle the CTRL-C signal.
	case CTRL_C_EVENT:
		GetServer().Stop();
		return TRUE;
	default:
		break;
	}
	return FALSE;
}

int main()
{

	::SetConsoleCtrlHandler(CtrlHandler, TRUE);

	// ���� �� �������� ���������� � ������������ ��������� �������
	// 0x280 - ������ �� ����, �� �������� � MSDN
	::SetProcessShutdownParameters(0x280, SHUTDOWN_NORETRY);

	HWND hwnd = ::GetConsoleWindow();
	HMENU hMenu = ::GetSystemMenu(hwnd, FALSE);
	::DeleteMenu(hMenu, SC_CLOSE, MF_BYCOMMAND);

	auto& server = GetServer();

	server.Start();

	auto& stopEvent = server.StopEvent();

	stopEvent.Wait(INFINITE);

	// ������ ��� ������ �������
	server.WaitForStopped();

	return 0;
}