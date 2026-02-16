#include <assist.h>
#include <config.h>

#ifdef _WIN32
    #include <Windows.h>
#elif defined(__APPLE__)
    #include <mach-o/dyld.h>
    #include <climits>
#else
    #include <unistd.h>
    #include <climits>
#endif

// Color conversion for visual purposes and ease
// You are absolutely correct! They all do the same thing and are redundant asl.
double red(int r) { return (double)r / 255.0; }
double green(int g) { return (double)g / 255.0; }
double blue(int b) { return (double)b / 255.0; }

// Get the exe directory path (cross-platform)
std::filesystem::path getPath() {
#ifdef _WIN32
    char buffer[MAX_PATH];
    GetModuleFileNameA(NULL, buffer, MAX_PATH);
    return std::filesystem::path(buffer).parent_path();
#elif defined(__APPLE__)
    char buffer[PATH_MAX];
    uint32_t size = sizeof(buffer);
    _NSGetExecutablePath(buffer, &size);
    return std::filesystem::path(buffer).parent_path();
#else
    char buffer[PATH_MAX];
    ssize_t len = readlink("/proc/self/exe", buffer, sizeof(buffer) - 1);
    if (len != -1) buffer[len] = '\0';
    return std::filesystem::path(buffer).parent_path();
#endif
}

std::string extractFontName(const std::string& filename) {
    // Example: "Roboto-Regular.ttf" -> "Roboto"
    size_t lastSlash = filename.find_last_of("/\\");
    size_t start = (lastSlash == std::string::npos) ? 0 : lastSlash + 1;
    size_t end = filename.find_first_of("-.", start);
    if (end == std::string::npos) end = filename.length();
    return filename.substr(start, end - start);
}

std::vector<FontDef> retrieveFonts(std::string fontPath) {

    std::filesystem::path fontDir = getPath() / fontPath;
    if (!std::filesystem::exists(fontDir) || !std::filesystem::is_directory(fontDir)) {
        throw std::runtime_error("Font directory " + fontPath + " not found");
    }

    std::vector<FontDef> fontDefs;

    MSG("[Font] Retrieving fonts from: " << fontPath.c_str());
    for (const auto& entry : std::filesystem::directory_iterator(fontDir)) {
        if (entry.is_regular_file()) {
            if (entry.path().extension() == ".ttf") {
                std::string filename = entry.path().string();
                std::string fontName = extractFontName(entry.path().filename().string());
                fontDefs.push_back({ fontName.c_str(), filename.c_str() });
                MSG("- " << filename.c_str() << " (" << fontName.c_str() << ")");
            } else {
                continue; // Skip non .ttf files
            }
        }
    }

    return fontDefs;
}

// Helper to map Math -> NDC (stands for Normalized Device Coordinates btw)
float mapToScreen(float value, float min, float max) {
    // Returns value between -1.0 and 1.0
    return ((value - min) / (max - min)) * 2.0f - 1.0f;
}
