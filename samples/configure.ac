AC_PREREQ([2.69])
AC_INIT([tdr_samples], [0.1], [joe.brown@gitlab.com])
AC_CONFIG_SRCDIR([sample01.cpp])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CXX

# Checks for libraries.
AC_CHECK_LIB([GL], [glGetString])
AC_CHECK_LIB([dl], [dlopen])
AC_CHECK_LIB([glfw], [glfwInit])
AC_CHECK_LIB([libtdrmain], [tdrmain_version])
AC_CHECK_LIB([librenderer], [tdrrenderer_version])

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL

# Checks for library functions.

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
