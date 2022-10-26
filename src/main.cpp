// system headers
#include <sys/ioctl.h>

// stdlib headers
#include <filesystem>

// third party libraries headers
#include <CLI/CLI.hpp>
#include <fmt/core.h>
#include <spdlog/spdlog.h>

// project headers
#include "config.h"

namespace fs = std::filesystem;

size_t get_terminal_width() {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    return w.ws_col;
}

auto main(int argc, char **argv) -> int {
    // Parse CLI args
    CLI::App app;

    std::string path;
    app.add_option("file/folder", path, "File/directory to examine.");

    try {
        app.parse(argc, argv);
    } catch (const CLI::ParseError &e) {
        return app.exit(e);
    }


    spdlog::set_pattern("[%^%l%$] %v");

    size_t width = get_terminal_width();

    // Check that file/folder exists
    fs::path path_to_examine;
    // if not path has been specified, use current directory
    if (path.empty()) {
        path_to_examine = fs::current_path();
    } else {
        path_to_examine = path;
    }

    if (!fs::exists(path_to_examine)) {
        spdlog::error("Cannot find file/directory named {}", path);
        return 1;
    }

    // pour directory's content into a vector
    std::vector<fs::directory_entry> entries{};
    for (auto const &dir_entry : std::filesystem::directory_iterator{ path_to_examine }) {
        entries.emplace_back(dir_entry);
    }

    // operations on content
    size_t longest{0};
    std::for_each(entries.begin(), entries.end(), [&longest](const fs::directory_entry& entry) {
        size_t size = entry.path().filename().string().size();
        longest = (size > longest? size:longest);
    });

    // print vector's content
    size_t column_width = longest + 4;
    int n_columns = width / column_width;
    int column{1};

    for(const fs::directory_entry& entry: entries) {
        std::string filename = entry.path().filename().string();
        fmt::print("{:<{}}{}", filename, column_width, column == n_columns ? '\n':'\0');
        column = (column == n_columns? 1:column+1);
    }

    fmt::print("\n");

    return 0;
}