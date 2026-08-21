#include "AssetPath.h"

#include <filesystem>

#include <Windows.h>

namespace {
	std::filesystem::path FindAssetRoot() {
		std::wstring buffer(MAX_PATH, L'\0');
		DWORD length = GetModuleFileNameW(nullptr, buffer.data(), static_cast<DWORD>(buffer.size()));
		while (length == buffer.size() && GetLastError() == ERROR_INSUFFICIENT_BUFFER) {
			buffer.resize(buffer.size() * 2);
			length = GetModuleFileNameW(nullptr, buffer.data(), static_cast<DWORD>(buffer.size()));
		}
		buffer.resize(length);

		const std::filesystem::path exeDir = std::filesystem::path(buffer).parent_path();

		// Assets may sit next to the executable (copied there after a build) or
		// higher up the tree, when the executable is built into x64/Debug and
		// the assets stay in the project folder.
		std::filesystem::path dir = exeDir;
		for (int level = 0; level < 5; ++level) {
			std::error_code error;
			if (std::filesystem::exists(dir / "models", error)) {
				return dir;
			}
			if (!dir.has_parent_path() || dir.parent_path() == dir) {
				break;
			}
			dir = dir.parent_path();
		}
		return exeDir;
	}
}

std::string assetPath(const std::string& relative) {
	static const std::filesystem::path root = FindAssetRoot();
	// generic_string, not string: the path must keep forward slashes
	return (root / relative).generic_string();
}
