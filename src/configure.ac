#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT(prime_select, 0.0.1, jjh@sipmeeting.com)
AC_CONFIG_SRCDIR([prime_switch.c])
AC_CONFIG_HEADERS([config.h])

AM_PATH_GTK_3_0([3.0.0],,AC_MSG_ERROR([Gtk+ 3.0.0 or higher required.]))

AC_CHECK_PROG(XRANDR_CHECK,xrandr,yes)
if test x"$XRANDR_CHECK" != x"yes" ; then
    AC_MSG_ERROR([Please install xrandr before installing.])
fi

AC_CHECK_PROG(LSPCI_CHECK,lspci,yes)
if test x"$LSPCI_CHECK" != x"yes" ; then
    AC_MSG_ERROR([Please install lspci (pciutils) before installing.])
fi

AC_CHECK_PROG(GREP_CHECK,grep,yes)
if test x"$GREP_CHECK" != x"yes" ; then
    AC_MSG_ERROR([Please install grep before installing.])
fi

# Checks for programs.
AC_PROG_CC

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h unistd.h stdbool.h stdio.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL

# Checks for library functions.

AC_OUTPUT

dnl Process Makefile.in to create Makefile
AC_OUTPUT(Makefile)
