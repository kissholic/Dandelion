
add_requires("spdlog")
add_requires("nlohmann_json")

add_includedirs(".")

includes("common")
includes("cdc")
includes("p2p")


target("Dandelion")
    set_kind("binary")
    add_files("*.cpp")
    add_deps("task_system", "p2p")
    add_packages("spdlog", "nlohmann_json")
    