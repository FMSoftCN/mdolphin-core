
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.4.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 1

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 0

/* Substitute the variable and function names.  */
#define yyparse         cssyyparse
#define yylex           cssyylex
#define yyerror         cssyyerror
#define yylval          cssyylval
#define yychar          cssyychar
#define yydebug         cssyydebug
#define yynerrs         cssyynerrs


/* Copy the first part of user declarations.  */

/* Line 189 of yacc.c  */
#line 1 "Source/WebCore/css/CSSGrammar.y"


/*
 *  Copyright (C) 2002-2003 Lars Knoll (knoll@kde.org)
 *  Copyright (C) 2004, 2005, 2006, 2007, 2008, 2009, 2010 Apple Inc. All rights reserved.
 *  Copyright (C) 2006 Alexey Proskuryakov (ap@nypop.com)
 *  Copyright (C) 2008 Eric Seidel <eric@webkit.org>
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#include "config.h"

#include "CSSMediaRule.h"
#include "CSSParser.h"
#include "CSSPrimitiveValue.h"
#include "CSSPropertyNames.h"
#include "CSSRuleList.h"
#include "CSSSelector.h"
#include "CSSStyleSheet.h"
#include "Document.h"
#include "HTMLNames.h"
#include "MediaList.h"
#include "MediaQueryExp.h"
#include "WebKitCSSKeyframeRule.h"
#include "WebKitCSSKeyframesRule.h"
#include <wtf/FastMalloc.h>
#include <stdlib.h>
#include <string.h>

using namespace WebCore;
using namespace HTMLNames;

#define YYMALLOC fastMalloc
#define YYFREE fastFree

#define YYENABLE_NLS 0
#define YYLTYPE_IS_TRIVIAL 1
#define YYMAXDEPTH 10000
#define YYDEBUG 0

// FIXME: Replace with %parse-param { CSSParser* parser } once we can depend on bison 2.x
#define YYPARSE_PARAM parser
#define YYLEX_PARAM parser



/* Line 189 of yacc.c  */
#line 142 "./DerivedSources/WebCore/CSSGrammar.cpp"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     TOKEN_EOF = 0,
     LOWEST_PREC = 258,
     UNIMPORTANT_TOK = 259,
     WHITESPACE = 260,
     SGML_CD = 261,
     INCLUDES = 262,
     DASHMATCH = 263,
     BEGINSWITH = 264,
     ENDSWITH = 265,
     CONTAINS = 266,
     STRING = 267,
     IDENT = 268,
     NTH = 269,
     HEX = 270,
     IDSEL = 271,
     IMPORT_SYM = 272,
     PAGE_SYM = 273,
     MEDIA_SYM = 274,
     FONT_FACE_SYM = 275,
     CHARSET_SYM = 276,
     NAMESPACE_SYM = 277,
     WEBKIT_RULE_SYM = 278,
     WEBKIT_DECLS_SYM = 279,
     WEBKIT_KEYFRAME_RULE_SYM = 280,
     WEBKIT_KEYFRAMES_SYM = 281,
     WEBKIT_VALUE_SYM = 282,
     WEBKIT_MEDIAQUERY_SYM = 283,
     WEBKIT_SELECTOR_SYM = 284,
     TOPLEFTCORNER_SYM = 285,
     TOPLEFT_SYM = 286,
     TOPCENTER_SYM = 287,
     TOPRIGHT_SYM = 288,
     TOPRIGHTCORNER_SYM = 289,
     BOTTOMLEFTCORNER_SYM = 290,
     BOTTOMLEFT_SYM = 291,
     BOTTOMCENTER_SYM = 292,
     BOTTOMRIGHT_SYM = 293,
     BOTTOMRIGHTCORNER_SYM = 294,
     LEFTTOP_SYM = 295,
     LEFTMIDDLE_SYM = 296,
     LEFTBOTTOM_SYM = 297,
     RIGHTTOP_SYM = 298,
     RIGHTMIDDLE_SYM = 299,
     RIGHTBOTTOM_SYM = 300,
     ATKEYWORD = 301,
     IMPORTANT_SYM = 302,
     MEDIA_ONLY = 303,
     MEDIA_NOT = 304,
     MEDIA_AND = 305,
     REMS = 306,
     QEMS = 307,
     EMS = 308,
     EXS = 309,
     PXS = 310,
     CMS = 311,
     MMS = 312,
     INS = 313,
     PTS = 314,
     PCS = 315,
     DEGS = 316,
     RADS = 317,
     GRADS = 318,
     TURNS = 319,
     MSECS = 320,
     SECS = 321,
     HERTZ = 322,
     KHERTZ = 323,
     DIMEN = 324,
     PERCENTAGE = 325,
     FLOATTOKEN = 326,
     INTEGER = 327,
     URI = 328,
     FUNCTION = 329,
     ANYFUNCTION = 330,
     NOTFUNCTION = 331,
     UNICODERANGE = 332
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 214 of yacc.c  */
#line 63 "Source/WebCore/css/CSSGrammar.y"

    bool boolean;
    char character;
    int integer;
    double number;
    CSSParserString string;

    CSSRule* rule;
    CSSRuleList* ruleList;
    CSSParserSelector* selector;
    Vector<OwnPtr<CSSParserSelector> >* selectorList;
    CSSSelector::MarginBoxType marginBox;
    CSSSelector::Relation relation;
    MediaList* mediaList;
    MediaQuery* mediaQuery;
    MediaQuery::Restrictor mediaQueryRestrictor;
    MediaQueryExp* mediaQueryExp;
    CSSParserValue value;
    CSSParserValueList* valueList;
    Vector<OwnPtr<MediaQueryExp> >* mediaQueryExpList;
    WebKitCSSKeyframeRule* keyframeRule;
    WebKitCSSKeyframesRule* keyframesRule;
    float val;



/* Line 214 of yacc.c  */
#line 283 "./DerivedSources/WebCore/CSSGrammar.cpp"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */

/* Line 264 of yacc.c  */
#line 88 "Source/WebCore/css/CSSGrammar.y"


static inline int cssyyerror(const char*)
{
    return 1;
}

static int cssyylex(YYSTYPE* yylval, void* parser)
{
    return static_cast<CSSParser*>(parser)->lex(yylval);
}



