# directory where the scripts will be installed
# srptdscriptsdir = ${sysconfdir}/@PACKAGE@.d
# initdir = ${sysconfdir}/init.d

bin_PROGRAMS = passgen
passgen_SOURCES = main.c

# man5_MANS = passgen.man thesame.5
# man3_MANS = passgen.man thesame.3
# EXTRA_DIST = $(man5_MANS) $(man3_MANS)

# config files and test scripts
# dist_sysconf_DATA = srptd.conf
# dist_srptdscripts_SCRIPTS = test.sh

# set the include path found by configure
AM_CFLAGS= $(all_includes) -O0 -g
AM_CPPFLAGS = -DSYSCONFDIR=\"$(sysconfdir)\" #-D$(GCC_PREPROCESSOR_DEFINITIONS)

# the library search path.
passgen_LDFLAGS = $(all_libraries)
