#pragma once

#include "Includes.h"

inline void ThrowIfFailed(bool successCondition, const char* errorMessage)
{
	if (!successCondition)
		throw std::exception(errorMessage);
}

template<typename RandomEngine>
inline float Random(RandomEngine& randomEngine)
{
	auto minMaxDistance = randomEngine.max() - randomEngine.min();

	return static_cast<float>(randomEngine() - randomEngine.min()) / static_cast<float>(minMaxDistance);
}

inline int AlignToNextPowerOf2(int value) noexcept
{
	return static_cast<int>(std::pow(2, static_cast<int>(std::ceil(std::log(static_cast<float>(value)) / std::log(2.0f)))));
}

inline void ConvertWCharStringToCharString(const PWSTR wcString, std::shared_ptr<char[]>& cString)
{
	size_t wcFilePathLength = wcslen(wcString);

	cString = std::shared_ptr<char[]>(new char[wcFilePathLength + 1]);

	cString[wcFilePathLength] = '\0';

	WideCharToMultiByte(CP_UTF8, 0, wcString, wcFilePathLength, cString.get(), wcFilePathLength, nullptr, nullptr);
}

inline void GetOpenFilePathFromDialog(std::string& filePath)
{
	ThrowIfFailed(SUCCEEDED(CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE)),
		"GetOpenFilePathFromDialog: COM Library does not initialized!");

	ComPtr<IFileOpenDialog> fileOpenDialog;

	ThrowIfFailed(SUCCEEDED(CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_IFileOpenDialog, reinterpret_cast<void**>(fileOpenDialog.GetAddressOf()))),
		"GetOpenFilePathFromDialog: FileOpenDialog does not created!");

	COMDLG_FILTERSPEC filterSpec[] =
	{
		{ L"Bitmap", L"*.bmp" },
	};

	fileOpenDialog->SetFileTypes(ARRAYSIZE(filterSpec), filterSpec);

	ThrowIfFailed(SUCCEEDED(fileOpenDialog->Show(nullptr)), "GetOpenFilePathFromDialog: Dialog window does not show!");

	ComPtr<IShellItem> shellItem;

	ThrowIfFailed(SUCCEEDED(fileOpenDialog->GetResult(shellItem.GetAddressOf())), "GetOpenFilePathFromDialog: Result does not received!");

	PWSTR wcFilePath;

	ThrowIfFailed(SUCCEEDED(shellItem->GetDisplayName(SIGDN_FILESYSPATH, &wcFilePath)), "GetOpenFilePathFromDialog: File path does not created!");

	std::shared_ptr<char[]> cFilePath;

	ConvertWCharStringToCharString(wcFilePath, cFilePath);

	filePath = cFilePath.get();

	CoTaskMemFree(wcFilePath);

	CoUninitialize();
}

inline void GetSaveFilePathFromDialog(std::string& filePath)
{
	ThrowIfFailed(SUCCEEDED(CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE)),
		"GetSaveFilePathFromDialog: COM Library does not initialized!");

	ComPtr<IFileSaveDialog> fileSaveDialog;

	ThrowIfFailed(SUCCEEDED(CoCreateInstance(CLSID_FileSaveDialog, NULL, CLSCTX_ALL, IID_IFileSaveDialog, reinterpret_cast<void**>(fileSaveDialog.GetAddressOf()))),
		"GetSaveFilePathFromDialog: FileSaveDialog does not created!");

	COMDLG_FILTERSPEC filterSpec[] =
	{
		{ L"Bitmap", L"*.bmp" },
	};

	fileSaveDialog->SetFileTypes(ARRAYSIZE(filterSpec), filterSpec);

	ThrowIfFailed(SUCCEEDED(fileSaveDialog->Show(nullptr)), "GetSaveFilePathFromDialog: Dialog window does not show!");

	ComPtr<IShellItem> shellItem;

	ThrowIfFailed(SUCCEEDED(fileSaveDialog->GetResult(shellItem.GetAddressOf())), "GetSaveFilePathFromDialog: Result does not received!");

	PWSTR wcFilePath;

	ThrowIfFailed(SUCCEEDED(shellItem->GetDisplayName(SIGDN_FILESYSPATH, &wcFilePath)), "GetSaveFilePathFromDialog: File path does not created!");

	std::shared_ptr<char[]> cFilePath;

	ConvertWCharStringToCharString(wcFilePath, cFilePath);

	filePath = cFilePath.get();

	CoTaskMemFree(wcFilePath);

	CoUninitialize();
}
