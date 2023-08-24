#include <twob_filesystem.h>
#include <string>
#include <Windows.h>

using namespace twob;

std::string Path::get_absolute_path()
{
    char buffer[1024];
    GetModuleFileNameA(NULL, buffer, 1024);
    std::string path(buffer);
    path = path.substr(0, path.rfind("\\"));

	return path;
}

std::string Path::get_absolute_path(const char* filename)
{
    std::string path = Path::get_absolute_path();
    path.append("\\"); path.append(filename);

    return path;
}
