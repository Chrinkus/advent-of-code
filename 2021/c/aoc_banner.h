#pragma once

#include <sxc_term_color.h>

const char* aoc_tree_plain = ""	\
	"        |        "	\
	"       \\|/       "	\
	"      --*--      "	\
	"       >o<       "	\
	"      >O<<<      "	\
	"     >>o>>*<     "	\
	"    >o<<<o<<<    "	\
	"   >>@>*<<O<<<   "	\
	"  >o>>@>>>o>o<<  "	\
	" >*>>*<o<@<o<<<< "	\
	">o>o<<<O<*>>*>>O<"	\
	"   _ __| |__ _   ";

const char* aoc_sub_plain = ""		\
	"           o           \n"	\
	"  _|______/|           \n"	\
	" /* * * * *\\/\\  . ' , '\n"	\
	"( )\\\\\\\\___ =|>~. ' .   \n"	\
	" \\____(___)/\\/  ' .  , \n"	\
	",/,/      \\|           \n"	\
	"           o           ";

const char* aoc_sub_color = ""		\
	"           o           \n"	\
	"  _|______/|           \n"	\
	" /" TCBGRN "* " TCBRED "* " TCBGRN "* "	\
	TCBRED "* " TCBGRN "*" TCDEF "\\/\\" TCNBLU "  . ' , '\n" TCDEF	\
	TCBCYN "( )" TCDEF "\\\\\\\\___ =|>" TCNBLU "~. ' .   \n" TCDEF	\
	" \\____(___)/\\/" TCNBLU "  ' .  , \n"	TCDEF	\
	",/,/      \\|           \n"	\
	"           o           ";

void aoc_banner_2021(const char* day, const char* title)
{
	int xmas_colors[3] = { SXC_TCB_RED, SXC_TCB_GRN, SXC_TCB_WHT };
	sxc_termcolor_altc("Advent of Code 2021\n", xmas_colors, 3);
	printf("%s\n", aoc_sub_color);
	printf(TCBWHT "Day %s: " TCDEF, day);
	sxc_termcolor_altw(title, xmas_colors, 3);
	printf("\n");
}

