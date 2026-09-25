# Lichens CPP

## Version 0.1

* initial version with many basic utilities

## Version 0.2

* Example basic now shutdown logger properly at the end of execution
* MQTT helper nous use publisher listener to avoid dead lock if sending message from rx handler.
* Add debouncing 

## Version 0.3

* Windows (MSVC) support: Lichens-CPP can be pulled in with FetchContent and builds, links and runs on Windows
* Lichens-CPP is built as a static library by default on Windows, and stays a shared library by default elsewhere
* New `LICHENS_CPP_BUILD_SHARED` option to choose shared or static linkage. `BUILD_SHARED_LIBS` is ignored and fetched dependencies are always static. A shared build on Windows exports all symbols
* The syslog logger does nothing on Windows except log a warning
* Warning flags only apply to Lichens-CPP's own targets, and warnings are errors only when Lichens-CPP is the top-level project
* fmt and nlohmann_json use an installed copy if found and are fetched otherwise (fmt 11.1.4, nlohmann_json 3.12.0). spdlog uses the same fmt
* googletest is pinned to 1.17.0
