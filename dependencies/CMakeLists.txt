include(FetchContent)

pkg_search_module(REQUIRED imgui)
find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)

FetchContent_Declare(
  imgui-sfml
  GIT_REPOSITORY https://github.com/eliasdaler/imgui-sfml.git
  GIT_TAG        master
  # GIT_COMMIT 5f54b69b6aee18db846c81633f86f78c2586dded
  # ^ or like this - sometimes it's better because FetchContent won't look
  # into remote to see if branch head was updated or not - good for stable
  # tags like 'vX.X' corresponding to releases
)
FetchContent_MakeAvailable(imgui-sfml)

