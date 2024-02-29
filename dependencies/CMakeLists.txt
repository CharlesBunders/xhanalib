include(FetchContent)

FetchContent_Declare(
  acutest
  GIT_REPOSITORY https://github.com/mity/acutest.git
  GIT_TAG "master"
)


FetchContent_MakeAvailable(acutest)
