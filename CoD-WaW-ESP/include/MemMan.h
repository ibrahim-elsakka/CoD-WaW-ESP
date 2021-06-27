#pragma once
#include <Windows.h>
#include <vector>
#include <TlHelp32.h>
#include <iomanip>

class MemMan
{
public:
	~MemMan()
	{
		CloseHandle(Handle);
	}

	template <class T>
	static T ReadMem(uintptr_t addr)
	{
		T x;
		ReadProcessMemory(Handle, (LPBYTE*)addr, &x, sizeof(x), NULL);
		return x;
	}

	template <class T>
	static T WriteMem(uintptr_t addr, T x)
	{
		WriteProcessMemory(Handle, (LPBYTE*)addr, &x, sizeof(x), NULL);
		return x;
	}

	template<class T>
	static T FromBuffer(std::vector<uint8_t> buffer, size_t offset)
	{
		T t_buf{};
		if (buffer.empty())
			return t_buf;

		memcpy(&t_buf, buffer.data() + offset, sizeof(T));
		return t_buf;
	}

	static std::vector<uint8_t> ReadBytes(DWORD Address, int Size)
	{
		std::vector<uint8_t> arr;
		arr.resize(Size);
		ReadProcessMemory(Handle, (LPVOID)Address, arr.data(), arr.size() * sizeof(uint8_t), 0);
		return arr;
	}

	static uintptr_t GetProcess(const wchar_t* proc)
	{
		HANDLE hProcessId = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		uintptr_t process = 0;
		PROCESSENTRY32 pEntry;
		pEntry.dwSize = sizeof(pEntry);

		do
		{
			if (!_wcsicmp(pEntry.szExeFile, proc))
			{
				process = pEntry.th32ProcessID;
				CloseHandle(hProcessId);
				Handle = OpenProcess(PROCESS_ALL_ACCESS, false, process);
			}

		} while (Process32Next(hProcessId, &pEntry));
		return process;
	}

	static uintptr_t GetModule(uintptr_t procId, const wchar_t* modName)
	{
		HANDLE hModule = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procId);
		MODULEENTRY32 mEntry;
		mEntry.dwSize = sizeof(mEntry);

		do
		{
			if (!_wcsicmp(mEntry.szModule, modName))
			{
				CloseHandle(hModule);
				return (uintptr_t)mEntry.hModule;
			}
		} while (Module32Next(hModule, &mEntry));
		return 0;
	}

	static uintptr_t GetAddress(uintptr_t addr, std::vector<uintptr_t> vect)
	{
		for (int i = 0; i < vect.size(); i++)
		{
			ReadProcessMemory(Handle, (BYTE*)addr, &addr, sizeof(addr), 0);
			addr += vect[i];
		}
		return addr;
	}
	static HANDLE GetHandle() { return Handle; }

public:
	static inline HANDLE Handle = NULL;
};