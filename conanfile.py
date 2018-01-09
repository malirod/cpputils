from conans.model.conan_file import ConanFile


class CppUtilsConan(ConanFile):
    name = "CppUtils"
    version = "1.0"
    settings = "os", "compiler", "arch", "build_type"
    url = "https://github.com/malirod/cpputils"
    license = "https://github.com/malirod/cpputils/blob/master/LICENSE"
    description = "C++ Utils"
    generators = "cmake"
    requires = ("gtest/1.8.0@malirod/stable",
                "log4cplus/2.0.0-RC2@malirod/stable")
