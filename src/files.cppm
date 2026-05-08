module;

#include <fmt/base.h>
#include <fmt/format.h>
#include <string>
#include <filesystem>

export module files;

// this is similar to python staticmethod.
export namespace file_module {

    void bulk_rename(const std::string& path) {
        namespace fs = std::filesystem;

        for (const auto& file: fs::directory_iterator(path)) {
            if (not file.is_regular_file()) continue;
            const std::filesystem::path& file_path = file.path();
            const std::filesystem::path& parent_path = file_path.parent_path();
            const std::filesystem::path& filename = file_path.stem();
            const std::filesystem::path& extension = file_path.extension();

            const auto new_filename = filename.string().append("new_extension").append(extension.string());
        
            const auto new_file_path = parent_path / new_filename;
            fs::rename(file_path, new_filename);
        }
    }
}