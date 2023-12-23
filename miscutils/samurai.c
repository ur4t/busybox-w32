/* vi: set sw=4 ts=4: */
/*
 * Samurai, a ninja-compatible build tool, for busybox
 *
 * Copyright (C) 2024 ur4t
 */

//config:config	SAMURAI
//config:	bool "samurai"
//config:	default n
//config:	help
//config:	A ninja-compatible build tool for busybox.
//config:
//config:config NINJA
//config:	bool "ninja"
//config:	default n
//config:	depends on SAMURAI
//config:	help
//config:	Alias for "samurai"

//usage:#define samurai_trivial_usage "[-dnv] [-C DIR] [-f FILE] [-j N] [-k N] [-t TOOL]"
//usage:
//usage:#define samurai_full_usage "\n\n"
//usage:	"Samurai, a ninja-compatible build tool\n"
//usage:	"\n	-n	dry run"
//usage:	"\n	-v	verbose output"
//usage:	"\n	-C DIR	change to DIR before doing anything else"
//usage:	"\n	-f FILE	use FILE as input (default is 'build.ninja')"
//usage:	"\n	-j N	run N jobs in parallel (0 means infinity)"
//usage:	"\n	-k N	keep going until N jobs fail (0 means infinity)"
//usage:	"\n	-t TOOL	run TOOL (clean, commands, compdb, graph, query, targets)"
//usage:	"\n	-d MODE	debug MODE (explian, keepdepfile, keeprsp)"
//usage:	"\n	-w FLAG	asjust warning (dupbuild=err,dupbuild=warn)"

//applet:IF_SAMURAI(APPLET(samurai, BB_DIR_BIN, BB_SUID_DROP))
//applet:IF_NINJA(APPLET_ODDNAME(ninja, samurai, BB_DIR_BIN, BB_SUID_DROP, samurai))

//kbuild:lib-$(CONFIG_SAMURAI) += samurai.o
//kbuild:lib-$(CONFIG_SAMURAI) += samurai/build.o
//kbuild:lib-$(CONFIG_SAMURAI) += samurai/deps.o
//kbuild:lib-$(CONFIG_SAMURAI) += samurai/env.o
//kbuild:lib-$(CONFIG_SAMURAI) += samurai/graph.o
//kbuild:lib-$(CONFIG_SAMURAI) += samurai/htab.o
//kbuild:lib-$(CONFIG_SAMURAI) += samurai/log.o
//kbuild:lib-$(CONFIG_SAMURAI) += samurai/parse.o
//kbuild:lib-$(CONFIG_SAMURAI) += samurai/samu.o
//kbuild:lib-$(CONFIG_SAMURAI) += samurai/scan.o
//kbuild:lib-$(CONFIG_SAMURAI) += samurai/tool.o
//kbuild:lib-$(CONFIG_SAMURAI) += samurai/tree.o
//kbuild:lib-$(CONFIG_SAMURAI) += samurai/util.o

#pragma GCC diagnostic ignored "-Wunused-parameter"

int samurai_main(int argc, char **argv);
#define main(A, B) samurai_main(A, B)

#include "samurai/samu.c"
