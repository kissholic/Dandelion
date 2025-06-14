
set_project("Dandelion")

add_rules("mode.debug", "mode.release")

set_languages("c++20")
set_warnings("all", "error")
set_toolchains("clang")

add_cxxflags("-g", "-O0")

includes("src")
