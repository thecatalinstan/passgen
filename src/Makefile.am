

pkglib_LIBRARIES = libpassgen
libpassgen_SOURCES = passgen.c
libpassgen_LDFLAGS = -shared

bin_PROGRAMS = passgen
passgen_SOURCES = main.c
passgen_LDADD = libpassgen.so

# man5_MANS = passgen.man thesame.5
# man3_MANS = passgen.man thesame.3
# EXTRA_DIST = $(man5_MANS) $(man3_MANS)

# config files and test scripts
# dist_sysconf_DATA = srptd.conf
# dist_srptdscripts_SCRIPTS = test.sh

# set the include path found by configure
AM_CFLAGS= $(all_includes) -O0 -g
AM_CPPFLAGS = -DSYSCONFDIR=\"$(sysconfdir)\"

# the library search path.
passgen_LDFLAGS = $(all_libraries)
