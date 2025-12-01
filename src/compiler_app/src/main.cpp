#include <print>
#include <string>
#include <vector>
#include <iostream>
#include <exception>
#include <common/common.h>

#include <compiler/cliparser.h>
#include <compiler/compiledriver.h>

int main(int argc, char** argv) { try {
    common::debug_break();
    compiler_app::CLIParser cliparser;
    auto options = cliparser.process(std::vector<std::string>(argv + 1, argv + argc));
    compiler_app::CompileDriver driver;
    driver.run(options);
    } catch (const std::exception& e) {
        std::println(std::cerr, "ERROR: {}", e.what());
        return -1;
    }
}
