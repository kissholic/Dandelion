
add_requires("spdlog")
add_requires("nlohmann_json")
add_requires("libuv")

add_includedirs(".")

includes("common")
includes("cdc")
includes("p2p")


target("Dandelion")
    set_kind("binary")
    add_files("*.cpp")
    add_deps("task_system", "p2p", "cdc")
    add_packages("spdlog", "nlohmann_json", "libuv")
    