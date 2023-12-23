/* vi: set sw=4 ts=4: */

/*
 * Lua interpreter and compiler for busybox
 *
 * Copyright (C) 2022-2023 ur4t
 *
 * Copyright (C) 1994-2023 Lua.org, PUC-Rio.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

//config:config	LUA
//config:	bool "lua"
//config:	default y
//config:	help
//config:	Lua interpreter for busybox.
//config:
//config:config	LUAC
//config:	bool "luac"
//config:	default y
//config:	depends on LUA
//config:	help
//config:	Lua compiler for busybox.
//config:
//config:config	LUA_PROGNAME
//config:	string "lua program name"
//config:	default "lua"
//config:	depends on LUA
//config:	help
//config:	Sets lua program name in messeges. The default is "lua".
//config:
//config:config	LUA_INIT_VAR
//config:	string "lua init var name"
//config:	default "LUA_INIT"
//config:	depends on LUA
//config:	help
//config:	Sets lua init variable name. The default is "LUA_INIT".
//config:
//config:config	LUA_PROMPT
//config:	string "prompt in lua REPL"
//config:	default "> "
//config:	depends on LUA
//config:	help
//config:	Sets prompt string in lua REPL. The default is "> ".
//config:
//config:config	LUA_PROMPT2
//config:	string "secondary prompt in lua REPL"
//config:	default ">> "
//config:	depends on LUA
//config:	help
//config:	Sets secondary prompt string in lua REPL. The default is ">> ".

//usage:#define lua_trivial_usage "[-ivEW] [-e STAT] [-l MOD] [SCRIPT [ARGS]]"
//usage:
//usage:#define lua_full_usage "\n\n"
//usage:	"Lua interpreter\n"
//usage:	"\n	-e STAT	execute string 'STAT'"
//usage:	"\n	-i	enter interactive mode after executing 'SCRIPT'"
//usage:	"\n	-l MOD	require library 'MOD' into global 'MOD'"
//usage:	"\n	-l G=M	require library 'M' into global 'G'"
//usage:	"\n	-v	show version information"
//usage:	"\n	-E	ignore environment variables"
//usage:	"\n	-W	turn warnings on"
//usage:	"\n	--	stop handling options"
//usage:	"\n	-	stop handling options and execute stdin"
//usage:
//usage:#define luac_trivial_usage "[-lpsv] [-o NAME] [SOURCES]"
//usage:
//usage:#define luac_full_usage "\n\n"
//usage:	"Lua interpreter\n"
//usage:	"\n	-l 	list (use -l -l for full listing)"
//usage:	"\n	-o NAME	output to file NAME (default is 'luac.out')"
//usage:	"\n	-p	parse only"
//usage:	"\n	-s	strip debug information"
//usage:	"\n	-v	show version information"
//usage:	"\n	--	stop handling options"
//usage:	"\n	-	stop handling options and process stdin"

//applet:IF_LUA (APPLET(lua,  BB_DIR_BIN, BB_SUID_DROP))
//applet:IF_LUAC(APPLET(luac, BB_DIR_BIN, BB_SUID_DROP))

//kbuild:lib-$(CONFIG_LUA) += lua.o

/* core -- used by all */
//kbuild:lib-$(CONFIG_LUA) += lua/lzio.o
//kbuild:lib-$(CONFIG_LUA) += lua/lctype.o
//kbuild:lib-$(CONFIG_LUA) += lua/lopcodes.o
//kbuild:lib-$(CONFIG_LUA) += lua/lmem.o
//kbuild:lib-$(CONFIG_LUA) += lua/lundump.o
//kbuild:lib-$(CONFIG_LUA) += lua/ldump.o
//kbuild:lib-$(CONFIG_LUA) += lua/lstate.o
//kbuild:lib-$(CONFIG_LUA) += lua/lgc.o
//kbuild:lib-$(CONFIG_LUA) += lua/llex.o
//kbuild:lib-$(CONFIG_LUA) += lua/lcode.o
//kbuild:lib-$(CONFIG_LUA) += lua/lparser.o
//kbuild:lib-$(CONFIG_LUA) += lua/ldebug.o
//kbuild:lib-$(CONFIG_LUA) += lua/lfunc.o
//kbuild:lib-$(CONFIG_LUA) += lua/lobject.o
//kbuild:lib-$(CONFIG_LUA) += lua/ltm.o
//kbuild:lib-$(CONFIG_LUA) += lua/lstring.o
//kbuild:lib-$(CONFIG_LUA) += lua/ltable.o
//kbuild:lib-$(CONFIG_LUA) += lua/ldo.o
//kbuild:lib-$(CONFIG_LUA) += lua/lvm.o
//kbuild:lib-$(CONFIG_LUA) += lua/lapi.o

/* auxiliary library -- used by all */
//kbuild:lib-$(CONFIG_LUA) += lua/lauxlib.o

/* standard library  -- not used by luac */
//kbuild:lib-$(CONFIG_LUA) += lua/lbaselib.o
//kbuild:lib-$(CONFIG_LUA) += lua/lcorolib.o
//kbuild:lib-$(CONFIG_LUA) += lua/ldblib.o
//kbuild:lib-$(CONFIG_LUA) += lua/liolib.o
//kbuild:lib-$(CONFIG_LUA) += lua/lmathlib.o
//kbuild:lib-$(CONFIG_LUA) += lua/loadlib.o
//kbuild:lib-$(CONFIG_LUA) += lua/loslib.o
//kbuild:lib-$(CONFIG_LUA) += lua/lstrlib.o
//kbuild:lib-$(CONFIG_LUA) += lua/ltablib.o
//kbuild:lib-$(CONFIG_LUA) += lua/lutf8lib.o
//kbuild:lib-$(CONFIG_LUA) += lua/linit.o

#include "libbb.h"

#if ENABLE_LUA

#define LUA_PROGNAME CONFIG_LUA_PROGNAME
#define LUA_INIT_VAR CONFIG_LUA_INIT_VAR
#define LUA_PROMPT   CONFIG_LUA_PROMPT
#define LUA_PROMPT2  CONFIG_LUA_PROMPT2
#define LUA_MAXINPUT CONFIG_FEATURE_EDITING_MAX_LEN
#if ENABLE_PLATFORM_MINGW32
#define LUA_USE_WINDOWS
#else
#define LUA_USE_POSIX
#endif

static line_input_t line_input;
#define lua_initreadline(L) \
	do { \
		line_input_t *p = new_line_input_t(DO_HISTORY); \
		memcpy(&line_input, p, sizeof(line_input_t)); \
		free(p); \
	} while (0)
#define lua_readline(L, b, p) (read_line_input(&line_input, p, b, LUA_MAXINPUT) > 0 ? 1 : 0)
#define lua_saveline(L, line) /* empty */
#define lua_freeline(L, b)    /* empty */

int lua_main(int argc, char **argv) MAIN_EXTERNALLY_VISIBLE;
#define main(A, B) lua_main(A, B)
#define pmain      lua_pmain
#define progname   lua_progname

#include "lua/lua.c"

#undef main
#undef pmain
#undef progname

#endif /* ENABLE_LUA */

#if ENABLE_LUAC

int luac_main(int argc, char **argv) MAIN_EXTERNALLY_VISIBLE;
#define main(A, B) luac_main(A, B)
#define pmain      luac_pmain
#define progname   luac_progname
#define writer     luac_writer

#undef isprint
#define isprint(a) isprint_asciionly(a)

#include "lua/luac.c"

#undef main
#undef pmain
#undef progname
#undef writer

#endif /* ENABLE_LUAC */
