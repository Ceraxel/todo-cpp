cmake_minimum_required(VERSION 3.27)
set(CMAKE_CXX_STANDARD 17)
# --- Fetch FTXUI --------------------------------------------------------------
include(FetchContent)

set(FETCHCONTENT_UPDATES_DISCONNECTED TRUE)
FetchContent_Declare(
  ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
  # Important: Specify a GIT_TAG XXXXX here.
  GIT_TAG v5.0.0)

FetchContent_GetProperties(ftxui)
if(NOT ftxui_POPULATED)
  FetchContent_Populate(ftxui)
  add_subdirectory(${ftxui_SOURCE_DIR} ${ftxui_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

# ------------------------------------------------------------
project(todo)

file(GLOB_RECURSE SRC_FILES src/*.cpp)
add_executable(${PROJECT_NAME} ${SRC_FILES})

target_include_directories(
  ${PROJECT_NAME}
  PRIVATE src
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(
  ${PROJECT_NAME}
  PRIVATE ftxui::screen
  PRIVATE ftxui::dom
  PRIVATE ftxui::component # Not needed for this example.
          -lpqxx -lpq)
