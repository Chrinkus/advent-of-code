#pragma once

#include <stdint.h>

#include <sxc_term_color.h>

const char* aoc_tree_plain = ""		\
	"          |        \n"		\
	"         \\|/       \n"	\
	"        --*--      \n"		\
	"         >o<       \n"		\
	"        >O<<<      \n"		\
	"       >>o>>*<     \n"		\
	"      >o<<<o<<<    \n"		\
	"     >>@>*<<O<<<   \n"		\
	"    >o>>@>>>o>o<<  \n"		\
	"   >*>>*<o<@<o<<<< \n"		\
	"  >o>o<<<O<*>>*>>O<\n"		\
	"     _ __| |__ _   ";

const char* aoc_tree_color = ""		\
	TC5YEL "          |        \n"		\
	"         \\|/       \n"	\
	"        --" TCBLNK "*" TCRBLNK "--      \n"		\
	TCNGRN "         >" TC5ORG "o" TCNGRN "<       \n"		\
	TCNGRN "        >" TCBBLU "O" TCNGRN "<<<      \n"		\
	TCNGRN "       >>" TC5ORG "o" TCNGRN ">>"
	TC5YEL "*" TCNGRN "<     \n"		\
	TCNGRN "      >" TC5ORG "o" TCNGRN "<<<"
	TC5ORG "o" TCNGRN "<<<    \n"		\
	TCNGRN "     >>" TCBRED "@" TCNGRN ">"
	TC5YEL "*" TCNGRN "<<" TCBBLU "O" TCNGRN "<<<   \n"		\
	TCNGRN "    >" TC5ORG "o" TCNGRN ">>"
	TCBRED "@" TCNGRN ">>>" TC5ORG "o"
	TCNGRN ">" TC5ORG "o" TCNGRN "<<  \n"		\
	TCNGRN "   >" TC5YEL "*" TCNGRN ">>"
	TC5YEL "*" TCNGRN "<" TC5ORG "o" TCNGRN "<"
	TCBRED "@" TCNGRN "<" TC5ORG "o" TCNGRN "<<<< \n"		\
	TCNGRN "  >" TC5ORG "o" TCNGRN ">"
	TC5ORG "o" TCNGRN "<<<" TCBBLU "O"
	TCNGRN "<" TC5YEL "*" TCNGRN ">>" TC5YEL "*"
	TCNGRN ">>" TCBBLU "O" TCNGRN "<\n"		\
	TC5GRY "     _ __| |__ _   " TCDEF;

const char* aoc_sub_plain = ""		\
	"           o           \n"	\
	"  _|______/|           \n"	\
	" /' ' ' ' '\\/\\  . ' , '\n"	\
	"( )\\\\\\\\___ =|>~. ' .   \n"	\
	" \\____(___)/\\/  ' .  , \n"	\
	",/,/      \\|           \n"	\
	"           o           ";

const char* aoc_sub_color = ""		\
	"           o           \n"	\
	"  _|______/|           \n"	\
	" /" TCBGRN "' " TCBRED "' " TCBGRN "' "	\
	TCBRED "' " TCBGRN "'" TCDEF "\\/\\" TCNBLU "  . ' , '\n" TCDEF	\
	TCBCYN "( )" TCDEF "\\\\\\\\___ =|>" TCNBLU "~. ' .   \n" TCDEF	\
	" \\____(___)/\\/" TCNBLU "  ' .  , \n"	TCDEF	\
	",/,/      \\|           \n"	\
	"           o           ";

// Shortened to match event banner length
const char* aoc_sub_plain_right = ""	\
	"           o           \n"	\
	"           |\\______|_  \n"	\
	"' , ' .  /\\/' ' ' ' '\\ \n"	\
	"   . ' .~<|= ___////( )\n"	\
	" ,  . '  \\/\\(___)____/ \n"	\
	"           |/      \\,\\,\n"	\
	"           o           ";

const char* aoc_sub_color_right = ""	\
	"       o           \n"	\
	"       |\\______|_  \n"	\
	TCBBLU "' ." TCDEF "  /\\/" TCBGRN "' " TCBRED "' "	\
	TCBGRN "' " TCBRED "' " TCBGRN "'" TCDEF "\\ \n"	\
	TCBBLU " ' ." TCDEF "~<|= ___////" TCBCYN "( )\n" TCDEF	\
	TCBBLU ". '" TCDEF "  \\/\\(___)____/ \n"	\
	"       |/      \\,\\,\n"	\
	"       o           ";

enum { AOC_NUM_COLORS = 3, AOC_BUFF_LEN = 32 };

const int aoc_xmas_colors[AOC_NUM_COLORS] = {
	SXC_TCB_RED,
	SXC_TCB_GRN,
	SXC_TCB_WHT,
};

void aoc_banner(const char* year, const char* day, const char* title)
{
	char banner[AOC_BUFF_LEN];
	snprintf(banner, AOC_BUFF_LEN, "Advent of Code %s\n", year);
	sxc_termcolor_altc(banner, aoc_xmas_colors, AOC_NUM_COLORS);

	printf("%s\n", aoc_tree_color);
	printf(TCBWHT "Day %s:" TCDEF " ", day);
	sxc_termcolor_altw(title, aoc_xmas_colors, AOC_NUM_COLORS);
	printf("\n\n");
}

void aoc_banner_2021(const char* day, const char* title)
{
	printf(TCRES "\n");
	sxc_termcolor_altc("Advent of Code 2021\n",
			aoc_xmas_colors,
			AOC_NUM_COLORS);

	printf("%s\n", aoc_sub_color_right);
	printf(TCBWHT "Day %s:" TCDEF " ", day);
	sxc_termcolor_altw(title, aoc_xmas_colors, AOC_NUM_COLORS);
	printf("\n\n");
}