/* Line 264 of yacc.c  */
#line 310 "./DerivedSources/WebCore/CSSGrammar.cpp"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  21
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1424

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  98
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  86
/* YYNRULES -- Number of rules.  */
#define YYNRULES  267
/* YYNRULES -- Number of states.  */
#define YYNSTATES  497

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   332

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,    96,     2,    97,     2,     2,
      86,    87,    20,    89,    88,    92,    18,    95,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    17,    85,
       2,    94,    91,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    19,     2,    93,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    83,    21,    84,    90,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     8,    11,    14,    17,    20,    23,    26,
      33,    40,    46,    52,    58,    64,    65,    68,    69,    72,
      75,    76,    78,    80,    82,    88,    92,    96,   102,   103,
     107,   110,   112,   114,   116,   118,   120,   122,   124,   126,
     128,   130,   131,   135,   137,   139,   141,   143,   145,   147,
     149,   151,   153,   155,   162,   169,   173,   177,   184,   191,
     195,   199,   200,   203,   205,   207,   210,   211,   216,   224,
     226,   232,   233,   237,   238,   240,   242,   244,   249,   250,
     252,   254,   259,   262,   270,   277,   280,   289,   291,   293,
     294,   298,   305,   307,   313,   315,   317,   326,   330,   334,
     336,   339,   341,   342,   344,   349,   350,   358,   360,   362,
     364,   366,   368,   370,   372,   374,   376,   378,   380,   382,
     384,   386,   388,   390,   398,   402,   406,   409,   412,   415,
     417,   418,   420,   422,   424,   425,   426,   433,   435,   440,
     443,   446,   448,   450,   453,   457,   460,   462,   465,   468,
     470,   473,   475,   478,   482,   485,   487,   493,   496,   498,
     500,   502,   505,   508,   510,   512,   514,   516,   518,   521,
     524,   529,   538,   544,   554,   556,   558,   560,   562,   564,
     566,   568,   570,   573,   576,   580,   587,   594,   602,   609,
     616,   618,   621,   623,   627,   629,   632,   635,   639,   643,
     648,   652,   658,   663,   668,   675,   681,   684,   691,   698,
     701,   705,   710,   713,   716,   719,   720,   722,   726,   729,
     733,   736,   739,   742,   743,   745,   748,   751,   754,   757,
     761,   764,   767,   770,   773,   775,   778,   781,   784,   787,
     790,   793,   796,   799,   802,   805,   808,   811,   814,   817,
     820,   823,   826,   829,   832,   835,   838,   841,   847,   851,
     853,   856,   860,   864,   867,   873,   877,   879
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
      99,     0,    -1,   106,   108,   107,   112,    -1,   100,   106,
      -1,   102,   106,    -1,   103,   106,    -1,   104,   106,    -1,
     105,   106,    -1,   101,   106,    -1,    28,    83,   106,   113,
     106,    84,    -1,    30,    83,   106,   136,   106,    84,    -1,
      29,    83,   149,   169,    84,    -1,    32,    83,   106,   174,
      84,    -1,    33,     5,   106,   128,    84,    -1,    34,    83,
     106,   153,    84,    -1,    -1,   106,     5,    -1,    -1,   107,
       6,    -1,   107,     5,    -1,    -1,   110,    -1,    84,    -1,
       0,    -1,    26,   106,    12,   106,    85,    -1,    26,     1,
     182,    -1,    26,     1,    85,    -1,    26,   106,    12,   106,
      85,    -1,    -1,   112,   114,   107,    -1,   150,   152,    -1,
     131,    -1,   139,    -1,   145,    -1,   133,    -1,   119,    -1,
     118,    -1,   113,    -1,   111,    -1,   181,    -1,   180,    -1,
      -1,   115,   117,   107,    -1,   152,    -1,   139,    -1,   145,
      -1,   133,    -1,   116,    -1,   181,    -1,   180,    -1,   119,
      -1,   118,    -1,   131,    -1,    22,   106,   121,   106,   129,
      85,    -1,    22,   106,   121,   106,   129,   182,    -1,    22,
       1,    85,    -1,    22,     1,   182,    -1,    27,   106,   120,
     121,   106,    85,    -1,    27,   106,   120,   121,   106,   182,
      -1,    27,     1,   182,    -1,    27,     1,    85,    -1,    -1,
      13,   106,    -1,    12,    -1,    78,    -1,    13,   106,    -1,
      -1,    17,   106,   174,   106,    -1,    86,   106,   122,   106,
     123,    87,   106,    -1,   124,    -1,   125,   106,    55,   106,
     124,    -1,    -1,    55,   106,   125,    -1,    -1,    53,    -1,
      54,    -1,   125,    -1,   127,   106,   132,   126,    -1,    -1,
     130,    -1,   128,    -1,   130,    88,   106,   128,    -1,   130,
       1,    -1,    24,   106,   130,    83,   106,   115,   179,    -1,
      24,   106,    83,   106,   115,   179,    -1,    13,   106,    -1,
      31,   106,   134,   106,    83,   106,   135,    84,    -1,    13,
      -1,    12,    -1,    -1,   135,   136,   106,    -1,   137,   106,
      83,   106,   169,    84,    -1,   138,    -1,   137,   106,    88,
     106,   138,    -1,    75,    -1,    13,    -1,    23,   106,   140,
     106,    83,   106,   141,   109,    -1,    23,     1,   182,    -1,
      23,     1,    85,    -1,    13,    -1,    13,   167,    -1,   167,
      -1,    -1,   169,    -1,   141,   142,   106,   169,    -1,    -1,
     144,   143,   106,    83,   106,   169,   109,    -1,    35,    -1,
      36,    -1,    37,    -1,    38,    -1,    39,    -1,    40,    -1,
      41,    -1,    42,    -1,    43,    -1,    44,    -1,    45,    -1,
      46,    -1,    47,    -1,    48,    -1,    49,    -1,    50,    -1,
      25,   106,    83,   106,   169,    84,   106,    -1,    25,     1,
     182,    -1,    25,     1,    85,    -1,    89,   106,    -1,    90,
     106,    -1,    91,   106,    -1,   148,    -1,    -1,    92,    -1,
      89,    -1,   106,    -1,    -1,    -1,   153,   151,    83,   149,
     169,   109,    -1,   155,    -1,   153,    88,   106,   155,    -1,
     153,     1,    -1,   155,     5,    -1,   157,    -1,   154,    -1,
     154,   157,    -1,   155,   146,   157,    -1,   155,     1,    -1,
      21,    -1,    20,    21,    -1,    13,    21,    -1,   159,    -1,
     159,   160,    -1,   160,    -1,   156,   159,    -1,   156,   159,
     160,    -1,   156,   160,    -1,   157,    -1,   158,   106,    88,
     106,   157,    -1,   158,     1,    -1,    13,    -1,    20,    -1,
     161,    -1,   160,   161,    -1,   160,     1,    -1,    16,    -1,
      15,    -1,   162,    -1,   164,    -1,   168,    -1,    18,    13,
      -1,    13,   106,    -1,    19,   106,   163,    93,    -1,    19,
     106,   163,   165,   106,   166,   106,    93,    -1,    19,   106,
     156,   163,    93,    -1,    19,   106,   156,   163,   165,   106,
     166,   106,    93,    -1,    94,    -1,     7,    -1,     8,    -1,
       9,    -1,    10,    -1,    11,    -1,    13,    -1,    12,    -1,
      17,    13,    -1,    17,    13,    -1,    17,    17,    13,    -1,
      17,    80,   106,   158,   106,    87,    -1,    17,    79,   106,
      14,   106,    87,    -1,    17,    79,   106,   147,    77,   106,
      87,    -1,    17,    79,   106,    13,   106,    87,    -1,    17,
      81,   106,   157,   106,    87,    -1,   171,    -1,   170,   171,
      -1,   170,    -1,     1,   183,     1,    -1,     1,    -1,   170,
       1,    -1,   170,   183,    -1,   171,    85,   106,    -1,   171,
     183,   106,    -1,   171,   183,    85,   106,    -1,     1,    85,
     106,    -1,     1,   183,     1,    85,   106,    -1,   170,   171,
      85,   106,    -1,   170,     1,    85,   106,    -1,   170,     1,
     183,     1,    85,   106,    -1,   172,    17,   106,   174,   173,
      -1,   172,     1,    -1,   172,    17,   106,     1,   174,   173,
      -1,   172,    17,   106,   174,   173,     1,    -1,    52,   106,
      -1,   172,    17,   106,    -1,   172,    17,   106,     1,    -1,
     172,   182,    -1,    13,   106,    -1,    52,   106,    -1,    -1,
     176,    -1,   174,   175,   176,    -1,   174,   183,    -1,   174,
     183,     1,    -1,   174,     1,    -1,    95,   106,    -1,    88,
     106,    -1,    -1,   177,    -1,   148,   177,    -1,    12,   106,
      -1,    13,   106,    -1,    74,   106,    -1,   148,    74,   106,
      -1,    78,   106,    -1,    82,   106,    -1,    15,   106,    -1,
      96,   106,    -1,   178,    -1,    97,   106,    -1,    77,   106,
      -1,    76,   106,    -1,    75,   106,    -1,    60,   106,    -1,
      61,   106,    -1,    62,   106,    -1,    63,   106,    -1,    64,
     106,    -1,    65,   106,    -1,    66,   106,    -1,    67,   106,
      -1,    68,   106,    -1,    69,   106,    -1,    70,   106,    -1,
      71,   106,    -1,    72,   106,    -1,    73,   106,    -1,    58,
     106,    -1,    57,   106,    -1,    59,   106,    -1,    56,   106,
      -1,    79,   106,   174,    87,   106,    -1,    79,   106,     1,
      -1,   109,    -1,     1,   109,    -1,    51,     1,   182,    -1,
      51,     1,    85,    -1,     1,   182,    -1,    83,     1,   183,
       1,   109,    -1,    83,     1,   109,    -1,   182,    -1,   183,
       1,   182,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   281,   281,   282,   283,   284,   285,   286,   287,   291,
     297,   303,   309,   323,   330,   340,   341,   344,   346,   347,
     350,   352,   357,   358,   362,   368,   370,   375,   381,   383,
     391,   394,   395,   396,   397,   398,   399,   403,   406,   407,
     408,   412,   413,   424,   425,   426,   427,   431,   432,   433,
     434,   435,   436,   441,   444,   447,   450,   456,   460,   463,
     466,   472,   473,   477,   478,   482,   488,   491,   497,   504,
     509,   516,   519,   525,   528,   531,   537,   542,   550,   553,
     557,   562,   567,   573,   576,   582,   588,   595,   596,   600,
     601,   609,   615,   620,   629,   630,   643,   655,   658,   664,
     670,   678,   683,   691,   692,   696,   696,   704,   707,   710,
     713,   716,   719,   722,   725,   728,   731,   734,   737,   740,
     743,   746,   749,   755,   759,   762,   768,   769,   770,   774,
     775,   779,   780,   784,   791,   798,   805,   812,   821,   830,
     836,   842,   845,   849,   863,   876,   882,   883,   884,   888,
     893,   898,   903,   913,   918,   926,   934,   942,   948,   956,
     964,   967,   989,   995,  1003,  1015,  1016,  1017,  1021,  1032,
    1043,  1048,  1054,  1062,  1074,  1077,  1080,  1083,  1086,  1089,
    1095,  1096,  1100,  1111,  1120,  1133,  1148,  1159,  1170,  1189,
    1208,  1211,  1216,  1219,  1222,  1225,  1228,  1234,  1239,  1242,
    1245,  1250,  1253,  1260,  1265,  1273,  1291,  1295,  1304,  1311,
    1316,  1323,  1330,  1337,  1343,  1344,  1348,  1353,  1367,  1370,
    1373,  1379,  1382,  1385,  1391,  1392,  1393,  1394,  1400,  1401,
    1402,  1403,  1404,  1405,  1407,  1410,  1416,  1417,  1418,  1419,
    1420,  1421,  1422,  1423,  1424,  1425,  1426,  1427,  1428,  1429,
    1430,  1431,  1432,  1433,  1434,  1435,  1436,  1447,  1456,  1470,
    1473,  1479,  1482,  1488,  1506,  1509,  1515,  1516
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "TOKEN_EOF", "error", "$undefined", "LOWEST_PREC", "UNIMPORTANT_TOK",
  "WHITESPACE", "SGML_CD", "INCLUDES", "DASHMATCH", "BEGINSWITH",
  "ENDSWITH", "CONTAINS", "STRING", "IDENT", "NTH", "HEX", "IDSEL", "':'",
  "'.'", "'['", "'*'", "'|'", "IMPORT_SYM", "PAGE_SYM", "MEDIA_SYM",
  "FONT_FACE_SYM", "CHARSET_SYM", "NAMESPACE_SYM", "WEBKIT_RULE_SYM",
  "WEBKIT_DECLS_SYM", "WEBKIT_KEYFRAME_RULE_SYM", "WEBKIT_KEYFRAMES_SYM",
  "WEBKIT_VALUE_SYM", "WEBKIT_MEDIAQUERY_SYM", "WEBKIT_SELECTOR_SYM",
  "TOPLEFTCORNER_SYM", "TOPLEFT_SYM", "TOPCENTER_SYM", "TOPRIGHT_SYM",
  "TOPRIGHTCORNER_SYM", "BOTTOMLEFTCORNER_SYM", "BOTTOMLEFT_SYM",
  "BOTTOMCENTER_SYM", "BOTTOMRIGHT_SYM", "BOTTOMRIGHTCORNER_SYM",
  "LEFTTOP_SYM", "LEFTMIDDLE_SYM", "LEFTBOTTOM_SYM", "RIGHTTOP_SYM",
  "RIGHTMIDDLE_SYM", "RIGHTBOTTOM_SYM", "ATKEYWORD", "IMPORTANT_SYM",
  "MEDIA_ONLY", "MEDIA_NOT", "MEDIA_AND", "REMS", "QEMS", "EMS", "EXS",
  "PXS", "CMS", "MMS", "INS", "PTS", "PCS", "DEGS", "RADS", "GRADS",
  "TURNS", "MSECS", "SECS", "HERTZ", "KHERTZ", "DIMEN", "PERCENTAGE",
  "FLOATTOKEN", "INTEGER", "URI", "FUNCTION", "ANYFUNCTION", "NOTFUNCTION",
  "UNICODERANGE", "'{'", "'}'", "';'", "'('", "')'", "','", "'+'", "'~'",
  "'>'", "'-'", "']'", "'='", "'/'", "'#'", "'%'", "$accept", "stylesheet",
  "webkit_rule", "webkit_keyframe_rule", "webkit_decls", "webkit_value",
  "webkit_mediaquery", "webkit_selector", "maybe_space", "maybe_sgml",
  "maybe_charset", "closing_brace", "charset", "ignored_charset",
  "rule_list", "valid_rule", "rule", "block_rule_list", "block_valid_rule",
  "block_rule", "import", "namespace", "maybe_ns_prefix", "string_or_uri",
  "media_feature", "maybe_media_value", "media_query_exp",
  "media_query_exp_list", "maybe_and_media_query_exp_list",
  "maybe_media_restrictor", "media_query", "maybe_media_list",
  "media_list", "media", "medium", "keyframes", "keyframe_name",
  "keyframes_rule", "keyframe_rule", "key_list", "key", "page",
  "page_selector", "declarations_and_margins", "margin_box", "$@1",
  "margin_sym", "font_face", "combinator", "maybe_unary_operator",
  "unary_operator", "maybe_space_before_declaration", "before_ruleset",
  "before_rule_opening_brace", "ruleset", "selector_list",
  "selector_with_trailing_whitespace", "selector", "namespace_selector",
  "simple_selector", "simple_selector_list", "element_name",
  "specifier_list", "specifier", "class", "attr_name", "attrib", "match",
  "ident_or_string", "pseudo_page", "pseudo", "declaration_list",
  "decl_list", "declaration", "property", "prio", "expr", "operator",
  "term", "unary_term", "function", "save_block", "invalid_at",
  "invalid_rule", "invalid_block", "invalid_block_list", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,    58,    46,    91,
      42,   124,   272,   273,   274,   275,   276,   277,   278,   279,
     280,   281,   282,   283,   284,   285,   286,   287,   288,   289,
     290,   291,   292,   293,   294,   295,   296,   297,   298,   299,
     300,   301,   302,   303,   304,   305,   306,   307,   308,   309,
     310,   311,   312,   313,   314,   315,   316,   317,   318,   319,
     320,   321,   322,   323,   324,   325,   326,   327,   328,   329,
     330,   331,   332,   123,   125,    59,    40,    41,    44,    43,
     126,    62,    45,    93,    61,    47,    35,    37
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    98,    99,    99,    99,    99,    99,    99,    99,   100,
     101,   102,   103,   104,   105,   106,   106,   107,   107,   107,
     108,   108,   109,   109,   110,   110,   110,   111,   112,   112,
     113,   113,   113,   113,   113,   113,   113,   114,   114,   114,
     114,   115,   115,   116,   116,   116,   116,   117,   117,   117,
     117,   117,   117,   118,   118,   118,   118,   119,   119,   119,
     119,   120,   120,   121,   121,   122,   123,   123,   124,   125,
     125,   126,   126,   127,   127,   127,   128,   128,   129,   129,
     130,   130,   130,   131,   131,   132,   133,   134,   134,   135,
     135,   136,   137,   137,   138,   138,   139,   139,   139,   140,
     140,   140,   140,   141,   141,   143,   142,   144,   144,   144,
     144,   144,   144,   144,   144,   144,   144,   144,   144,   144,
     144,   144,   144,   145,   145,   145,   146,   146,   146,   147,
     147,   148,   148,   149,   150,   151,   152,   153,   153,   153,
     154,   155,   155,   155,   155,   155,   156,   156,   156,   157,
     157,   157,   157,   157,   157,   158,   158,   158,   159,   159,
     160,   160,   160,   161,   161,   161,   161,   161,   162,   163,
     164,   164,   164,   164,   165,   165,   165,   165,   165,   165,
     166,   166,   167,   168,   168,   168,   168,   168,   168,   168,
     169,   169,   169,   169,   169,   169,   169,   170,   170,   170,
     170,   170,   170,   170,   170,   171,   171,   171,   171,   171,
     171,   171,   171,   172,   173,   173,   174,   174,   174,   174,
     174,   175,   175,   175,   176,   176,   176,   176,   176,   176,
     176,   176,   176,   176,   176,   176,   177,   177,   177,   177,
     177,   177,   177,   177,   177,   177,   177,   177,   177,   177,
     177,   177,   177,   177,   177,   177,   177,   178,   178,   179,
     179,   180,   180,   181,   182,   182,   183,   183
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     4,     2,     2,     2,     2,     2,     2,     6,
       6,     5,     5,     5,     5,     0,     2,     0,     2,     2,
       0,     1,     1,     1,     5,     3,     3,     5,     0,     3,
       2,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     0,     3,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     6,     6,     3,     3,     6,     6,     3,
       3,     0,     2,     1,     1,     2,     0,     4,     7,     1,
       5,     0,     3,     0,     1,     1,     1,     4,     0,     1,
       1,     4,     2,     7,     6,     2,     8,     1,     1,     0,
       3,     6,     1,     5,     1,     1,     8,     3,     3,     1,
       2,     1,     0,     1,     4,     0,     7,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     7,     3,     3,     2,     2,     2,     1,
       0,     1,     1,     1,     0,     0,     6,     1,     4,     2,
       2,     1,     1,     2,     3,     2,     1,     2,     2,     1,
       2,     1,     2,     3,     2,     1,     5,     2,     1,     1,
       1,     2,     2,     1,     1,     1,     1,     1,     2,     2,
       4,     8,     5,     9,     1,     1,     1,     1,     1,     1,
       1,     1,     2,     2,     3,     6,     6,     7,     6,     6,
       1,     2,     1,     3,     1,     2,     2,     3,     3,     4,
       3,     5,     4,     4,     6,     5,     2,     6,     6,     2,
       3,     4,     2,     2,     2,     0,     1,     3,     2,     3,
       2,     2,     2,     0,     1,     2,     2,     2,     2,     3,
       2,     2,     2,     2,     1,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     5,     3,     1,
       2,     3,     3,     2,     5,     3,     1,     3
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
      15,     0,     0,     0,     0,     0,     0,     0,    15,    15,
      15,    15,    15,    15,    20,    15,    15,    15,    15,    15,
      15,     1,     3,     8,     4,     5,     6,     7,    16,     0,
      17,    21,   134,   133,     0,     0,     0,    73,     0,     0,
       0,    28,     0,     0,    15,     0,     0,    15,    15,    36,
      35,    31,    34,    32,    33,     0,   194,    15,    15,     0,
       0,   190,     0,    95,    94,    15,    15,    92,    15,    15,
      15,    15,    15,    15,    15,    15,    15,    15,    15,    15,
      15,    15,    15,    15,    15,    15,    15,    15,    15,    15,
      15,    15,    15,    15,    15,    15,   132,   131,    15,    15,
       0,     0,   216,   224,   234,    74,    75,    15,    69,    76,
      15,     0,   158,   164,   163,     0,     0,    15,   159,   146,
       0,   142,     0,     0,   141,   149,     0,   160,   165,   166,
     167,     0,    26,    25,    15,    19,    18,     0,     0,     0,
       0,   102,    73,     0,     0,     0,    61,     0,     0,    30,
       0,    15,   266,     0,   213,   209,    11,   195,   191,     0,
      15,     0,   206,    15,   212,     0,     0,   226,   227,   232,
     256,   254,   253,   255,   239,   240,   241,   242,   243,   244,
     245,   246,   247,   248,   249,   250,   251,   252,   228,   238,
     237,   236,   230,     0,   231,   233,   235,    15,   225,   220,
      12,    15,    15,     0,     0,     0,     0,     0,    13,   148,
     183,     0,    15,    15,    15,   168,     0,   147,   139,    14,
      15,   143,   145,   140,    15,    15,    15,     0,   158,   159,
     152,     0,     0,   162,   161,     0,     0,     0,    15,     0,
      38,    37,    17,    40,    39,    55,    56,    63,    64,    15,
      98,    97,    99,     0,    15,   101,    15,    80,     0,   125,
     124,    15,    60,    59,    15,     0,    88,    87,    15,     9,
       0,   200,   193,    15,     0,    15,     0,   197,    15,   198,
       0,    10,    15,    15,   258,     0,   229,   222,   221,   217,
     219,    15,    15,    15,    15,    71,   184,   130,     0,     0,
      15,     0,     0,     0,     0,   126,   127,   128,   144,     0,
      23,    22,   265,     0,    24,   263,     0,     0,    29,    78,
     100,   182,     0,    41,    82,    15,    15,     0,    62,    15,
       0,    15,    15,   267,   203,     0,   202,   199,   211,     0,
       0,     0,    15,    65,    66,     0,    85,    15,    77,    15,
      15,     0,   129,   155,     0,    15,   169,    15,     0,   175,
     176,   177,   178,   179,   170,   174,    15,     0,     0,    15,
     262,   261,     0,     0,    15,     0,    41,    73,     0,     0,
      15,     0,   201,    15,     0,    15,     0,     0,    93,   257,
      15,     0,    70,     0,     0,     0,    15,   157,     0,     0,
     172,    15,     0,   264,     0,    53,    54,     0,     0,   259,
      47,    17,    51,    50,    52,    46,    44,    45,    43,    84,
      49,    48,     0,    81,    15,    57,    58,    89,     0,   204,
     207,   214,   208,    91,     0,    15,    72,   188,   186,     0,
     185,    15,   189,     0,   181,   180,    15,    27,     0,   103,
     260,    42,    83,   123,     0,   136,     0,    68,   187,     0,
      15,     0,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,    96,    15,
     105,    86,    15,    67,   156,     0,   171,     0,    15,    90,
     173,   104,     0,    15,     0,     0,   106
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     7,     8,     9,    10,    11,    12,    13,    33,    41,
      30,   409,    31,   240,   137,    48,   242,   375,   410,   411,
      49,    50,   265,   249,   292,   391,   108,   109,   348,   110,
     257,   372,   258,    51,   295,    52,   268,   454,    65,    66,
      67,    53,   254,   448,   479,   488,   480,    54,   227,   351,
     100,    34,    55,   270,   418,   150,   121,   122,   123,   124,
     354,   125,   126,   127,   128,   303,   129,   366,   446,   255,
     130,    59,    60,    61,    62,   386,   101,   203,   102,   103,
     104,   419,   420,   421,   152,   204
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -371
static const yytype_int16 yypact[] =
{
     457,   -50,    83,    92,   121,   144,   134,   178,  -371,  -371,
    -371,  -371,  -371,  -371,     2,  -371,  -371,  -371,  -371,  -371,
    -371,  -371,   236,   236,   236,   236,   236,   236,  -371,   399,
    -371,  -371,   407,   236,   330,   343,  1069,   298,   647,   -49,
     159,    50,   359,   252,  -371,   167,   275,  -371,  -371,  -371,
    -371,  -371,  -371,  -371,  -371,   656,   196,  -371,  -371,   176,
    1286,   261,   140,  -371,  -371,  -371,  -371,  -371,  -371,  -371,
    -371,  -371,  -371,  -371,  -371,  -371,  -371,  -371,  -371,  -371,
    -371,  -371,  -371,  -371,  -371,  -371,  -371,  -371,  -371,  -371,
    -371,  -371,  -371,  -371,  -371,  -371,  -371,  -371,  -371,  -371,
     865,   819,  -371,  -371,  -371,  -371,  -371,  -371,  -371,   269,
    -371,   201,   278,  -371,  -371,   386,   277,  -371,   309,  -371,
      66,   656,    30,   731,  -371,   459,   227,  -371,  -371,  -371,
    -371,   311,  -371,  -371,  -371,  -371,  -371,   561,   334,   290,
     340,    36,   253,   357,   179,   369,   202,    25,    18,  -371,
     105,  -371,  -371,   358,   236,   236,  -371,   372,   273,   815,
    -371,  1217,  -371,  -371,  -371,   137,   147,   236,   236,   236,
     236,   236,   236,   236,   236,   236,   236,   236,   236,   236,
     236,   236,   236,   236,   236,   236,   236,   236,   236,   236,
     236,   236,   236,   994,   236,   236,   236,  -371,  -371,  -371,
    -371,  -371,  -371,  1137,   468,   291,   279,   402,  -371,  -371,
    -371,   349,  -371,  -371,  -371,  -371,   400,  -371,  -371,  -371,
    -371,  -371,  -371,  -371,  -371,  -371,  -371,   656,  -371,  -371,
     459,   304,   360,  -371,  -371,   156,   142,   289,  -371,   385,
    -371,  -371,  -371,  -371,  -371,  -371,  -371,  -371,  -371,  -371,
    -371,  -371,   356,   384,  -371,  -371,  -371,  -371,   115,  -371,
    -371,  -371,  -371,  -371,  -371,   155,  -371,  -371,  -371,  -371,
     326,   236,   379,  -371,   415,  -371,   289,   236,  -371,   236,
     644,  -371,  -371,  -371,  -371,   905,   236,   236,   236,  -371,
     289,  -371,  -371,  -371,  -371,   355,  -371,   130,   647,   647,
     278,   309,   414,    52,   647,   236,   236,   236,  -371,   752,
    -371,  -371,  -371,   421,  -371,  -371,   296,   409,    50,   284,
    -371,  -371,   187,   236,  -371,  -371,  -371,   401,   236,  -371,
     189,  -371,  -371,  -371,   236,   412,   236,   236,  1137,   558,
     401,   343,  -371,   236,   157,   181,   236,  -371,  -371,  -371,
    -371,   351,  -371,  -371,    95,  -371,   236,  -371,   116,  -371,
    -371,  -371,  -371,  -371,  -371,  -371,  -371,    49,   156,  -371,
    -371,  -371,   413,   102,  -371,  1340,   236,   298,   361,   123,
    -371,   330,   236,  -371,   558,  -371,  1235,   374,  -371,   236,
    -371,   346,  -371,   181,   150,   164,  -371,  -371,   160,   165,
    -371,  -371,    27,  -371,   186,  -371,  -371,   401,   156,  -371,
    -371,  -371,  -371,  -371,  -371,  -371,  -371,  -371,  -371,  -371,
    -371,  -371,  1340,  -371,  -371,  -371,  -371,   236,    97,   236,
    -371,   236,  -371,  -371,  1069,  -371,   269,  -371,  -371,   172,
    -371,  -371,  -371,    27,  -371,  -371,  -371,  -371,   991,  -371,
    -371,    50,  -371,   236,   314,  -371,   730,   236,  -371,   647,
    -371,    11,  -371,  -371,  -371,  -371,  -371,  -371,  -371,  -371,
    -371,  -371,  -371,  -371,  -371,  -371,  -371,  -371,  -371,  -371,
    -371,  -371,  -371,   236,  -371,    22,  -371,   401,  -371,   236,
    -371,  -371,   194,  -371,   401,    97,  -371
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -371,  -371,  -371,  -371,  -371,  -371,  -371,  -371,     0,  -238,
    -371,  -232,  -371,  -371,  -371,   302,  -371,    85,  -371,  -371,
    -370,  -227,  -371,   206,  -371,  -371,   127,   107,  -371,  -371,
     -36,  -371,   182,  -217,  -371,  -109,  -371,  -371,    48,  -371,
     143,   -89,  -371,  -371,  -371,  -371,  -371,   -77,  -371,  -371,
     222,   190,  -371,  -371,   467,   485,  -371,   244,   333,  -119,
    -371,   431,  -101,  -120,  -371,   264,  -371,   209,   125,   317,
    -371,  -276,  -371,   512,  -371,   191,  -179,  -371,   387,   489,
    -371,   169,   472,   474,    91,   -35
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -224
static const yytype_int16 yytable[] =
{
      14,   111,   221,   312,   318,   412,   234,    28,    22,    23,
      24,    25,    26,    27,   285,    32,    28,    35,    36,    37,
      38,   153,   231,    28,   232,   159,   161,    28,    29,    40,
      28,   222,    28,    15,   131,   223,   132,   266,   267,   444,
     445,    28,   139,   141,   142,   144,   146,   147,   148,   252,
     222,   378,   412,   253,   223,   135,   136,   154,   155,   359,
     360,   361,   362,   363,   387,   165,   166,   218,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   397,   310,   195,   196,
     -15,   339,   269,   324,   486,   428,   218,   205,   308,   206,
     207,   234,   234,  -137,  -137,   490,   324,   216,  -137,   224,
     225,   226,   274,   359,   360,   361,   362,   363,    28,   309,
     133,   449,  -138,  -138,   236,    28,   403,  -138,   224,   225,
     226,   162,    28,   349,   350,   364,   365,    28,   413,    19,
     219,   271,    28,   164,   220,    28,   310,   163,   414,   384,
     277,   279,    28,   280,    28,    28,    16,   247,   143,    28,
      28,   134,   -15,   451,   390,    17,   450,    28,    21,   353,
     355,   311,   -15,   -15,    28,   -79,    28,   -79,  -135,   234,
     326,    28,    28,   220,    28,   413,   455,   286,   325,    28,
     313,   287,   288,   326,    18,   414,   131,    28,   425,   400,
     365,   491,   297,   298,   299,   264,   478,    20,   495,    96,
     304,   281,    97,   131,   305,   306,   307,   314,   233,   246,
     282,   251,  -151,   248,   260,   283,   263,   437,   316,   131,
     311,    28,   113,   114,   115,   116,   117,   440,   441,   319,
     -15,   438,   442,   140,   322,   456,   323,   -15,    28,   458,
     156,   327,   261,   496,   328,   -15,   415,   107,   330,   -15,
     374,   447,   380,   334,   -15,   336,   145,   493,   337,   131,
     -15,   151,   340,   341,    28,   208,   416,   -15,   -15,    28,
     215,   343,   344,   345,   346,    28,    28,   -73,   417,   209,
     356,    28,   247,    28,   291,   233,   105,   106,   369,  -154,
    -151,  -151,   235,   415,  -151,  -151,  -151,  -151,  -151,   113,
     114,   115,   116,   117,   -15,   376,   377,    63,   315,   379,
     217,    56,   382,   416,   293,   -15,   256,   105,   106,   107,
     484,   423,   389,    57,   131,   417,   160,   393,    28,   394,
     395,   105,   106,   -15,   398,   399,    63,   356,   275,   272,
     138,   233,   296,   333,   -15,  -150,   402,   333,   248,   404,
     107,   -15,   131,   253,   407,   113,   114,   115,   116,   117,
     427,   333,    58,   429,   107,   431,   317,  -154,  -154,    64,
     434,  -154,  -154,  -154,  -154,  -154,   439,   321,   481,   210,
      39,   443,    56,   211,   -15,    28,    28,    28,   371,   331,
     347,   -15,    28,   300,    57,   294,   335,   131,    64,   245,
     301,   119,   368,   131,   453,   250,   333,   357,   396,    42,
      43,    44,    45,   435,    46,   457,   206,   -15,    47,   241,
     131,   459,   259,  -150,  -150,   424,   461,  -150,  -150,  -150,
    -150,  -150,   131,    58,   262,   131,   483,   273,   433,   333,
     485,   422,   131,   406,   332,   212,   213,   214,  -218,   290,
     426,   329,   392,  -218,   113,   114,   115,   116,   117,   487,
    -218,  -218,   489,  -218,   388,     1,     2,     3,   492,     4,
       5,     6,   131,   494,   370,   131,   131,   383,   405,   315,
     436,   373,   482,  -218,  -218,  -218,  -218,  -218,  -218,  -218,
    -218,  -218,  -218,  -218,  -218,  -218,  -218,  -218,  -218,   352,
    -218,   381,   149,   120,  -218,  -218,  -218,  -218,  -218,  -218,
    -218,  -218,  -218,  -218,  -218,  -218,  -218,  -218,  -218,  -218,
    -218,  -218,  -218,  -218,  -218,  -218,  -218,  -218,   367,   302,
    -218,  -218,  -218,  -218,   230,  -218,  -218,  -218,  -215,   199,
    -218,    -2,   237,  -218,  -218,  -218,   358,   401,   460,   320,
    -223,  -223,   158,  -223,  -134,   430,  -134,  -134,  -134,  -134,
    -134,  -134,  -134,    42,    43,    44,    45,   238,    46,   198,
     289,   452,    47,  -215,  -215,  -215,  -215,  -215,  -215,  -215,
    -215,  -215,  -215,  -215,  -215,  -215,  -215,  -215,  -215,   243,
     385,   244,   239,     0,  -223,  -223,  -223,  -223,  -223,  -223,
    -223,  -223,  -223,  -223,  -223,  -223,  -223,  -223,  -223,  -223,
    -223,  -223,  -223,  -223,  -223,  -223,  -223,  -223,     0,     0,
    -223,   131,  -215,  -215,  -210,   338,   201,  -223,     0,    28,
    -223,     0,    28,   202,  -223,  -223,    68,    69,     0,    70,
     112,     0,   113,   114,   115,   116,   117,   118,   119,   112,
       0,   113,   114,   115,   116,   117,   118,   119,     0,  -210,
    -210,  -210,  -210,  -210,  -210,  -210,  -210,  -210,  -210,  -210,
    -210,  -210,  -210,  -210,  -210,     0,     0,     0,     0,     0,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,     0,     0,    95,  -210,  -210,  -210,
       0,   199,     0,    96,     0,   -15,    97,     0,     0,     0,
      98,    99,  -223,  -223,   228,  -223,   113,   114,   115,   116,
     117,   229,     0,   233,     0,     0,     0,  -153,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   113,   114,   115,
     116,   117,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  -223,  -223,  -223,  -223,
    -223,  -223,  -223,  -223,  -223,  -223,  -223,  -223,  -223,  -223,
    -223,  -223,  -223,  -223,  -223,  -223,  -223,  -223,  -223,  -223,
       0,     0,  -223,   131,     0,  -196,   276,   -15,   201,  -223,
     199,     0,  -223,     0,     0,   202,  -223,  -223,     0,     0,
       0,  -223,  -223,     0,  -223,  -153,  -153,     0,     0,  -153,
    -153,  -153,  -153,  -153,     0,     0,     0,     0,     0,     0,
    -196,  -196,  -196,  -196,  -196,  -196,  -196,  -196,  -196,  -196,
    -196,  -196,  -196,  -196,  -196,  -196,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  -223,  -223,  -223,  -223,  -223,
    -223,  -223,  -223,  -223,  -223,  -223,  -223,  -223,  -223,  -223,
    -223,  -223,  -223,  -223,  -223,  -223,  -223,  -223,  -223,  -196,
       0,  -223,   131,   200,     0,     0,   199,   201,  -223,     0,
       0,  -223,     0,     0,   202,  -223,  -223,  -223,  -223,     0,
    -223,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,   197,
      90,    91,    92,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  -223,  -223,  -223,  -223,  -223,  -223,  -223,  -223,  -223,
    -223,  -223,  -223,  -223,  -223,  -223,  -223,  -223,  -223,  -223,
    -223,  -223,  -223,  -223,  -223,     0,     0,  -223,   131,     0,
       0,   310,   342,   201,  -223,   284,     0,  -223,     0,    28,
     202,  -223,  -223,     0,     0,     0,    68,    69,     0,    70,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   462,   463,   464,   465,
     466,   467,   468,   469,   470,   471,   472,   473,   474,   475,
     476,   477,     0,     0,     0,     0,     0,     0,     0,     0,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    28,   311,    95,     0,     0,     0,
       0,    68,    69,    96,    70,     0,    97,     0,     0,     0,
      98,    99,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    68,
      69,    95,    70,     0,     0,     0,     0,     0,    96,     0,
       0,    97,     0,     0,     0,    98,    99,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,   -15,   276,    95,
       0,     0,   -15,     0,     0,     0,    96,     0,     0,    97,
     -15,     0,     0,    98,    99,  -205,   432,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   -15,   -15,   -15,   -15,   -15,   -15,   -15,   -15,
     -15,   -15,   -15,   -15,   -15,   -15,   -15,   -15,     0,   -15,
    -205,  -205,  -205,  -205,  -205,  -205,  -205,  -205,  -205,  -205,
    -205,  -205,  -205,  -205,  -205,  -205,  -192,   157,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    57,
     -15,   -15,   278,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  -205,  -205,
    -205,  -192,  -192,  -192,  -192,  -192,  -192,  -192,  -192,  -192,
    -192,  -192,  -192,  -192,  -192,  -192,  -192,     0,    58,     0,
     310,   408,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   112,     0,   113,   114,   115,   116,   117,
     118,   119,    42,    43,    44,    45,     0,    46,     0,   131,
    -192,    47,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   239,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   311
};

static const yytype_int16 yycheck[] =
{
       0,    37,   121,   235,   242,   375,   126,     5,     8,     9,
      10,    11,    12,    13,   193,    15,     5,    17,    18,    19,
      20,    56,   123,     5,   125,    60,    61,     5,    26,    29,
       5,     1,     5,    83,    83,     5,    85,    12,    13,    12,
      13,     5,    42,    43,    44,    45,    46,    47,    48,    13,
       1,   327,   422,    17,     5,     5,     6,    57,    58,     7,
       8,     9,    10,    11,   340,    65,    66,     1,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,     1,     0,    98,    99,
       5,   280,    84,     1,    93,   381,     1,   107,   227,   109,
     110,   231,   232,    83,    84,    93,     1,   117,    88,    89,
      90,    91,   157,     7,     8,     9,    10,    11,     5,   230,
      39,   407,    83,    84,   134,     5,   368,    88,    89,    90,
      91,     1,     5,    13,    14,    93,    94,     5,   375,     5,
      84,   151,     5,    62,    88,     5,     0,    17,   375,   338,
     160,   161,     5,   163,     5,     5,    83,    12,     1,     5,
       5,    12,     5,   411,    17,    83,   408,     5,     0,   298,
     299,    84,    87,    88,     5,    83,     5,    85,    83,   309,
      88,     5,     5,    88,     5,   422,   428,   197,    83,     5,
     235,   201,   202,    88,    83,   422,    83,     5,    85,    93,
      94,   487,   212,   213,   214,    13,   448,    83,   494,    89,
     220,    84,    92,    83,   224,   225,   226,    85,     1,   138,
      83,   140,     5,    78,   143,    88,   145,    87,   238,    83,
      84,     5,    15,    16,    17,    18,    19,    87,    88,   249,
      83,    87,    87,     1,   254,   434,   256,     5,     5,    87,
      84,   261,    83,   495,   264,    13,   375,    86,   268,    17,
      83,    85,    83,   273,     5,   275,     1,    83,   278,    83,
       5,    85,   282,   283,     5,    84,   375,    12,    13,     5,
      13,   291,   292,   293,   294,     5,     5,    13,   375,    21,
     300,     5,    12,     5,    13,     1,    53,    54,    12,     5,
      83,    84,     1,   422,    87,    88,    89,    90,    91,    15,
      16,    17,    18,    19,    55,   325,   326,    13,   237,   329,
      21,     1,   332,   422,    55,    83,    83,    53,    54,    86,
     459,   377,   342,    13,    83,   422,    85,   347,     5,   349,
     350,    53,    54,    78,   354,   355,    13,   357,    85,     1,
       1,     1,    13,   272,     5,     5,   366,   276,    78,   369,
      86,    12,    83,    17,   374,    15,    16,    17,    18,    19,
     380,   290,    52,   383,    86,   385,     1,    83,    84,    75,
     390,    87,    88,    89,    90,    91,   396,    13,    84,    13,
       1,   401,     1,    17,     5,     5,     5,     5,   317,    83,
      55,    12,     5,    13,    13,    13,     1,    83,    75,    85,
      20,    21,     1,    83,   424,    85,   335,    13,    77,    22,
      23,    24,    25,    87,    27,   435,   436,    78,    31,   137,
      83,   441,    85,    83,    84,    84,   446,    87,    88,    89,
      90,    91,    83,    52,    85,    83,   456,    85,    84,   368,
     460,   376,    83,   372,    85,    79,    80,    81,     0,     1,
     379,   265,   345,     5,    15,    16,    17,    18,    19,   479,
      12,    13,   482,    15,   341,    28,    29,    30,   488,    32,
      33,    34,    83,   493,    85,    83,    83,    85,    85,   408,
     393,   319,   454,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,   297,
      52,   331,    55,    38,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,   304,   216,
      82,    83,    84,    85,   123,    87,    88,    89,     0,     1,
      92,     0,     1,    95,    96,    97,   302,   358,   443,   252,
      12,    13,    60,    15,    13,   384,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,   100,
     203,   422,    31,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,   137,
      52,   137,    51,    -1,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    -1,    -1,
      82,    83,    84,    85,     0,     1,    88,    89,    -1,     5,
      92,    -1,     5,    95,    96,    97,    12,    13,    -1,    15,
      13,    -1,    15,    16,    17,    18,    19,    20,    21,    13,
      -1,    15,    16,    17,    18,    19,    20,    21,    -1,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    -1,    -1,    -1,    -1,    -1,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    -1,    -1,    82,    83,    84,    85,
      -1,     1,    -1,    89,    -1,     5,    92,    -1,    -1,    -1,
      96,    97,    12,    13,    13,    15,    15,    16,    17,    18,
      19,    20,    -1,     1,    -1,    -1,    -1,     5,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    15,    16,    17,
      18,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      -1,    -1,    82,    83,    -1,     0,     1,    87,    88,    89,
       1,    -1,    92,    -1,    -1,    95,    96,    97,    -1,    -1,
      -1,    12,    13,    -1,    15,    83,    84,    -1,    -1,    87,
      88,    89,    90,    91,    -1,    -1,    -1,    -1,    -1,    -1,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    84,
      -1,    82,    83,    84,    -1,    -1,     1,    88,    89,    -1,
      -1,    92,    -1,    -1,    95,    96,    97,    12,    13,    -1,
      15,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    -1,    -1,    82,    83,    -1,
      -1,     0,    87,    88,    89,     1,    -1,    92,    -1,     5,
      95,    96,    97,    -1,    -1,    -1,    12,    13,    -1,    15,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,     5,    84,    82,    -1,    -1,    -1,
      -1,    12,    13,    89,    15,    -1,    92,    -1,    -1,    -1,
      96,    97,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    12,
      13,    82,    15,    -1,    -1,    -1,    -1,    -1,    89,    -1,
      -1,    92,    -1,    -1,    -1,    96,    97,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,     0,     1,    82,
      -1,    -1,     5,    -1,    -1,    -1,    89,    -1,    -1,    92,
      13,    -1,    -1,    96,    97,     0,     1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    -1,    52,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,     0,     1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    13,
      83,    84,    85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    83,    84,
      85,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    -1,    52,    -1,
       0,     1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    13,    -1,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    -1,    27,    -1,    83,
      84,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    84
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    28,    29,    30,    32,    33,    34,    99,   100,   101,
     102,   103,   104,   105,   106,    83,    83,    83,    83,     5,
      83,     0,   106,   106,   106,   106,   106,   106,     5,    26,
     108,   110,   106,   106,   149,   106,   106,   106,   106,     1,
     106,   107,    22,    23,    24,    25,    27,    31,   113,   118,
     119,   131,   133,   139,   145,   150,     1,    13,    52,   169,
     170,   171,   172,    13,    75,   136,   137,   138,    12,    13,
      15,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    82,    89,    92,    96,    97,
     148,   174,   176,   177,   178,    53,    54,    86,   124,   125,
     127,   128,    13,    15,    16,    17,    18,    19,    20,    21,
     153,   154,   155,   156,   157,   159,   160,   161,   162,   164,
     168,    83,    85,   182,    12,     5,     6,   112,     1,   106,
       1,   106,   106,     1,   106,     1,   106,   106,   106,   152,
     153,    85,   182,   183,   106,   106,    84,     1,   171,   183,
      85,   183,     1,    17,   182,   106,   106,   106,   106,   106,
     106,   106,   106,   106,   106,   106,   106,   106,   106,   106,
     106,   106,   106,   106,   106,   106,   106,   106,   106,   106,
     106,   106,   106,   106,   106,   106,   106,    74,   177,     1,
      84,    88,    95,   175,   183,   106,   106,   106,    84,    21,
      13,    17,    79,    80,    81,    13,   106,    21,     1,    84,
      88,   157,     1,     5,    89,    90,    91,   146,    13,    20,
     159,   160,   160,     1,   161,     1,   106,     1,    26,    51,
     111,   113,   114,   180,   181,    85,   182,    12,    78,   121,
      85,   182,    13,    17,   140,   167,    83,   128,   130,    85,
     182,    83,    85,   182,    13,   120,    12,    13,   134,    84,
     151,   106,     1,    85,   183,    85,     1,   106,    85,   106,
     106,    84,    83,    88,     1,   174,   106,   106,   106,   176,
       1,    13,   122,    55,    13,   132,    13,   106,   106,   106,
      13,    20,   156,   163,   106,   106,   106,   106,   157,   160,
       0,    84,   109,   183,    85,   182,   106,     1,   107,   106,
     167,    13,   106,   106,     1,    83,    88,   106,   106,   121,
     106,    83,    85,   182,   106,     1,   106,   106,     1,   174,
     106,   106,    87,   106,   106,   106,   106,    55,   126,    13,
      14,   147,   148,   157,   158,   157,   106,    13,   163,     7,
       8,     9,    10,    11,    93,    94,   165,   155,     1,    12,
      85,   182,   129,   130,    83,   115,   106,   106,   169,   106,
      83,   149,   106,    85,   174,    52,   173,   169,   138,   106,
      17,   123,   124,   106,   106,   106,    77,     1,   106,   106,
      93,   165,   106,   109,   106,    85,   182,   106,     1,   109,
     116,   117,   118,   119,   131,   133,   139,   145,   152,   179,
     180,   181,   115,   128,    84,    85,   182,   106,   169,   106,
     173,   106,     1,    84,   106,    87,   125,    87,    87,   106,
      87,    88,    87,   106,    12,    13,   166,    85,   141,   169,
     109,   107,   179,   106,   135,   109,   174,   106,    87,   106,
     166,   106,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,   109,   142,
     144,    84,   136,   106,   157,   106,    93,   106,   143,   106,
      93,   169,   106,    83,   106,   169,   109
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (&yylval, YYLEX_PARAM)
#else
# define YYLEX yylex (&yylval)
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}

/* Prevent warnings from -Wmissing-prototypes.  */
#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */





/*-------------------------.
| yyparse or yypush_parse.  |
`-------------------------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

    /* Number of syntax errors so far.  */
    int yynerrs;

    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks thru separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */
  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 9:

/* Line 1455 of yacc.c  */
#line 291 "Source/WebCore/css/CSSGrammar.y"
    {
        static_cast<CSSParser*>(parser)->m_rule = (yyvsp[(4) - (6)].rule);
    ;}
    break;

  case 10:

/* Line 1455 of yacc.c  */
#line 297 "Source/WebCore/css/CSSGrammar.y"
    {
        static_cast<CSSParser*>(parser)->m_keyframe = (yyvsp[(4) - (6)].keyframeRule);
    ;}
    break;

  case 11:

/* Line 1455 of yacc.c  */
#line 303 "Source/WebCore/css/CSSGrammar.y"
    {
        /* can be empty */
    ;}
    break;

  case 12:

/* Line 1455 of yacc.c  */
#line 309 "Source/WebCore/css/CSSGrammar.y"
    {
        CSSParser* p = static_cast<CSSParser*>(parser);
        if ((yyvsp[(4) - (5)].valueList)) {
            p->m_valueList = p->sinkFloatingValueList((yyvsp[(4) - (5)].valueList));
            int oldParsedProperties = p->m_numParsedProperties;
            if (!p->parseValue(p->m_id, p->m_important))
                p->rollbackLastProperties(p->m_numParsedProperties - oldParsedProperties);
            delete p->m_valueList;
            p->m_valueList = 0;
        }
    ;}
    break;

  case 13:

/* Line 1455 of yacc.c  */
#line 323 "Source/WebCore/css/CSSGrammar.y"
    {
         CSSParser* p = static_cast<CSSParser*>(parser);
         p->m_mediaQuery = p->sinkFloatingMediaQuery((yyvsp[(4) - (5)].mediaQuery));
     ;}
    break;

  case 14:

/* Line 1455 of yacc.c  */
#line 330 "Source/WebCore/css/CSSGrammar.y"
    {
        if ((yyvsp[(4) - (5)].selectorList)) {
            CSSParser* p = static_cast<CSSParser*>(parser);
            if (p->m_selectorListForParseSelector)
                p->m_selectorListForParseSelector->adoptSelectorVector(*(yyvsp[(4) - (5)].selectorList));
        }
    ;}
    break;

  case 21:

/* Line 1455 of yacc.c  */
#line 352 "Source/WebCore/css/CSSGrammar.y"
    {
  ;}
    break;

  case 24:

/* Line 1455 of yacc.c  */
#line 362 "Source/WebCore/css/CSSGrammar.y"
    {
     CSSParser* p = static_cast<CSSParser*>(parser);
     (yyval.rule) = static_cast<CSSParser*>(parser)->createCharsetRule((yyvsp[(3) - (5)].string));
     if ((yyval.rule) && p->m_styleSheet)
         p->m_styleSheet->append((yyval.rule));
  ;}
    break;

  case 25:

/* Line 1455 of yacc.c  */
#line 368 "Source/WebCore/css/CSSGrammar.y"
    {
  ;}
    break;

  case 26:

/* Line 1455 of yacc.c  */
#line 370 "Source/WebCore/css/CSSGrammar.y"
    {
  ;}
    break;

  case 27:

/* Line 1455 of yacc.c  */
#line 375 "Source/WebCore/css/CSSGrammar.y"
    {
        // Ignore any @charset rule not at the beginning of the style sheet.
        (yyval.rule) = 0;
    ;}
    break;

  case 29:

/* Line 1455 of yacc.c  */
#line 383 "Source/WebCore/css/CSSGrammar.y"
    {
     CSSParser* p = static_cast<CSSParser*>(parser);
     if ((yyvsp[(2) - (3)].rule) && p->m_styleSheet)
         p->m_styleSheet->append((yyvsp[(2) - (3)].rule));
 ;}
    break;

  case 30:

/* Line 1455 of yacc.c  */
#line 391 "Source/WebCore/css/CSSGrammar.y"
    {
        (yyval.rule) = (yyvsp[(2) - (2)].rule);
    ;}
    break;

  case 37:

/* Line 1455 of yacc.c  */
#line 403 "Source/WebCore/css/CSSGrammar.y"
    {
        static_cast<CSSParser*>(parser)->m_hadSyntacticallyValidCSSRule = true;
    ;}
    break;

  case 41:

/* Line 1455 of yacc.c  */
#line 412 "Source/WebCore/css/CSSGrammar.y"
    { (yyval.ruleList) = 0; ;}
    break;

  case 42:

/* Line 1455 of yacc.c  */
#line 413 "Source/WebCore/css/CSSGrammar.y"
    {
      (yyval.ruleList) = (yyvsp[(1) - (3)].ruleList);
      if ((yyvsp[(2) - (3)].rule)) {
          if (!(yyval.ruleList))
              (yyval.ruleList) = static_cast<CSSParser*>(parser)->createRuleList();
          (yyval.ruleList)->append((yyvsp[(2) - (3)].rule));
      }
  ;}
    break;

  case 53:

/* Line 1455 of yacc.c  */
#line 441 "Source/WebCore/css/CSSGrammar.y"
    {
        (yyval.rule) = static_cast<CSSParser*>(parser)->createImportRule((yyvsp[(3) - (6)].string), (yyvsp[(5) - (6)].mediaList));
    ;}
    break;

  case 54:

/* Line 1455 of yacc.c  */
#line 444 "Source/WebCore/css/CSSGrammar.y"
    {
        (yyval.rule) = 0;
    ;}
    break;

  case 55:

/* Line 1455 of yacc.c  */
#line 447 "Source/WebCore/css/CSSGrammar.y"
    {
        (yyval.rule) = 0;
    ;}
    break;

  case 56:

/* Line 1455 of yacc.c  */
#line 450 "Source/WebCore/css/CSSGrammar.y"
    {
        (yyval.rule) = 0;
    ;}
    break;

  case 57:

/* Line 1455 of yacc.c  */
#line 456 "Source/WebCore/css/CSSGrammar.y"
    {
    static_cast<CSSParser*>(parser)->addNamespace((yyvsp[(3) - (6)].string), (yyvsp[(4) - (6)].string));
    (yyval.rule) = 0;
;}
    break;

  case 58:

/* Line 1455 of yacc.c  */
#line 460 "Source/WebCore/css/CSSGrammar.y"
    {
    (yyval.rule) = 0;
;}
    break;

  case 59:

/* Line 1455 of yacc.c  */
#line 463 "Source/WebCore/css/CSSGrammar.y"
    {
    (yyval.rule) = 0;
;}
    break;

  case 60:

/* Line 1455 of yacc.c  */
#line 466 "Source/WebCore/css/CSSGrammar.y"
    {
    (yyval.rule) = 0;
;}
    break;

  case 61:

/* Line 1455 of yacc.c  */
#line 472 "Source/WebCore/css/CSSGrammar.y"
    { (yyval.string).characters = 0; ;}
    break;

  case 62:

/* Line 1455 of yacc.c  */
#line 473 "Source/WebCore/css/CSSGrammar.y"
    { (yyval.string) = (yyvsp[(1) - (2)].string); ;}
    break;

  case 65:

/* Line 1455 of yacc.c  */
#line 482 "Source/WebCore/css/CSSGrammar.y"
    {
        (yyval.string) = (yyvsp[(1) - (2)].string);
    ;}
    break;

  case 66:

/* Line 1455 of yacc.c  */
#line 488 "Source/WebCore/css/CSSGrammar.y"
    {
        (yyval.valueList) = 0;
    ;}
    break;

  case 67:

/* Line 1455 of yacc.c  */
#line 491 "Source/WebCore/css/CSSGrammar.y"
    {
        (yyval.valueList) = (yyvsp[(3) - (4)].valueList);
    ;}
    break;

  case 68:

/* Line 1455 of yacc.c  */
#line 497 "Source/WebCore/css/CSSGrammar.y"
    {
        (yyvsp[(3) - (7)].string).lower();
        (yyval.mediaQueryExp) = static_cast<CSSParser*>(parser)->createFloatingMediaQueryExp((yyvsp[(3) - (7)].string), (yyvsp[(5) - (7)].valueList));
    ;}
    break;

  case 69:

/* Line 1455 of yacc.c  */
#line 504 "Source/WebCore/css/CSSGrammar.y"
    {
        CSSParser* p = static_cast<CSSParser*>(parser);
        (yyval.mediaQueryExpList) = p->createFloatingMediaQueryExpList();
        (yyval.mediaQueryExpList)->append(p->sinkFloatingMediaQueryExp((yyvsp[(1) - (1)].mediaQueryExp)));
    ;}
    break;

  case 70:

/* Line 1455 of yacc.c  */
#line 509 "Source/WebCore/css/CSSGrammar.y"
    {
        (yyval.mediaQueryExpList) = (yyvsp[(1) - (5)].mediaQueryExpList);
        (yyval.mediaQueryExpList)->append(static_cast<CSSParser*>(parser)->sinkFloatingMediaQueryExp((yyvsp[(5) - (5)].mediaQueryExp)));
    ;}
    break;

  case 71:

/* Line 1455 of yacc.c  */
#line 516 "Source/WebCore/css/CSSGrammar.y"
    {
        (yyval.mediaQueryExpList) = static_cast<CSSParser*>(parser)->createFloatingMediaQueryExpList();
    ;}
    break;

  case 72:

/* Line 1455 of yacc.c  */
#line 519 "Source/WebCore/css/CSSGrammar.y"
    {
        (yyval.mediaQueryExpList) = (yyvsp[(3) - (3)].mediaQueryExpList);
    ;}
    break;

  case 73:

/* Line 1455 of yacc.c  */
#line 525 "Source/WebCore/css/CSSGrammar.y"
    {
        (yyval.mediaQueryRestrictor) = MediaQuery::None;
    ;}
    break;

  case 74:

/* Line 1455 of yacc.c  */
#line 528 "Source/WebCore/css/CSSGrammar.y"
    {
        (yyval.mediaQueryRestrictor) = MediaQuery::Only;
    ;}
    break;

  case 75:

/* Line 1455 of yacc.c  */
#line 531 "Source/WebCore/css/CSSGrammar.y"
    {
        (yyval.mediaQueryRestrictor) = MediaQuery::Not;
    ;}
    break;

  case 76:

/* Line 1455 of yacc.c  */
#line 537 "Source/WebCore/css/CSSGrammar.y"
    {
        CSSParser* p = static_cast<CSSParser*>(parser);
        (yyval.mediaQuery) = p->createFloatingMediaQuery(p->sinkFloatingMediaQueryExpList((yyvsp[(1) - (1)].mediaQueryExpList)));
    ;}
    break;

  case 77:

/* Line 1455 of yacc.c  */
#line 542 "Source/WebCore/css/CSSGrammar.y"
    {
        CSSParser* p = static_cast<CSSParser*>(parser);
        (yyvsp[(3) - (4)].string).lower();
        (yyval.mediaQuery) = p->createFloatingMediaQuery((yyvsp[(1) - (4)].mediaQueryRestrictor), (yyvsp[(3) - (4)].string), p->sinkFloatingMediaQueryExpList((yyvsp[(4) - (4)].mediaQueryExpList)));
    ;}
    break;

  case 78:

/* Line 1455 of yacc.c  */
#line 550 "Source/WebCore/css/CSSGrammar.y"
    {
        (yyval.mediaList) = static_cast<CSSParser*>(parser)->createMediaList();
     ;}
    break;

  case 80:

/* Line 1455 of yacc.c  */
#line 557 "Source/WebCore/css/CSSGrammar.y"
    {
        CSSParser* p = static_cast<CSSParser*>(parser);
        (yyval.mediaList) = p->createMediaList();
        (yyval.mediaList)->appendMediaQuery(p->sinkFloatingMediaQuery((yyvsp[(1) - (1)].mediaQuery)));
    ;}
    break;

  case 81:

/* Line 1455 of yacc.c  */
#line 562 "Source/WebCore/css/CSSGrammar.y"
    {
        (yyval.mediaList) = (yyvsp[(1) - (4)].mediaList);
        if ((yyval.mediaList))
            (yyval.mediaList)->appendMediaQuery(static_cast<CSSParser*>(parser)->sinkFloatingMediaQuery((yyvsp[(4) - (4)].mediaQuery)));
    ;}
    break;

  case 82:

/* Line 1455 of yacc.c  */
#line 567 "Source/WebCore/css/CSSGrammar.y"
    {
        (yyval.mediaList) = 0;
    ;}
    break;

  case 83:

/* Line 1455 of yacc.c  */
#line 573 "Source/WebCore/css/CSSGrammar.y"
    {
        (yyval.rule) = static_cast<CSSParser*>(parser)->createMediaRule((yyvsp[(3) - (7)].mediaList), (yyvsp[(6) - (7)].ruleList));
    ;}
    break;

  case 84:

/* Line 1455 of yacc.c  */
#line 576 "Source/WebCore/css/CSSGrammar.y"
    {
        (yyval.rule) = static_cast<CSSParser*>(parser)->createMediaRule(0, (yyvsp[(5) - (6)].ruleList));
    ;}
    break;

  case 85:

/* Line 1455 of yacc.c  */
#line 582 "Source/WebCore/css/CSSGrammar.y"
    {
      (yyval.string) = (yyvsp[(1) - (2)].string);
  ;}
    break;

  case 86:

/* Line 1455 of yacc.c  */
#line 588 "Source/WebCore/css/CSSGrammar.y"
    {
        (yyval.rule) = (yyvsp[(7) - (8)].keyframesRule);
        (yyvsp[(7) - (8)].keyframesRule)->setNameInternal((yyvsp[(3) - (8)].string));
    ;}
    break;

  case 89:

/* Line 1455 of yacc.c  */
#line 600 "Source/WebCore/css/CSSGrammar.y"
    { (yyval.keyframesRule) = static_cast<CSSParser*>(parser)->createKeyframesRule(); ;}
    break;

  case 90:

/* Line 1455 of yacc.c  */
#line 601 "Source/WebCore/css/CSSGrammar.y"
    {
        (yyval.keyframesRule) = (yyvsp[(1) - (3)].keyframesRule);
        if ((yyvsp[(2) - (3)].keyframeRule))
            (yyval.keyframesRule)->append((yyvsp[(2) - (3)].keyframeRule));
    ;}
    break;

  case 91:

/* Line 1455 of yacc.c  */
#line 609 "Source/WebCore/css/CSSGrammar.y"
    {
        (yyval.keyframeRule) = static_cast<CSSParser*>(parser)->createKeyframeRule((yyvsp[(1) - (6)].valueList));
    ;}
    break;

  case 92:

/* Line 1455 of yacc.c  */
#line 615 "Source/WebCore/css/CSSGrammar.y"
    {
        CSSParser* p = static_cast<CSSParser*>(parser);
        (yyval.valueList) = p->createFloatingValueList();
        (yyval.valueList)->addValue(p->sinkFloatingValue((yyvsp[(1) - (1)].value)));
    ;}
    break;

  case 93:

/* Line 1455 of yacc.c  */
#line 620 "Source/WebCore/css/CSSGrammar.y"
    {
        CSSParser* p = static_cast<CSSParser*>(parser);
        (yyval.valueList) = (yyvsp[(1) - (5)].valueList);
        if ((yyval.valueList))
            (yyval.valueList)->addValue(p->sinkFloatingValue((yyvsp[(5) - (5)].value)));
    ;}
    break;

  case 94:

/* Line 1455 of yacc.c  */
#line 629 "Source/WebCore/css/CSSGrammar.y"
    { (yyval.value).id = 0; (yyval.value).isInt = false; (yyval.value).fValue = (yyvsp[(1) - (1)].number); (yyval.value).unit = CSSPrimitiveValue::CSS_NUMBER; ;}
    break;

  case 95:

/* Line 1455 of yacc.c  */
#line 630 "Source/WebCore/css/CSSGrammar.y"
    {
        (yyval.value).id = 0; (yyval.value).isInt = false; (yyval.value).unit = CSSPrimitiveValue::CSS_NUMBER;
        CSSParserString& str = (yyvsp[(1) - (1)].string);
        if (equalIgnoringCase("from", str.characters, str.length))
            (yyval.value).fValue = 0;
        else if (equalIgnoringCase("to", str.characters, str.length))
            (yyval.value).fValue = 100;
        else
            YYERROR;
    ;}
    break;

  case 96:

/* Line 1455 of yacc.c  */
#line 644 "Source/WebCore/css/CSSGrammar.y"
    {
        CSSParser* p = static_cast<CSSParser*>(parser);
        if ((yyvsp[(3) - (8)].selector))
            (yyval.rule) = p->createPageRule(p->sinkFloatingSelector((yyvsp[(3) - (8)].selector)));
        else {
            // Clear properties in the invalid @page rule.
            p->clearProperties();
            // Also clear margin at-rules here once we fully implement margin at-rules parsing.
            (yyval.rule) = 0;
        }
    ;}
    break;

  case 97:

/* Line 1455 of yacc.c  */
#line 655 "Source/WebCore/css/CSSGrammar.y"
    {
      (yyval.rule) = 0;
    ;}
    break;

  case 98:

/* Line 1455 of yacc.c  */
#line 658 "Source/WebCore/css/CSSGrammar.y"
    {
      (yyval.rule) = 0;
    ;}
    break;

  case 99:

/* Line 1455 of yacc.c  */
#line 664 "Source/WebCore/css/CSSGrammar.y"
    {
        CSSParser* p = static_cast<CSSParser*>(parser);
        (yyval.selector) = p->createFloatingSelector();
        (yyval.selector)->setTag(QualifiedName(nullAtom, (yyvsp[(1) - (1)].string), p->m_defaultNamespace));
        (yyval.selector)->setForPage();
    ;}
    break;

  case 100:

/* Line 1455 of yacc.c  */
#line 670 "Source/WebCore/css/CSSGrammar.y"
    {
        CSSParser* p = static_cast<CSSParser*>(parser);
        (yyval.selector) = (yyvsp[(2) - (2)].selector);
        if ((yyval.selector)) {
            (yyval.selector)->setTag(QualifiedName(nullAtom, (yyvsp[(1) - (2)].string), p->m_defaultNamespace));
            (yyval.selector)->setForPage();
        }
    ;}
    break;

  case 101:

/* Line 1455 of yacc.c  */
#line 678 "Source/WebCore/css/CSSGrammar.y"
    {
        (yyval.selector) = (yyvsp[(1) - (1)].selector);
        if ((yyval.selector))
            (yyval.selector)->setForPage();
    ;}
    break;

  case 102:

/* Line 1455 of yacc.c  */
#line 683 "Source/WebCore/css/CSSGrammar.y"
    {
        CSSParser* p = static_cast<CSSParser*>(parser);
        (yyval.selector) = p->createFloatingSelector();
        (yyval.selector)->setForPage();
    ;}
    break;

  case 105:

/* Line 1455 of yacc.c  */
#line 696 "Source/WebCore/css/CSSGrammar.y"
    {
        static_cast<CSSParser*>(parser)->startDeclarationsForMarginBox();
    ;}
    break;

  case 106:

/* Line 1455 of yacc.c  */
#line 698 "Source/WebCore/css/CSSGrammar.y"
    {
        (yyval.rule) = static_cast<CSSParser*>(parser)->createMarginAtRule((yyvsp[(1) - (7)].marginBox));
    ;}
    break;

  case 107:

/* Line 1455 of yacc.c  */
#line 704 "Source/WebCore/css/CSSGrammar.y"
    {
        (yyval.marginBox) = CSSSelector::TopLeftCornerMarginBox;
    ;}
    break;

  case 108:

/* Line 1455 of yacc.c  */
#line 707 "Source/WebCore/css/CSSGrammar.y"
    {
        (yyval.marginBox) = CSSSelector::TopLeftMarginBox;
    ;}
    break;

  case 109:

/* Line 1455 of yacc.c  */
#line 710 "Source/WebCore/css/CSSGrammar.y"
    {
        (yyval.marginBox) = CSSSelector::TopCenterMarginBox;
    ;}
    break;

  case 110:

/* Line 1455 of yacc.c  */
#line 713 "Source/WebCore/css/CSSGrammar.y"
    {
        (yyval.marginBox) = CSSSelector::TopRightMarginBox;
    ;}
    break;

  case 111:

/* Line 1455 of yacc.c  */
#line 716 "Source/WebCore/css/CSSGrammar.y"
    {
        (yyval.marginBox) = CSSSelector::TopRightCornerMarginBox;
    ;}
    break;

  case 112:

/* Line 1455 of yacc.c  */
#line 719 "Source/WebCore/css/CSSGrammar.y"
    {
        (yyval.marginBox) = CSSSelector::BottomLeftCornerMarginBox;
    ;}
    break;

  case 113:

/* Line 1455 of yacc.c  */
#line 722 "Source/WebCore/css/CSSGrammar.y"
    {
        (yyval.marginBox) = CSSSelector::BottomLeftMarginBox;
    ;}
    break;

  case 114:

/* Line 1455 of yacc.c  */
#line 725 "Source/WebCore/css/CSSGrammar.y"
    {
        (yyval.marginBox) = CSSSelector::BottomCenterMarginBox;
    ;}
    break;

  case 115:

/* Line 1455 of yacc.c  */
#line 728 "Source/WebCore/css/CSSGrammar.y"
    {
        (yyval.marginBox) = CSSSelector::BottomRightMarginBox;
    ;}
    break;

  case 116:

/* Line 1455 of yacc.c  */
#line 731 "Source/WebCore/css/CSSGrammar.y"
    {
        (yyval.marginBox) = CSSSelector::BottomRightCornerMarginBox;
    ;}
    break;

  case 117:

/* Line 1455 of yacc.c  */
#line 734 "Source/WebCore/css/CSSGrammar.y"
    {
        (yyval.marginBox) = CSSSelector::LeftTopMarginBox;
    ;}
    break;

  case 118:

/* Line 1455 of yacc.c  */
#line 737 "Source/WebCore/css/CSSGrammar.y"
    {
        (yyval.marginBox) = CSSSelector::LeftMiddleMarginBox;
    ;}
    break;

  case 119:

/* Line 1455 of yacc.c  */
#line 740 "Source/WebCore/css/CSSGrammar.y"
    {
        (yyval.marginBox) = CSSSelector::LeftBottomMarginBox;
    ;}
    break;

  case 120:

/* Line 1455 of yacc.c  */
#line 743 "Source/WebCore/css/CSSGrammar.y"
    {
        (yyval.marginBox) = CSSSelector::RightTopMarginBox;
    ;}
    break;

  case 121:

/* Line 1455 of yacc.c  */
#line 746 "Source/WebCore/css/CSSGrammar.y"
    {
        (yyval.marginBox) = CSSSelector::RightMiddleMarginBox;
    ;}
    break;

  case 122:

/* Line 1455 of yacc.c  */
#line 749 "Source/WebCore/css/CSSGrammar.y"
    {
        (yyval.marginBox) = CSSSelector::RightBottomMarginBox;
    ;}
    break;

  case 123:

/* Line 1455 of yacc.c  */
#line 756 "Source/WebCore/css/CSSGrammar.y"
    {
        (yyval.rule) = static_cast<CSSParser*>(parser)->createFontFaceRule();
    ;}
    break;

  case 124:

/* Line 1455 of yacc.c  */
#line 759 "Source/WebCore/css/CSSGrammar.y"
    {
      (yyval.rule) = 0;
    ;}
    break;

  case 125:

/* Line 1455 of yacc.c  */
#line 762 "Source/WebCore/css/CSSGrammar.y"
    {
      (yyval.rule) = 0;
    ;}
    break;

  case 126:

/* Line 1455 of yacc.c  */
#line 768 "Source/WebCore/css/CSSGrammar.y"
    { (yyval.relation) = CSSSelector::DirectAdjacent; ;}
    break;

  case 127:

/* Line 1455 of yacc.c  */
#line 769 "Source/WebCore/css/CSSGrammar.y"
    { (yyval.relation) = CSSSelector::IndirectAdjacent; ;}
    break;

  case 128:

/* Line 1455 of yacc.c  */
#line 770 "Source/WebCore/css/CSSGrammar.y"
    { (yyval.relation) = CSSSelector::Child; ;}
    break;

  case 129:

/* Line 1455 of yacc.c  */
#line 774 "Source/WebCore/css/CSSGrammar.y"
    { (yyval.integer) = (yyvsp[(1) - (1)].integer); ;}
    break;

  case 130:

/* Line 1455 of yacc.c  */
#line 775 "Source/WebCore/css/CSSGrammar.y"
    { (yyval.integer) = 1; ;}
    break;

  case 131:

/* Line 1455 of yacc.c  */
#line 779 "Source/WebCore/css/CSSGrammar.y"
    { (yyval.integer) = -1; ;}
    break;

  case 132:

/* Line 1455 of yacc.c  */
#line 780 "Source/WebCore/css/CSSGrammar.y"
    { (yyval.integer) = 1; ;}
    break;

  case 133:

/* Line 1455 of yacc.c  */
#line 784 "Source/WebCore/css/CSSGrammar.y"
    {
        CSSParser* p = static_cast<CSSParser*>(parser);
        p->markPropertyStart();
    ;}
    break;

  case 134:

/* Line 1455 of yacc.c  */
#line 791 "Source/WebCore/css/CSSGrammar.y"
    {
        CSSParser* p = static_cast<CSSParser*>(parser);
        p->markSelectorListStart();
    ;}
    break;

  case 135:

/* Line 1455 of yacc.c  */
#line 798 "Source/WebCore/css/CSSGrammar.y"
    {
        CSSParser* p = static_cast<CSSParser*>(parser);
        p->markSelectorListEnd();
    ;}
    break;

  case 136:

/* Line 1455 of yacc.c  */
#line 805 "Source/WebCore/css/CSSGrammar.y"
    {
        CSSParser* p = static_cast<CSSParser*>(parser);
        (yyval.rule) = p->createStyleRule((yyvsp[(1) - (6)].selectorList));
    ;}
    break;

  case 137:

/* Line 1455 of yacc.c  */
#line 812 "Source/WebCore/css/CSSGrammar.y"
    {
        if ((yyvsp[(1) - (1)].selector)) {
            CSSParser* p = static_cast<CSSParser*>(parser);
            (yyval.selectorList) = p->reusableSelectorVector();
            (yyval.selectorList)->shrink(0);
            (yyval.selectorList)->append(p->sinkFloatingSelector((yyvsp[(1) - (1)].selector)));
            p->updateLastSelectorLineAndPosition();
        }
    ;}
    break;

  case 138:

/* Line 1455 of yacc.c  */
#line 821 "Source/WebCore/css/CSSGrammar.y"
    {
        if ((yyvsp[(1) - (4)].selectorList) && (yyvsp[(4) - (4)].selector)) {
            CSSParser* p = static_cast<CSSParser*>(parser);
            (yyval.selectorList) = (yyvsp[(1) - (4)].selectorList);
            (yyval.selectorList)->append(p->sinkFloatingSelector((yyvsp[(4) - (4)].selector)));
            p->updateLastSelectorLineAndPosition();
        } else
            (yyval.selectorList) = 0;
    ;}
    break;

  case 139:

/* Line 1455 of yacc.c  */
#line 830 "Source/WebCore/css/CSSGrammar.y"
    {
        (yyval.selectorList) = 0;
    ;}
    break;

  case 140:

/* Line 1455 of yacc.c  */
#line 836 "Source/WebCore/css/CSSGrammar.y"
    {
        (yyval.selector) = (yyvsp[(1) - (2)].selector);
    ;}
    break;

  case 141:

/* Line 1455 of yacc.c  */
#line 842 "Source/WebCore/css/CSSGrammar.y"
    {
        (yyval.selector) = (yyvsp[(1) - (1)].selector);
    ;}
    break;

  case 142:

/* Line 1455 of yacc.c  */
#line 846 "Source/WebCore/css/CSSGrammar.y"
    {
        (yyval.selector) = (yyvsp[(1) - (1)].selector);
    ;}
    break;

  case 143:

/* Line 1455 of yacc.c  */
#line 850 "Source/WebCore/css/CSSGrammar.y"
    {
        (yyval.selector) = (yyvsp[(2) - (2)].selector);
        if (!(yyvsp[(1) - (2)].selector))
            (yyval.selector) = 0;
        else if ((yyval.selector)) {
            CSSParser* p = static_cast<CSSParser*>(parser);
            CSSParserSelector* end = (yyval.selector);
            while (end->tagHistory())
                end = end->tagHistory();
            end->setRelation(CSSSelector::Descendant);
            end->setTagHistory(p->sinkFloatingSelector((yyvsp[(1) - (2)].selector)));
        }
    ;}
    break;

  case 144:

/* Line 1455 of yacc.c  */
#line 863 "Source/WebCore/css/CSSGrammar.y"
    {
        (yyval.selector) = (yyvsp[(3) - (3)].selector);
        if (!(yyvsp[(1) - (3)].selector))
            (yyval.selector) = 0;
        else if ((yyval.selector)) {
            CSSParser* p = static_cast<CSSParser*>(parser);
            CSSParserSelector* end = (yyval.selector);
            while (end->tagHistory())
                end = end->tagHistory();
            end->setRelation((yyvsp[(2) - (3)].relation));
            end->setTagHistory(p->sinkFloatingSelector((yyvsp[(1) - (3)].selector)));
        }
    ;}
    break;

  case 145:

/* Line 1455 of yacc.c  */
#line 876 "Source/WebCore/css/CSSGrammar.y"
    {
        (yyval.selector) = 0;
    ;}
    break;

  case 146:

/* Line 1455 of yacc.c  */
#line 882 "Source/WebCore/css/CSSGrammar.y"
    { (yyval.string).characters = 0; (yyval.string).length = 0; ;}
    break;

  case 147:

/* Line 1455 of yacc.c  */
#line 883 "Source/WebCore/css/CSSGrammar.y"
    { static UChar star = '*'; (yyval.string).characters = &star; (yyval.string).length = 1; ;}
    break;

  case 148:

/* Line 1455 of yacc.c  */
#line 884 "Source/WebCore/css/CSSGrammar.y"
    { (yyval.string) = (yyvsp[(1) - (2)].string); ;}
    break;

  case 149:

/* Line 1455 of yacc.c  */
#line 888 "Source/WebCore/css/CSSGrammar.y"
    {
        CSSParser* p = static_cast<CSSParser*>(parser);
        (yyval.selector) = p->createFloatingSelector();
        (yyval.selector)->setTag(QualifiedName(nullAtom, (yyvsp[(1) - (1)].string), p->m_defaultNamespace));
    ;}
    break;

  case 150:

/* Line 1455 of yacc.c  */
#line 893 "Source/WebCore/css/CSSGrammar.y"
    {
        (yyval.selector) = (yyvsp[(2) - (2)].selector);
        if ((yyval.selector))
            static_cast<CSSParser*>(parser)->updateSpecifiersWithElementName(nullAtom, (yyvsp[(1) - (2)].string), (yyval.selector));
    ;}
    break;

  case 151:

/* Line 1455 of yacc.c  */
#line 898 "Source/WebCore/css/CSSGrammar.y"
    {
        (yyval.selector) = (yyvsp[(1) - (1)].selector);
        if ((yyval.selector))
            static_cast<CSSParser*>(parser)->updateSpecifiersWithElementName(nullAtom, starAtom, (yyval.selector));
    ;}
    break;

  case 152:

/* Line 1455 of yacc.c  */
#line 903 "Source/WebCore/css/CSSGrammar.y"
    {
        AtomicString namespacePrefix = (yyvsp[(1) - (2)].string);
        CSSParser* p = static_cast<CSSParser*>(parser);
        (yyval.selector) = p->createFloatingSelector();
        if (p->m_styleSheet)
            (yyval.selector)->setTag(QualifiedName(namespacePrefix, (yyvsp[(2) - (2)].string),
                                      p->m_styleSheet->determineNamespace(namespacePrefix)));
        else // FIXME: Shouldn't this case be an error?
            (yyval.selector)->setTag(QualifiedName(nullAtom, (yyvsp[(2) - (2)].string), p->m_defaultNamespace));
    ;}
    break;

  case 153:

/* Line 1455 of yacc.c  */
#line 913 "Source/WebCore/css/CSSGrammar.y"
    {
        (yyval.selector) = (yyvsp[(3) - (3)].selector);
        if ((yyval.selector))
            static_cast<CSSParser*>(parser)->updateSpecifiersWithElementName((yyvsp[(1) - (3)].string), (yyvsp[(2) - (3)].string), (yyval.selector));
    ;}
    break;

  case 154:

/* Line 1455 of yacc.c  */
#line 918 "Source/WebCore/css/CSSGrammar.y"
    {
        (yyval.selector) = (yyvsp[(2) - (2)].selector);
        if ((yyval.selector))
            static_cast<CSSParser*>(parser)->updateSpecifiersWithElementName((yyvsp[(1) - (2)].string), starAtom, (yyval.selector));
    ;}
    break;

  case 155:

/* Line 1455 of yacc.c  */
#line 926 "Source/WebCore/css/CSSGrammar.y"
    {
        if ((yyvsp[(1) - (1)].selector)) {
            CSSParser* p = static_cast<CSSParser*>(parser);
            (yyval.selectorList) = p->createFloatingSelectorVector();
            (yyval.selectorList)->append(p->sinkFloatingSelector((yyvsp[(1) - (1)].selector)));
        } else
            (yyval.selectorList) = 0
    ;}
    break;

  case 156:

/* Line 1455 of yacc.c  */
#line 934 "Source/WebCore/css/CSSGrammar.y"
    {
        if ((yyvsp[(1) - (5)].selectorList) && (yyvsp[(5) - (5)].selector)) {
            CSSParser* p = static_cast<CSSParser*>(parser);
            (yyval.selectorList) = (yyvsp[(1) - (5)].selectorList);
            (yyval.selectorList)->append(p->sinkFloatingSelector((yyvsp[(5) - (5)].selector)));
        } else
            (yyval.selectorList) = 0;
    ;}
    break;

  case 157:

/* Line 1455 of yacc.c  */
#line 942 "Source/WebCore/css/CSSGrammar.y"
    {
        (yyval.selectorList) = 0;
    ;}
    break;

  case 158:

/* Line 1455 of yacc.c  */
#line 948 "Source/WebCore/css/CSSGrammar.y"
    {
        CSSParserString& str = (yyvsp[(1) - (1)].string);
        CSSParser* p = static_cast<CSSParser*>(parser);
        Document* doc = p->document();
        if (doc && doc->isHTMLDocument())
            str.lower();
        (yyval.string) = str;
    ;}
    break;

  case 159:

/* Line 1455 of yacc.c  */
#line 956 "Source/WebCore/css/CSSGrammar.y"
    {
        static UChar star = '*';
        (yyval.string).characters = &star;
        (yyval.string).length = 1;
    ;}
    break;

  case 160:

/* Line 1455 of yacc.c  */
#line 964 "Source/WebCore/css/CSSGrammar.y"
    {
        (yyval.selector) = (yyvsp[(1) - (1)].selector);
    ;}
    break;

  case 161:

/* Line 1455 of yacc.c  */
#line 967 "Source/WebCore/css/CSSGrammar.y"
    {
        if (!(yyvsp[(2) - (2)].selector))
            (yyval.selector) = 0;
        else if ((yyvsp[(1) - (2)].selector)) {
            CSSParser* p = static_cast<CSSParser*>(parser);
            CSSParserSelector* end;
            CSSParserSelector* history;
            // Ensure that unknown pseudo element always stays at the top of selector chain.
            if ((yyvsp[(2) - (2)].selector)->isUnknownPseudoElement()) {
                end = (yyvsp[(2) - (2)].selector);
                history = (yyvsp[(1) - (2)].selector);
            } else {
                end = (yyvsp[(1) - (2)].selector);
                history = (yyvsp[(2) - (2)].selector);
            }
            (yyval.selector) = end;
            while(end->tagHistory())
                end = end->tagHistory();
            end->setRelation(CSSSelector::SubSelector);
            end->setTagHistory(p->sinkFloatingSelector(history));
        }
    ;}
    break;

  case 162:

/* Line 1455 of yacc.c  */
#line 989 "Source/WebCore/css/CSSGrammar.y"
    {
        (yyval.selector) = 0;
    ;}
    break;

  case 163:

/* Line 1455 of yacc.c  */
#line 995 "Source/WebCore/css/CSSGrammar.y"
    {
        CSSParser* p = static_cast<CSSParser*>(parser);
        (yyval.selector) = p->createFloatingSelector();
        (yyval.selector)->setMatch(CSSSelector::Id);
        if (!p->m_strict)
            (yyvsp[(1) - (1)].string).lower();
        (yyval.selector)->setValue((yyvsp[(1) - (1)].string));
    ;}
    break;

  case 164:

/* Line 1455 of yacc.c  */
#line 1003 "Source/WebCore/css/CSSGrammar.y"
    {
        if ((yyvsp[(1) - (1)].string).characters[0] >= '0' && (yyvsp[(1) - (1)].string).characters[0] <= '9') {
            (yyval.selector) = 0;
        } else {
            CSSParser* p = static_cast<CSSParser*>(parser);
            (yyval.selector) = p->createFloatingSelector();
            (yyval.selector)->setMatch(CSSSelector::Id);
            if (!p->m_strict)
                (yyvsp[(1) - (1)].string).lower();
            (yyval.selector)->setValue((yyvsp[(1) - (1)].string));
        }
    ;}
    break;

  case 168:

/* Line 1455 of yacc.c  */
#line 1021 "Source/WebCore/css/CSSGrammar.y"
    {
        CSSParser* p = static_cast<CSSParser*>(parser);
        (yyval.selector) = p->createFloatingSelector();
        (yyval.selector)->setMatch(CSSSelector::Class);
        if (!p->m_strict)
            (yyvsp[(2) - (2)].string).lower();
        (yyval.selector)->setValue((yyvsp[(2) - (2)].string));
    ;}
    break;

  case 169:

/* Line 1455 of yacc.c  */
#line 1032 "Source/WebCore/css/CSSGrammar.y"
    {
        CSSParserString& str = (yyvsp[(1) - (2)].string);
        CSSParser* p = static_cast<CSSParser*>(parser);
        Document* doc = p->document();
        if (doc && doc->isHTMLDocument())
            str.lower();
        (yyval.string) = str;
    ;}
    break;

  case 170:

/* Line 1455 of yacc.c  */
#line 1043 "Source/WebCore/css/CSSGrammar.y"
    {
        (yyval.selector) = static_cast<CSSParser*>(parser)->createFloatingSelector();
        (yyval.selector)->setAttribute(QualifiedName(nullAtom, (yyvsp[(3) - (4)].string), nullAtom));
        (yyval.selector)->setMatch(CSSSelector::Set);
    ;}
    break;

  case 171:

/* Line 1455 of yacc.c  */
#line 1048 "Source/WebCore/css/CSSGrammar.y"
    {
        (yyval.selector) = static_cast<CSSParser*>(parser)->createFloatingSelector();
        (yyval.selector)->setAttribute(QualifiedName(nullAtom, (yyvsp[(3) - (8)].string), nullAtom));
        (yyval.selector)->setMatch((CSSSelector::Match)(yyvsp[(4) - (8)].integer));
        (yyval.selector)->setValue((yyvsp[(6) - (8)].string));
    ;}
    break;

  case 172:

/* Line 1455 of yacc.c  */
#line 1054 "Source/WebCore/css/CSSGrammar.y"
    {
        AtomicString namespacePrefix = (yyvsp[(3) - (5)].string);
        CSSParser* p = static_cast<CSSParser*>(parser);
        (yyval.selector) = p->createFloatingSelector();
        (yyval.selector)->setAttribute(QualifiedName(namespacePrefix, (yyvsp[(4) - (5)].string),
                                   p->m_styleSheet->determineNamespace(namespacePrefix)));
        (yyval.selector)->setMatch(CSSSelector::Set);
    ;}
    break;

  case 173:

/* Line 1455 of yacc.c  */
#line 1062 "Source/WebCore/css/CSSGrammar.y"
    {
        AtomicString namespacePrefix = (yyvsp[(3) - (9)].string);
        CSSParser* p = static_cast<CSSParser*>(parser);
        (yyval.selector) = p->createFloatingSelector();
        (yyval.selector)->setAttribute(QualifiedName(namespacePrefix, (yyvsp[(4) - (9)].string),
                                   p->m_styleSheet->determineNamespace(namespacePrefix)));
        (yyval.selector)->setMatch((CSSSelector::Match)(yyvsp[(5) - (9)].integer));
        (yyval.selector)->setValue((yyvsp[(7) - (9)].string));
    ;}
    break;

  case 174:

/* Line 1455 of yacc.c  */
#line 1074 "Source/WebCore/css/CSSGrammar.y"
    {
        (yyval.integer) = CSSSelector::Exact;
    ;}
    break;

  case 175:

/* Line 1455 of yacc.c  */
#line 1077 "Source/WebCore/css/CSSGrammar.y"
    {
        (yyval.integer) = CSSSelector::List;
    ;}
    break;

  case 176:

/* Line 1455 of yacc.c  */
#line 1080 "Source/WebCore/css/CSSGrammar.y"
    {
        (yyval.integer) = CSSSelector::Hyphen;
    ;}
    break;

  case 177:

/* Line 1455 of yacc.c  */
#line 1083 "Source/WebCore/css/CSSGrammar.y"
    {
        (yyval.integer) = CSSSelector::Begin;
    ;}
    break;

  case 178:

/* Line 1455 of yacc.c  */
#line 1086 "Source/WebCore/css/CSSGrammar.y"
    {
        (yyval.integer) = CSSSelector::End;
    ;}
    break;

  case 179:

/* Line 1455 of yacc.c  */
#line 1089 "Source/WebCore/css/CSSGrammar.y"
    {
        (yyval.integer) = CSSSelector::Contain;
    ;}
    break;

  case 182:

/* Line 1455 of yacc.c  */
#line 1100 "Source/WebCore/css/CSSGrammar.y"
    {
        (yyval.selector) = static_cast<CSSParser*>(parser)->createFloatingSelector();
        (yyval.selector)->setMatch(CSSSelector::PagePseudoClass);
        (yyvsp[(2) - (2)].string).lower();
        (yyval.selector)->setValue((yyvsp[(2) - (2)].string));
        CSSSelector::PseudoType type = (yyval.selector)->pseudoType();
        if (type == CSSSelector::PseudoUnknown)
            (yyval.selector) = 0;
    ;}
    break;

  case 183:

/* Line 1455 of yacc.c  */
#line 1111 "Source/WebCore/css/CSSGrammar.y"
    {
        (yyval.selector) = static_cast<CSSParser*>(parser)->createFloatingSelector();
        (yyval.selector)->setMatch(CSSSelector::PseudoClass);
        (yyvsp[(2) - (2)].string).lower();
        (yyval.selector)->setValue((yyvsp[(2) - (2)].string));
        CSSSelector::PseudoType type = (yyval.selector)->pseudoType();
        if (type == CSSSelector::PseudoUnknown)
            (yyval.selector) = 0;
    ;}
    break;

  case 184:

/* Line 1455 of yacc.c  */
#line 1120 "Source/WebCore/css/CSSGrammar.y"
    {
        (yyval.selector) = static_cast<CSSParser*>(parser)->createFloatingSelector();
        (yyval.selector)->setMatch(CSSSelector::PseudoElement);
        (yyvsp[(3) - (3)].string).lower();
        (yyval.selector)->setValue((yyvsp[(3) - (3)].string));
        // FIXME: This call is needed to force selector to compute the pseudoType early enough.
        (yyval.selector)->pseudoType();
    ;}
    break;

  case 185:

/* Line 1455 of yacc.c  */
#line 1133 "Source/WebCore/css/CSSGrammar.y"
    {
        if ((yyvsp[(4) - (6)].selectorList)) {
            CSSParser *p = static_cast<CSSParser*>(parser);
            (yyval.selector) = p->createFloatingSelector();
            (yyval.selector)->setMatch(CSSSelector::PseudoClass);
            (yyval.selector)->adoptSelectorVector(*p->sinkFloatingSelectorVector((yyvsp[(4) - (6)].selectorList)));
            (yyvsp[(2) - (6)].string).lower();
            (yyval.selector)->setValue((yyvsp[(2) - (6)].string));
            CSSSelector::PseudoType type = (yyval.selector)->pseudoType();
            if (type != CSSSelector::PseudoAny)
                (yyval.selector) = 0;
        } else
            (yyval.selector) = 0;
    ;}
    break;

  case 186:

/* Line 1455 of yacc.c  */
#line 1148 "Source/WebCore/css/CSSGrammar.y"
    {
        CSSParser *p = static_cast<CSSParser*>(parser);
        (yyval.selector) = p->createFloatingSelector();
        (yyval.selector)->setMatch(CSSSelector::PseudoClass);
        (yyval.selector)->setArgument((yyvsp[(4) - (6)].string));
        (yyval.selector)->setValue((yyvsp[(2) - (6)].string));
        CSSSelector::PseudoType type = (yyval.selector)->pseudoType();
        if (type == CSSSelector::PseudoUnknown)
            (yyval.selector) = 0;
    ;}
    break;

  case 187:

/* Line 1455 of yacc.c  */
#line 1159 "Source/WebCore/css/CSSGrammar.y"
    {
        CSSParser *p = static_cast<CSSParser*>(parser);
        (yyval.selector) = p->createFloatingSelector();
        (yyval.selector)->setMatch(CSSSelector::PseudoClass);
        (yyval.selector)->setArgument(String::number((yyvsp[(4) - (7)].integer) * (yyvsp[(5) - (7)].number)));
        (yyval.selector)->setValue((yyvsp[(2) - (7)].string));
        CSSSelector::PseudoType type = (yyval.selector)->pseudoType();
        if (type == CSSSelector::PseudoUnknown)
            (yyval.selector) = 0;
    ;}
    break;

  case 188:

/* Line 1455 of yacc.c  */
#line 1170 "Source/WebCore/css/CSSGrammar.y"
    {
        CSSParser *p = static_cast<CSSParser*>(parser);
        (yyval.selector) = p->createFloatingSelector();
        (yyval.selector)->setMatch(CSSSelector::PseudoClass);
        (yyval.selector)->setArgument((yyvsp[(4) - (6)].string));
        (yyvsp[(2) - (6)].string).lower();
        (yyval.selector)->setValue((yyvsp[(2) - (6)].string));
        CSSSelector::PseudoType type = (yyval.selector)->pseudoType();
        if (type == CSSSelector::PseudoUnknown)
            (yyval.selector) = 0;
        else if (type == CSSSelector::PseudoNthChild ||
                 type == CSSSelector::PseudoNthOfType ||
                 type == CSSSelector::PseudoNthLastChild ||
                 type == CSSSelector::PseudoNthLastOfType) {
            if (!isValidNthToken((yyvsp[(4) - (6)].string)))
                (yyval.selector) = 0;
        }
    ;}
    break;

  case 189:

/* Line 1455 of yacc.c  */
#line 1189 "Source/WebCore/css/CSSGrammar.y"
    {
        if (!(yyvsp[(4) - (6)].selector) || !(yyvsp[(4) - (6)].selector)->isSimple())
            (yyval.selector) = 0;
        else {
            CSSParser* p = static_cast<CSSParser*>(parser);
            (yyval.selector) = p->createFloatingSelector();
            (yyval.selector)->setMatch(CSSSelector::PseudoClass);

            Vector<OwnPtr<CSSParserSelector> > selectorVector;
            selectorVector.append(p->sinkFloatingSelector((yyvsp[(4) - (6)].selector)));
            (yyval.selector)->adoptSelectorVector(selectorVector);

            (yyvsp[(2) - (6)].string).lower();
            (yyval.selector)->setValue((yyvsp[(2) - (6)].string));
        }
    ;}
    break;

  case 190:

/* Line 1455 of yacc.c  */
#line 1208 "Source/WebCore/css/CSSGrammar.y"
    {
        (yyval.boolean) = (yyvsp[(1) - (1)].boolean);
    ;}
    break;

  case 191:

/* Line 1455 of yacc.c  */
#line 1211 "Source/WebCore/css/CSSGrammar.y"
    {
        (yyval.boolean) = (yyvsp[(1) - (2)].boolean);
        if ( (yyvsp[(2) - (2)].boolean) )
            (yyval.boolean) = (yyvsp[(2) - (2)].boolean);
    ;}
    break;

  case 192:

/* Line 1455 of yacc.c  */
#line 1216 "Source/WebCore/css/CSSGrammar.y"
    {
        (yyval.boolean) = (yyvsp[(1) - (1)].boolean);
    ;}
    break;

  case 193:

/* Line 1455 of yacc.c  */
#line 1219 "Source/WebCore/css/CSSGrammar.y"
    {
        (yyval.boolean) = false;
    ;}
    break;

  case 194:

/* Line 1455 of yacc.c  */
#line 1222 "Source/WebCore/css/CSSGrammar.y"
    {
        (yyval.boolean) = false;
    ;}
    break;

  case 195:

/* Line 1455 of yacc.c  */
#line 1225 "Source/WebCore/css/CSSGrammar.y"
    {
        (yyval.boolean) = (yyvsp[(1) - (2)].boolean);
    ;}
    break;

  case 196:

/* Line 1455 of yacc.c  */
#line 1228 "Source/WebCore/css/CSSGrammar.y"
    {
        (yyval.boolean) = (yyvsp[(1) - (2)].boolean);
    ;}
    break;

  case 197:

/* Line 1455 of yacc.c  */
#line 1234 "Source/WebCore/css/CSSGrammar.y"
    {
        CSSParser* p = static_cast<CSSParser*>(parser);
        p->markPropertyStart();
        (yyval.boolean) = (yyvsp[(1) - (3)].boolean);
    ;}
    break;

  case 198:

/* Line 1455 of yacc.c  */
#line 1239 "Source/WebCore/css/CSSGrammar.y"
    {
        (yyval.boolean) = false;
    ;}
    break;

  case 199:

/* Line 1455 of yacc.c  */
#line 1242 "Source/WebCore/css/CSSGrammar.y"
    {
        (yyval.boolean) = false;
    ;}
    break;

  case 200:

/* Line 1455 of yacc.c  */
#line 1245 "Source/WebCore/css/CSSGrammar.y"
    {
        CSSParser* p = static_cast<CSSParser*>(parser);
        p->markPropertyStart();
        (yyval.boolean) = false;
    ;}
    break;

  case 201:

/* Line 1455 of yacc.c  */
#line 1250 "Source/WebCore/css/CSSGrammar.y"
    {
        (yyval.boolean) = false;
    ;}
    break;

  case 202:

/* Line 1455 of yacc.c  */
#line 1253 "Source/WebCore/css/CSSGrammar.y"
    {
        CSSParser* p = static_cast<CSSParser*>(parser);
        p->markPropertyStart();
        (yyval.boolean) = (yyvsp[(1) - (4)].boolean);
        if ((yyvsp[(2) - (4)].boolean))
            (yyval.boolean) = (yyvsp[(2) - (4)].boolean);
    ;}
    break;

  case 203:

/* Line 1455 of yacc.c  */
#line 1260 "Source/WebCore/css/CSSGrammar.y"
    {
        CSSParser* p = static_cast<CSSParser*>(parser);
        p->markPropertyStart();
        (yyval.boolean) = (yyvsp[(1) - (4)].boolean);
    ;}
    break;

  case 204:

/* Line 1455 of yacc.c  */
#line 1265 "Source/WebCore/css/CSSGrammar.y"
    {
        CSSParser* p = static_cast<CSSParser*>(parser);
        p->markPropertyStart();
        (yyval.boolean) = (yyvsp[(1) - (6)].boolean);
    ;}
    break;

  case 205:

/* Line 1455 of yacc.c  */
#line 1273 "Source/WebCore/css/CSSGrammar.y"
    {
        (yyval.boolean) = false;
        CSSParser* p = static_cast<CSSParser*>(parser);
        bool isPropertyParsed = false;
        if ((yyvsp[(1) - (5)].integer) && (yyvsp[(4) - (5)].valueList)) {
            p->m_valueList = p->sinkFloatingValueList((yyvsp[(4) - (5)].valueList));
            int oldParsedProperties = p->m_numParsedProperties;
            (yyval.boolean) = p->parseValue((yyvsp[(1) - (5)].integer), (yyvsp[(5) - (5)].boolean));
            if (!(yyval.boolean))
                p->rollbackLastProperties(p->m_numParsedProperties - oldParsedProperties);
            else
                isPropertyParsed = true;
            delete p->m_valueList;
            p->m_valueList = 0;
        }
        p->markPropertyEnd((yyvsp[(5) - (5)].boolean), isPropertyParsed);
    ;}
    break;

  case 206:

/* Line 1455 of yacc.c  */
#line 1291 "Source/WebCore/css/CSSGrammar.y"
    {
        (yyval.boolean) = false;
    ;}
    break;

  case 207:

/* Line 1455 of yacc.c  */
#line 1295 "Source/WebCore/css/CSSGrammar.y"
    {
        /* The default movable type template has letter-spacing: .none;  Handle this by looking for
        error tokens at the start of an expr, recover the expr and then treat as an error, cleaning
        up and deleting the shifted expr.  */
        CSSParser* p = static_cast<CSSParser*>(parser);
        p->markPropertyEnd(false, false);
        (yyval.boolean) = false;
    ;}
    break;

  case 208:

/* Line 1455 of yacc.c  */
#line 1304 "Source/WebCore/css/CSSGrammar.y"
    {
        /* When we encounter something like p {color: red !important fail;} we should drop the declaration */
        CSSParser* p = static_cast<CSSParser*>(parser);
        p->markPropertyEnd(false, false);
        (yyval.boolean) = false;
    ;}
    break;

  case 209:

/* Line 1455 of yacc.c  */
#line 1311 "Source/WebCore/css/CSSGrammar.y"
    {
        /* Handle this case: div { text-align: center; !important } Just reduce away the stray !important. */
        (yyval.boolean) = false;
    ;}
    break;

  case 210:

/* Line 1455 of yacc.c  */
#line 1316 "Source/WebCore/css/CSSGrammar.y"
    {
        /* div { font-family: } Just reduce away this property with no value. */
        CSSParser* p = static_cast<CSSParser*>(parser);
        p->markPropertyEnd(false, false);
        (yyval.boolean) = false;
    ;}
    break;

  case 211:

/* Line 1455 of yacc.c  */
#line 1323 "Source/WebCore/css/CSSGrammar.y"
    {
        /* if we come across rules with invalid values like this case: p { weight: *; }, just discard the rule */
        CSSParser* p = static_cast<CSSParser*>(parser);
        p->markPropertyEnd(false, false);
        (yyval.boolean) = false;
    ;}
    break;

  case 212:

/* Line 1455 of yacc.c  */
#line 1330 "Source/WebCore/css/CSSGrammar.y"
    {
        /* if we come across: div { color{;color:maroon} }, ignore everything within curly brackets */
        (yyval.boolean) = false;
    ;}
    break;

  case 213:

/* Line 1455 of yacc.c  */
#line 1337 "Source/WebCore/css/CSSGrammar.y"
    {
        (yyval.integer) = cssPropertyID((yyvsp[(1) - (2)].string));
    ;}
    break;

  case 214:

/* Line 1455 of yacc.c  */
#line 1343 "Source/WebCore/css/CSSGrammar.y"
    { (yyval.boolean) = true; ;}
    break;

  case 215:

/* Line 1455 of yacc.c  */
#line 1344 "Source/WebCore/css/CSSGrammar.y"
    { (yyval.boolean) = false; ;}
    break;

  case 216:

/* Line 1455 of yacc.c  */
#line 1348 "Source/WebCore/css/CSSGrammar.y"
    {
        CSSParser* p = static_cast<CSSParser*>(parser);
        (yyval.valueList) = p->createFloatingValueList();
        (yyval.valueList)->addValue(p->sinkFloatingValue((yyvsp[(1) - (1)].value)));
    ;}
    break;

  case 217:

/* Line 1455 of yacc.c  */
#line 1353 "Source/WebCore/css/CSSGrammar.y"
    {
        CSSParser* p = static_cast<CSSParser*>(parser);
        (yyval.valueList) = (yyvsp[(1) - (3)].valueList);
        if ((yyval.valueList)) {
            if ((yyvsp[(2) - (3)].character)) {
                CSSParserValue v;
                v.id = 0;
                v.unit = CSSParserValue::Operator;
                v.iValue = (yyvsp[(2) - (3)].character);
                (yyval.valueList)->addValue(v);
            }
            (yyval.valueList)->addValue(p->sinkFloatingValue((yyvsp[(3) - (3)].value)));
        }
    ;}
    break;

  case 218:

/* Line 1455 of yacc.c  */
#line 1367 "Source/WebCore/css/CSSGrammar.y"
    {
        (yyval.valueList) = 0;
    ;}
    break;

  case 219:

/* Line 1455 of yacc.c  */
#line 1370 "Source/WebCore/css/CSSGrammar.y"
    {
        (yyval.valueList) = 0;
    ;}
    break;

  case 220:

/* Line 1455 of yacc.c  */
#line 1373 "Source/WebCore/css/CSSGrammar.y"
    {
        (yyval.valueList) = 0;
    ;}
    break;

  case 221:

/* Line 1455 of yacc.c  */
#line 1379 "Source/WebCore/css/CSSGrammar.y"
    {
        (yyval.character) = '/';
    ;}
    break;

  case 222:

/* Line 1455 of yacc.c  */
#line 1382 "Source/WebCore/css/CSSGrammar.y"
    {
        (yyval.character) = ',';
    ;}
    break;

  case 223:

/* Line 1455 of yacc.c  */
#line 1385 "Source/WebCore/css/CSSGrammar.y"
    {
        (yyval.character) = 0;
  ;}
    break;

  case 224:

/* Line 1455 of yacc.c  */
#line 1391 "Source/WebCore/css/CSSGrammar.y"
    { (yyval.value) = (yyvsp[(1) - (1)].value); ;}
    break;

  case 225:

/* Line 1455 of yacc.c  */
#line 1392 "Source/WebCore/css/CSSGrammar.y"
    { (yyval.value) = (yyvsp[(2) - (2)].value); (yyval.value).fValue *= (yyvsp[(1) - (2)].integer); ;}
    break;

  case 226:

/* Line 1455 of yacc.c  */
#line 1393 "Source/WebCore/css/CSSGrammar.y"
    { (yyval.value).id = 0; (yyval.value).string = (yyvsp[(1) - (2)].string); (yyval.value).unit = CSSPrimitiveValue::CSS_STRING; ;}
    break;

  case 227:

/* Line 1455 of yacc.c  */
#line 1394 "Source/WebCore/css/CSSGrammar.y"
    {
      (yyval.value).id = cssValueKeywordID((yyvsp[(1) - (2)].string));
      (yyval.value).unit = CSSPrimitiveValue::CSS_IDENT;
      (yyval.value).string = (yyvsp[(1) - (2)].string);
  ;}
    break;

  case 228:

/* Line 1455 of yacc.c  */
#line 1400 "Source/WebCore/css/CSSGrammar.y"
    { (yyval.value).id = 0; (yyval.value).string = (yyvsp[(1) - (2)].string); (yyval.value).unit = CSSPrimitiveValue::CSS_DIMENSION; ;}
    break;

  case 229:

/* Line 1455 of yacc.c  */
#line 1401 "Source/WebCore/css/CSSGrammar.y"
    { (yyval.value).id = 0; (yyval.value).string = (yyvsp[(2) - (3)].string); (yyval.value).unit = CSSPrimitiveValue::CSS_DIMENSION; ;}
    break;

  case 230:

/* Line 1455 of yacc.c  */
#line 1402 "Source/WebCore/css/CSSGrammar.y"
    { (yyval.value).id = 0; (yyval.value).string = (yyvsp[(1) - (2)].string); (yyval.value).unit = CSSPrimitiveValue::CSS_URI; ;}
    break;

  case 231:

/* Line 1455 of yacc.c  */
#line 1403 "Source/WebCore/css/CSSGrammar.y"
    { (yyval.value).id = 0; (yyval.value).string = (yyvsp[(1) - (2)].string); (yyval.value).unit = CSSPrimitiveValue::CSS_UNICODE_RANGE; ;}
    break;

  case 232:

/* Line 1455 of yacc.c  */
#line 1404 "Source/WebCore/css/CSSGrammar.y"
    { (yyval.value).id = 0; (yyval.value).string = (yyvsp[(1) - (2)].string); (yyval.value).unit = CSSPrimitiveValue::CSS_PARSER_HEXCOLOR; ;}
    break;

  case 233:

/* Line 1455 of yacc.c  */
#line 1405 "Source/WebCore/css/CSSGrammar.y"
    { (yyval.value).id = 0; (yyval.value).string = CSSParserString(); (yyval.value).unit = CSSPrimitiveValue::CSS_PARSER_HEXCOLOR; ;}
    break;

  case 234:

/* Line 1455 of yacc.c  */
#line 1407 "Source/WebCore/css/CSSGrammar.y"
    {
      (yyval.value) = (yyvsp[(1) - (1)].value);
  ;}
    break;

  case 235:

/* Line 1455 of yacc.c  */
#line 1410 "Source/WebCore/css/CSSGrammar.y"
    { /* Handle width: %; */
      (yyval.value).id = 0; (yyval.value).unit = 0;
  ;}
    break;

  case 236:

/* Line 1455 of yacc.c  */
#line 1416 "Source/WebCore/css/CSSGrammar.y"
    { (yyval.value).id = 0; (yyval.value).isInt = true; (yyval.value).fValue = (yyvsp[(1) - (2)].number); (yyval.value).unit = CSSPrimitiveValue::CSS_NUMBER; ;}
    break;

  case 237:

/* Line 1455 of yacc.c  */
#line 1417 "Source/WebCore/css/CSSGrammar.y"
    { (yyval.value).id = 0; (yyval.value).isInt = false; (yyval.value).fValue = (yyvsp[(1) - (2)].number); (yyval.value).unit = CSSPrimitiveValue::CSS_NUMBER; ;}
    break;

  case 238:

/* Line 1455 of yacc.c  */
#line 1418 "Source/WebCore/css/CSSGrammar.y"
    { (yyval.value).id = 0; (yyval.value).fValue = (yyvsp[(1) - (2)].number); (yyval.value).unit = CSSPrimitiveValue::CSS_PERCENTAGE; ;}
    break;

  case 239:

/* Line 1455 of yacc.c  */
#line 1419 "Source/WebCore/css/CSSGrammar.y"
    { (yyval.value).id = 0; (yyval.value).fValue = (yyvsp[(1) - (2)].number); (yyval.value).unit = CSSPrimitiveValue::CSS_PX; ;}
    break;

  case 240:

/* Line 1455 of yacc.c  */
#line 1420 "Source/WebCore/css/CSSGrammar.y"
    { (yyval.value).id = 0; (yyval.value).fValue = (yyvsp[(1) - (2)].number); (yyval.value).unit = CSSPrimitiveValue::CSS_CM; ;}
    break;

  case 241:

/* Line 1455 of yacc.c  */
#line 1421 "Source/WebCore/css/CSSGrammar.y"
    { (yyval.value).id = 0; (yyval.value).fValue = (yyvsp[(1) - (2)].number); (yyval.value).unit = CSSPrimitiveValue::CSS_MM; ;}
    break;

  case 242:

/* Line 1455 of yacc.c  */
#line 1422 "Source/WebCore/css/CSSGrammar.y"
    { (yyval.value).id = 0; (yyval.value).fValue = (yyvsp[(1) - (2)].number); (yyval.value).unit = CSSPrimitiveValue::CSS_IN; ;}
    break;

  case 243:

/* Line 1455 of yacc.c  */
#line 1423 "Source/WebCore/css/CSSGrammar.y"
    { (yyval.value).id = 0; (yyval.value).fValue = (yyvsp[(1) - (2)].number); (yyval.value).unit = CSSPrimitiveValue::CSS_PT; ;}
    break;

  case 244:

/* Line 1455 of yacc.c  */
#line 1424 "Source/WebCore/css/CSSGrammar.y"
    { (yyval.value).id = 0; (yyval.value).fValue = (yyvsp[(1) - (2)].number); (yyval.value).unit = CSSPrimitiveValue::CSS_PC; ;}
    break;

  case 245:

/* Line 1455 of yacc.c  */
#line 1425 "Source/WebCore/css/CSSGrammar.y"
    { (yyval.value).id = 0; (yyval.value).fValue = (yyvsp[(1) - (2)].number); (yyval.value).unit = CSSPrimitiveValue::CSS_DEG; ;}
    break;

  case 246:

/* Line 1455 of yacc.c  */
#line 1426 "Source/WebCore/css/CSSGrammar.y"
    { (yyval.value).id = 0; (yyval.value).fValue = (yyvsp[(1) - (2)].number); (yyval.value).unit = CSSPrimitiveValue::CSS_RAD; ;}
    break;

  case 247:

/* Line 1455 of yacc.c  */
#line 1427 "Source/WebCore/css/CSSGrammar.y"
    { (yyval.value).id = 0; (yyval.value).fValue = (yyvsp[(1) - (2)].number); (yyval.value).unit = CSSPrimitiveValue::CSS_GRAD; ;}
    break;

  case 248:

/* Line 1455 of yacc.c  */
#line 1428 "Source/WebCore/css/CSSGrammar.y"
    { (yyval.value).id = 0; (yyval.value).fValue = (yyvsp[(1) - (2)].number); (yyval.value).unit = CSSPrimitiveValue::CSS_TURN; ;}
    break;

  case 249:

/* Line 1455 of yacc.c  */
#line 1429 "Source/WebCore/css/CSSGrammar.y"
    { (yyval.value).id = 0; (yyval.value).fValue = (yyvsp[(1) - (2)].number); (yyval.value).unit = CSSPrimitiveValue::CSS_MS; ;}
    break;

  case 250:

/* Line 1455 of yacc.c  */
#line 1430 "Source/WebCore/css/CSSGrammar.y"
    { (yyval.value).id = 0; (yyval.value).fValue = (yyvsp[(1) - (2)].number); (yyval.value).unit = CSSPrimitiveValue::CSS_S; ;}
    break;

  case 251:

/* Line 1455 of yacc.c  */
#line 1431 "Source/WebCore/css/CSSGrammar.y"
    { (yyval.value).id = 0; (yyval.value).fValue = (yyvsp[(1) - (2)].number); (yyval.value).unit = CSSPrimitiveValue::CSS_HZ; ;}
    break;

  case 252:

/* Line 1455 of yacc.c  */
#line 1432 "Source/WebCore/css/CSSGrammar.y"
    { (yyval.value).id = 0; (yyval.value).fValue = (yyvsp[(1) - (2)].number); (yyval.value).unit = CSSPrimitiveValue::CSS_KHZ; ;}
    break;

  case 253:

/* Line 1455 of yacc.c  */
#line 1433 "Source/WebCore/css/CSSGrammar.y"
    { (yyval.value).id = 0; (yyval.value).fValue = (yyvsp[(1) - (2)].number); (yyval.value).unit = CSSPrimitiveValue::CSS_EMS; ;}
    break;

  case 254:

/* Line 1455 of yacc.c  */
#line 1434 "Source/WebCore/css/CSSGrammar.y"
    { (yyval.value).id = 0; (yyval.value).fValue = (yyvsp[(1) - (2)].number); (yyval.value).unit = CSSParserValue::Q_EMS; ;}
    break;

  case 255:

/* Line 1455 of yacc.c  */
#line 1435 "Source/WebCore/css/CSSGrammar.y"
    { (yyval.value).id = 0; (yyval.value).fValue = (yyvsp[(1) - (2)].number); (yyval.value).unit = CSSPrimitiveValue::CSS_EXS; ;}
    break;

  case 256:

/* Line 1455 of yacc.c  */
#line 1436 "Source/WebCore/css/CSSGrammar.y"
    {
      (yyval.value).id = 0;
      (yyval.value).fValue = (yyvsp[(1) - (2)].number);
      (yyval.value).unit = CSSPrimitiveValue::CSS_REMS;
      CSSParser* p = static_cast<CSSParser*>(parser);
      if (Document* doc = p->document())
          doc->setUsesRemUnits(true);
  ;}
    break;

  case 257:

/* Line 1455 of yacc.c  */
#line 1447 "Source/WebCore/css/CSSGrammar.y"
    {
        CSSParser* p = static_cast<CSSParser*>(parser);
        CSSParserFunction* f = p->createFloatingFunction();
        f->name = (yyvsp[(1) - (5)].string);
        f->args = p->sinkFloatingValueList((yyvsp[(3) - (5)].valueList));
        (yyval.value).id = 0;
        (yyval.value).unit = CSSParserValue::Function;
        (yyval.value).function = f;
    ;}
    break;

  case 258:

/* Line 1455 of yacc.c  */
#line 1456 "Source/WebCore/css/CSSGrammar.y"
    {
        CSSParser* p = static_cast<CSSParser*>(parser);
        CSSParserFunction* f = p->createFloatingFunction();
        f->name = (yyvsp[(1) - (3)].string);
        f->args = 0;
        (yyval.value).id = 0;
        (yyval.value).unit = CSSParserValue::Function;
        (yyval.value).function = f;
  ;}
    break;

  case 259:

/* Line 1455 of yacc.c  */
#line 1470 "Source/WebCore/css/CSSGrammar.y"
    {
        (yyval.rule) = 0;
    ;}
    break;

  case 260:

/* Line 1455 of yacc.c  */
#line 1473 "Source/WebCore/css/CSSGrammar.y"
    {
        (yyval.rule) = 0;
    ;}
    break;

  case 261:

/* Line 1455 of yacc.c  */
#line 1479 "Source/WebCore/css/CSSGrammar.y"
    {
        (yyval.rule) = 0;
    ;}
    break;

  case 262:

/* Line 1455 of yacc.c  */
#line 1482 "Source/WebCore/css/CSSGrammar.y"
    {
        (yyval.rule) = 0;
    ;}
    break;

  case 263:

/* Line 1455 of yacc.c  */
#line 1488 "Source/WebCore/css/CSSGrammar.y"
    {
        (yyval.rule) = 0;
    ;}
    break;

  case 264:

/* Line 1455 of yacc.c  */
#line 1506 "Source/WebCore/css/CSSGrammar.y"
    {
        static_cast<CSSParser*>(parser)->invalidBlockHit();
    ;}
    break;

  case 265:

/* Line 1455 of yacc.c  */
#line 1509 "Source/WebCore/css/CSSGrammar.y"
    {
        static_cast<CSSParser*>(parser)->invalidBlockHit();
    ;}
    break;



/* Line 1455 of yacc.c  */
#line 4383 "./DerivedSources/WebCore/CSSGrammar.cpp"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined(yyoverflow) || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}



/* Line 1675 of yacc.c  */
#line 1519 "Source/WebCore/css/CSSGrammar.y"


