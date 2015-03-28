/* Generated from magplot.scm by the CHICKEN compiler
   http://www.call-with-current-continuation.org
   2009-10-10 18:49
   Version 3.2.7 - linux-unix-gnu-x86	[ manyargs dload ptables applyhook hostpcre ]
   compiled 2008-11-05 on debian (Linux)
   command line: magplot.scm
   used units: library eval data_structures ports extras srfi_69 srfi_1 srfi_4 posix srfi_18 srfi_69
*/

#include "chicken.h"

static C_PTABLE_ENTRY *create_ptable(void);
C_noret_decl(C_library_toplevel)
C_externimport void C_ccall C_library_toplevel(C_word c,C_word d,C_word k) C_noret;
C_noret_decl(C_eval_toplevel)
C_externimport void C_ccall C_eval_toplevel(C_word c,C_word d,C_word k) C_noret;
C_noret_decl(C_data_structures_toplevel)
C_externimport void C_ccall C_data_structures_toplevel(C_word c,C_word d,C_word k) C_noret;
C_noret_decl(C_ports_toplevel)
C_externimport void C_ccall C_ports_toplevel(C_word c,C_word d,C_word k) C_noret;
C_noret_decl(C_extras_toplevel)
C_externimport void C_ccall C_extras_toplevel(C_word c,C_word d,C_word k) C_noret;
C_noret_decl(C_srfi_69_toplevel)
C_externimport void C_ccall C_srfi_69_toplevel(C_word c,C_word d,C_word k) C_noret;
C_noret_decl(C_srfi_1_toplevel)
C_externimport void C_ccall C_srfi_1_toplevel(C_word c,C_word d,C_word k) C_noret;
C_noret_decl(C_srfi_4_toplevel)
C_externimport void C_ccall C_srfi_4_toplevel(C_word c,C_word d,C_word k) C_noret;
C_noret_decl(C_posix_toplevel)
C_externimport void C_ccall C_posix_toplevel(C_word c,C_word d,C_word k) C_noret;
C_noret_decl(C_srfi_18_toplevel)
C_externimport void C_ccall C_srfi_18_toplevel(C_word c,C_word d,C_word k) C_noret;
C_noret_decl(C_srfi_69_toplevel)
C_externimport void C_ccall C_srfi_69_toplevel(C_word c,C_word d,C_word k) C_noret;

static C_TLS C_word lf[248];
static double C_possibly_force_alignment;
static C_char C_TLS li0[] C_aligned={C_lihdr(0,0,19),40,115,113,117,97,114,101,32,120,46,49,54,50,57,49,54,51,48,41,0,0,0,0,0};
static C_char C_TLS li1[] C_aligned={C_lihdr(0,0,17),40,99,117,98,101,32,120,46,49,54,51,49,49,54,51,50,41,0,0,0,0,0,0,0};
static C_char C_TLS li2[] C_aligned={C_lihdr(0,0,24),40,109,97,103,110,105,116,117,100,101,32,118,101,99,46,49,54,51,51,49,54,51,52,41};
static C_char C_TLS li3[] C_aligned={C_lihdr(0,0,18),40,97,50,50,48,53,32,120,46,49,54,51,55,49,54,52,48,41,0,0,0,0,0,0};
static C_char C_TLS li4[] C_aligned={C_lihdr(0,0,24),40,110,111,114,109,97,108,105,122,101,32,118,101,99,46,49,54,51,53,49,54,51,56,41};
static C_char C_TLS li5[] C_aligned={C_lihdr(0,0,21),40,100,111,116,32,46,32,118,101,99,115,46,49,54,52,49,49,54,52,50,41,0,0,0};
static C_char C_TLS li6[] C_aligned={C_lihdr(0,0,20),40,100,101,103,50,114,97,100,32,120,46,49,54,52,51,49,54,52,52,41,0,0,0,0};
static C_char C_TLS li7[] C_aligned={C_lihdr(0,0,69),40,116,114,105,97,110,103,108,101,103,114,105,100,45,97,100,100,112,111,105,110,116,32,120,46,49,54,52,53,49,54,53,48,32,121,46,49,54,52,54,49,54,53,49,32,122,46,49,54,52,55,49,54,53,50,32,109,97,103,46,49,54,52,56,49,54,53,51,41,0,0,0};
static C_char C_TLS li8[] C_aligned={C_lihdr(0,0,18),40,97,50,50,53,55,32,112,46,49,54,53,53,49,54,53,55,41,0,0,0,0,0,0};
static C_char C_TLS li9[] C_aligned={C_lihdr(0,0,19),40,114,101,99,97,108,99,45,99,111,108,111,114,115,112,97,99,101,41,0,0,0,0,0};
static C_char C_TLS li10[] C_aligned={C_lihdr(0,0,18),40,97,50,50,57,56,32,105,46,49,54,54,51,49,54,54,55,41,0,0,0,0,0,0};
static C_char C_TLS li11[] C_aligned={C_lihdr(0,0,25),40,99,111,108,111,114,115,112,97,99,101,32,109,97,103,46,49,54,54,48,49,54,54,52,41,0,0,0,0,0,0,0};
static C_char C_TLS li12[] C_aligned={C_lihdr(0,0,26),40,109,97,107,101,45,103,108,45,112,111,105,110,116,32,112,46,49,54,54,57,49,54,55,48,41,0,0,0,0,0,0};
static C_char C_TLS li13[] C_aligned={C_lihdr(0,0,22),40,97,50,51,55,55,32,112,111,105,110,116,46,49,54,55,50,49,54,55,51,41,0,0};
static C_char C_TLS li14[] C_aligned={C_lihdr(0,0,19),40,116,114,105,97,110,103,108,101,103,114,105,100,45,100,114,97,119,41,0,0,0,0,0};
static C_char C_TLS li15[] C_aligned={C_lihdr(0,0,37),40,110,101,119,45,112,111,105,110,116,32,112,111,115,46,49,54,55,53,49,54,55,55,32,109,97,103,46,49,54,55,54,49,54,55,56,41,0,0,0};
static C_char C_TLS li16[] C_aligned={C_lihdr(0,0,27),40,100,114,97,119,45,112,111,105,110,116,32,112,111,105,110,116,46,49,54,55,57,49,54,56,49,41,0,0,0,0,0};
static C_char C_TLS li17[] C_aligned={C_lihdr(0,0,25),40,97,50,52,51,55,32,116,114,105,97,110,103,108,101,46,49,55,48,57,49,55,49,51,41,0,0,0,0,0,0,0};
static C_char C_TLS li18[] C_aligned={C_lihdr(0,0,35),40,100,114,97,119,45,116,114,105,97,110,103,108,101,115,32,116,114,105,97,110,103,108,101,115,46,49,55,48,54,49,55,49,48,41,0,0,0,0,0};
static C_char C_TLS li19[] C_aligned={C_lihdr(0,0,59),40,82,111,116,97,116,101,65,102,116,101,114,32,97,110,103,46,49,55,49,56,49,55,50,51,32,120,46,49,55,49,57,49,55,50,52,32,121,46,49,55,50,48,49,55,50,53,32,122,46,49,55,50,49,49,55,50,54,41,0,0,0,0,0};
static C_char C_TLS li20[] C_aligned={C_lihdr(0,0,31),40,114,101,115,104,97,112,101,32,119,46,49,55,51,49,49,55,51,52,32,104,46,49,55,51,50,49,55,51,53,41,0};
static C_char C_TLS li21[] C_aligned={C_lihdr(0,0,24),40,100,111,108,111,111,112,46,49,55,52,54,32,120,46,49,55,52,55,49,55,53,50,41};
static C_char C_TLS li22[] C_aligned={C_lihdr(0,0,54),40,112,111,115,45,100,114,97,119,32,112,111,115,46,49,55,52,51,49,55,52,56,32,114,97,100,105,117,115,46,49,55,52,52,49,55,52,57,32,99,111,108,111,114,46,49,55,52,53,49,55,53,48,41,0,0};
static C_char C_TLS li23[] C_aligned={C_lihdr(0,0,12),40,103,108,45,100,105,115,112,108,97,121,41,0,0,0,0};
static C_char C_TLS li24[] C_aligned={C_lihdr(0,0,32),40,97,50,54,54,49,32,101,108,46,49,55,55,51,49,55,55,54,32,97,99,99,46,49,55,55,52,49,55,55,55,41};
static C_char C_TLS li25[] C_aligned={C_lihdr(0,0,23),40,110,111,110,101,45,102,97,108,115,101,32,108,46,49,55,55,50,49,55,55,53,41,0};
static C_char C_TLS li26[] C_aligned={C_lihdr(0,0,30),40,112,97,114,115,101,45,109,97,103,45,108,105,110,101,32,108,105,110,101,46,49,55,55,56,49,55,56,49,41,0,0};
static C_char C_TLS li27[] C_aligned={C_lihdr(0,0,57),40,99,111,110,118,101,114,116,45,112,111,115,105,116,105,111,110,32,108,97,116,46,49,55,56,55,49,55,57,52,32,108,111,110,46,49,55,56,56,49,55,57,53,32,97,108,116,46,49,55,56,57,49,55,57,54,41,0,0,0,0,0,0,0};
static C_char C_TLS li28[] C_aligned={C_lihdr(0,0,31),40,99,111,110,118,101,114,116,45,100,101,103,46,49,56,49,50,32,118,97,108,46,49,56,49,51,49,56,50,54,41,0};
static C_char C_TLS li29[] C_aligned={C_lihdr(0,0,30),40,112,97,114,115,101,45,103,112,115,45,108,105,110,101,32,108,105,110,101,46,49,56,48,49,49,56,49,52,41,0,0};
static C_char C_TLS li30[] C_aligned={C_lihdr(0,0,11),40,108,111,111,112,46,49,56,51,53,41,0,0,0,0,0};
static C_char C_TLS li31[] C_aligned={C_lihdr(0,0,22),40,102,95,50,57,56,54,32,112,111,114,116,46,49,56,51,52,49,56,51,57,41,0,0};
static C_char C_TLS li32[] C_aligned={C_lihdr(0,0,18),40,109,97,107,101,45,108,105,110,101,45,114,101,97,100,101,114,41,0,0,0,0,0,0};
static C_char C_TLS li33[] C_aligned={C_lihdr(0,0,20),40,101,97,99,104,45,103,112,115,45,108,105,110,101,46,49,56,53,48,41,0,0,0,0};
static C_char C_TLS li34[] C_aligned={C_lihdr(0,0,20),40,101,97,99,104,45,109,97,103,45,108,105,110,101,46,49,56,52,56,41,0,0,0,0};
static C_char C_TLS li35[] C_aligned={C_lihdr(0,0,6),40,105,100,108,101,41,0,0};
static C_char C_TLS li36[] C_aligned={C_lihdr(0,0,38),40,97,116,116,114,45,115,101,116,33,32,110,97,109,101,46,49,56,54,52,49,56,54,54,32,118,97,108,46,49,56,54,53,49,56,54,55,41,0,0};
static C_char C_TLS li37[] C_aligned={C_lihdr(0,0,24),40,97,116,116,114,45,103,101,116,32,110,97,109,101,46,49,56,54,56,49,56,54,57,41};
static C_char C_TLS li38[] C_aligned={C_lihdr(0,0,40),40,103,108,117,116,45,72,97,115,77,111,100,105,102,105,101,114,115,32,46,32,109,111,100,105,102,105,101,114,115,46,49,56,55,48,49,56,55,50,41};
static C_char C_TLS li39[] C_aligned={C_lihdr(0,0,45),40,107,101,121,98,111,97,114,100,32,107,101,121,46,49,56,55,52,49,56,55,56,32,120,46,49,56,55,53,49,56,55,57,32,121,46,49,56,55,54,49,56,56,48,41,0,0,0};
static C_char C_TLS li40[] C_aligned={C_lihdr(0,0,44),40,115,112,101,99,105,97,108,32,107,101,121,46,49,56,56,52,49,56,56,57,32,120,46,49,56,56,53,49,56,57,48,32,121,46,49,56,56,54,49,56,57,49,41,0,0,0,0};
static C_char C_TLS li41[] C_aligned={C_lihdr(0,0,7),40,117,115,97,103,101,41,0};
static C_char C_TLS li42[] C_aligned={C_lihdr(0,0,7),40,97,51,52,51,52,41,0};
static C_char C_TLS li43[] C_aligned={C_lihdr(0,0,25),40,114,101,112,111,114,116,100,97,116,97,32,112,111,115,46,49,57,50,48,49,57,50,53,41,0,0,0,0,0,0,0};
static C_char C_TLS li44[] C_aligned={C_lihdr(0,0,11),40,108,111,97,100,45,100,97,116,97,41,0,0,0,0,0};
static C_char C_TLS li45[] C_aligned={C_lihdr(0,0,51),40,104,97,110,100,108,101,45,111,112,101,110,45,102,105,108,101,32,102,105,108,101,110,97,109,101,46,49,57,52,49,49,57,52,51,32,116,104,117,110,107,46,49,57,52,50,49,57,52,52,41,0,0,0,0,0};
static C_char C_TLS li46[] C_aligned={C_lihdr(0,0,11),40,108,111,111,112,46,50,48,52,52,41,0,0,0,0,0};
static C_char C_TLS li47[] C_aligned={C_lihdr(0,0,18),40,97,51,53,57,53,32,110,46,49,57,56,54,50,48,52,50,41,0,0,0,0,0,0};
static C_char C_TLS li48[] C_aligned={C_lihdr(0,0,7),40,97,51,54,48,49,41,0};
static C_char C_TLS li49[] C_aligned={C_lihdr(0,0,82),40,97,51,53,53,52,32,111,112,116,46,49,57,56,49,50,48,51,54,32,110,97,109,101,46,49,57,56,50,50,48,51,55,32,97,114,103,46,49,57,56,51,50,48,51,56,32,111,112,116,105,111,110,115,46,49,57,56,52,50,48,51,57,32,111,112,101,114,97,110,100,115,46,49,57,56,53,50,48,52,48,41,0,0,0,0,0,0};
static C_char C_TLS li50[] C_aligned={C_lihdr(0,0,18),40,97,51,54,54,50,32,110,46,49,57,56,48,50,48,51,51,41,0,0,0,0,0,0};
static C_char C_TLS li51[] C_aligned={C_lihdr(0,0,7),40,97,51,54,54,56,41,0};
static C_char C_TLS li52[] C_aligned={C_lihdr(0,0,82),40,97,51,54,49,56,32,111,112,116,46,49,57,55,53,50,48,50,55,32,110,97,109,101,46,49,57,55,54,50,48,50,56,32,97,114,103,46,49,57,55,55,50,48,50,57,32,111,112,116,105,111,110,115,46,49,57,55,56,50,48,51,48,32,111,112,101,114,97,110,100,115,46,49,57,55,57,50,48,51,49,41,0,0,0,0,0,0};
static C_char C_TLS li53[] C_aligned={C_lihdr(0,0,18),40,97,51,55,50,56,32,110,46,49,57,55,52,50,48,50,53,41,0,0,0,0,0,0};
static C_char C_TLS li54[] C_aligned={C_lihdr(0,0,82),40,97,51,54,56,57,32,111,112,116,46,49,57,54,57,50,48,49,57,32,110,97,109,101,46,49,57,55,48,50,48,50,48,32,97,114,103,46,49,57,55,49,50,48,50,49,32,111,112,116,105,111,110,115,46,49,57,55,50,50,48,50,50,32,111,112,101,114,97,110,100,115,46,49,57,55,51,50,48,50,51,41,0,0,0,0,0,0};
static C_char C_TLS li55[] C_aligned={C_lihdr(0,0,18),40,97,51,55,56,51,32,110,46,49,57,54,56,50,48,49,55,41,0,0,0,0,0,0};
static C_char C_TLS li56[] C_aligned={C_lihdr(0,0,82),40,97,51,55,52,52,32,111,112,116,46,49,57,54,51,50,48,49,49,32,110,97,109,101,46,49,57,54,52,50,48,49,50,32,97,114,103,46,49,57,54,53,50,48,49,51,32,111,112,116,105,111,110,115,46,49,57,54,54,50,48,49,52,32,111,112,101,114,97,110,100,115,46,49,57,54,55,50,48,49,53,41,0,0,0,0,0,0};
static C_char C_TLS li57[] C_aligned={C_lihdr(0,0,18),40,97,51,56,52,48,32,110,46,49,57,54,50,50,48,48,57,41,0,0,0,0,0,0};
static C_char C_TLS li58[] C_aligned={C_lihdr(0,0,7),40,97,51,56,53,54,41,0};
static C_char C_TLS li59[] C_aligned={C_lihdr(0,0,82),40,97,51,55,57,57,32,111,112,116,46,49,57,53,55,50,48,48,51,32,110,97,109,101,46,49,57,53,56,50,48,48,52,32,97,114,103,46,49,57,53,57,50,48,48,53,32,111,112,116,105,111,110,115,46,49,57,54,48,50,48,48,54,32,111,112,101,114,97,110,100,115,46,49,57,54,49,50,48,48,55,41,0,0,0,0,0,0};
static C_char C_TLS li60[] C_aligned={C_lihdr(0,0,18),40,97,51,57,49,52,32,110,46,49,57,53,54,50,48,48,49,41,0,0,0,0,0,0};
static C_char C_TLS li61[] C_aligned={C_lihdr(0,0,7),40,97,51,57,50,48,41,0};
static C_char C_TLS li62[] C_aligned={C_lihdr(0,0,82),40,97,51,56,55,51,32,111,112,116,46,49,57,53,49,49,57,57,53,32,110,97,109,101,46,49,57,53,50,49,57,57,54,32,97,114,103,46,49,57,53,51,49,57,57,55,32,111,112,116,105,111,110,115,46,49,57,53,52,49,57,57,56,32,111,112,101,114,97,110,100,115,46,49,57,53,53,49,57,57,57,41,0,0,0,0,0,0};
static C_char C_TLS li63[] C_aligned={C_lihdr(0,0,18),40,97,51,57,57,48,32,110,46,49,57,53,48,49,57,57,51,41,0,0,0,0,0,0};
static C_char C_TLS li64[] C_aligned={C_lihdr(0,0,82),40,97,51,57,52,57,32,111,112,116,46,49,57,52,53,49,57,56,55,32,110,97,109,101,46,49,57,52,54,49,57,56,56,32,97,114,103,46,49,57,52,55,49,57,56,57,32,111,112,116,105,111,110,115,46,49,57,52,56,49,57,57,48,32,111,112,101,114,97,110,100,115,46,49,57,52,57,49,57,57,49,41,0,0,0,0,0,0};
static C_char C_TLS li65[] C_aligned={C_lihdr(0,0,10),40,116,111,112,108,101,118,101,108,41,0,0,0,0,0,0};


C_noret_decl(C_toplevel)
C_externexport void C_ccall C_toplevel(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_2130)
static void C_ccall f_2130(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_2133)
static void C_ccall f_2133(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_2136)
static void C_ccall f_2136(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_2139)
static void C_ccall f_2139(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_2142)
static void C_ccall f_2142(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_2145)
static void C_ccall f_2145(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_2148)
static void C_ccall f_2148(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_2151)
static void C_ccall f_2151(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_2154)
static void C_ccall f_2154(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_2157)
static void C_ccall f_2157(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_2160)
static void C_ccall f_2160(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_2163)
static void C_ccall f_2163(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_2166)
static void C_ccall f_2166(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_2169)
static void C_ccall f_2169(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_3033)
static void C_ccall f_3033(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_3036)
static void C_ccall f_3036(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_3099)
static void C_ccall f_3099(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_3114)
static void C_ccall f_3114(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_3117)
static void C_ccall f_3117(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_3120)
static void C_ccall f_3120(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_4005)
static void C_ccall f_4005(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_3338)
static void C_ccall f_3338(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_3341)
static void C_ccall f_3341(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_3344)
static void C_ccall f_3344(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_3347)
static void C_ccall f_3347(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_3350)
static void C_ccall f_3350(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_3353)
static void C_ccall f_3353(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_3356)
static void C_ccall f_3356(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_3359)
static void C_ccall f_3359(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_3362)
static void C_ccall f_3362(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_3950)
static void C_ccall f_3950(C_word c,C_word t0,C_word t1,C_word t2,C_word t3,C_word t4,C_word t5,C_word t6) C_noret;
C_noret_decl(f_3978)
static void C_ccall f_3978(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_3991)
static void C_ccall f_3991(C_word c,C_word t0,C_word t1,C_word t2) C_noret;
C_noret_decl(f_3989)
static void C_ccall f_3989(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_3985)
static void C_ccall f_3985(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_3967)
static void C_ccall f_3967(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_3975)
static void C_ccall f_3975(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_3960)
static void C_ccall f_3960(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_3948)
static void C_ccall f_3948(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_3525)
static void C_ccall f_3525(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_3874)
static void C_ccall f_3874(C_word c,C_word t0,C_word t1,C_word t2,C_word t3,C_word t4,C_word t5,C_word t6) C_noret;
C_noret_decl(f_3921)
static void C_ccall f_3921(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_3929)
static void C_ccall f_3929(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_3902)
static void C_ccall f_3902(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_3915)
static void C_ccall f_3915(C_word c,C_word t0,C_word t1,C_word t2) C_noret;
C_noret_decl(f_3913)
static void C_ccall f_3913(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_3909)
static void C_ccall f_3909(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_3891)
static void C_ccall f_3891(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_3899)
static void C_ccall f_3899(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_3884)
static void C_ccall f_3884(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_3872)
static void C_ccall f_3872(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_3529)
static void C_ccall f_3529(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_3800)
static void C_ccall f_3800(C_word c,C_word t0,C_word t1,C_word t2,C_word t3,C_word t4,C_word t5,C_word t6) C_noret;
C_noret_decl(f_3857)
static void C_ccall f_3857(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_3862)
static void C_ccall f_3862(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_3852)
static void C_ccall f_3852(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_3828)
static void C_ccall f_3828(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_3841)
static void C_ccall f_3841(C_word c,C_word t0,C_word t1,C_word t2) C_noret;
C_noret_decl(f_3839)
static void C_ccall f_3839(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_3835)
static void C_ccall f_3835(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_3817)
static void C_ccall f_3817(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_3825)
static void C_ccall f_3825(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_3810)
static void C_ccall f_3810(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_3798)
static void C_ccall f_3798(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_3533)
static void C_ccall f_3533(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_3745)
static void C_ccall f_3745(C_word c,C_word t0,C_word t1,C_word t2,C_word t3,C_word t4,C_word t5,C_word t6) C_noret;
C_noret_decl(f_3784)
static void C_ccall f_3784(C_word c,C_word t0,C_word t1,C_word t2) C_noret;
C_noret_decl(f_3782)
static void C_ccall f_3782(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_3778)
static void C_ccall f_3778(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_3762)
static void C_ccall f_3762(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_3770)
static void C_ccall f_3770(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_3755)
static void C_ccall f_3755(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_3743)
static void C_ccall f_3743(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_3537)
static void C_ccall f_3537(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_3690)
static void C_ccall f_3690(C_word c,C_word t0,C_word t1,C_word t2,C_word t3,C_word t4,C_word t5,C_word t6) C_noret;
C_noret_decl(f_3729)
static void C_ccall f_3729(C_word c,C_word t0,C_word t1,C_word t2) C_noret;
C_noret_decl(f_3727)
static void C_ccall f_3727(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_3723)
static void C_ccall f_3723(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_3707)
static void C_ccall f_3707(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_3715)
static void C_ccall f_3715(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_3700)
static void C_ccall f_3700(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_3688)
static void C_ccall f_3688(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_3541)
static void C_ccall f_3541(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_3619)
static void C_ccall f_3619(C_word c,C_word t0,C_word t1,C_word t2,C_word t3,C_word t4,C_word t5,C_word t6) C_noret;
C_noret_decl(f_3678)
static void C_ccall f_3678(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_3647)
static void C_ccall f_3647(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_3669)
static void C_ccall f_3669(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_3674)
static void C_ccall f_3674(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_3650)
static void C_ccall f_3650(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_3663)
static void C_ccall f_3663(C_word c,C_word t0,C_word t1,C_word t2) C_noret;
C_noret_decl(f_3661)
static void C_ccall f_3661(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_3657)
static void C_ccall f_3657(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_3636)
static void C_ccall f_3636(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_3644)
static void C_ccall f_3644(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_3629)
static void C_ccall f_3629(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_3617)
static void C_ccall f_3617(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_3545)
static void C_ccall f_3545(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_3555)
static void C_ccall f_3555(C_word c,C_word t0,C_word t1,C_word t2,C_word t3,C_word t4,C_word t5,C_word t6) C_noret;
C_noret_decl(f_3602)
static void C_ccall f_3602(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_3607)
static void C_ccall f_3607(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_3583)
static void C_ccall f_3583(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_3596)
static void C_ccall f_3596(C_word c,C_word t0,C_word t1,C_word t2) C_noret;
C_noret_decl(f_3594)
static void C_ccall f_3594(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_3590)
static void C_ccall f_3590(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_3572)
static void C_ccall f_3572(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_3580)
static void C_ccall f_3580(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_3565)
static void C_ccall f_3565(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_3553)
static void C_ccall f_3553(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_3549)
static void C_ccall f_3549(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_3521)
static void C_ccall f_3521(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_3493)
static void C_ccall f_3493(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_3507)
static void C_fcall f_3507(C_word t0,C_word t1) C_noret;
C_noret_decl(f_3511)
static void C_ccall f_3511(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_3496)
static void C_ccall f_3496(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_3502)
static void C_ccall f_3502(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_3499)
static void C_ccall f_3499(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_3482)
static void C_ccall f_3482(C_word c,C_word t0,C_word t1,C_word t2,C_word t3) C_noret;
C_noret_decl(f_3444)
static void C_ccall f_3444(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_3448)
static void C_ccall f_3448(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_3451)
static void C_ccall f_3451(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_3454)
static void C_ccall f_3454(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_3457)
static void C_ccall f_3457(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_3466)
static void C_ccall f_3466(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_3399)
static void C_ccall f_3399(C_word c,C_word t0,C_word t1,C_word t2) C_noret;
C_noret_decl(f_3435)
static void C_ccall f_3435(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_3415)
static void C_ccall f_3415(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_3418)
static void C_ccall f_3418(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_3364)
static void C_ccall f_3364(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_3392)
static void C_ccall f_3392(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_3368)
static void C_ccall f_3368(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_3371)
static void C_ccall f_3371(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_3384)
static void C_ccall f_3384(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_3374)
static void C_ccall f_3374(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_3377)
static void C_ccall f_3377(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_3193)
static void C_ccall f_3193(C_word c,C_word t0,C_word t1,C_word t2,C_word t3,C_word t4) C_noret;
C_noret_decl(f_3203)
static void C_ccall f_3203(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_3266)
static void C_ccall f_3266(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_3323)
static void C_ccall f_3323(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_3301)
static void C_ccall f_3301(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_3288)
static void C_ccall f_3288(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_3206)
static void C_ccall f_3206(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_3197)
static void C_ccall f_3197(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_3139)
static void C_ccall f_3139(C_word c,C_word t0,C_word t1,C_word t2,C_word t3,C_word t4) C_noret;
C_noret_decl(f_3179)
static void C_ccall f_3179(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_3161)
static void C_ccall f_3161(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_3171)
static void C_ccall f_3171(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_3143)
static void C_ccall f_3143(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_3122)
static void C_ccall f_3122(C_word c,C_word t0,C_word t1,...) C_noret;
C_noret_decl(f_3122)
static void C_ccall f_3122r(C_word t0,C_word t1,C_word t3) C_noret;
C_noret_decl(f_3126)
static void C_ccall f_3126(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_3137)
static void C_ccall f_3137(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_3107)
static void C_ccall f_3107(C_word c,C_word t0,C_word t1,C_word t2) C_noret;
C_noret_decl(f_3101)
static void C_ccall f_3101(C_word c,C_word t0,C_word t1,C_word t2,C_word t3) C_noret;
C_noret_decl(f_3037)
static void C_ccall f_3037(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_3076)
static void C_fcall f_3076(C_word t0,C_word t1) C_noret;
C_noret_decl(f_3080)
static void C_ccall f_3080(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_3086)
static void C_ccall f_3086(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_3041)
static void C_ccall f_3041(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_3052)
static void C_fcall f_3052(C_word t0,C_word t1) C_noret;
C_noret_decl(f_3056)
static void C_ccall f_3056(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_3062)
static void C_ccall f_3062(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_3044)
static void C_ccall f_3044(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_2984)
static void C_ccall f_2984(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_2986)
static void C_ccall f_2986(C_word c,C_word t0,C_word t1,C_word t2) C_noret;
C_noret_decl(f_2993)
static void C_ccall f_2993(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_2998)
static void C_fcall f_2998(C_word t0,C_word t1) C_noret;
C_noret_decl(f_3002)
static void C_ccall f_3002(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_3022)
static void C_ccall f_3022(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_3017)
static void C_ccall f_3017(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_2788)
static void C_ccall f_2788(C_word c,C_word t0,C_word t1,C_word t2) C_noret;
C_noret_decl(f_2792)
static void C_ccall f_2792(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_2804)
static void C_fcall f_2804(C_word t0,C_word t1) C_noret;
C_noret_decl(f_2807)
static void C_ccall f_2807(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_2810)
static void C_ccall f_2810(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_2816)
static void C_ccall f_2816(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_2822)
static void C_ccall f_2822(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_2825)
static void C_ccall f_2825(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_2828)
static void C_ccall f_2828(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_2831)
static void C_ccall f_2831(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_2934)
static void C_ccall f_2934(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_2874)
static void C_fcall f_2874(C_word t0,C_word t1) C_noret;
C_noret_decl(f_2924)
static void C_ccall f_2924(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_2877)
static void C_fcall f_2877(C_word t0,C_word t1) C_noret;
C_noret_decl(f_2880)
static void C_ccall f_2880(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_2900)
static void C_ccall f_2900(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_2904)
static void C_ccall f_2904(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_2892)
static void C_ccall f_2892(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_2884)
static void C_ccall f_2884(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_2842)
static void C_fcall f_2842(C_word t0,C_word t1) C_noret;
C_noret_decl(f_2850)
static void C_ccall f_2850(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_2858)
static void C_ccall f_2858(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_2722)
static void C_ccall f_2722(C_word c,C_word t0,C_word t1,C_word t2,C_word t3,C_word t4) C_noret;
C_noret_decl(f_2726)
static void C_fcall f_2726(C_word t0,C_word t1) C_noret;
C_noret_decl(f_2773)
static void C_ccall f_2773(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_2769)
static void C_ccall f_2769(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_2733)
static void C_ccall f_2733(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_2753)
static void C_ccall f_2753(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_2669)
static void C_ccall f_2669(C_word c,C_word t0,C_word t1,C_word t2) C_noret;
C_noret_decl(f_2673)
static void C_ccall f_2673(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_2679)
static void C_fcall f_2679(C_word t0,C_word t1) C_noret;
C_noret_decl(f_2699)
static void C_ccall f_2699(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_2682)
static void C_ccall f_2682(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_2688)
static void C_ccall f_2688(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_2692)
static void C_ccall f_2692(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_2656)
static void C_ccall f_2656(C_word c,C_word t0,C_word t1,C_word t2) C_noret;
C_noret_decl(f_2662)
static void C_ccall f_2662(C_word c,C_word t0,C_word t1,C_word t2,C_word t3) C_noret;
C_noret_decl(f_2632)
static void C_ccall f_2632(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_2636)
static void C_ccall f_2636(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_2639)
static void C_ccall f_2639(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_2642)
static void C_ccall f_2642(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_2645)
static void C_ccall f_2645(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_2648)
static void C_ccall f_2648(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_2651)
static void C_ccall f_2651(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_2515)
static void C_ccall f_2515(C_word c,C_word t0,C_word t1,C_word t2,C_word t3,C_word t4) C_noret;
C_noret_decl(f_2519)
static void C_ccall f_2519(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_2522)
static void C_ccall f_2522(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_2525)
static void C_ccall f_2525(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_2528)
static void C_ccall f_2528(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_2531)
static void C_ccall f_2531(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_2534)
static void C_ccall f_2534(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_2537)
static void C_ccall f_2537(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_2540)
static void C_ccall f_2540(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_2554)
static void C_fcall f_2554(C_word t0,C_word t1,C_word t2) C_noret;
C_noret_decl(f_2564)
static void C_ccall f_2564(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_2543)
static void C_ccall f_2543(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_2546)
static void C_ccall f_2546(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_2549)
static void C_ccall f_2549(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_2480)
static void C_ccall f_2480(C_word c,C_word t0,C_word t1,C_word t2,C_word t3) C_noret;
C_noret_decl(f_2484)
static void C_ccall f_2484(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_2487)
static void C_ccall f_2487(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_2490)
static void C_ccall f_2490(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_2496)
static void C_ccall f_2496(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_2499)
static void C_ccall f_2499(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_2502)
static void C_ccall f_2502(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_2462)
static void C_ccall f_2462(C_word c,C_word t0,C_word t1,C_word t2,C_word t3,C_word t4,C_word t5) C_noret;
C_noret_decl(f_2466)
static void C_ccall f_2466(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_2469)
static void C_ccall f_2469(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_2472)
static void C_ccall f_2472(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_2475)
static void C_ccall f_2475(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_2420)
static void C_ccall f_2420(C_word c,C_word t0,C_word t1,C_word t2) C_noret;
C_noret_decl(f_2424)
static void C_ccall f_2424(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_2427)
static void C_ccall f_2427(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_2430)
static void C_ccall f_2430(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_2438)
static void C_ccall f_2438(C_word c,C_word t0,C_word t1,C_word t2) C_noret;
C_noret_decl(f_2442)
static void C_ccall f_2442(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_2445)
static void C_ccall f_2445(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_2433)
static void C_ccall f_2433(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_2400)
static void C_ccall f_2400(C_word c,C_word t0,C_word t1,C_word t2) C_noret;
C_noret_decl(f_2418)
static void C_ccall f_2418(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_2404)
static void C_ccall f_2404(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_2407)
static void C_ccall f_2407(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_2414)
static void C_ccall f_2414(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_2394)
static void C_ccall f_2394(C_word c,C_word t0,C_word t1,C_word t2,C_word t3) C_noret;
C_noret_decl(f_2369)
static void C_ccall f_2369(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_2373)
static void C_ccall f_2373(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_2378)
static void C_ccall f_2378(C_word c,C_word t0,C_word t1,C_word t2) C_noret;
C_noret_decl(f_2386)
static void C_ccall f_2386(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_2340)
static void C_ccall f_2340(C_word c,C_word t0,C_word t1,C_word t2) C_noret;
C_noret_decl(f_2363)
static void C_ccall f_2363(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_2344)
static void C_ccall f_2344(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_2284)
static void C_ccall f_2284(C_word c,C_word t0,C_word t1,C_word t2) C_noret;
C_noret_decl(f_2291)
static void C_fcall f_2291(C_word t0,C_word t1) C_noret;
C_noret_decl(f_2299)
static void C_ccall f_2299(C_word c,C_word t0,C_word t1,C_word t2) C_noret;
C_noret_decl(f_2252)
static void C_ccall f_2252(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_2258)
static void C_ccall f_2258(C_word c,C_word t0,C_word t1,C_word t2) C_noret;
C_noret_decl(f_2265)
static void C_fcall f_2265(C_word t0,C_word t1) C_noret;
C_noret_decl(f_2240)
static void C_ccall f_2240(C_word c,C_word t0,C_word t1,C_word t2,C_word t3,C_word t4,C_word t5) C_noret;
C_noret_decl(f_2226)
static void C_ccall f_2226(C_word c,C_word t0,C_word t1,C_word t2) C_noret;
C_noret_decl(f_2212)
static void C_ccall f_2212(C_word c,C_word t0,C_word t1,...) C_noret;
C_noret_decl(f_2212)
static void C_ccall f_2212r(C_word t0,C_word t1,C_word t3) C_noret;
C_noret_decl(f_2220)
static void C_ccall f_2220(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_2197)
static void C_ccall f_2197(C_word c,C_word t0,C_word t1,C_word t2) C_noret;
C_noret_decl(f_2201)
static void C_ccall f_2201(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_2206)
static void C_ccall f_2206(C_word c,C_word t0,C_word t1,C_word t2) C_noret;
C_noret_decl(f_2183)
static void C_ccall f_2183(C_word c,C_word t0,C_word t1,C_word t2) C_noret;
C_noret_decl(f_2195)
static void C_ccall f_2195(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_2191)
static void C_ccall f_2191(C_word c,C_word t0,C_word t1) C_noret;
C_noret_decl(f_2177)
static void C_ccall f_2177(C_word c,C_word t0,C_word t1,C_word t2) C_noret;
C_noret_decl(f_2171)
static void C_ccall f_2171(C_word c,C_word t0,C_word t1,C_word t2) C_noret;

C_noret_decl(trf_3507)
static void C_fcall trf_3507(void *dummy) C_regparm C_noret;
C_regparm static void C_fcall trf_3507(void *dummy){
C_word t1=C_pick(0);
C_word t0=C_pick(1);
C_adjust_stack(-2);
f_3507(t0,t1);}

C_noret_decl(trf_3076)
static void C_fcall trf_3076(void *dummy) C_regparm C_noret;
C_regparm static void C_fcall trf_3076(void *dummy){
C_word t1=C_pick(0);
C_word t0=C_pick(1);
C_adjust_stack(-2);
f_3076(t0,t1);}

C_noret_decl(trf_3052)
static void C_fcall trf_3052(void *dummy) C_regparm C_noret;
C_regparm static void C_fcall trf_3052(void *dummy){
C_word t1=C_pick(0);
C_word t0=C_pick(1);
C_adjust_stack(-2);
f_3052(t0,t1);}

C_noret_decl(trf_2998)
static void C_fcall trf_2998(void *dummy) C_regparm C_noret;
C_regparm static void C_fcall trf_2998(void *dummy){
C_word t1=C_pick(0);
C_word t0=C_pick(1);
C_adjust_stack(-2);
f_2998(t0,t1);}

C_noret_decl(trf_2804)
static void C_fcall trf_2804(void *dummy) C_regparm C_noret;
C_regparm static void C_fcall trf_2804(void *dummy){
C_word t1=C_pick(0);
C_word t0=C_pick(1);
C_adjust_stack(-2);
f_2804(t0,t1);}

C_noret_decl(trf_2874)
static void C_fcall trf_2874(void *dummy) C_regparm C_noret;
C_regparm static void C_fcall trf_2874(void *dummy){
C_word t1=C_pick(0);
C_word t0=C_pick(1);
C_adjust_stack(-2);
f_2874(t0,t1);}

C_noret_decl(trf_2877)
static void C_fcall trf_2877(void *dummy) C_regparm C_noret;
C_regparm static void C_fcall trf_2877(void *dummy){
C_word t1=C_pick(0);
C_word t0=C_pick(1);
C_adjust_stack(-2);
f_2877(t0,t1);}

C_noret_decl(trf_2842)
static void C_fcall trf_2842(void *dummy) C_regparm C_noret;
C_regparm static void C_fcall trf_2842(void *dummy){
C_word t1=C_pick(0);
C_word t0=C_pick(1);
C_adjust_stack(-2);
f_2842(t0,t1);}

C_noret_decl(trf_2726)
static void C_fcall trf_2726(void *dummy) C_regparm C_noret;
C_regparm static void C_fcall trf_2726(void *dummy){
C_word t1=C_pick(0);
C_word t0=C_pick(1);
C_adjust_stack(-2);
f_2726(t0,t1);}

C_noret_decl(trf_2679)
static void C_fcall trf_2679(void *dummy) C_regparm C_noret;
C_regparm static void C_fcall trf_2679(void *dummy){
C_word t1=C_pick(0);
C_word t0=C_pick(1);
C_adjust_stack(-2);
f_2679(t0,t1);}

C_noret_decl(trf_2554)
static void C_fcall trf_2554(void *dummy) C_regparm C_noret;
C_regparm static void C_fcall trf_2554(void *dummy){
C_word t2=C_pick(0);
C_word t1=C_pick(1);
C_word t0=C_pick(2);
C_adjust_stack(-3);
f_2554(t0,t1,t2);}

C_noret_decl(trf_2291)
static void C_fcall trf_2291(void *dummy) C_regparm C_noret;
C_regparm static void C_fcall trf_2291(void *dummy){
C_word t1=C_pick(0);
C_word t0=C_pick(1);
C_adjust_stack(-2);
f_2291(t0,t1);}

C_noret_decl(trf_2265)
static void C_fcall trf_2265(void *dummy) C_regparm C_noret;
C_regparm static void C_fcall trf_2265(void *dummy){
C_word t1=C_pick(0);
C_word t0=C_pick(1);
C_adjust_stack(-2);
f_2265(t0,t1);}

C_noret_decl(tr6)
static void C_fcall tr6(C_proc6 k) C_regparm C_noret;
C_regparm static void C_fcall tr6(C_proc6 k){
C_word t5=C_pick(0);
C_word t4=C_pick(1);
C_word t3=C_pick(2);
C_word t2=C_pick(3);
C_word t1=C_pick(4);
C_word t0=C_pick(5);
C_adjust_stack(-6);
(k)(6,t0,t1,t2,t3,t4,t5);}

C_noret_decl(tr5)
static void C_fcall tr5(C_proc5 k) C_regparm C_noret;
C_regparm static void C_fcall tr5(C_proc5 k){
C_word t4=C_pick(0);
C_word t3=C_pick(1);
C_word t2=C_pick(2);
C_word t1=C_pick(3);
C_word t0=C_pick(4);
C_adjust_stack(-5);
(k)(5,t0,t1,t2,t3,t4);}

C_noret_decl(tr4)
static void C_fcall tr4(C_proc4 k) C_regparm C_noret;
C_regparm static void C_fcall tr4(C_proc4 k){
C_word t3=C_pick(0);
C_word t2=C_pick(1);
C_word t1=C_pick(2);
C_word t0=C_pick(3);
C_adjust_stack(-4);
(k)(4,t0,t1,t2,t3);}

C_noret_decl(tr3)
static void C_fcall tr3(C_proc3 k) C_regparm C_noret;
C_regparm static void C_fcall tr3(C_proc3 k){
C_word t2=C_pick(0);
C_word t1=C_pick(1);
C_word t0=C_pick(2);
C_adjust_stack(-3);
(k)(3,t0,t1,t2);}

C_noret_decl(tr7)
static void C_fcall tr7(C_proc7 k) C_regparm C_noret;
C_regparm static void C_fcall tr7(C_proc7 k){
C_word t6=C_pick(0);
C_word t5=C_pick(1);
C_word t4=C_pick(2);
C_word t3=C_pick(3);
C_word t2=C_pick(4);
C_word t1=C_pick(5);
C_word t0=C_pick(6);
C_adjust_stack(-7);
(k)(7,t0,t1,t2,t3,t4,t5,t6);}

C_noret_decl(tr2)
static void C_fcall tr2(C_proc2 k) C_regparm C_noret;
C_regparm static void C_fcall tr2(C_proc2 k){
C_word t1=C_pick(0);
C_word t0=C_pick(1);
C_adjust_stack(-2);
(k)(2,t0,t1);}

C_noret_decl(tr2r)
static void C_fcall tr2r(C_proc2 k) C_regparm C_noret;
C_regparm static void C_fcall tr2r(C_proc2 k){
int n;
C_word *a,t2;
C_word t1=C_pick(0);
C_word t0=C_pick(1);
C_adjust_stack(-2);
n=C_rest_count(0);
a=C_alloc(n*3);
t2=C_restore_rest(a,n);
(k)(t0,t1,t2);}

/* toplevel */
static C_TLS int toplevel_initialized=0;
C_main_entry_point
C_noret_decl(toplevel_trampoline)
static void C_fcall toplevel_trampoline(void *dummy) C_regparm C_noret;
C_regparm static void C_fcall toplevel_trampoline(void *dummy){
C_toplevel(2,C_SCHEME_UNDEFINED,C_restore);}

void C_ccall C_toplevel(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word *a;
if(toplevel_initialized) C_kontinue(t1,C_SCHEME_UNDEFINED);
else C_toplevel_entry(C_text("toplevel"));
C_resize_stack(131072);
C_check_nursery_minimum(3);
if(!C_demand(3)){
C_save(t1);
C_reclaim((void*)toplevel_trampoline,NULL);}
toplevel_initialized=1;
if(!C_demand_2(1962)){
C_save(t1);
C_rereclaim2(1962*sizeof(C_word), 1);
t1=C_restore;}
a=C_alloc(3);
C_initialize_lf(lf,248);
lf[0]=C_h_intern(&lf[0],6,"square");
lf[1]=C_h_intern(&lf[1],4,"cube");
lf[2]=C_h_intern(&lf[2],9,"magnitude");
lf[3]=C_h_intern(&lf[3],1,"+");
lf[4]=C_h_intern(&lf[4],7,"\003sysmap");
lf[5]=C_h_intern(&lf[5],9,"normalize");
lf[6]=C_h_intern(&lf[6],3,"dot");
lf[7]=C_h_intern(&lf[7],3,"map");
lf[8]=C_h_intern(&lf[8],1,"*");
lf[9]=C_h_intern(&lf[9],2,"pi");
lf[10]=C_decode_literal(C_heaptop,"\376U3.14159265358979\000");
lf[11]=C_h_intern(&lf[11],7,"deg2rad");
lf[12]=C_h_intern(&lf[12],13,"horizdilution");
lf[13]=C_decode_literal(C_heaptop,"\376U1.5\000");
lf[14]=C_h_intern(&lf[14],6,"curpos");
lf[15]=C_decode_literal(C_heaptop,"\376\003\000\000\002\376\377\001\000\000\000\000\376\003\000\000\002\376\377\001\000\000\000\000\376\003\000\000\002\376\377\001\000\000\000\000\376\003\000\000\002\376\377\001\000\000\000\000\376\377\016");
lf[16]=C_h_intern(&lf[16],6,"points");
lf[17]=C_h_intern(&lf[17],9,"triangles");
lf[18]=C_h_intern(&lf[18],21,"trianglegrid-addpoint");
lf[19]=C_h_intern(&lf[19],14,"colorspace-min");
lf[20]=C_h_intern(&lf[20],14,"colorspace-max");
lf[21]=C_h_intern(&lf[21],17,"recalc-colorspace");
lf[22]=C_h_intern(&lf[22],19,"\003sysundefined-value");
lf[23]=C_h_intern(&lf[23],12,"\003sysfor-each");
lf[24]=C_h_intern(&lf[24],10,"colorspace");
lf[25]=C_decode_literal(C_heaptop,"\376U0.5\000");
lf[26]=C_decode_literal(C_heaptop,"\376\003\000\000\002\376\377\001\000\000\000\000\376\003\000\000\002\376U0.5\000\376\003\000\000\002\376\377\001\000\000\000\001\376\377\016");
lf[27]=C_decode_literal(C_heaptop,"\376U0.01\000");
lf[28]=C_h_intern(&lf[28],13,"make-gl-point");
lf[29]=C_h_intern(&lf[29],11,"gl:Vertex3f");
lf[30]=C_h_intern(&lf[30],10,"gl:Color3f");
lf[31]=C_h_intern(&lf[31],17,"trianglegrid-draw");
lf[32]=C_h_intern(&lf[32],8,"pos-draw");
lf[33]=C_decode_literal(C_heaptop,"\376U0.1\000");
lf[34]=C_h_intern(&lf[34],9,"point-pos");
lf[35]=C_h_intern(&lf[35],3,"car");
lf[36]=C_h_intern(&lf[36],11,"point-color");
lf[37]=C_h_intern(&lf[37],4,"cadr");
lf[38]=C_h_intern(&lf[38],9,"new-point");
lf[39]=C_h_intern(&lf[39],10,"draw-point");
lf[40]=C_h_intern(&lf[40],12,"gl:Vertex3fv");
lf[41]=C_h_intern(&lf[41],14,"draw-triangles");
lf[42]=C_h_intern(&lf[42],6,"gl:End");
lf[43]=C_h_intern(&lf[43],5,"point");
lf[44]=C_h_intern(&lf[44],8,"gl:Begin");
lf[45]=C_h_intern(&lf[45],12,"gl:TRIANGLES");
lf[46]=C_h_intern(&lf[46],3,"max");
lf[47]=C_h_intern(&lf[47],3,"min");
lf[48]=C_h_intern(&lf[48],11,"RotateAfter");
lf[49]=C_h_intern(&lf[49],14,"gl:MultMatrixf");
lf[50]=C_h_intern(&lf[50],10,"gl:Rotatef");
lf[51]=C_h_intern(&lf[51],15,"gl:LoadIdentity");
lf[52]=C_h_intern(&lf[52],12,"gl:GetFloatv");
lf[53]=C_h_intern(&lf[53],19,"gl:MODELVIEW_MATRIX");
lf[54]=C_h_intern(&lf[54],9,"f32vector");
lf[55]=C_h_intern(&lf[55],7,"reshape");
lf[56]=C_h_intern(&lf[56],13,"gl:Translatef");
lf[57]=C_decode_literal(C_heaptop,"\376U0.0\000");
lf[58]=C_decode_literal(C_heaptop,"\376U0.0\000");
lf[59]=C_decode_literal(C_heaptop,"\376U-10.0\000");
lf[60]=C_h_intern(&lf[60],13,"gl:MatrixMode");
lf[61]=C_h_intern(&lf[61],12,"gl:MODELVIEW");
lf[62]=C_decode_literal(C_heaptop,"\376U-0.1\000");
lf[63]=C_decode_literal(C_heaptop,"\376U0.1\000");
lf[64]=C_h_intern(&lf[64],10,"gl:Frustum");
lf[65]=C_decode_literal(C_heaptop,"\376U-0.1\000");
lf[66]=C_decode_literal(C_heaptop,"\376U0.1\000");
lf[67]=C_decode_literal(C_heaptop,"\376U0.1\000");
lf[68]=C_h_intern(&lf[68],13,"gl:PROJECTION");
lf[69]=C_h_intern(&lf[69],11,"gl:Viewport");
lf[70]=C_h_intern(&lf[70],10,"gl:Disable");
lf[71]=C_h_intern(&lf[71],8,"gl:BLEND");
lf[72]=C_h_intern(&lf[72],12,"gl:PopMatrix");
lf[73]=C_decode_literal(C_heaptop,"\376U0.1\000");
lf[74]=C_h_intern(&lf[74],10,"gl:Color4f");
lf[75]=C_decode_literal(C_heaptop,"\376U0.5\000");
lf[76]=C_decode_literal(C_heaptop,"\376U0.5\000");
lf[77]=C_decode_literal(C_heaptop,"\376U0.5\000");
lf[78]=C_h_intern(&lf[78],15,"gl:TRIANGLE_FAN");
lf[79]=C_h_intern(&lf[79],13,"gl:PushMatrix");
lf[80]=C_h_intern(&lf[80],9,"gl:Enable");
lf[81]=C_h_intern(&lf[81],12,"gl:BlendFunc");
lf[82]=C_h_intern(&lf[82],12,"gl:SRC_ALPHA");
lf[83]=C_h_intern(&lf[83],22,"gl:ONE_MINUS_SRC_ALPHA");
lf[84]=C_h_intern(&lf[84],10,"gl-display");
lf[85]=C_h_intern(&lf[85],16,"glut:SwapBuffers");
lf[86]=C_decode_literal(C_heaptop,"\376\003\000\000\002\376\377\001\000\000\000\001\376\003\000\000\002\376\377\001\000\000\000\000\376\003\000\000\002\376\377\001\000\000\000\000\376\377\016");
lf[87]=C_h_intern(&lf[87],8,"gl:Clear");
lf[88]=C_h_intern(&lf[88],19,"gl:COLOR_BUFFER_BIT");
lf[89]=C_h_intern(&lf[89],10,"none-false");
lf[90]=C_h_intern(&lf[90],4,"fold");
lf[91]=C_h_intern(&lf[91],6,"magmag");
lf[92]=C_h_intern(&lf[92],14,"parse-mag-line");
lf[93]=C_h_intern(&lf[93],14,"string->number");
lf[94]=C_h_intern(&lf[94],4,"take");
lf[95]=C_decode_literal(C_heaptop,"\376B\000\000\004mag:");
lf[96]=C_h_intern(&lf[96],12,"string-split");
lf[97]=C_h_intern(&lf[97],16,"convert-position");
lf[98]=C_h_intern(&lf[98],14,"parse-gps-line");
lf[99]=C_h_intern(&lf[99],5,"print");
lf[100]=C_decode_literal(C_heaptop,"\376B\000\000\022no gps fix, sats: ");
lf[101]=C_h_intern(&lf[101],9,"remainder");
lf[102]=C_h_intern(&lf[102],5,"floor");
lf[103]=C_decode_literal(C_heaptop,"\376B\000\0001warning, only normal GPS not DGPS fix, discarding");
lf[104]=C_h_intern(&lf[104],10,"reportdata");
lf[105]=C_h_intern(&lf[105],6,"append");
lf[106]=C_h_intern(&lf[106],7,"verbose");
lf[107]=C_decode_literal(C_heaptop,"\376B\000\000\011 GPS fix ");
lf[108]=C_decode_literal(C_heaptop,"\376B\000\000\011DGPS fix ");
lf[109]=C_decode_literal(C_heaptop,"\376B\000\000\002N ");
lf[110]=C_decode_literal(C_heaptop,"\376B\000\000\002E ");
lf[111]=C_decode_literal(C_heaptop,"\376B\000\000\002M ");
lf[112]=C_decode_literal(C_heaptop,"\376B\000\000\004 HD ");
lf[113]=C_decode_literal(C_heaptop,"\376B\000\000\005 sats");
lf[114]=C_decode_literal(C_heaptop,"\376B\000\000\001W");
lf[115]=C_decode_literal(C_heaptop,"\376B\000\000\001S");
lf[116]=C_decode_literal(C_heaptop,"\376B\000\000\006$GPGGA");
lf[117]=C_decode_literal(C_heaptop,"\376B\000\000\001,");
lf[118]=C_h_intern(&lf[118],16,"make-line-reader");
lf[119]=C_decode_literal(C_heaptop,"\376B\000\000\000");
lf[120]=C_decode_literal(C_heaptop,"\376B\000\000\000");
lf[121]=C_h_intern(&lf[121],11,"string-copy");
lf[122]=C_h_intern(&lf[122],13,"string-append");
lf[123]=C_h_intern(&lf[123],18,"\003sysread-char/port");
lf[124]=C_h_intern(&lf[124],11,"char-ready\077");
lf[125]=C_h_intern(&lf[125],4,"idle");
lf[126]=C_h_intern(&lf[126],13,"thread-sleep!");
lf[127]=C_decode_literal(C_heaptop,"\376U0.001\000");
lf[128]=C_h_intern(&lf[128],10,"gps-device");
lf[129]=C_h_intern(&lf[129],10,"mag-device");
lf[130]=C_h_intern(&lf[130],10,"attributes");
lf[131]=C_h_intern(&lf[131],9,"attr-set!");
lf[132]=C_h_intern(&lf[132],15,"hash-table-set!");
lf[133]=C_h_intern(&lf[133],8,"attr-get");
lf[134]=C_h_intern(&lf[134],14,"hash-table-ref");
lf[135]=C_h_intern(&lf[135],17,"glut-HasModifiers");
lf[136]=C_h_intern(&lf[136],17,"glut:GetModifiers");
lf[137]=C_h_intern(&lf[137],11,"bitwise-ior");
lf[138]=C_h_intern(&lf[138],8,"keyboard");
lf[139]=C_h_intern(&lf[139],18,"glut:PostRedisplay");
lf[140]=C_h_intern(&lf[140],4,"exit");
lf[141]=C_h_intern(&lf[141],7,"gl:LINE");
lf[142]=C_h_intern(&lf[142],7,"gl:FILL");
lf[143]=C_h_intern(&lf[143],14,"gl:PolygonMode");
lf[144]=C_h_intern(&lf[144],17,"gl:FRONT_AND_BACK");
lf[145]=C_decode_literal(C_heaptop,"\376B\000\000\010linemode");
lf[146]=C_decode_literal(C_heaptop,"\376B\000\000\010linemode");
lf[147]=C_decode_literal(C_heaptop,"\376B\000\000\010linemode");
lf[148]=C_h_intern(&lf[148],15,"glut:FullScreen");
lf[149]=C_h_intern(&lf[149],7,"special");
lf[150]=C_h_intern(&lf[150],13,"glut:KEY_LEFT");
lf[151]=C_h_intern(&lf[151],14,"glut:KEY_RIGHT");
lf[152]=C_h_intern(&lf[152],11,"glut:KEY_UP");
lf[153]=C_h_intern(&lf[153],13,"glut:KEY_DOWN");
lf[154]=C_h_intern(&lf[154],16,"glut:KEY_PAGE_UP");
lf[155]=C_h_intern(&lf[155],18,"glut:KEY_PAGE_DOWN");
lf[156]=C_decode_literal(C_heaptop,"\376B\000\000\013rotatespeed");
lf[157]=C_decode_literal(C_heaptop,"\376B\000\000\016translatespeed");
lf[158]=C_h_intern(&lf[158],17,"glut:ACTIVE_SHIFT");
lf[159]=C_h_intern(&lf[159],5,"usage");
lf[160]=C_decode_literal(C_heaptop,"\376B\000\000%Report bugs to geckosenator at gmail.");
lf[161]=C_h_intern(&lf[161],10,"args:usage");
lf[162]=C_h_intern(&lf[162],4,"opts");
lf[163]=C_h_intern(&lf[163],7,"newline");
lf[164]=C_decode_literal(C_heaptop,"\376B\000\000\007Usage: ");
lf[165]=C_decode_literal(C_heaptop,"\376B\000\000\030 [options...] [files...]");
lf[166]=C_h_intern(&lf[166],4,"argv");
lf[167]=C_h_intern(&lf[167],11,"output-file");
lf[168]=C_h_intern(&lf[168],6,"noshow");
lf[169]=C_decode_literal(C_heaptop,"\376B\000\000\001 ");
lf[170]=C_decode_literal(C_heaptop,"\376B\000\000\001 ");
lf[171]=C_decode_literal(C_heaptop,"\376B\000\000\001 ");
lf[172]=C_h_intern(&lf[172],19,"with-output-to-port");
lf[173]=C_h_intern(&lf[173],9,"load-data");
lf[174]=C_h_intern(&lf[174],4,"read");
lf[175]=C_h_intern(&lf[175],16,"handle-open-file");
lf[176]=C_h_intern(&lf[176],25,"\003sysimplicit-exit-handler");
lf[177]=C_h_intern(&lf[177],13,"glut:MainLoop");
lf[178]=C_h_intern(&lf[178],10,"args:parse");
lf[179]=C_h_intern(&lf[179],22,"command-line-arguments");
lf[180]=C_h_intern(&lf[180],16,"make-args:option");
lf[181]=C_decode_literal(C_heaptop,"\376B\000\000\006DEVICE");
lf[182]=C_decode_literal(C_heaptop,"\376B\000\000\034device for magnetometer data");
lf[183]=C_h_intern(&lf[183],7,"fprintf");
lf[184]=C_decode_literal(C_heaptop,"\376B\000\000#~A: option ~A requires an argument\012");
lf[185]=C_h_intern(&lf[185],18,"current-error-port");
lf[186]=C_decode_literal(C_heaptop,"\376\003\000\000\002\376\001\000\000\001m\376\003\000\000\002\376\001\000\000\003mag\376\377\016");
lf[187]=C_h_intern(&lf[187],15,"open-input-file");
lf[188]=C_h_intern(&lf[188],6,"option");
lf[189]=C_decode_literal(C_heaptop,"\376\003\000\000\002\376\377\012\000\000m\376\003\000\000\002\376B\000\000\003mag\376\377\016");
lf[190]=C_decode_literal(C_heaptop,"\376B\000\000\006DEVICE");
lf[191]=C_decode_literal(C_heaptop,"\376B\000\000\023device for gps data");
lf[192]=C_decode_literal(C_heaptop,"\376\003\000\000\002\376\001\000\000\001g\376\003\000\000\002\376\001\000\000\003gps\376\377\016");
lf[193]=C_h_intern(&lf[193],6,"system");
lf[194]=C_decode_literal(C_heaptop,"\376B\000\000\010stty -F ");
lf[195]=C_decode_literal(C_heaptop,"\376B\000\000\006 19200");
lf[196]=C_decode_literal(C_heaptop,"\376\003\000\000\002\376\377\012\000\000g\376\003\000\000\002\376B\000\000\003gps\376\377\016");
lf[197]=C_decode_literal(C_heaptop,"\376B\000\000\003ARG");
lf[198]=C_decode_literal(C_heaptop,"\376B\000\000\026Don\047t display anything");
lf[199]=C_h_intern(&lf[199],5,"\000node");
lf[200]=C_h_intern(&lf[200],9,"\000required");
lf[201]=C_decode_literal(C_heaptop,"\376\003\000\000\002\376\001\000\000\001v\376\003\000\000\002\376\001\000\000\007verbose\376\377\016");
lf[202]=C_decode_literal(C_heaptop,"\376\003\000\000\002\376\377\012\000\000v\376\003\000\000\002\376B\000\000\007verbose\376\377\016");
lf[203]=C_decode_literal(C_heaptop,"\376B\000\000\026Don\047t display anything");
lf[204]=C_decode_literal(C_heaptop,"\376\003\000\000\002\376\001\000\000\001n\376\003\000\000\002\376\001\000\000\006noshow\376\377\016");
lf[205]=C_decode_literal(C_heaptop,"\376\003\000\000\002\376\377\012\000\000n\376\003\000\000\002\376B\000\000\006noshow\376\377\016");
lf[206]=C_decode_literal(C_heaptop,"\376B\000\000\010FILENAME");
lf[207]=C_decode_literal(C_heaptop,"\376B\000\000\026write output to a file");
lf[208]=C_decode_literal(C_heaptop,"\376\003\000\000\002\376\001\000\000\001o\376\003\000\000\002\376\001\000\000\006output\376\377\016");
lf[209]=C_decode_literal(C_heaptop,"\376B\000\000\001-");
lf[210]=C_h_intern(&lf[210],19,"current-output-port");
lf[211]=C_h_intern(&lf[211],16,"open-output-file");
lf[212]=C_decode_literal(C_heaptop,"\376\003\000\000\002\376\377\012\000\000o\376\003\000\000\002\376B\000\000\006output\376\377\016");
lf[213]=C_decode_literal(C_heaptop,"\376B\000\000\010FILENAME");
lf[214]=C_decode_literal(C_heaptop,"\376B\000\000\026read input from a file");
lf[215]=C_decode_literal(C_heaptop,"\376\003\000\000\002\376\001\000\000\001f\376\003\000\000\002\376\001\000\000\004file\376\377\016");
lf[216]=C_h_intern(&lf[216],20,"with-input-from-port");
lf[217]=C_decode_literal(C_heaptop,"\376B\000\000\001-");
lf[218]=C_h_intern(&lf[218],18,"current-input-port");
lf[219]=C_decode_literal(C_heaptop,"\376\003\000\000\002\376\377\012\000\000f\376\003\000\000\002\376B\000\000\004file\376\377\016");
lf[220]=C_decode_literal(C_heaptop,"\376B\000\000\021Display this text");
lf[221]=C_h_intern(&lf[221],5,"\000none");
lf[222]=C_decode_literal(C_heaptop,"\376\003\000\000\002\376\001\000\000\001h\376\003\000\000\002\376\001\000\000\004help\376\377\016");
lf[223]=C_decode_literal(C_heaptop,"\376\003\000\000\002\376\377\012\000\000h\376\003\000\000\002\376B\000\000\004help\376\377\016");
lf[224]=C_h_intern(&lf[224],11,"\003sysrequire");
lf[225]=C_h_intern(&lf[225],12,"args-support");
lf[226]=C_h_intern(&lf[226],13,"gl:ClearColor");
lf[227]=C_decode_literal(C_heaptop,"\376U0.0\000");
lf[228]=C_decode_literal(C_heaptop,"\376U0.0\000");
lf[229]=C_decode_literal(C_heaptop,"\376U0.0\000");
lf[230]=C_h_intern(&lf[230],16,"glut:SpecialFunc");
lf[231]=C_h_intern(&lf[231],17,"glut:KeyboardFunc");
lf[232]=C_h_intern(&lf[232],13,"glut:IdleFunc");
lf[233]=C_h_intern(&lf[233],16,"glut:DisplayFunc");
lf[234]=C_h_intern(&lf[234],16,"glut:ReshapeFunc");
lf[235]=C_h_intern(&lf[235],17,"glut:CreateWindow");
lf[236]=C_decode_literal(C_heaptop,"\376B\000\000\007magplot");
lf[237]=C_h_intern(&lf[237],20,"glut:InitDisplayMode");
lf[238]=C_h_intern(&lf[238],11,"glut:DOUBLE");
lf[239]=C_h_intern(&lf[239],8,"glut:RGB");
lf[240]=C_h_intern(&lf[240],10,"glut:ALPHA");
lf[241]=C_decode_literal(C_heaptop,"\376B\000\000\016translatespeed");
lf[242]=C_decode_literal(C_heaptop,"\376B\000\000\013rotatespeed");
lf[243]=C_decode_literal(C_heaptop,"\376B\000\000\010linemode");
lf[244]=C_h_intern(&lf[244],15,"make-hash-table");
lf[245]=C_h_intern(&lf[245],11,"pos-support");
lf[246]=C_h_intern(&lf[246],4,"glut");
lf[247]=C_h_intern(&lf[247],2,"gl");
C_register_lf2(lf,248,create_ptable());
t2=(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_2130,a[2]=t1,tmp=(C_word)a,a+=3,tmp);
C_library_toplevel(2,C_SCHEME_UNDEFINED,t2);}

/* k2128 */
static void C_ccall f_2130(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word ab[3],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_2130,2,t0,t1);}
t2=(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_2133,a[2]=((C_word*)t0)[2],tmp=(C_word)a,a+=3,tmp);
C_eval_toplevel(2,C_SCHEME_UNDEFINED,t2);}

/* k2131 in k2128 */
static void C_ccall f_2133(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word ab[3],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_2133,2,t0,t1);}
t2=(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_2136,a[2]=((C_word*)t0)[2],tmp=(C_word)a,a+=3,tmp);
C_data_structures_toplevel(2,C_SCHEME_UNDEFINED,t2);}

/* k2134 in k2131 in k2128 */
static void C_ccall f_2136(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word ab[3],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_2136,2,t0,t1);}
t2=(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_2139,a[2]=((C_word*)t0)[2],tmp=(C_word)a,a+=3,tmp);
C_ports_toplevel(2,C_SCHEME_UNDEFINED,t2);}

/* k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_2139(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word ab[3],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_2139,2,t0,t1);}
t2=(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_2142,a[2]=((C_word*)t0)[2],tmp=(C_word)a,a+=3,tmp);
C_extras_toplevel(2,C_SCHEME_UNDEFINED,t2);}

/* k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_2142(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word ab[3],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_2142,2,t0,t1);}
t2=(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_2145,a[2]=((C_word*)t0)[2],tmp=(C_word)a,a+=3,tmp);
C_srfi_69_toplevel(2,C_SCHEME_UNDEFINED,t2);}

/* k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_2145(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word ab[3],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_2145,2,t0,t1);}
t2=(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_2148,a[2]=((C_word*)t0)[2],tmp=(C_word)a,a+=3,tmp);
C_srfi_1_toplevel(2,C_SCHEME_UNDEFINED,t2);}

/* k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_2148(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word ab[3],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_2148,2,t0,t1);}
t2=(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_2151,a[2]=((C_word*)t0)[2],tmp=(C_word)a,a+=3,tmp);
C_srfi_4_toplevel(2,C_SCHEME_UNDEFINED,t2);}

/* k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_2151(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word ab[3],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_2151,2,t0,t1);}
t2=(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_2154,a[2]=((C_word*)t0)[2],tmp=(C_word)a,a+=3,tmp);
C_posix_toplevel(2,C_SCHEME_UNDEFINED,t2);}

/* k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_2154(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word ab[3],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_2154,2,t0,t1);}
t2=(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_2157,a[2]=((C_word*)t0)[2],tmp=(C_word)a,a+=3,tmp);
C_srfi_18_toplevel(2,C_SCHEME_UNDEFINED,t2);}

/* k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_2157(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word ab[3],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_2157,2,t0,t1);}
t2=(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_2160,a[2]=((C_word*)t0)[2],tmp=(C_word)a,a+=3,tmp);
C_srfi_69_toplevel(2,C_SCHEME_UNDEFINED,t2);}

/* k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_2160(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word ab[3],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_2160,2,t0,t1);}
t2=(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_2163,a[2]=((C_word*)t0)[2],tmp=(C_word)a,a+=3,tmp);
C_trace("##sys#require");
t3=C_retrieve(lf[224]);
((C_proc3)C_retrieve_proc(t3))(3,t3,t2,lf[247]);}

/* k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_2163(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word ab[3],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_2163,2,t0,t1);}
t2=(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_2166,a[2]=((C_word*)t0)[2],tmp=(C_word)a,a+=3,tmp);
C_trace("##sys#require");
t3=C_retrieve(lf[224]);
((C_proc3)C_retrieve_proc(t3))(3,t3,t2,lf[246]);}

/* k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_2166(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word ab[3],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_2166,2,t0,t1);}
t2=(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_2169,a[2]=((C_word*)t0)[2],tmp=(C_word)a,a+=3,tmp);
C_trace("##sys#require");
t3=C_retrieve(lf[224]);
((C_proc3)C_retrieve_proc(t3))(3,t3,t2,lf[245]);}

/* k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_2169(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word t4;
C_word t5;
C_word t6;
C_word t7;
C_word t8;
C_word t9;
C_word t10;
C_word t11;
C_word t12;
C_word t13;
C_word t14;
C_word t15;
C_word t16;
C_word t17;
C_word t18;
C_word t19;
C_word t20;
C_word t21;
C_word t22;
C_word t23;
C_word t24;
C_word t25;
C_word t26;
C_word t27;
C_word t28;
C_word t29;
C_word t30;
C_word t31;
C_word t32;
C_word t33;
C_word t34;
C_word t35;
C_word t36;
C_word t37;
C_word t38;
C_word t39;
C_word t40;
C_word t41;
C_word t42;
C_word ab[81],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_2169,2,t0,t1);}
t2=C_mutate((C_word*)lf[0]+1,(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_2171,a[2]=((C_word)li0),tmp=(C_word)a,a+=3,tmp));
t3=C_mutate((C_word*)lf[1]+1,(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_2177,a[2]=((C_word)li1),tmp=(C_word)a,a+=3,tmp));
t4=C_mutate((C_word*)lf[2]+1,(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_2183,a[2]=((C_word)li2),tmp=(C_word)a,a+=3,tmp));
t5=C_mutate((C_word*)lf[5]+1,(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_2197,a[2]=((C_word)li4),tmp=(C_word)a,a+=3,tmp));
t6=C_mutate((C_word*)lf[6]+1,(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_2212,a[2]=((C_word)li5),tmp=(C_word)a,a+=3,tmp));
t7=C_mutate((C_word*)lf[9]+1,lf[10]);
t8=C_mutate((C_word*)lf[11]+1,(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_2226,a[2]=((C_word)li6),tmp=(C_word)a,a+=3,tmp));
t9=C_mutate((C_word*)lf[12]+1,lf[13]);
t10=C_mutate((C_word*)lf[14]+1,lf[15]);
t11=C_set_block_item(lf[16],0,C_SCHEME_END_OF_LIST);
t12=C_set_block_item(lf[17],0,C_SCHEME_END_OF_LIST);
t13=C_mutate((C_word*)lf[18]+1,(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_2240,a[2]=((C_word)li7),tmp=(C_word)a,a+=3,tmp));
t14=C_set_block_item(lf[19],0,C_fix(1));
t15=C_set_block_item(lf[20],0,C_fix(0));
t16=C_mutate((C_word*)lf[21]+1,(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_2252,a[2]=((C_word)li9),tmp=(C_word)a,a+=3,tmp));
t17=C_mutate((C_word*)lf[24]+1,(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_2284,a[2]=((C_word)li11),tmp=(C_word)a,a+=3,tmp));
t18=C_mutate((C_word*)lf[28]+1,(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_2340,a[2]=((C_word)li12),tmp=(C_word)a,a+=3,tmp));
t19=C_mutate((C_word*)lf[31]+1,(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_2369,a[2]=((C_word)li14),tmp=(C_word)a,a+=3,tmp));
t20=C_mutate((C_word*)lf[34]+1,*((C_word*)lf[35]+1));
t21=C_mutate((C_word*)lf[36]+1,*((C_word*)lf[37]+1));
t22=C_mutate((C_word*)lf[38]+1,(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_2394,a[2]=((C_word)li15),tmp=(C_word)a,a+=3,tmp));
t23=C_mutate((C_word*)lf[39]+1,(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_2400,a[2]=((C_word)li16),tmp=(C_word)a,a+=3,tmp));
t24=C_mutate((C_word*)lf[41]+1,(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_2420,a[2]=((C_word)li18),tmp=(C_word)a,a+=3,tmp));
t25=C_mutate((C_word*)lf[48]+1,(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_2462,a[2]=((C_word)li19),tmp=(C_word)a,a+=3,tmp));
t26=C_mutate((C_word*)lf[55]+1,(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_2480,a[2]=((C_word)li20),tmp=(C_word)a,a+=3,tmp));
t27=C_mutate((C_word*)lf[32]+1,(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_2515,a[2]=((C_word)li22),tmp=(C_word)a,a+=3,tmp));
t28=C_mutate((C_word*)lf[84]+1,(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_2632,a[2]=((C_word)li23),tmp=(C_word)a,a+=3,tmp));
t29=C_mutate((C_word*)lf[89]+1,(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_2656,a[2]=((C_word)li25),tmp=(C_word)a,a+=3,tmp));
t30=C_set_block_item(lf[91],0,C_fix(0));
t31=C_mutate((C_word*)lf[92]+1,(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_2669,a[2]=((C_word)li26),tmp=(C_word)a,a+=3,tmp));
t32=C_fix(100);
t33=(*a=C_VECTOR_TYPE|1,a[1]=t32,tmp=(C_word)a,a+=2,tmp);
t34=C_fix(100);
t35=(*a=C_VECTOR_TYPE|1,a[1]=t34,tmp=(C_word)a,a+=2,tmp);
t36=C_fix(100);
t37=(*a=C_VECTOR_TYPE|1,a[1]=t36,tmp=(C_word)a,a+=2,tmp);
t38=C_mutate((C_word*)lf[97]+1,(*a=C_CLOSURE_TYPE|5,a[1]=(C_word)f_2722,a[2]=t35,a[3]=t33,a[4]=t37,a[5]=((C_word)li27),tmp=(C_word)a,a+=6,tmp));
t39=C_mutate((C_word*)lf[98]+1,(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_2788,a[2]=((C_word)li29),tmp=(C_word)a,a+=3,tmp));
t40=C_mutate((C_word*)lf[118]+1,(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_2984,a[2]=((C_word)li32),tmp=(C_word)a,a+=3,tmp));
t41=(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_3033,a[2]=((C_word*)t0)[2],tmp=(C_word)a,a+=3,tmp);
C_trace("make-line-reader");
t42=C_retrieve(lf[118]);
((C_proc2)C_retrieve_proc(t42))(2,t42,t41);}

/* k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3033(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word ab[4],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_3033,2,t0,t1);}
t2=(*a=C_CLOSURE_TYPE|3,a[1]=(C_word)f_3036,a[2]=((C_word*)t0)[2],a[3]=t1,tmp=(C_word)a,a+=4,tmp);
C_trace("make-line-reader");
t3=C_retrieve(lf[118]);
((C_proc2)C_retrieve_proc(t3))(2,t3,t2);}

/* k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3036(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word t4;
C_word ab[8],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_3036,2,t0,t1);}
t2=C_mutate((C_word*)lf[125]+1,(*a=C_CLOSURE_TYPE|4,a[1]=(C_word)f_3037,a[2]=((C_word*)t0)[3],a[3]=t1,a[4]=((C_word)li35),tmp=(C_word)a,a+=5,tmp));
t3=(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_3099,a[2]=((C_word*)t0)[2],tmp=(C_word)a,a+=3,tmp);
C_trace("make-hash-table");
t4=C_retrieve(lf[244]);
((C_proc2)C_retrieve_proc(t4))(2,t4,t3);}

/* k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3099(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word t4;
C_word t5;
C_word t6;
C_word ab[9],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_3099,2,t0,t1);}
t2=C_mutate((C_word*)lf[130]+1,t1);
t3=C_mutate((C_word*)lf[131]+1,(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_3101,a[2]=((C_word)li36),tmp=(C_word)a,a+=3,tmp));
t4=C_mutate((C_word*)lf[133]+1,(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_3107,a[2]=((C_word)li37),tmp=(C_word)a,a+=3,tmp));
t5=(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_3114,a[2]=((C_word*)t0)[2],tmp=(C_word)a,a+=3,tmp);
C_trace("attr-set!");
t6=C_retrieve(lf[131]);
((C_proc4)C_retrieve_proc(t6))(4,t6,t5,lf[243],C_SCHEME_FALSE);}

/* k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3114(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word ab[3],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_3114,2,t0,t1);}
t2=(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_3117,a[2]=((C_word*)t0)[2],tmp=(C_word)a,a+=3,tmp);
C_trace("attr-set!");
t3=C_retrieve(lf[131]);
((C_proc4)C_retrieve_proc(t3))(4,t3,t2,lf[242],C_fix(5));}

/* k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3117(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word ab[3],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_3117,2,t0,t1);}
t2=(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_3120,a[2]=((C_word*)t0)[2],tmp=(C_word)a,a+=3,tmp);
C_trace("attr-set!");
t3=C_retrieve(lf[131]);
((C_proc4)C_retrieve_proc(t3))(4,t3,t2,lf[241],C_fix(1));}

/* k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3120(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word t4;
C_word t5;
C_word t6;
C_word t7;
C_word ab[15],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_3120,2,t0,t1);}
t2=C_mutate((C_word*)lf[135]+1,(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_3122,a[2]=((C_word)li38),tmp=(C_word)a,a+=3,tmp));
t3=C_mutate((C_word*)lf[138]+1,(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_3139,a[2]=((C_word)li39),tmp=(C_word)a,a+=3,tmp));
t4=C_mutate((C_word*)lf[149]+1,(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_3193,a[2]=((C_word)li40),tmp=(C_word)a,a+=3,tmp));
t5=(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_3338,a[2]=((C_word*)t0)[2],tmp=(C_word)a,a+=3,tmp);
t6=(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_4005,a[2]=t5,tmp=(C_word)a,a+=3,tmp);
C_trace("+");
C_plus(5,0,t6,C_retrieve(lf[238]),C_retrieve(lf[239]),C_retrieve(lf[240]));}

/* k4003 in k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_4005(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word *a;
C_trace("glut:InitDisplayMode");
t2=C_retrieve(lf[237]);
((C_proc3)C_retrieve_proc(t2))(3,t2,((C_word*)t0)[2],t1);}

/* k3336 in k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3338(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word ab[3],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_3338,2,t0,t1);}
t2=(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_3341,a[2]=((C_word*)t0)[2],tmp=(C_word)a,a+=3,tmp);
C_trace("glut:CreateWindow");
t3=C_retrieve(lf[235]);
((C_proc3)C_retrieve_proc(t3))(3,t3,t2,lf[236]);}

/* k3339 in k3336 in k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3341(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word ab[3],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_3341,2,t0,t1);}
t2=(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_3344,a[2]=((C_word*)t0)[2],tmp=(C_word)a,a+=3,tmp);
C_trace("glut:ReshapeFunc");
t3=C_retrieve(lf[234]);
((C_proc3)C_retrieve_proc(t3))(3,t3,t2,C_retrieve(lf[55]));}

/* k3342 in k3339 in k3336 in k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3344(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word ab[3],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_3344,2,t0,t1);}
t2=(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_3347,a[2]=((C_word*)t0)[2],tmp=(C_word)a,a+=3,tmp);
C_trace("glut:DisplayFunc");
t3=C_retrieve(lf[233]);
((C_proc3)C_retrieve_proc(t3))(3,t3,t2,C_retrieve(lf[84]));}

/* k3345 in k3342 in k3339 in k3336 in k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3347(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word ab[3],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_3347,2,t0,t1);}
t2=(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_3350,a[2]=((C_word*)t0)[2],tmp=(C_word)a,a+=3,tmp);
C_trace("glut:IdleFunc");
t3=C_retrieve(lf[232]);
((C_proc3)C_retrieve_proc(t3))(3,t3,t2,C_retrieve(lf[125]));}

/* k3348 in k3345 in k3342 in k3339 in k3336 in k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3350(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word ab[3],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_3350,2,t0,t1);}
t2=(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_3353,a[2]=((C_word*)t0)[2],tmp=(C_word)a,a+=3,tmp);
C_trace("glut:KeyboardFunc");
t3=C_retrieve(lf[231]);
((C_proc3)C_retrieve_proc(t3))(3,t3,t2,C_retrieve(lf[138]));}

/* k3351 in k3348 in k3345 in k3342 in k3339 in k3336 in k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3353(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word ab[3],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_3353,2,t0,t1);}
t2=(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_3356,a[2]=((C_word*)t0)[2],tmp=(C_word)a,a+=3,tmp);
C_trace("glut:SpecialFunc");
t3=C_retrieve(lf[230]);
((C_proc3)C_retrieve_proc(t3))(3,t3,t2,C_retrieve(lf[149]));}

/* k3354 in k3351 in k3348 in k3345 in k3342 in k3339 in k3336 in k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3356(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word ab[3],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_3356,2,t0,t1);}
t2=(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_3359,a[2]=((C_word*)t0)[2],tmp=(C_word)a,a+=3,tmp);
C_trace("gl:ClearColor");
t3=C_retrieve(lf[226]);
((C_proc6)C_retrieve_proc(t3))(6,t3,t2,lf[227],lf[228],lf[229],C_fix(0));}

/* k3357 in k3354 in k3351 in k3348 in k3345 in k3342 in k3339 in k3336 in k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3359(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word ab[3],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_3359,2,t0,t1);}
t2=(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_3362,a[2]=((C_word*)t0)[2],tmp=(C_word)a,a+=3,tmp);
C_trace("##sys#require");
t3=C_retrieve(lf[224]);
((C_proc3)C_retrieve_proc(t3))(3,t3,t2,lf[225]);}

/* k3360 in k3357 in k3354 in k3351 in k3348 in k3345 in k3342 in k3339 in k3336 in k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3362(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word t4;
C_word t5;
C_word t6;
C_word t7;
C_word t8;
C_word t9;
C_word t10;
C_word t11;
C_word t12;
C_word t13;
C_word t14;
C_word ab[21],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_3362,2,t0,t1);}
t2=C_mutate((C_word*)lf[159]+1,(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_3364,a[2]=((C_word)li41),tmp=(C_word)a,a+=3,tmp));
t3=C_set_block_item(lf[167],0,C_SCHEME_FALSE);
t4=C_set_block_item(lf[168],0,C_SCHEME_FALSE);
t5=C_set_block_item(lf[106],0,C_SCHEME_FALSE);
t6=C_set_block_item(lf[128],0,C_SCHEME_FALSE);
t7=C_set_block_item(lf[129],0,C_SCHEME_FALSE);
t8=C_mutate((C_word*)lf[104]+1,(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_3399,a[2]=((C_word)li43),tmp=(C_word)a,a+=3,tmp));
t9=C_mutate((C_word*)lf[173]+1,(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_3444,a[2]=((C_word)li44),tmp=(C_word)a,a+=3,tmp));
t10=C_mutate((C_word*)lf[175]+1,(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_3482,a[2]=((C_word)li45),tmp=(C_word)a,a+=3,tmp));
t11=(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_3525,a[2]=((C_word*)t0)[2],tmp=(C_word)a,a+=3,tmp);
t12=(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_3948,a[2]=t11,tmp=(C_word)a,a+=3,tmp);
t13=(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_3950,a[2]=((C_word)li64),tmp=(C_word)a,a+=3,tmp);
C_trace("option");
t14=C_retrieve(lf[188]);
((C_proc6)C_retrieve_proc(t14))(6,t14,t12,lf[223],C_SCHEME_FALSE,C_SCHEME_FALSE,t13);}

/* a3949 in k3360 in k3357 in k3354 in k3351 in k3348 in k3345 in k3342 in k3339 in k3336 in k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3950(C_word c,C_word t0,C_word t1,C_word t2,C_word t3,C_word t4,C_word t5,C_word t6){
C_word tmp;
C_word t7;
C_word t8;
C_word t9;
C_word t10;
C_word t11;
C_word t12;
C_word ab[15],*a=ab;
if(c!=7) C_bad_argc_2(c,7,t0);
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr7,(void*)f_3950,7,t0,t1,t2,t3,t4,t5,t6);}
t7=t4;
t8=(C_truep(t7)?C_SCHEME_FALSE:(C_word)C_eqp(lf[221],lf[200]));
if(C_truep(t8)){
t9=(*a=C_CLOSURE_TYPE|4,a[1]=(C_word)f_3960,a[2]=t6,a[3]=t5,a[4]=t1,tmp=(C_word)a,a+=5,tmp);
t10=(*a=C_CLOSURE_TYPE|3,a[1]=(C_word)f_3967,a[2]=t3,a[3]=t9,tmp=(C_word)a,a+=4,tmp);
C_trace("current-error-port");
t11=C_retrieve(lf[185]);
((C_proc2)C_retrieve_proc(t11))(2,t11,t10);}
else{
t9=(*a=C_CLOSURE_TYPE|5,a[1]=(C_word)f_3978,a[2]=t4,a[3]=t5,a[4]=t6,a[5]=t1,tmp=(C_word)a,a+=6,tmp);
C_trace("usage");
t10=C_retrieve(lf[159]);
((C_proc2)C_retrieve_proc(t10))(2,t10,t9);}}

/* k3976 in a3949 in k3360 in k3357 in k3354 in k3351 in k3348 in k3345 in k3342 in k3339 in k3336 in k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3978(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word t4;
C_word t5;
C_word ab[12],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_3978,2,t0,t1);}
t2=(*a=C_CLOSURE_TYPE|3,a[1]=(C_word)f_3985,a[2]=((C_word*)t0)[4],a[3]=((C_word*)t0)[5],tmp=(C_word)a,a+=4,tmp);
t3=(*a=C_CLOSURE_TYPE|3,a[1]=(C_word)f_3989,a[2]=((C_word*)t0)[3],a[3]=t2,tmp=(C_word)a,a+=4,tmp);
t4=(*a=C_CLOSURE_TYPE|3,a[1]=(C_word)f_3991,a[2]=((C_word*)t0)[2],a[3]=((C_word)li63),tmp=(C_word)a,a+=4,tmp);
C_trace("map");
t5=*((C_word*)lf[4]+1);
((C_proc4)(void*)(*((C_word*)t5+1)))(4,t5,t3,t4,lf[222]);}

/* a3990 in k3976 in a3949 in k3360 in k3357 in k3354 in k3351 in k3348 in k3345 in k3342 in k3339 in k3336 in k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3991(C_word c,C_word t0,C_word t1,C_word t2){
C_word tmp;
C_word t3;
C_word ab[3],*a=ab;
if(c!=3) C_bad_argc_2(c,3,t0);
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr3,(void*)f_3991,3,t0,t1,t2);}
t3=t1;
((C_proc2)(void*)(*((C_word*)t3+1)))(2,t3,(C_word)C_a_i_cons(&a,2,t2,((C_word*)t0)[2]));}

/* k3987 in k3976 in a3949 in k3360 in k3357 in k3354 in k3351 in k3348 in k3345 in k3342 in k3339 in k3336 in k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3989(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word *a;
C_trace("append");
t2=*((C_word*)lf[105]+1);
((C_proc4)C_retrieve_proc(t2))(4,t2,((C_word*)t0)[3],t1,((C_word*)t0)[2]);}

/* k3983 in k3976 in a3949 in k3360 in k3357 in k3354 in k3351 in k3348 in k3345 in k3342 in k3339 in k3336 in k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3985(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word *a;
C_trace("values");
C_values(4,0,((C_word*)t0)[3],t1,((C_word*)t0)[2]);}

/* k3965 in a3949 in k3360 in k3357 in k3354 in k3351 in k3348 in k3345 in k3342 in k3339 in k3336 in k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3967(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word ab[5],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_3967,2,t0,t1);}
t2=(*a=C_CLOSURE_TYPE|4,a[1]=(C_word)f_3975,a[2]=((C_word*)t0)[2],a[3]=t1,a[4]=((C_word*)t0)[3],tmp=(C_word)a,a+=5,tmp);
C_trace("argv");
t3=C_retrieve(lf[166]);
((C_proc2)C_retrieve_proc(t3))(2,t3,t2);}

/* k3973 in k3965 in a3949 in k3360 in k3357 in k3354 in k3351 in k3348 in k3345 in k3342 in k3339 in k3336 in k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3975(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word *a;
t2=(C_word)C_i_car(t1);
C_trace("fprintf");
t3=C_retrieve(lf[183]);
((C_proc6)C_retrieve_proc(t3))(6,t3,((C_word*)t0)[4],((C_word*)t0)[3],lf[184],t2,((C_word*)t0)[2]);}

/* k3958 in a3949 in k3360 in k3357 in k3354 in k3351 in k3348 in k3345 in k3342 in k3339 in k3336 in k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3960(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word *a;
C_trace("values");
C_values(4,0,((C_word*)t0)[4],((C_word*)t0)[3],((C_word*)t0)[2]);}

/* k3946 in k3360 in k3357 in k3354 in k3351 in k3348 in k3345 in k3342 in k3339 in k3336 in k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3948(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word *a;
C_trace("make-args:option");
t2=C_retrieve(lf[180]);
((C_proc5)C_retrieve_proc(t2))(5,t2,((C_word*)t0)[2],t1,lf[197],lf[220]);}

/* k3523 in k3360 in k3357 in k3354 in k3351 in k3348 in k3345 in k3342 in k3339 in k3336 in k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3525(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word t4;
C_word t5;
C_word ab[10],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_3525,2,t0,t1);}
t2=(*a=C_CLOSURE_TYPE|3,a[1]=(C_word)f_3529,a[2]=((C_word*)t0)[2],a[3]=t1,tmp=(C_word)a,a+=4,tmp);
t3=(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_3872,a[2]=t2,tmp=(C_word)a,a+=3,tmp);
t4=(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_3874,a[2]=((C_word)li62),tmp=(C_word)a,a+=3,tmp);
C_trace("option");
t5=C_retrieve(lf[188]);
((C_proc6)C_retrieve_proc(t5))(6,t5,t3,lf[219],C_SCHEME_TRUE,C_SCHEME_FALSE,t4);}

/* a3873 in k3523 in k3360 in k3357 in k3354 in k3351 in k3348 in k3345 in k3342 in k3339 in k3336 in k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3874(C_word c,C_word t0,C_word t1,C_word t2,C_word t3,C_word t4,C_word t5,C_word t6){
C_word tmp;
C_word t7;
C_word t8;
C_word t9;
C_word t10;
C_word t11;
C_word t12;
C_word t13;
C_word ab[19],*a=ab;
if(c!=7) C_bad_argc_2(c,7,t0);
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr7,(void*)f_3874,7,t0,t1,t2,t3,t4,t5,t6);}
t7=t4;
t8=(C_truep(t7)?C_SCHEME_FALSE:C_SCHEME_TRUE);
if(C_truep(t8)){
t9=(*a=C_CLOSURE_TYPE|4,a[1]=(C_word)f_3884,a[2]=t6,a[3]=t5,a[4]=t1,tmp=(C_word)a,a+=5,tmp);
t10=(*a=C_CLOSURE_TYPE|3,a[1]=(C_word)f_3891,a[2]=t3,a[3]=t9,tmp=(C_word)a,a+=4,tmp);
C_trace("current-error-port");
t11=C_retrieve(lf[185]);
((C_proc2)C_retrieve_proc(t11))(2,t11,t10);}
else{
t9=(*a=C_CLOSURE_TYPE|5,a[1]=(C_word)f_3902,a[2]=t4,a[3]=t5,a[4]=t6,a[5]=t1,tmp=(C_word)a,a+=6,tmp);
t10=(*a=C_CLOSURE_TYPE|3,a[1]=(C_word)f_3921,a[2]=t4,a[3]=((C_word)li61),tmp=(C_word)a,a+=4,tmp);
C_trace("handle-open-file");
t11=C_retrieve(lf[175]);
((C_proc4)C_retrieve_proc(t11))(4,t11,t9,t4,t10);}}

/* a3920 in a3873 in k3523 in k3360 in k3357 in k3354 in k3351 in k3348 in k3345 in k3342 in k3339 in k3336 in k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3921(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word ab[3],*a=ab;
if(c!=2) C_bad_argc_2(c,2,t0);
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_3921,2,t0,t1);}
t2=(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_3929,a[2]=t1,tmp=(C_word)a,a+=3,tmp);
if(C_truep((C_word)C_i_equalp(lf[217],((C_word*)t0)[2]))){
C_trace("current-input-port");
t3=*((C_word*)lf[218]+1);
((C_proc2)C_retrieve_proc(t3))(2,t3,t2);}
else{
C_trace("open-input-file");
t3=*((C_word*)lf[187]+1);
((C_proc3)C_retrieve_proc(t3))(3,t3,t2,((C_word*)t0)[2]);}}

/* k3927 in a3920 in a3873 in k3523 in k3360 in k3357 in k3354 in k3351 in k3348 in k3345 in k3342 in k3339 in k3336 in k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3929(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word *a;
C_trace("with-input-from-port");
t2=C_retrieve(lf[216]);
((C_proc4)C_retrieve_proc(t2))(4,t2,((C_word*)t0)[2],t1,C_retrieve(lf[173]));}

/* k3900 in a3873 in k3523 in k3360 in k3357 in k3354 in k3351 in k3348 in k3345 in k3342 in k3339 in k3336 in k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3902(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word t4;
C_word t5;
C_word ab[12],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_3902,2,t0,t1);}
t2=(*a=C_CLOSURE_TYPE|3,a[1]=(C_word)f_3909,a[2]=((C_word*)t0)[4],a[3]=((C_word*)t0)[5],tmp=(C_word)a,a+=4,tmp);
t3=(*a=C_CLOSURE_TYPE|3,a[1]=(C_word)f_3913,a[2]=((C_word*)t0)[3],a[3]=t2,tmp=(C_word)a,a+=4,tmp);
t4=(*a=C_CLOSURE_TYPE|3,a[1]=(C_word)f_3915,a[2]=((C_word*)t0)[2],a[3]=((C_word)li60),tmp=(C_word)a,a+=4,tmp);
C_trace("map");
t5=*((C_word*)lf[4]+1);
((C_proc4)(void*)(*((C_word*)t5+1)))(4,t5,t3,t4,lf[215]);}

/* a3914 in k3900 in a3873 in k3523 in k3360 in k3357 in k3354 in k3351 in k3348 in k3345 in k3342 in k3339 in k3336 in k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3915(C_word c,C_word t0,C_word t1,C_word t2){
C_word tmp;
C_word t3;
C_word ab[3],*a=ab;
if(c!=3) C_bad_argc_2(c,3,t0);
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr3,(void*)f_3915,3,t0,t1,t2);}
t3=t1;
((C_proc2)(void*)(*((C_word*)t3+1)))(2,t3,(C_word)C_a_i_cons(&a,2,t2,((C_word*)t0)[2]));}

/* k3911 in k3900 in a3873 in k3523 in k3360 in k3357 in k3354 in k3351 in k3348 in k3345 in k3342 in k3339 in k3336 in k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3913(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word *a;
C_trace("append");
t2=*((C_word*)lf[105]+1);
((C_proc4)C_retrieve_proc(t2))(4,t2,((C_word*)t0)[3],t1,((C_word*)t0)[2]);}

/* k3907 in k3900 in a3873 in k3523 in k3360 in k3357 in k3354 in k3351 in k3348 in k3345 in k3342 in k3339 in k3336 in k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3909(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word *a;
C_trace("values");
C_values(4,0,((C_word*)t0)[3],t1,((C_word*)t0)[2]);}

/* k3889 in a3873 in k3523 in k3360 in k3357 in k3354 in k3351 in k3348 in k3345 in k3342 in k3339 in k3336 in k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3891(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word ab[5],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_3891,2,t0,t1);}
t2=(*a=C_CLOSURE_TYPE|4,a[1]=(C_word)f_3899,a[2]=((C_word*)t0)[2],a[3]=t1,a[4]=((C_word*)t0)[3],tmp=(C_word)a,a+=5,tmp);
C_trace("argv");
t3=C_retrieve(lf[166]);
((C_proc2)C_retrieve_proc(t3))(2,t3,t2);}

/* k3897 in k3889 in a3873 in k3523 in k3360 in k3357 in k3354 in k3351 in k3348 in k3345 in k3342 in k3339 in k3336 in k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3899(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word *a;
t2=(C_word)C_i_car(t1);
C_trace("fprintf");
t3=C_retrieve(lf[183]);
((C_proc6)C_retrieve_proc(t3))(6,t3,((C_word*)t0)[4],((C_word*)t0)[3],lf[184],t2,((C_word*)t0)[2]);}

/* k3882 in a3873 in k3523 in k3360 in k3357 in k3354 in k3351 in k3348 in k3345 in k3342 in k3339 in k3336 in k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3884(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word *a;
C_trace("values");
C_values(4,0,((C_word*)t0)[4],((C_word*)t0)[3],((C_word*)t0)[2]);}

/* k3870 in k3523 in k3360 in k3357 in k3354 in k3351 in k3348 in k3345 in k3342 in k3339 in k3336 in k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3872(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word *a;
C_trace("make-args:option");
t2=C_retrieve(lf[180]);
((C_proc5)C_retrieve_proc(t2))(5,t2,((C_word*)t0)[2],t1,lf[213],lf[214]);}

/* k3527 in k3523 in k3360 in k3357 in k3354 in k3351 in k3348 in k3345 in k3342 in k3339 in k3336 in k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3529(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word t4;
C_word t5;
C_word ab[11],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_3529,2,t0,t1);}
t2=(*a=C_CLOSURE_TYPE|4,a[1]=(C_word)f_3533,a[2]=((C_word*)t0)[2],a[3]=t1,a[4]=((C_word*)t0)[3],tmp=(C_word)a,a+=5,tmp);
t3=(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_3798,a[2]=t2,tmp=(C_word)a,a+=3,tmp);
t4=(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_3800,a[2]=((C_word)li59),tmp=(C_word)a,a+=3,tmp);
C_trace("option");
t5=C_retrieve(lf[188]);
((C_proc6)C_retrieve_proc(t5))(6,t5,t3,lf[212],C_SCHEME_TRUE,C_SCHEME_FALSE,t4);}

/* a3799 in k3527 in k3523 in k3360 in k3357 in k3354 in k3351 in k3348 in k3345 in k3342 in k3339 in k3336 in k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3800(C_word c,C_word t0,C_word t1,C_word t2,C_word t3,C_word t4,C_word t5,C_word t6){
C_word tmp;
C_word t7;
C_word t8;
C_word t9;
C_word t10;
C_word t11;
C_word t12;
C_word t13;
C_word t14;
C_word ab[22],*a=ab;
if(c!=7) C_bad_argc_2(c,7,t0);
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr7,(void*)f_3800,7,t0,t1,t2,t3,t4,t5,t6);}
t7=t4;
t8=(C_truep(t7)?C_SCHEME_FALSE:C_SCHEME_TRUE);
if(C_truep(t8)){
t9=(*a=C_CLOSURE_TYPE|4,a[1]=(C_word)f_3810,a[2]=t6,a[3]=t5,a[4]=t1,tmp=(C_word)a,a+=5,tmp);
t10=(*a=C_CLOSURE_TYPE|3,a[1]=(C_word)f_3817,a[2]=t3,a[3]=t9,tmp=(C_word)a,a+=4,tmp);
C_trace("current-error-port");
t11=C_retrieve(lf[185]);
((C_proc2)C_retrieve_proc(t11))(2,t11,t10);}
else{
t9=(*a=C_CLOSURE_TYPE|5,a[1]=(C_word)f_3828,a[2]=t4,a[3]=t5,a[4]=t6,a[5]=t1,tmp=(C_word)a,a+=6,tmp);
if(C_truep((C_word)C_i_equalp(lf[209],t4))){
t10=(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_3852,a[2]=t9,tmp=(C_word)a,a+=3,tmp);
C_trace("current-output-port");
t11=*((C_word*)lf[210]+1);
((C_proc2)C_retrieve_proc(t11))(2,t11,t10);}
else{
t10=(*a=C_CLOSURE_TYPE|3,a[1]=(C_word)f_3857,a[2]=t4,a[3]=((C_word)li58),tmp=(C_word)a,a+=4,tmp);
C_trace("handle-open-file");
t11=C_retrieve(lf[175]);
((C_proc4)C_retrieve_proc(t11))(4,t11,t9,t4,t10);}}}

/* a3856 in a3799 in k3527 in k3523 in k3360 in k3357 in k3354 in k3351 in k3348 in k3345 in k3342 in k3339 in k3336 in k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3857(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word ab[3],*a=ab;
if(c!=2) C_bad_argc_2(c,2,t0);
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_3857,2,t0,t1);}
t2=(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_3862,a[2]=t1,tmp=(C_word)a,a+=3,tmp);
C_trace("open-output-file");
t3=*((C_word*)lf[211]+1);
((C_proc3)C_retrieve_proc(t3))(3,t3,t2,((C_word*)t0)[2]);}

/* k3860 in a3856 in a3799 in k3527 in k3523 in k3360 in k3357 in k3354 in k3351 in k3348 in k3345 in k3342 in k3339 in k3336 in k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3862(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word *a;
t2=C_mutate((C_word*)lf[167]+1,t1);
t3=((C_word*)t0)[2];
((C_proc2)(void*)(*((C_word*)t3+1)))(2,t3,t2);}

/* k3850 in a3799 in k3527 in k3523 in k3360 in k3357 in k3354 in k3351 in k3348 in k3345 in k3342 in k3339 in k3336 in k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3852(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word *a;
t2=C_mutate((C_word*)lf[167]+1,t1);
t3=((C_word*)t0)[2];
f_3828(2,t3,t2);}

/* k3826 in a3799 in k3527 in k3523 in k3360 in k3357 in k3354 in k3351 in k3348 in k3345 in k3342 in k3339 in k3336 in k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3828(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word t4;
C_word t5;
C_word ab[12],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_3828,2,t0,t1);}
t2=(*a=C_CLOSURE_TYPE|3,a[1]=(C_word)f_3835,a[2]=((C_word*)t0)[4],a[3]=((C_word*)t0)[5],tmp=(C_word)a,a+=4,tmp);
t3=(*a=C_CLOSURE_TYPE|3,a[1]=(C_word)f_3839,a[2]=((C_word*)t0)[3],a[3]=t2,tmp=(C_word)a,a+=4,tmp);
t4=(*a=C_CLOSURE_TYPE|3,a[1]=(C_word)f_3841,a[2]=((C_word*)t0)[2],a[3]=((C_word)li57),tmp=(C_word)a,a+=4,tmp);
C_trace("map");
t5=*((C_word*)lf[4]+1);
((C_proc4)(void*)(*((C_word*)t5+1)))(4,t5,t3,t4,lf[208]);}

/* a3840 in k3826 in a3799 in k3527 in k3523 in k3360 in k3357 in k3354 in k3351 in k3348 in k3345 in k3342 in k3339 in k3336 in k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3841(C_word c,C_word t0,C_word t1,C_word t2){
C_word tmp;
C_word t3;
C_word ab[3],*a=ab;
if(c!=3) C_bad_argc_2(c,3,t0);
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr3,(void*)f_3841,3,t0,t1,t2);}
t3=t1;
((C_proc2)(void*)(*((C_word*)t3+1)))(2,t3,(C_word)C_a_i_cons(&a,2,t2,((C_word*)t0)[2]));}

/* k3837 in k3826 in a3799 in k3527 in k3523 in k3360 in k3357 in k3354 in k3351 in k3348 in k3345 in k3342 in k3339 in k3336 in k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3839(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word *a;
C_trace("append");
t2=*((C_word*)lf[105]+1);
((C_proc4)C_retrieve_proc(t2))(4,t2,((C_word*)t0)[3],t1,((C_word*)t0)[2]);}

/* k3833 in k3826 in a3799 in k3527 in k3523 in k3360 in k3357 in k3354 in k3351 in k3348 in k3345 in k3342 in k3339 in k3336 in k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3835(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word *a;
C_trace("values");
C_values(4,0,((C_word*)t0)[3],t1,((C_word*)t0)[2]);}

/* k3815 in a3799 in k3527 in k3523 in k3360 in k3357 in k3354 in k3351 in k3348 in k3345 in k3342 in k3339 in k3336 in k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3817(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word ab[5],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_3817,2,t0,t1);}
t2=(*a=C_CLOSURE_TYPE|4,a[1]=(C_word)f_3825,a[2]=((C_word*)t0)[2],a[3]=t1,a[4]=((C_word*)t0)[3],tmp=(C_word)a,a+=5,tmp);
C_trace("argv");
t3=C_retrieve(lf[166]);
((C_proc2)C_retrieve_proc(t3))(2,t3,t2);}

/* k3823 in k3815 in a3799 in k3527 in k3523 in k3360 in k3357 in k3354 in k3351 in k3348 in k3345 in k3342 in k3339 in k3336 in k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3825(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word *a;
t2=(C_word)C_i_car(t1);
C_trace("fprintf");
t3=C_retrieve(lf[183]);
((C_proc6)C_retrieve_proc(t3))(6,t3,((C_word*)t0)[4],((C_word*)t0)[3],lf[184],t2,((C_word*)t0)[2]);}

/* k3808 in a3799 in k3527 in k3523 in k3360 in k3357 in k3354 in k3351 in k3348 in k3345 in k3342 in k3339 in k3336 in k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3810(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word *a;
C_trace("values");
C_values(4,0,((C_word*)t0)[4],((C_word*)t0)[3],((C_word*)t0)[2]);}

/* k3796 in k3527 in k3523 in k3360 in k3357 in k3354 in k3351 in k3348 in k3345 in k3342 in k3339 in k3336 in k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3798(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word *a;
C_trace("make-args:option");
t2=C_retrieve(lf[180]);
((C_proc5)C_retrieve_proc(t2))(5,t2,((C_word*)t0)[2],t1,lf[206],lf[207]);}

/* k3531 in k3527 in k3523 in k3360 in k3357 in k3354 in k3351 in k3348 in k3345 in k3342 in k3339 in k3336 in k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3533(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word t4;
C_word t5;
C_word ab[12],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_3533,2,t0,t1);}
t2=(*a=C_CLOSURE_TYPE|5,a[1]=(C_word)f_3537,a[2]=((C_word*)t0)[2],a[3]=t1,a[4]=((C_word*)t0)[3],a[5]=((C_word*)t0)[4],tmp=(C_word)a,a+=6,tmp);
t3=(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_3743,a[2]=t2,tmp=(C_word)a,a+=3,tmp);
t4=(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_3745,a[2]=((C_word)li56),tmp=(C_word)a,a+=3,tmp);
C_trace("option");
t5=C_retrieve(lf[188]);
((C_proc6)C_retrieve_proc(t5))(6,t5,t3,lf[205],C_SCHEME_FALSE,C_SCHEME_FALSE,t4);}

/* a3744 in k3531 in k3527 in k3523 in k3360 in k3357 in k3354 in k3351 in k3348 in k3345 in k3342 in k3339 in k3336 in k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3745(C_word c,C_word t0,C_word t1,C_word t2,C_word t3,C_word t4,C_word t5,C_word t6){
C_word tmp;
C_word t7;
C_word t8;
C_word t9;
C_word t10;
C_word t11;
C_word t12;
C_word t13;
C_word t14;
C_word t15;
C_word ab[21],*a=ab;
if(c!=7) C_bad_argc_2(c,7,t0);
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr7,(void*)f_3745,7,t0,t1,t2,t3,t4,t5,t6);}
t7=t4;
t8=(C_truep(t7)?C_SCHEME_FALSE:(C_word)C_eqp(lf[199],lf[200]));
if(C_truep(t8)){
t9=(*a=C_CLOSURE_TYPE|4,a[1]=(C_word)f_3755,a[2]=t6,a[3]=t5,a[4]=t1,tmp=(C_word)a,a+=5,tmp);
t10=(*a=C_CLOSURE_TYPE|3,a[1]=(C_word)f_3762,a[2]=t3,a[3]=t9,tmp=(C_word)a,a+=4,tmp);
C_trace("current-error-port");
t11=C_retrieve(lf[185]);
((C_proc2)C_retrieve_proc(t11))(2,t11,t10);}
else{
t9=C_set_block_item(lf[168],0,C_SCHEME_TRUE);
t10=(*a=C_CLOSURE_TYPE|3,a[1]=(C_word)f_3778,a[2]=t6,a[3]=t1,tmp=(C_word)a,a+=4,tmp);
t11=(*a=C_CLOSURE_TYPE|3,a[1]=(C_word)f_3782,a[2]=t5,a[3]=t10,tmp=(C_word)a,a+=4,tmp);
t12=(*a=C_CLOSURE_TYPE|3,a[1]=(C_word)f_3784,a[2]=t4,a[3]=((C_word)li55),tmp=(C_word)a,a+=4,tmp);
C_trace("map");
t13=*((C_word*)lf[4]+1);
((C_proc4)(void*)(*((C_word*)t13+1)))(4,t13,t11,t12,lf[204]);}}

/* a3783 in a3744 in k3531 in k3527 in k3523 in k3360 in k3357 in k3354 in k3351 in k3348 in k3345 in k3342 in k3339 in k3336 in k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3784(C_word c,C_word t0,C_word t1,C_word t2){
C_word tmp;
C_word t3;
C_word ab[3],*a=ab;
if(c!=3) C_bad_argc_2(c,3,t0);
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr3,(void*)f_3784,3,t0,t1,t2);}
t3=t1;
((C_proc2)(void*)(*((C_word*)t3+1)))(2,t3,(C_word)C_a_i_cons(&a,2,t2,((C_word*)t0)[2]));}

/* k3780 in a3744 in k3531 in k3527 in k3523 in k3360 in k3357 in k3354 in k3351 in k3348 in k3345 in k3342 in k3339 in k3336 in k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3782(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word *a;
C_trace("append");
t2=*((C_word*)lf[105]+1);
((C_proc4)C_retrieve_proc(t2))(4,t2,((C_word*)t0)[3],t1,((C_word*)t0)[2]);}

/* k3776 in a3744 in k3531 in k3527 in k3523 in k3360 in k3357 in k3354 in k3351 in k3348 in k3345 in k3342 in k3339 in k3336 in k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3778(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word *a;
C_trace("values");
C_values(4,0,((C_word*)t0)[3],t1,((C_word*)t0)[2]);}

/* k3760 in a3744 in k3531 in k3527 in k3523 in k3360 in k3357 in k3354 in k3351 in k3348 in k3345 in k3342 in k3339 in k3336 in k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3762(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word ab[5],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_3762,2,t0,t1);}
t2=(*a=C_CLOSURE_TYPE|4,a[1]=(C_word)f_3770,a[2]=((C_word*)t0)[2],a[3]=t1,a[4]=((C_word*)t0)[3],tmp=(C_word)a,a+=5,tmp);
C_trace("argv");
t3=C_retrieve(lf[166]);
((C_proc2)C_retrieve_proc(t3))(2,t3,t2);}

/* k3768 in k3760 in a3744 in k3531 in k3527 in k3523 in k3360 in k3357 in k3354 in k3351 in k3348 in k3345 in k3342 in k3339 in k3336 in k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3770(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word *a;
t2=(C_word)C_i_car(t1);
C_trace("fprintf");
t3=C_retrieve(lf[183]);
((C_proc6)C_retrieve_proc(t3))(6,t3,((C_word*)t0)[4],((C_word*)t0)[3],lf[184],t2,((C_word*)t0)[2]);}

/* k3753 in a3744 in k3531 in k3527 in k3523 in k3360 in k3357 in k3354 in k3351 in k3348 in k3345 in k3342 in k3339 in k3336 in k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3755(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word *a;
C_trace("values");
C_values(4,0,((C_word*)t0)[4],((C_word*)t0)[3],((C_word*)t0)[2]);}

/* k3741 in k3531 in k3527 in k3523 in k3360 in k3357 in k3354 in k3351 in k3348 in k3345 in k3342 in k3339 in k3336 in k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3743(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word *a;
C_trace("make-args:option");
t2=C_retrieve(lf[180]);
((C_proc5)C_retrieve_proc(t2))(5,t2,((C_word*)t0)[2],t1,lf[197],lf[203]);}

/* k3535 in k3531 in k3527 in k3523 in k3360 in k3357 in k3354 in k3351 in k3348 in k3345 in k3342 in k3339 in k3336 in k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3537(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word t4;
C_word t5;
C_word ab[13],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_3537,2,t0,t1);}
t2=(*a=C_CLOSURE_TYPE|6,a[1]=(C_word)f_3541,a[2]=((C_word*)t0)[2],a[3]=t1,a[4]=((C_word*)t0)[3],a[5]=((C_word*)t0)[4],a[6]=((C_word*)t0)[5],tmp=(C_word)a,a+=7,tmp);
t3=(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_3688,a[2]=t2,tmp=(C_word)a,a+=3,tmp);
t4=(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_3690,a[2]=((C_word)li54),tmp=(C_word)a,a+=3,tmp);
C_trace("option");
t5=C_retrieve(lf[188]);
((C_proc6)C_retrieve_proc(t5))(6,t5,t3,lf[202],C_SCHEME_FALSE,C_SCHEME_FALSE,t4);}

/* a3689 in k3535 in k3531 in k3527 in k3523 in k3360 in k3357 in k3354 in k3351 in k3348 in k3345 in k3342 in k3339 in k3336 in k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3690(C_word c,C_word t0,C_word t1,C_word t2,C_word t3,C_word t4,C_word t5,C_word t6){
C_word tmp;
C_word t7;
C_word t8;
C_word t9;
C_word t10;
C_word t11;
C_word t12;
C_word t13;
C_word t14;
C_word t15;
C_word ab[21],*a=ab;
if(c!=7) C_bad_argc_2(c,7,t0);
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr7,(void*)f_3690,7,t0,t1,t2,t3,t4,t5,t6);}
t7=t4;
t8=(C_truep(t7)?C_SCHEME_FALSE:(C_word)C_eqp(lf[199],lf[200]));
if(C_truep(t8)){
t9=(*a=C_CLOSURE_TYPE|4,a[1]=(C_word)f_3700,a[2]=t6,a[3]=t5,a[4]=t1,tmp=(C_word)a,a+=5,tmp);
t10=(*a=C_CLOSURE_TYPE|3,a[1]=(C_word)f_3707,a[2]=t3,a[3]=t9,tmp=(C_word)a,a+=4,tmp);
C_trace("current-error-port");
t11=C_retrieve(lf[185]);
((C_proc2)C_retrieve_proc(t11))(2,t11,t10);}
else{
t9=C_set_block_item(lf[106],0,C_SCHEME_TRUE);
t10=(*a=C_CLOSURE_TYPE|3,a[1]=(C_word)f_3723,a[2]=t6,a[3]=t1,tmp=(C_word)a,a+=4,tmp);
t11=(*a=C_CLOSURE_TYPE|3,a[1]=(C_word)f_3727,a[2]=t5,a[3]=t10,tmp=(C_word)a,a+=4,tmp);
t12=(*a=C_CLOSURE_TYPE|3,a[1]=(C_word)f_3729,a[2]=t4,a[3]=((C_word)li53),tmp=(C_word)a,a+=4,tmp);
C_trace("map");
t13=*((C_word*)lf[4]+1);
((C_proc4)(void*)(*((C_word*)t13+1)))(4,t13,t11,t12,lf[201]);}}

/* a3728 in a3689 in k3535 in k3531 in k3527 in k3523 in k3360 in k3357 in k3354 in k3351 in k3348 in k3345 in k3342 in k3339 in k3336 in k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3729(C_word c,C_word t0,C_word t1,C_word t2){
C_word tmp;
C_word t3;
C_word ab[3],*a=ab;
if(c!=3) C_bad_argc_2(c,3,t0);
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr3,(void*)f_3729,3,t0,t1,t2);}
t3=t1;
((C_proc2)(void*)(*((C_word*)t3+1)))(2,t3,(C_word)C_a_i_cons(&a,2,t2,((C_word*)t0)[2]));}

/* k3725 in a3689 in k3535 in k3531 in k3527 in k3523 in k3360 in k3357 in k3354 in k3351 in k3348 in k3345 in k3342 in k3339 in k3336 in k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3727(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word *a;
C_trace("append");
t2=*((C_word*)lf[105]+1);
((C_proc4)C_retrieve_proc(t2))(4,t2,((C_word*)t0)[3],t1,((C_word*)t0)[2]);}

/* k3721 in a3689 in k3535 in k3531 in k3527 in k3523 in k3360 in k3357 in k3354 in k3351 in k3348 in k3345 in k3342 in k3339 in k3336 in k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3723(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word *a;
C_trace("values");
C_values(4,0,((C_word*)t0)[3],t1,((C_word*)t0)[2]);}

/* k3705 in a3689 in k3535 in k3531 in k3527 in k3523 in k3360 in k3357 in k3354 in k3351 in k3348 in k3345 in k3342 in k3339 in k3336 in k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3707(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word ab[5],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_3707,2,t0,t1);}
t2=(*a=C_CLOSURE_TYPE|4,a[1]=(C_word)f_3715,a[2]=((C_word*)t0)[2],a[3]=t1,a[4]=((C_word*)t0)[3],tmp=(C_word)a,a+=5,tmp);
C_trace("argv");
t3=C_retrieve(lf[166]);
((C_proc2)C_retrieve_proc(t3))(2,t3,t2);}

/* k3713 in k3705 in a3689 in k3535 in k3531 in k3527 in k3523 in k3360 in k3357 in k3354 in k3351 in k3348 in k3345 in k3342 in k3339 in k3336 in k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3715(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word *a;
t2=(C_word)C_i_car(t1);
C_trace("fprintf");
t3=C_retrieve(lf[183]);
((C_proc6)C_retrieve_proc(t3))(6,t3,((C_word*)t0)[4],((C_word*)t0)[3],lf[184],t2,((C_word*)t0)[2]);}

/* k3698 in a3689 in k3535 in k3531 in k3527 in k3523 in k3360 in k3357 in k3354 in k3351 in k3348 in k3345 in k3342 in k3339 in k3336 in k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3700(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word *a;
C_trace("values");
C_values(4,0,((C_word*)t0)[4],((C_word*)t0)[3],((C_word*)t0)[2]);}

/* k3686 in k3535 in k3531 in k3527 in k3523 in k3360 in k3357 in k3354 in k3351 in k3348 in k3345 in k3342 in k3339 in k3336 in k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3688(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word *a;
C_trace("make-args:option");
t2=C_retrieve(lf[180]);
((C_proc5)C_retrieve_proc(t2))(5,t2,((C_word*)t0)[2],t1,lf[197],lf[198]);}

/* k3539 in k3535 in k3531 in k3527 in k3523 in k3360 in k3357 in k3354 in k3351 in k3348 in k3345 in k3342 in k3339 in k3336 in k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3541(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word t4;
C_word t5;
C_word ab[14],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_3541,2,t0,t1);}
t2=(*a=C_CLOSURE_TYPE|7,a[1]=(C_word)f_3545,a[2]=((C_word*)t0)[2],a[3]=t1,a[4]=((C_word*)t0)[3],a[5]=((C_word*)t0)[4],a[6]=((C_word*)t0)[5],a[7]=((C_word*)t0)[6],tmp=(C_word)a,a+=8,tmp);
t3=(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_3617,a[2]=t2,tmp=(C_word)a,a+=3,tmp);
t4=(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_3619,a[2]=((C_word)li52),tmp=(C_word)a,a+=3,tmp);
C_trace("option");
t5=C_retrieve(lf[188]);
((C_proc6)C_retrieve_proc(t5))(6,t5,t3,lf[196],C_SCHEME_TRUE,C_SCHEME_FALSE,t4);}

/* a3618 in k3539 in k3535 in k3531 in k3527 in k3523 in k3360 in k3357 in k3354 in k3351 in k3348 in k3345 in k3342 in k3339 in k3336 in k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3619(C_word c,C_word t0,C_word t1,C_word t2,C_word t3,C_word t4,C_word t5,C_word t6){
C_word tmp;
C_word t7;
C_word t8;
C_word t9;
C_word t10;
C_word t11;
C_word t12;
C_word t13;
C_word ab[18],*a=ab;
if(c!=7) C_bad_argc_2(c,7,t0);
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr7,(void*)f_3619,7,t0,t1,t2,t3,t4,t5,t6);}
t7=t4;
t8=(C_truep(t7)?C_SCHEME_FALSE:C_SCHEME_TRUE);
if(C_truep(t8)){
t9=(*a=C_CLOSURE_TYPE|4,a[1]=(C_word)f_3629,a[2]=t6,a[3]=t5,a[4]=t1,tmp=(C_word)a,a+=5,tmp);
t10=(*a=C_CLOSURE_TYPE|3,a[1]=(C_word)f_3636,a[2]=t3,a[3]=t9,tmp=(C_word)a,a+=4,tmp);
C_trace("current-error-port");
t11=C_retrieve(lf[185]);
((C_proc2)C_retrieve_proc(t11))(2,t11,t10);}
else{
t9=(*a=C_CLOSURE_TYPE|5,a[1]=(C_word)f_3647,a[2]=t4,a[3]=t5,a[4]=t6,a[5]=t1,tmp=(C_word)a,a+=6,tmp);
t10=(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_3678,a[2]=t9,tmp=(C_word)a,a+=3,tmp);
C_trace("string-append");
t11=*((C_word*)lf[122]+1);
((C_proc5)C_retrieve_proc(t11))(5,t11,t10,lf[194],t4,lf[195]);}}

/* k3676 in a3618 in k3539 in k3535 in k3531 in k3527 in k3523 in k3360 in k3357 in k3354 in k3351 in k3348 in k3345 in k3342 in k3339 in k3336 in k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3678(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word *a;
C_trace("system");
t2=C_retrieve(lf[193]);
((C_proc3)C_retrieve_proc(t2))(3,t2,((C_word*)t0)[2],t1);}

/* k3645 in a3618 in k3539 in k3535 in k3531 in k3527 in k3523 in k3360 in k3357 in k3354 in k3351 in k3348 in k3345 in k3342 in k3339 in k3336 in k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3647(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word t4;
C_word ab[10],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_3647,2,t0,t1);}
t2=(*a=C_CLOSURE_TYPE|5,a[1]=(C_word)f_3650,a[2]=((C_word*)t0)[2],a[3]=((C_word*)t0)[3],a[4]=((C_word*)t0)[4],a[5]=((C_word*)t0)[5],tmp=(C_word)a,a+=6,tmp);
t3=(*a=C_CLOSURE_TYPE|3,a[1]=(C_word)f_3669,a[2]=((C_word*)t0)[2],a[3]=((C_word)li51),tmp=(C_word)a,a+=4,tmp);
C_trace("handle-open-file");
t4=C_retrieve(lf[175]);
((C_proc4)C_retrieve_proc(t4))(4,t4,t2,((C_word*)t0)[2],t3);}

/* a3668 in k3645 in a3618 in k3539 in k3535 in k3531 in k3527 in k3523 in k3360 in k3357 in k3354 in k3351 in k3348 in k3345 in k3342 in k3339 in k3336 in k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3669(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word ab[3],*a=ab;
if(c!=2) C_bad_argc_2(c,2,t0);
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_3669,2,t0,t1);}
t2=(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_3674,a[2]=t1,tmp=(C_word)a,a+=3,tmp);
C_trace("open-input-file");
t3=*((C_word*)lf[187]+1);
((C_proc3)C_retrieve_proc(t3))(3,t3,t2,((C_word*)t0)[2]);}

/* k3672 in a3668 in k3645 in a3618 in k3539 in k3535 in k3531 in k3527 in k3523 in k3360 in k3357 in k3354 in k3351 in k3348 in k3345 in k3342 in k3339 in k3336 in k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3674(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word *a;
t2=C_mutate((C_word*)lf[128]+1,t1);
t3=((C_word*)t0)[2];
((C_proc2)(void*)(*((C_word*)t3+1)))(2,t3,t2);}

/* k3648 in k3645 in a3618 in k3539 in k3535 in k3531 in k3527 in k3523 in k3360 in k3357 in k3354 in k3351 in k3348 in k3345 in k3342 in k3339 in k3336 in k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3650(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word t4;
C_word t5;
C_word ab[12],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_3650,2,t0,t1);}
t2=(*a=C_CLOSURE_TYPE|3,a[1]=(C_word)f_3657,a[2]=((C_word*)t0)[4],a[3]=((C_word*)t0)[5],tmp=(C_word)a,a+=4,tmp);
t3=(*a=C_CLOSURE_TYPE|3,a[1]=(C_word)f_3661,a[2]=((C_word*)t0)[3],a[3]=t2,tmp=(C_word)a,a+=4,tmp);
t4=(*a=C_CLOSURE_TYPE|3,a[1]=(C_word)f_3663,a[2]=((C_word*)t0)[2],a[3]=((C_word)li50),tmp=(C_word)a,a+=4,tmp);
C_trace("map");
t5=*((C_word*)lf[4]+1);
((C_proc4)(void*)(*((C_word*)t5+1)))(4,t5,t3,t4,lf[192]);}

/* a3662 in k3648 in k3645 in a3618 in k3539 in k3535 in k3531 in k3527 in k3523 in k3360 in k3357 in k3354 in k3351 in k3348 in k3345 in k3342 in k3339 in k3336 in k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3663(C_word c,C_word t0,C_word t1,C_word t2){
C_word tmp;
C_word t3;
C_word ab[3],*a=ab;
if(c!=3) C_bad_argc_2(c,3,t0);
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr3,(void*)f_3663,3,t0,t1,t2);}
t3=t1;
((C_proc2)(void*)(*((C_word*)t3+1)))(2,t3,(C_word)C_a_i_cons(&a,2,t2,((C_word*)t0)[2]));}

/* k3659 in k3648 in k3645 in a3618 in k3539 in k3535 in k3531 in k3527 in k3523 in k3360 in k3357 in k3354 in k3351 in k3348 in k3345 in k3342 in k3339 in k3336 in k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3661(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word *a;
C_trace("append");
t2=*((C_word*)lf[105]+1);
((C_proc4)C_retrieve_proc(t2))(4,t2,((C_word*)t0)[3],t1,((C_word*)t0)[2]);}

/* k3655 in k3648 in k3645 in a3618 in k3539 in k3535 in k3531 in k3527 in k3523 in k3360 in k3357 in k3354 in k3351 in k3348 in k3345 in k3342 in k3339 in k3336 in k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3657(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word *a;
C_trace("values");
C_values(4,0,((C_word*)t0)[3],t1,((C_word*)t0)[2]);}

/* k3634 in a3618 in k3539 in k3535 in k3531 in k3527 in k3523 in k3360 in k3357 in k3354 in k3351 in k3348 in k3345 in k3342 in k3339 in k3336 in k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3636(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word ab[5],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_3636,2,t0,t1);}
t2=(*a=C_CLOSURE_TYPE|4,a[1]=(C_word)f_3644,a[2]=((C_word*)t0)[2],a[3]=t1,a[4]=((C_word*)t0)[3],tmp=(C_word)a,a+=5,tmp);
C_trace("argv");
t3=C_retrieve(lf[166]);
((C_proc2)C_retrieve_proc(t3))(2,t3,t2);}

/* k3642 in k3634 in a3618 in k3539 in k3535 in k3531 in k3527 in k3523 in k3360 in k3357 in k3354 in k3351 in k3348 in k3345 in k3342 in k3339 in k3336 in k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3644(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word *a;
t2=(C_word)C_i_car(t1);
C_trace("fprintf");
t3=C_retrieve(lf[183]);
((C_proc6)C_retrieve_proc(t3))(6,t3,((C_word*)t0)[4],((C_word*)t0)[3],lf[184],t2,((C_word*)t0)[2]);}

/* k3627 in a3618 in k3539 in k3535 in k3531 in k3527 in k3523 in k3360 in k3357 in k3354 in k3351 in k3348 in k3345 in k3342 in k3339 in k3336 in k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3629(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word *a;
C_trace("values");
C_values(4,0,((C_word*)t0)[4],((C_word*)t0)[3],((C_word*)t0)[2]);}

/* k3615 in k3539 in k3535 in k3531 in k3527 in k3523 in k3360 in k3357 in k3354 in k3351 in k3348 in k3345 in k3342 in k3339 in k3336 in k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3617(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word *a;
C_trace("make-args:option");
t2=C_retrieve(lf[180]);
((C_proc5)C_retrieve_proc(t2))(5,t2,((C_word*)t0)[2],t1,lf[190],lf[191]);}

/* k3543 in k3539 in k3535 in k3531 in k3527 in k3523 in k3360 in k3357 in k3354 in k3351 in k3348 in k3345 in k3342 in k3339 in k3336 in k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3545(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word t4;
C_word t5;
C_word ab[15],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_3545,2,t0,t1);}
t2=(*a=C_CLOSURE_TYPE|8,a[1]=(C_word)f_3549,a[2]=((C_word*)t0)[2],a[3]=t1,a[4]=((C_word*)t0)[3],a[5]=((C_word*)t0)[4],a[6]=((C_word*)t0)[5],a[7]=((C_word*)t0)[6],a[8]=((C_word*)t0)[7],tmp=(C_word)a,a+=9,tmp);
t3=(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_3553,a[2]=t2,tmp=(C_word)a,a+=3,tmp);
t4=(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_3555,a[2]=((C_word)li49),tmp=(C_word)a,a+=3,tmp);
C_trace("option");
t5=C_retrieve(lf[188]);
((C_proc6)C_retrieve_proc(t5))(6,t5,t3,lf[189],C_SCHEME_TRUE,C_SCHEME_FALSE,t4);}

/* a3554 in k3543 in k3539 in k3535 in k3531 in k3527 in k3523 in k3360 in k3357 in k3354 in k3351 in k3348 in k3345 in k3342 in k3339 in k3336 in k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3555(C_word c,C_word t0,C_word t1,C_word t2,C_word t3,C_word t4,C_word t5,C_word t6){
C_word tmp;
C_word t7;
C_word t8;
C_word t9;
C_word t10;
C_word t11;
C_word t12;
C_word t13;
C_word ab[19],*a=ab;
if(c!=7) C_bad_argc_2(c,7,t0);
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr7,(void*)f_3555,7,t0,t1,t2,t3,t4,t5,t6);}
t7=t4;
t8=(C_truep(t7)?C_SCHEME_FALSE:C_SCHEME_TRUE);
if(C_truep(t8)){
t9=(*a=C_CLOSURE_TYPE|4,a[1]=(C_word)f_3565,a[2]=t6,a[3]=t5,a[4]=t1,tmp=(C_word)a,a+=5,tmp);
t10=(*a=C_CLOSURE_TYPE|3,a[1]=(C_word)f_3572,a[2]=t3,a[3]=t9,tmp=(C_word)a,a+=4,tmp);
C_trace("current-error-port");
t11=C_retrieve(lf[185]);
((C_proc2)C_retrieve_proc(t11))(2,t11,t10);}
else{
t9=(*a=C_CLOSURE_TYPE|5,a[1]=(C_word)f_3583,a[2]=t4,a[3]=t5,a[4]=t6,a[5]=t1,tmp=(C_word)a,a+=6,tmp);
t10=(*a=C_CLOSURE_TYPE|3,a[1]=(C_word)f_3602,a[2]=t4,a[3]=((C_word)li48),tmp=(C_word)a,a+=4,tmp);
C_trace("handle-open-file");
t11=C_retrieve(lf[175]);
((C_proc4)C_retrieve_proc(t11))(4,t11,t9,t4,t10);}}

/* a3601 in a3554 in k3543 in k3539 in k3535 in k3531 in k3527 in k3523 in k3360 in k3357 in k3354 in k3351 in k3348 in k3345 in k3342 in k3339 in k3336 in k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3602(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word ab[3],*a=ab;
if(c!=2) C_bad_argc_2(c,2,t0);
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_3602,2,t0,t1);}
t2=(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_3607,a[2]=t1,tmp=(C_word)a,a+=3,tmp);
C_trace("open-input-file");
t3=*((C_word*)lf[187]+1);
((C_proc3)C_retrieve_proc(t3))(3,t3,t2,((C_word*)t0)[2]);}

/* k3605 in a3601 in a3554 in k3543 in k3539 in k3535 in k3531 in k3527 in k3523 in k3360 in k3357 in k3354 in k3351 in k3348 in k3345 in k3342 in k3339 in k3336 in k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3607(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word *a;
t2=C_mutate((C_word*)lf[129]+1,t1);
t3=((C_word*)t0)[2];
((C_proc2)(void*)(*((C_word*)t3+1)))(2,t3,t2);}

/* k3581 in a3554 in k3543 in k3539 in k3535 in k3531 in k3527 in k3523 in k3360 in k3357 in k3354 in k3351 in k3348 in k3345 in k3342 in k3339 in k3336 in k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3583(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word t4;
C_word t5;
C_word ab[12],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_3583,2,t0,t1);}
t2=(*a=C_CLOSURE_TYPE|3,a[1]=(C_word)f_3590,a[2]=((C_word*)t0)[4],a[3]=((C_word*)t0)[5],tmp=(C_word)a,a+=4,tmp);
t3=(*a=C_CLOSURE_TYPE|3,a[1]=(C_word)f_3594,a[2]=((C_word*)t0)[3],a[3]=t2,tmp=(C_word)a,a+=4,tmp);
t4=(*a=C_CLOSURE_TYPE|3,a[1]=(C_word)f_3596,a[2]=((C_word*)t0)[2],a[3]=((C_word)li47),tmp=(C_word)a,a+=4,tmp);
C_trace("map");
t5=*((C_word*)lf[4]+1);
((C_proc4)(void*)(*((C_word*)t5+1)))(4,t5,t3,t4,lf[186]);}

/* a3595 in k3581 in a3554 in k3543 in k3539 in k3535 in k3531 in k3527 in k3523 in k3360 in k3357 in k3354 in k3351 in k3348 in k3345 in k3342 in k3339 in k3336 in k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3596(C_word c,C_word t0,C_word t1,C_word t2){
C_word tmp;
C_word t3;
C_word ab[3],*a=ab;
if(c!=3) C_bad_argc_2(c,3,t0);
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr3,(void*)f_3596,3,t0,t1,t2);}
t3=t1;
((C_proc2)(void*)(*((C_word*)t3+1)))(2,t3,(C_word)C_a_i_cons(&a,2,t2,((C_word*)t0)[2]));}

/* k3592 in k3581 in a3554 in k3543 in k3539 in k3535 in k3531 in k3527 in k3523 in k3360 in k3357 in k3354 in k3351 in k3348 in k3345 in k3342 in k3339 in k3336 in k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3594(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word *a;
C_trace("append");
t2=*((C_word*)lf[105]+1);
((C_proc4)C_retrieve_proc(t2))(4,t2,((C_word*)t0)[3],t1,((C_word*)t0)[2]);}

/* k3588 in k3581 in a3554 in k3543 in k3539 in k3535 in k3531 in k3527 in k3523 in k3360 in k3357 in k3354 in k3351 in k3348 in k3345 in k3342 in k3339 in k3336 in k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3590(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word *a;
C_trace("values");
C_values(4,0,((C_word*)t0)[3],t1,((C_word*)t0)[2]);}

/* k3570 in a3554 in k3543 in k3539 in k3535 in k3531 in k3527 in k3523 in k3360 in k3357 in k3354 in k3351 in k3348 in k3345 in k3342 in k3339 in k3336 in k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3572(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word ab[5],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_3572,2,t0,t1);}
t2=(*a=C_CLOSURE_TYPE|4,a[1]=(C_word)f_3580,a[2]=((C_word*)t0)[2],a[3]=t1,a[4]=((C_word*)t0)[3],tmp=(C_word)a,a+=5,tmp);
C_trace("argv");
t3=C_retrieve(lf[166]);
((C_proc2)C_retrieve_proc(t3))(2,t3,t2);}

/* k3578 in k3570 in a3554 in k3543 in k3539 in k3535 in k3531 in k3527 in k3523 in k3360 in k3357 in k3354 in k3351 in k3348 in k3345 in k3342 in k3339 in k3336 in k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3580(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word *a;
t2=(C_word)C_i_car(t1);
C_trace("fprintf");
t3=C_retrieve(lf[183]);
((C_proc6)C_retrieve_proc(t3))(6,t3,((C_word*)t0)[4],((C_word*)t0)[3],lf[184],t2,((C_word*)t0)[2]);}

/* k3563 in a3554 in k3543 in k3539 in k3535 in k3531 in k3527 in k3523 in k3360 in k3357 in k3354 in k3351 in k3348 in k3345 in k3342 in k3339 in k3336 in k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3565(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word *a;
C_trace("values");
C_values(4,0,((C_word*)t0)[4],((C_word*)t0)[3],((C_word*)t0)[2]);}

/* k3551 in k3543 in k3539 in k3535 in k3531 in k3527 in k3523 in k3360 in k3357 in k3354 in k3351 in k3348 in k3345 in k3342 in k3339 in k3336 in k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3553(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word *a;
C_trace("make-args:option");
t2=C_retrieve(lf[180]);
((C_proc5)C_retrieve_proc(t2))(5,t2,((C_word*)t0)[2],t1,lf[181],lf[182]);}

/* k3547 in k3543 in k3539 in k3535 in k3531 in k3527 in k3523 in k3360 in k3357 in k3354 in k3351 in k3348 in k3345 in k3342 in k3339 in k3336 in k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3549(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word t4;
C_word t5;
C_word t6;
C_word ab[27],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_3549,2,t0,t1);}
t2=(C_word)C_a_i_list(&a,7,((C_word*)t0)[8],((C_word*)t0)[7],((C_word*)t0)[6],((C_word*)t0)[5],((C_word*)t0)[4],((C_word*)t0)[3],t1);
t3=C_mutate((C_word*)lf[162]+1,t2);
t4=(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_3493,a[2]=((C_word*)t0)[2],tmp=(C_word)a,a+=3,tmp);
t5=(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_3521,a[2]=t4,tmp=(C_word)a,a+=3,tmp);
C_trace("command-line-arguments");
t6=C_retrieve(lf[179]);
((C_proc2)C_retrieve_proc(t6))(2,t6,t5);}

/* k3519 in k3547 in k3543 in k3539 in k3535 in k3531 in k3527 in k3523 in k3360 in k3357 in k3354 in k3351 in k3348 in k3345 in k3342 in k3339 in k3336 in k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3521(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word *a;
C_trace("args:parse");
t2=C_retrieve(lf[178]);
((C_proc4)C_retrieve_proc(t2))(4,t2,((C_word*)t0)[2],t1,C_retrieve(lf[162]));}

/* k3491 in k3547 in k3543 in k3539 in k3535 in k3531 in k3527 in k3523 in k3360 in k3357 in k3354 in k3351 in k3348 in k3345 in k3342 in k3339 in k3336 in k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3493(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word t4;
C_word t5;
C_word t6;
C_word ab[9],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_3493,2,t0,t1);}
t2=(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_3496,a[2]=((C_word*)t0)[2],tmp=(C_word)a,a+=3,tmp);
if(C_truep(C_retrieve(lf[168]))){
t3=C_SCHEME_UNDEFINED;
t4=(*a=C_VECTOR_TYPE|1,a[1]=t3,tmp=(C_word)a,a+=2,tmp);
t5=C_set_block_item(t4,0,(*a=C_CLOSURE_TYPE|3,a[1]=(C_word)f_3507,a[2]=t4,a[3]=((C_word)li46),tmp=(C_word)a,a+=4,tmp));
t6=((C_word*)t4)[1];
f_3507(t6,t2);}
else{
C_trace("glut:MainLoop");
t3=C_retrieve(lf[177]);
((C_proc2)C_retrieve_proc(t3))(2,t3,t2);}}

/* loop.2044 in k3491 in k3547 in k3543 in k3539 in k3535 in k3531 in k3527 in k3523 in k3360 in k3357 in k3354 in k3351 in k3348 in k3345 in k3342 in k3339 in k3336 in k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_fcall f_3507(C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word ab[4],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)trf_3507,NULL,2,t0,t1);}
t2=(*a=C_CLOSURE_TYPE|3,a[1]=(C_word)f_3511,a[2]=t1,a[3]=((C_word*)t0)[2],tmp=(C_word)a,a+=4,tmp);
C_trace("idle");
t3=C_retrieve(lf[125]);
((C_proc2)C_retrieve_proc(t3))(2,t3,t2);}

/* k3509 in loop.2044 in k3491 in k3547 in k3543 in k3539 in k3535 in k3531 in k3527 in k3523 in k3360 in k3357 in k3354 in k3351 in k3348 in k3345 in k3342 in k3339 in k3336 in k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3511(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word *a;
C_trace("loop.20442045");
t2=((C_word*)((C_word*)t0)[3])[1];
f_3507(t2,((C_word*)t0)[2]);}

/* k3494 in k3491 in k3547 in k3543 in k3539 in k3535 in k3531 in k3527 in k3523 in k3360 in k3357 in k3354 in k3351 in k3348 in k3345 in k3342 in k3339 in k3336 in k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3496(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word t4;
C_word ab[6],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_3496,2,t0,t1);}
t2=(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_3499,a[2]=((C_word*)t0)[2],tmp=(C_word)a,a+=3,tmp);
t3=(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_3502,a[2]=t2,tmp=(C_word)a,a+=3,tmp);
C_trace("##sys#implicit-exit-handler");
t4=C_retrieve(lf[176]);
((C_proc2)C_retrieve_proc(t4))(2,t4,t3);}

/* k3500 in k3494 in k3491 in k3547 in k3543 in k3539 in k3535 in k3531 in k3527 in k3523 in k3360 in k3357 in k3354 in k3351 in k3348 in k3345 in k3342 in k3339 in k3336 in k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3502(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word *a;
t2=t1;
((C_proc2)C_retrieve_proc(t2))(2,t2,((C_word*)t0)[2]);}

/* k3497 in k3494 in k3491 in k3547 in k3543 in k3539 in k3535 in k3531 in k3527 in k3523 in k3360 in k3357 in k3354 in k3351 in k3348 in k3345 in k3342 in k3339 in k3336 in k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3499(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word *a;
t2=((C_word*)t0)[2];
((C_proc2)(void*)(*((C_word*)t2+1)))(2,t2,C_SCHEME_UNDEFINED);}

/* handle-open-file in k3360 in k3357 in k3354 in k3351 in k3348 in k3345 in k3342 in k3339 in k3336 in k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3482(C_word c,C_word t0,C_word t1,C_word t2,C_word t3){
C_word tmp;
C_word t4;
C_word *a;
if(c!=4) C_bad_argc_2(c,4,t0);
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr4,(void*)f_3482,4,t0,t1,t2,t3);}
C_trace("thunk.19421944");
t4=t3;
((C_proc2)C_retrieve_proc(t4))(2,t4,t1);}

/* load-data in k3360 in k3357 in k3354 in k3351 in k3348 in k3345 in k3342 in k3339 in k3336 in k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3444(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word ab[3],*a=ab;
if(c!=2) C_bad_argc_2(c,2,t0);
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_3444,2,t0,t1);}
t2=(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_3448,a[2]=t1,tmp=(C_word)a,a+=3,tmp);
C_trace("read");
t3=*((C_word*)lf[174]+1);
((C_proc2)C_retrieve_proc(t3))(2,t3,t2);}

/* k3446 in load-data in k3360 in k3357 in k3354 in k3351 in k3348 in k3345 in k3342 in k3339 in k3336 in k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3448(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word ab[4],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_3448,2,t0,t1);}
t2=(*a=C_CLOSURE_TYPE|3,a[1]=(C_word)f_3451,a[2]=t1,a[3]=((C_word*)t0)[2],tmp=(C_word)a,a+=4,tmp);
C_trace("read");
t3=*((C_word*)lf[174]+1);
((C_proc2)C_retrieve_proc(t3))(2,t3,t2);}

/* k3449 in k3446 in load-data in k3360 in k3357 in k3354 in k3351 in k3348 in k3345 in k3342 in k3339 in k3336 in k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3451(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word ab[5],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_3451,2,t0,t1);}
t2=(*a=C_CLOSURE_TYPE|4,a[1]=(C_word)f_3454,a[2]=t1,a[3]=((C_word*)t0)[2],a[4]=((C_word*)t0)[3],tmp=(C_word)a,a+=5,tmp);
C_trace("read");
t3=*((C_word*)lf[174]+1);
((C_proc2)C_retrieve_proc(t3))(2,t3,t2);}

/* k3452 in k3449 in k3446 in load-data in k3360 in k3357 in k3354 in k3351 in k3348 in k3345 in k3342 in k3339 in k3336 in k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3454(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word ab[6],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_3454,2,t0,t1);}
t2=(*a=C_CLOSURE_TYPE|5,a[1]=(C_word)f_3457,a[2]=t1,a[3]=((C_word*)t0)[2],a[4]=((C_word*)t0)[3],a[5]=((C_word*)t0)[4],tmp=(C_word)a,a+=6,tmp);
C_trace("read");
t3=*((C_word*)lf[174]+1);
((C_proc2)C_retrieve_proc(t3))(2,t3,t2);}

/* k3455 in k3452 in k3449 in k3446 in load-data in k3360 in k3357 in k3354 in k3351 in k3348 in k3345 in k3342 in k3339 in k3336 in k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3457(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word t4;
C_word ab[15],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_3457,2,t0,t1);}
if(C_truep((C_word)C_eofp(t1))){
t2=((C_word*)t0)[5];
((C_proc2)(void*)(*((C_word*)t2+1)))(2,t2,C_retrieve(lf[22]));}
else{
t2=(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_3466,a[2]=((C_word*)t0)[5],tmp=(C_word)a,a+=3,tmp);
t3=(C_word)C_a_i_list(&a,4,((C_word*)t0)[4],((C_word*)t0)[3],((C_word*)t0)[2],t1);
C_trace("reportdata");
t4=C_retrieve(lf[104]);
((C_proc3)C_retrieve_proc(t4))(3,t4,t2,t3);}}

/* k3464 in k3455 in k3452 in k3449 in k3446 in load-data in k3360 in k3357 in k3354 in k3351 in k3348 in k3345 in k3342 in k3339 in k3336 in k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3466(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word *a;
C_trace("load-data");
t2=C_retrieve(lf[173]);
((C_proc2)C_retrieve_proc(t2))(2,t2,((C_word*)t0)[2]);}

/* reportdata in k3360 in k3357 in k3354 in k3351 in k3348 in k3345 in k3342 in k3339 in k3336 in k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3399(C_word c,C_word t0,C_word t1,C_word t2){
C_word tmp;
C_word t3;
C_word t4;
C_word t5;
C_word t6;
C_word t7;
C_word t8;
C_word t9;
C_word ab[14],*a=ab;
if(c!=3) C_bad_argc_2(c,3,t0);
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr3,(void*)f_3399,3,t0,t1,t2);}
t3=(C_word)C_i_car(t2);
t4=(C_word)C_i_cadr(t2);
t5=(C_word)C_i_caddr(t2);
t6=(C_word)C_i_cadddr(t2);
t7=(*a=C_CLOSURE_TYPE|6,a[1]=(C_word)f_3415,a[2]=t6,a[3]=t5,a[4]=t4,a[5]=t3,a[6]=t1,tmp=(C_word)a,a+=7,tmp);
if(C_truep(C_retrieve(lf[167]))){
t8=(*a=C_CLOSURE_TYPE|6,a[1]=(C_word)f_3435,a[2]=t6,a[3]=t5,a[4]=t4,a[5]=t3,a[6]=((C_word)li42),tmp=(C_word)a,a+=7,tmp);
C_trace("with-output-to-port");
t9=C_retrieve(lf[172]);
((C_proc4)C_retrieve_proc(t9))(4,t9,t7,C_retrieve(lf[167]),t8);}
else{
t8=t7;
f_3415(2,t8,C_retrieve(lf[22]));}}

/* a3434 in reportdata in k3360 in k3357 in k3354 in k3351 in k3348 in k3345 in k3342 in k3339 in k3336 in k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3435(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word *a;
if(c!=2) C_bad_argc_2(c,2,t0);
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_3435,2,t0,t1);}
C_trace("print");
t2=*((C_word*)lf[99]+1);
((C_proc9)C_retrieve_proc(t2))(9,t2,t1,((C_word*)t0)[5],lf[169],((C_word*)t0)[4],lf[170],((C_word*)t0)[3],lf[171],((C_word*)t0)[2]);}

/* k3413 in reportdata in k3360 in k3357 in k3354 in k3351 in k3348 in k3345 in k3342 in k3339 in k3336 in k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3415(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word ab[3],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_3415,2,t0,t1);}
t2=(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_3418,a[2]=((C_word*)t0)[6],tmp=(C_word)a,a+=3,tmp);
C_trace("trianglegrid-addpoint");
t3=C_retrieve(lf[18]);
((C_proc6)C_retrieve_proc(t3))(6,t3,t2,((C_word*)t0)[5],((C_word*)t0)[4],((C_word*)t0)[3],((C_word*)t0)[2]);}

/* k3416 in k3413 in reportdata in k3360 in k3357 in k3354 in k3351 in k3348 in k3345 in k3342 in k3339 in k3336 in k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3418(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word *a;
t2=C_retrieve(lf[168]);
if(C_truep(t2)){
t3=((C_word*)t0)[2];
((C_proc2)(void*)(*((C_word*)t3+1)))(2,t3,C_retrieve(lf[22]));}
else{
C_trace("glut:PostRedisplay");
t3=C_retrieve(lf[139]);
((C_proc2)C_retrieve_proc(t3))(2,t3,((C_word*)t0)[2]);}}

/* usage in k3360 in k3357 in k3354 in k3351 in k3348 in k3345 in k3342 in k3339 in k3336 in k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3364(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word t4;
C_word ab[6],*a=ab;
if(c!=2) C_bad_argc_2(c,2,t0);
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_3364,2,t0,t1);}
t2=(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_3368,a[2]=t1,tmp=(C_word)a,a+=3,tmp);
t3=(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_3392,a[2]=t2,tmp=(C_word)a,a+=3,tmp);
C_trace("argv");
t4=C_retrieve(lf[166]);
((C_proc2)C_retrieve_proc(t4))(2,t4,t3);}

/* k3390 in usage in k3360 in k3357 in k3354 in k3351 in k3348 in k3345 in k3342 in k3339 in k3336 in k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3392(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word *a;
t2=(C_word)C_i_car(t1);
C_trace("print");
t3=*((C_word*)lf[99]+1);
((C_proc5)C_retrieve_proc(t3))(5,t3,((C_word*)t0)[2],lf[164],t2,lf[165]);}

/* k3366 in usage in k3360 in k3357 in k3354 in k3351 in k3348 in k3345 in k3342 in k3339 in k3336 in k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3368(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word ab[3],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_3368,2,t0,t1);}
t2=(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_3371,a[2]=((C_word*)t0)[2],tmp=(C_word)a,a+=3,tmp);
C_trace("newline");
t3=*((C_word*)lf[163]+1);
((C_proc2)C_retrieve_proc(t3))(2,t3,t2);}

/* k3369 in k3366 in usage in k3360 in k3357 in k3354 in k3351 in k3348 in k3345 in k3342 in k3339 in k3336 in k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3371(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word t4;
C_word ab[6],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_3371,2,t0,t1);}
t2=(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_3374,a[2]=((C_word*)t0)[2],tmp=(C_word)a,a+=3,tmp);
t3=(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_3384,a[2]=t2,tmp=(C_word)a,a+=3,tmp);
C_trace("args:usage");
t4=C_retrieve(lf[161]);
((C_proc3)C_retrieve_proc(t4))(3,t4,t3,C_retrieve(lf[162]));}

/* k3382 in k3369 in k3366 in usage in k3360 in k3357 in k3354 in k3351 in k3348 in k3345 in k3342 in k3339 in k3336 in k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3384(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word *a;
C_trace("print");
t2=*((C_word*)lf[99]+1);
((C_proc3)C_retrieve_proc(t2))(3,t2,((C_word*)t0)[2],t1);}

/* k3372 in k3369 in k3366 in usage in k3360 in k3357 in k3354 in k3351 in k3348 in k3345 in k3342 in k3339 in k3336 in k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3374(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word ab[3],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_3374,2,t0,t1);}
t2=(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_3377,a[2]=((C_word*)t0)[2],tmp=(C_word)a,a+=3,tmp);
C_trace("print");
t3=*((C_word*)lf[99]+1);
((C_proc3)C_retrieve_proc(t3))(3,t3,t2,lf[160]);}

/* k3375 in k3372 in k3369 in k3366 in usage in k3360 in k3357 in k3354 in k3351 in k3348 in k3345 in k3342 in k3339 in k3336 in k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3377(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word *a;
C_trace("exit");
t2=C_retrieve(lf[140]);
((C_proc3)C_retrieve_proc(t2))(3,t2,((C_word*)t0)[2],C_fix(1));}

/* special in k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3193(C_word c,C_word t0,C_word t1,C_word t2,C_word t3,C_word t4){
C_word tmp;
C_word t5;
C_word t6;
C_word t7;
C_word ab[7],*a=ab;
if(c!=5) C_bad_argc_2(c,5,t0);
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr5,(void*)f_3193,5,t0,t1,t2,t3,t4);}
t5=(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_3197,a[2]=t1,tmp=(C_word)a,a+=3,tmp);
t6=(*a=C_CLOSURE_TYPE|3,a[1]=(C_word)f_3203,a[2]=t5,a[3]=t2,tmp=(C_word)a,a+=4,tmp);
C_trace("glut-HasModifiers");
t7=C_retrieve(lf[135]);
((C_proc3)C_retrieve_proc(t7))(3,t7,t6,C_retrieve(lf[158]));}

/* k3201 in special in k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3203(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word t4;
C_word ab[8],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_3203,2,t0,t1);}
if(C_truep(t1)){
t2=(*a=C_CLOSURE_TYPE|3,a[1]=(C_word)f_3206,a[2]=((C_word*)t0)[2],a[3]=((C_word*)t0)[3],tmp=(C_word)a,a+=4,tmp);
C_trace("attr-get");
t3=C_retrieve(lf[133]);
((C_proc3)C_retrieve_proc(t3))(3,t3,t2,lf[156]);}
else{
t2=(*a=C_CLOSURE_TYPE|3,a[1]=(C_word)f_3266,a[2]=((C_word*)t0)[2],a[3]=((C_word*)t0)[3],tmp=(C_word)a,a+=4,tmp);
C_trace("attr-get");
t3=C_retrieve(lf[133]);
((C_proc3)C_retrieve_proc(t3))(3,t3,t2,lf[157]);}}

/* k3264 in k3201 in special in k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3266(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word t4;
C_word t5;
C_word ab[9],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_3266,2,t0,t1);}
if(C_truep((C_word)C_i_nequalp(((C_word*)t0)[3],C_retrieve(lf[150])))){
C_trace("gl:Translatef");
t2=C_retrieve(lf[56]);
((C_proc5)C_retrieve_proc(t2))(5,t2,((C_word*)t0)[2],t1,C_fix(0),C_fix(0));}
else{
if(C_truep((C_word)C_i_nequalp(((C_word*)t0)[3],C_retrieve(lf[151])))){
t2=(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_3288,a[2]=((C_word*)t0)[2],tmp=(C_word)a,a+=3,tmp);
C_trace("-");
C_minus(3,0,t2,t1);}
else{
if(C_truep((C_word)C_i_nequalp(((C_word*)t0)[3],C_retrieve(lf[152])))){
t2=(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_3301,a[2]=((C_word*)t0)[2],tmp=(C_word)a,a+=3,tmp);
C_trace("-");
C_minus(3,0,t2,t1);}
else{
if(C_truep((C_word)C_i_nequalp(((C_word*)t0)[3],C_retrieve(lf[153])))){
C_trace("gl:Translatef");
t2=C_retrieve(lf[56]);
((C_proc5)C_retrieve_proc(t2))(5,t2,((C_word*)t0)[2],C_fix(0),t1,C_fix(0));}
else{
if(C_truep((C_word)C_i_nequalp(((C_word*)t0)[3],C_retrieve(lf[154])))){
t2=(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_3323,a[2]=((C_word*)t0)[2],tmp=(C_word)a,a+=3,tmp);
C_trace("-");
C_minus(3,0,t2,t1);}
else{
if(C_truep((C_word)C_i_nequalp(((C_word*)t0)[3],C_retrieve(lf[155])))){
C_trace("gl:Translatef");
t2=C_retrieve(lf[56]);
((C_proc5)C_retrieve_proc(t2))(5,t2,((C_word*)t0)[2],C_fix(0),C_fix(0),t1);}
else{
t2=((C_word*)t0)[2];
f_3197(2,t2,C_retrieve(lf[22]));}}}}}}}

/* k3321 in k3264 in k3201 in special in k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3323(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word *a;
C_trace("gl:Translatef");
t2=C_retrieve(lf[56]);
((C_proc5)C_retrieve_proc(t2))(5,t2,((C_word*)t0)[2],C_fix(0),C_fix(0),t1);}

/* k3299 in k3264 in k3201 in special in k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3301(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word *a;
C_trace("gl:Translatef");
t2=C_retrieve(lf[56]);
((C_proc5)C_retrieve_proc(t2))(5,t2,((C_word*)t0)[2],C_fix(0),t1,C_fix(0));}

/* k3286 in k3264 in k3201 in special in k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3288(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word *a;
C_trace("gl:Translatef");
t2=C_retrieve(lf[56]);
((C_proc5)C_retrieve_proc(t2))(5,t2,((C_word*)t0)[2],t1,C_fix(0),C_fix(0));}

/* k3204 in k3201 in special in k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3206(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word *a;
if(C_truep((C_word)C_i_nequalp(((C_word*)t0)[3],C_retrieve(lf[150])))){
C_trace("RotateAfter");
t2=C_retrieve(lf[48]);
((C_proc6)C_retrieve_proc(t2))(6,t2,((C_word*)t0)[2],t1,C_fix(0),C_fix(1),C_fix(0));}
else{
if(C_truep((C_word)C_i_nequalp(((C_word*)t0)[3],C_retrieve(lf[151])))){
C_trace("RotateAfter");
t2=C_retrieve(lf[48]);
((C_proc6)C_retrieve_proc(t2))(6,t2,((C_word*)t0)[2],t1,C_fix(0),C_fix(-1),C_fix(0));}
else{
if(C_truep((C_word)C_i_nequalp(((C_word*)t0)[3],C_retrieve(lf[152])))){
C_trace("RotateAfter");
t2=C_retrieve(lf[48]);
((C_proc6)C_retrieve_proc(t2))(6,t2,((C_word*)t0)[2],t1,C_fix(1),C_fix(0),C_fix(0));}
else{
if(C_truep((C_word)C_i_nequalp(((C_word*)t0)[3],C_retrieve(lf[153])))){
C_trace("RotateAfter");
t2=C_retrieve(lf[48]);
((C_proc6)C_retrieve_proc(t2))(6,t2,((C_word*)t0)[2],t1,C_fix(-1),C_fix(0),C_fix(0));}
else{
if(C_truep((C_word)C_i_nequalp(((C_word*)t0)[3],C_retrieve(lf[154])))){
C_trace("RotateAfter");
t2=C_retrieve(lf[48]);
((C_proc6)C_retrieve_proc(t2))(6,t2,((C_word*)t0)[2],t1,C_fix(0),C_fix(0),C_fix(1));}
else{
if(C_truep((C_word)C_i_nequalp(((C_word*)t0)[3],C_retrieve(lf[155])))){
C_trace("RotateAfter");
t2=C_retrieve(lf[48]);
((C_proc6)C_retrieve_proc(t2))(6,t2,((C_word*)t0)[2],t1,C_fix(0),C_fix(0),C_fix(-1));}
else{
t2=((C_word*)t0)[2];
f_3197(2,t2,C_retrieve(lf[22]));}}}}}}}

/* k3195 in special in k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3197(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word *a;
C_trace("glut:PostRedisplay");
t2=C_retrieve(lf[139]);
((C_proc2)C_retrieve_proc(t2))(2,t2,((C_word*)t0)[2]);}

/* keyboard in k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3139(C_word c,C_word t0,C_word t1,C_word t2,C_word t3,C_word t4){
C_word tmp;
C_word t5;
C_word t6;
C_word t7;
C_word t8;
C_word t9;
C_word t10;
C_word t11;
C_word ab[9],*a=ab;
if(c!=5) C_bad_argc_2(c,5,t0);
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr5,(void*)f_3139,5,t0,t1,t2,t3,t4);}
t5=(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_3143,a[2]=t1,tmp=(C_word)a,a+=3,tmp);
t6=t2;
if(C_truep((C_truep((C_word)C_i_eqvp(t6,C_make_character(27)))?C_SCHEME_TRUE:(C_truep((C_word)C_i_eqvp(t6,C_make_character(113)))?C_SCHEME_TRUE:C_SCHEME_FALSE)))){
C_trace("exit");
t7=C_retrieve(lf[140]);
((C_proc2)C_retrieve_proc(t7))(2,t7,t5);}
else{
t7=t2;
if(C_truep((C_truep((C_word)C_i_eqvp(t7,C_make_character(119)))?C_SCHEME_TRUE:C_SCHEME_FALSE))){
t8=(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_3161,a[2]=t5,tmp=(C_word)a,a+=3,tmp);
t9=(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_3179,a[2]=t8,tmp=(C_word)a,a+=3,tmp);
C_trace("attr-get");
t10=C_retrieve(lf[133]);
((C_proc3)C_retrieve_proc(t10))(3,t10,t9,lf[147]);}
else{
t8=t2;
if(C_truep((C_truep((C_word)C_i_eqvp(t8,C_make_character(102)))?C_SCHEME_TRUE:C_SCHEME_FALSE))){
C_trace("glut:FullScreen");
t9=C_retrieve(lf[148]);
((C_proc2)C_retrieve_proc(t9))(2,t9,t5);}
else{
t9=t5;
f_3143(2,t9,C_retrieve(lf[22]));}}}}

/* k3177 in keyboard in k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3179(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word *a;
t2=(C_word)C_i_not(t1);
C_trace("attr-set!");
t3=C_retrieve(lf[131]);
((C_proc4)C_retrieve_proc(t3))(4,t3,((C_word*)t0)[2],lf[146],t2);}

/* k3159 in keyboard in k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3161(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word ab[3],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_3161,2,t0,t1);}
t2=(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_3171,a[2]=((C_word*)t0)[2],tmp=(C_word)a,a+=3,tmp);
C_trace("attr-get");
t3=C_retrieve(lf[133]);
((C_proc3)C_retrieve_proc(t3))(3,t3,t2,lf[145]);}

/* k3169 in k3159 in keyboard in k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3171(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word *a;
t2=(C_truep(t1)?C_retrieve(lf[141]):C_retrieve(lf[142]));
C_trace("gl:PolygonMode");
t3=C_retrieve(lf[143]);
((C_proc4)C_retrieve_proc(t3))(4,t3,((C_word*)t0)[2],C_retrieve(lf[144]),t2);}

/* k3141 in keyboard in k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3143(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word *a;
C_trace("glut:PostRedisplay");
t2=C_retrieve(lf[139]);
((C_proc2)C_retrieve_proc(t2))(2,t2,((C_word*)t0)[2]);}

/* glut-HasModifiers in k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3122(C_word c,C_word t0,C_word t1,...){
C_word tmp;
C_word t2;
va_list v;
C_word *a,c2=c;
C_save_rest(t1,c2,2);
C_check_for_interrupt;
if(!C_demand(c*C_SIZEOF_PAIR+3)){
C_save_and_reclaim((void*)tr2r,(void*)f_3122r,2,t0,t1);}
else{
a=C_alloc((c-2)*3);
t2=C_restore_rest(a,C_rest_count(0));
f_3122r(t0,t1,t2);}}

static void C_ccall f_3122r(C_word t0,C_word t1,C_word t2){
C_word tmp;
C_word t3;
C_word t4;
C_word *a=C_alloc(3);
t3=(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_3126,a[2]=t1,tmp=(C_word)a,a+=3,tmp);
C_apply(4,0,t3,*((C_word*)lf[137]+1),t2);}

/* k3124 in glut-HasModifiers in k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3126(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word ab[4],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_3126,2,t0,t1);}
t2=(*a=C_CLOSURE_TYPE|3,a[1]=(C_word)f_3137,a[2]=((C_word*)t0)[2],a[3]=t1,tmp=(C_word)a,a+=4,tmp);
C_trace("glut:GetModifiers");
t3=C_retrieve(lf[136]);
((C_proc2)C_retrieve_proc(t3))(2,t3,t2);}

/* k3135 in k3124 in glut-HasModifiers in k3118 in k3115 in k3112 in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3137(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word ab[4],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_3137,2,t0,t1);}
t2=(C_word)C_a_i_bitwise_and(&a,2,((C_word*)t0)[3],t1);
t3=((C_word*)t0)[2];
((C_proc2)(void*)(*((C_word*)t3+1)))(2,t3,(C_word)C_i_nequalp(((C_word*)t0)[3],t2));}

/* attr-get in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3107(C_word c,C_word t0,C_word t1,C_word t2){
C_word tmp;
C_word t3;
C_word *a;
if(c!=3) C_bad_argc_2(c,3,t0);
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr3,(void*)f_3107,3,t0,t1,t2);}
C_trace("hash-table-ref");
t3=*((C_word*)lf[134]+1);
((C_proc4)C_retrieve_proc(t3))(4,t3,t1,C_retrieve(lf[130]),t2);}

/* attr-set! in k3097 in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3101(C_word c,C_word t0,C_word t1,C_word t2,C_word t3){
C_word tmp;
C_word t4;
C_word *a;
if(c!=4) C_bad_argc_2(c,4,t0);
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr4,(void*)f_3101,4,t0,t1,t2,t3);}
C_trace("hash-table-set!");
t4=C_retrieve(lf[132]);
((C_proc5)C_retrieve_proc(t4))(5,t4,t1,C_retrieve(lf[130]),t2,t3);}

/* idle in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3037(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word t4;
C_word t5;
C_word t6;
C_word ab[11],*a=ab;
if(c!=2) C_bad_argc_2(c,2,t0);
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_3037,2,t0,t1);}
t2=(*a=C_CLOSURE_TYPE|3,a[1]=(C_word)f_3041,a[2]=((C_word*)t0)[3],a[3]=t1,tmp=(C_word)a,a+=4,tmp);
if(C_truep(C_retrieve(lf[129]))){
t3=C_SCHEME_UNDEFINED;
t4=(*a=C_VECTOR_TYPE|1,a[1]=t3,tmp=(C_word)a,a+=2,tmp);
t5=C_set_block_item(t4,0,(*a=C_CLOSURE_TYPE|4,a[1]=(C_word)f_3076,a[2]=((C_word*)t0)[2],a[3]=t4,a[4]=((C_word)li34),tmp=(C_word)a,a+=5,tmp));
t6=((C_word*)t4)[1];
f_3076(t6,t2);}
else{
t3=t2;
f_3041(2,t3,C_retrieve(lf[22]));}}

/* each-mag-line.1848 in idle in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_fcall f_3076(C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word ab[4],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)trf_3076,NULL,2,t0,t1);}
t2=(*a=C_CLOSURE_TYPE|3,a[1]=(C_word)f_3080,a[2]=t1,a[3]=((C_word*)t0)[3],tmp=(C_word)a,a+=4,tmp);
C_trace("mag-line.18461852");
t3=((C_word*)t0)[2];
((C_proc3)C_retrieve_proc(t3))(3,t3,t2,C_retrieve(lf[129]));}

/* k3078 in each-mag-line.1848 in idle in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3080(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word ab[4],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_3080,2,t0,t1);}
if(C_truep(t1)){
t2=(*a=C_CLOSURE_TYPE|3,a[1]=(C_word)f_3086,a[2]=((C_word*)t0)[2],a[3]=((C_word*)t0)[3],tmp=(C_word)a,a+=4,tmp);
C_trace("parse-mag-line");
t3=C_retrieve(lf[92]);
((C_proc3)C_retrieve_proc(t3))(3,t3,t2,t1);}
else{
t2=((C_word*)t0)[2];
((C_proc2)(void*)(*((C_word*)t2+1)))(2,t2,C_retrieve(lf[22]));}}

/* k3084 in k3078 in each-mag-line.1848 in idle in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3086(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word *a;
C_trace("each-mag-line.18481854");
t2=((C_word*)((C_word*)t0)[3])[1];
f_3076(t2,((C_word*)t0)[2]);}

/* k3039 in idle in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3041(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word t4;
C_word t5;
C_word t6;
C_word ab[10],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_3041,2,t0,t1);}
t2=(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_3044,a[2]=((C_word*)t0)[3],tmp=(C_word)a,a+=3,tmp);
if(C_truep(C_retrieve(lf[128]))){
t3=C_SCHEME_UNDEFINED;
t4=(*a=C_VECTOR_TYPE|1,a[1]=t3,tmp=(C_word)a,a+=2,tmp);
t5=C_set_block_item(t4,0,(*a=C_CLOSURE_TYPE|4,a[1]=(C_word)f_3052,a[2]=((C_word*)t0)[2],a[3]=t4,a[4]=((C_word)li33),tmp=(C_word)a,a+=5,tmp));
t6=((C_word*)t4)[1];
f_3052(t6,t2);}
else{
t3=t2;
f_3044(2,t3,C_retrieve(lf[22]));}}

/* each-gps-line.1850 in k3039 in idle in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_fcall f_3052(C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word ab[4],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)trf_3052,NULL,2,t0,t1);}
t2=(*a=C_CLOSURE_TYPE|3,a[1]=(C_word)f_3056,a[2]=t1,a[3]=((C_word*)t0)[3],tmp=(C_word)a,a+=4,tmp);
C_trace("gps-line.18471853");
t3=((C_word*)t0)[2];
((C_proc3)C_retrieve_proc(t3))(3,t3,t2,C_retrieve(lf[128]));}

/* k3054 in each-gps-line.1850 in k3039 in idle in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3056(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word ab[4],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_3056,2,t0,t1);}
if(C_truep(t1)){
t2=(*a=C_CLOSURE_TYPE|3,a[1]=(C_word)f_3062,a[2]=((C_word*)t0)[2],a[3]=((C_word*)t0)[3],tmp=(C_word)a,a+=4,tmp);
C_trace("parse-gps-line");
t3=C_retrieve(lf[98]);
((C_proc3)C_retrieve_proc(t3))(3,t3,t2,t1);}
else{
t2=((C_word*)t0)[2];
((C_proc2)(void*)(*((C_word*)t2+1)))(2,t2,C_retrieve(lf[22]));}}

/* k3060 in k3054 in each-gps-line.1850 in k3039 in idle in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3062(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word *a;
C_trace("each-gps-line.18501858");
t2=((C_word*)((C_word*)t0)[3])[1];
f_3052(t2,((C_word*)t0)[2]);}

/* k3042 in k3039 in idle in k3034 in k3031 in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_3044(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word *a;
C_trace("thread-sleep!");
t2=C_retrieve(lf[126]);
((C_proc3)C_retrieve_proc(t2))(3,t2,((C_word*)t0)[2],lf[127]);}

/* make-line-reader in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_2984(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word t4;
C_word ab[6],*a=ab;
if(c!=2) C_bad_argc_2(c,2,t0);
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_2984,2,t0,t1);}
t2=lf[119];
t3=(*a=C_VECTOR_TYPE|1,a[1]=t2,tmp=(C_word)a,a+=2,tmp);
t4=t1;
((C_proc2)(void*)(*((C_word*)t4+1)))(2,t4,(*a=C_CLOSURE_TYPE|3,a[1]=(C_word)f_2986,a[2]=t3,a[3]=((C_word)li31),tmp=(C_word)a,a+=4,tmp));}

/* f_2986 in make-line-reader in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_2986(C_word c,C_word t0,C_word t1,C_word t2){
C_word tmp;
C_word t3;
C_word t4;
C_word ab[5],*a=ab;
if(c!=3) C_bad_argc_2(c,3,t0);
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr3,(void*)f_2986,3,t0,t1,t2);}
t3=(*a=C_CLOSURE_TYPE|4,a[1]=(C_word)f_2993,a[2]=t1,a[3]=t2,a[4]=((C_word*)t0)[2],tmp=(C_word)a,a+=5,tmp);
C_trace("char-ready?");
t4=*((C_word*)lf[124]+1);
((C_proc3)C_retrieve_proc(t4))(3,t4,t3,t2);}

/* k2991 */
static void C_ccall f_2993(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word t4;
C_word t5;
C_word ab[8],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_2993,2,t0,t1);}
if(C_truep(t1)){
t2=C_SCHEME_UNDEFINED;
t3=(*a=C_VECTOR_TYPE|1,a[1]=t2,tmp=(C_word)a,a+=2,tmp);
t4=C_set_block_item(t3,0,(*a=C_CLOSURE_TYPE|5,a[1]=(C_word)f_2998,a[2]=((C_word*)t0)[3],a[3]=t3,a[4]=((C_word*)t0)[4],a[5]=((C_word)li30),tmp=(C_word)a,a+=6,tmp));
t5=((C_word*)t3)[1];
f_2998(t5,((C_word*)t0)[2]);}
else{
t2=((C_word*)t0)[2];
((C_proc2)(void*)(*((C_word*)t2+1)))(2,t2,C_SCHEME_FALSE);}}

/* loop.1835 in k2991 */
static void C_fcall f_2998(C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word ab[5],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)trf_2998,NULL,2,t0,t1);}
t2=(*a=C_CLOSURE_TYPE|4,a[1]=(C_word)f_3002,a[2]=((C_word*)t0)[3],a[3]=((C_word*)t0)[4],a[4]=t1,tmp=(C_word)a,a+=5,tmp);
C_trace("read-char/port");
t3=C_retrieve(lf[123]);
((C_proc3)(void*)(*((C_word*)t3+1)))(3,t3,t2,((C_word*)t0)[2]);}

/* k3000 in loop.1835 in k2991 */
static void C_ccall f_3002(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word t4;
C_word t5;
C_word t6;
C_word ab[11],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_3002,2,t0,t1);}
if(C_truep((C_word)C_eofp(t1))){
t2=((C_word*)t0)[4];
((C_proc2)(void*)(*((C_word*)t2+1)))(2,t2,C_SCHEME_FALSE);}
else{
t2=(C_word)C_eqp(t1,C_make_character(10));
if(C_truep(t2)){
t3=(*a=C_CLOSURE_TYPE|3,a[1]=(C_word)f_3017,a[2]=((C_word*)t0)[4],a[3]=((C_word*)t0)[3],tmp=(C_word)a,a+=4,tmp);
C_trace("string-copy");
t4=C_retrieve(lf[121]);
((C_proc3)C_retrieve_proc(t4))(3,t4,t3,((C_word*)((C_word*)t0)[3])[1]);}
else{
t3=(*a=C_CLOSURE_TYPE|4,a[1]=(C_word)f_3022,a[2]=((C_word*)t0)[4],a[3]=((C_word*)t0)[2],a[4]=((C_word*)t0)[3],tmp=(C_word)a,a+=5,tmp);
t4=(C_word)C_a_i_string(&a,1,t1);
C_trace("string-append");
t5=*((C_word*)lf[122]+1);
((C_proc4)C_retrieve_proc(t5))(4,t5,t3,((C_word*)((C_word*)t0)[3])[1],t4);}}}

/* k3020 in k3000 in loop.1835 in k2991 */
static void C_ccall f_3022(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word *a;
t2=C_mutate(((C_word *)((C_word*)t0)[4])+1,t1);
C_trace("loop.18351840");
t3=((C_word*)((C_word*)t0)[3])[1];
f_2998(t3,((C_word*)t0)[2]);}

/* k3015 in k3000 in loop.1835 in k2991 */
static void C_ccall f_3017(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word *a;
t2=C_mutate(((C_word *)((C_word*)t0)[3])+1,lf[120]);
t3=((C_word*)t0)[2];
((C_proc2)(void*)(*((C_word*)t3+1)))(2,t3,t1);}

/* parse-gps-line in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_2788(C_word c,C_word t0,C_word t1,C_word t2){
C_word tmp;
C_word t3;
C_word t4;
C_word ab[3],*a=ab;
if(c!=3) C_bad_argc_2(c,3,t0);
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr3,(void*)f_2788,3,t0,t1,t2);}
t3=(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_2792,a[2]=t1,tmp=(C_word)a,a+=3,tmp);
C_trace("string-split");
t4=C_retrieve(lf[96]);
((C_proc4)C_retrieve_proc(t4))(4,t4,t3,t2,lf[117]);}

/* k2790 in parse-gps-line in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_2792(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word t4;
C_word t5;
C_word ab[4],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_2792,2,t0,t1);}
if(C_truep((C_word)C_i_nullp(t1))){
t2=((C_word*)t0)[2];
((C_proc2)(void*)(*((C_word*)t2+1)))(2,t2,C_SCHEME_END_OF_LIST);}
else{
t2=(*a=C_CLOSURE_TYPE|3,a[1]=(C_word)f_2804,a[2]=((C_word*)t0)[2],a[3]=t1,tmp=(C_word)a,a+=4,tmp);
t3=(C_word)C_i_length(t1);
if(C_truep((C_word)C_i_greaterp(t3,C_fix(9)))){
t4=(C_word)C_i_car(t1);
t5=t2;
f_2804(t5,(C_word)C_i_equalp(t4,lf[116]));}
else{
t4=t2;
f_2804(t4,C_SCHEME_FALSE);}}}

/* k2802 in k2790 in parse-gps-line in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_fcall f_2804(C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word t4;
C_word ab[4],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)trf_2804,NULL,2,t0,t1);}
if(C_truep(t1)){
t2=(*a=C_CLOSURE_TYPE|3,a[1]=(C_word)f_2807,a[2]=((C_word*)t0)[2],a[3]=((C_word*)t0)[3],tmp=(C_word)a,a+=4,tmp);
t3=(C_word)C_i_list_ref(((C_word*)t0)[3],C_fix(1));
C_trace("string->number");
C_string_to_number(3,0,t2,t3);}
else{
t2=((C_word*)t0)[2];
((C_proc2)(void*)(*((C_word*)t2+1)))(2,t2,C_retrieve(lf[22]));}}

/* k2805 in k2802 in k2790 in parse-gps-line in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_2807(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word t4;
C_word ab[4],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_2807,2,t0,t1);}
t2=(*a=C_CLOSURE_TYPE|3,a[1]=(C_word)f_2810,a[2]=((C_word*)t0)[2],a[3]=((C_word*)t0)[3],tmp=(C_word)a,a+=4,tmp);
t3=(C_word)C_i_list_ref(((C_word*)t0)[3],C_fix(2));
C_trace("string->number");
C_string_to_number(3,0,t2,t3);}

/* k2808 in k2805 in k2802 in k2790 in parse-gps-line in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_2810(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word t4;
C_word t5;
C_word t6;
C_word t7;
C_word ab[8],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_2810,2,t0,t1);}
t2=t1;
t3=(*a=C_VECTOR_TYPE|1,a[1]=t2,tmp=(C_word)a,a+=2,tmp);
t4=(C_word)C_i_list_ref(((C_word*)t0)[3],C_fix(3));
t5=(*a=C_CLOSURE_TYPE|5,a[1]=(C_word)f_2816,a[2]=t4,a[3]=t3,a[4]=((C_word*)t0)[2],a[5]=((C_word*)t0)[3],tmp=(C_word)a,a+=6,tmp);
t6=(C_word)C_i_list_ref(((C_word*)t0)[3],C_fix(4));
C_trace("string->number");
C_string_to_number(3,0,t5,t6);}

/* k2814 in k2808 in k2805 in k2802 in k2790 in parse-gps-line in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_2816(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word t4;
C_word t5;
C_word t6;
C_word t7;
C_word ab[10],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_2816,2,t0,t1);}
t2=t1;
t3=(*a=C_VECTOR_TYPE|1,a[1]=t2,tmp=(C_word)a,a+=2,tmp);
t4=(C_word)C_i_list_ref(((C_word*)t0)[5],C_fix(5));
t5=(*a=C_CLOSURE_TYPE|7,a[1]=(C_word)f_2822,a[2]=((C_word*)t0)[5],a[3]=((C_word*)t0)[2],a[4]=t4,a[5]=((C_word*)t0)[3],a[6]=t3,a[7]=((C_word*)t0)[4],tmp=(C_word)a,a+=8,tmp);
t6=(C_word)C_i_list_ref(((C_word*)t0)[5],C_fix(6));
C_trace("string->number");
C_string_to_number(3,0,t5,t6);}

/* k2820 in k2814 in k2808 in k2805 in k2802 in k2790 in parse-gps-line in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_2822(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word t4;
C_word ab[9],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_2822,2,t0,t1);}
t2=(*a=C_CLOSURE_TYPE|8,a[1]=(C_word)f_2825,a[2]=((C_word*)t0)[2],a[3]=((C_word*)t0)[3],a[4]=((C_word*)t0)[4],a[5]=((C_word*)t0)[5],a[6]=((C_word*)t0)[6],a[7]=((C_word*)t0)[7],a[8]=t1,tmp=(C_word)a,a+=9,tmp);
t3=(C_word)C_i_list_ref(((C_word*)t0)[2],C_fix(7));
C_trace("string->number");
C_string_to_number(3,0,t2,t3);}

/* k2823 in k2820 in k2814 in k2808 in k2805 in k2802 in k2790 in parse-gps-line in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_2825(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word t4;
C_word ab[10],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_2825,2,t0,t1);}
t2=(*a=C_CLOSURE_TYPE|9,a[1]=(C_word)f_2828,a[2]=((C_word*)t0)[2],a[3]=((C_word*)t0)[3],a[4]=((C_word*)t0)[4],a[5]=((C_word*)t0)[5],a[6]=((C_word*)t0)[6],a[7]=t1,a[8]=((C_word*)t0)[7],a[9]=((C_word*)t0)[8],tmp=(C_word)a,a+=10,tmp);
t3=(C_word)C_i_list_ref(((C_word*)t0)[2],C_fix(8));
C_trace("string->number");
C_string_to_number(3,0,t2,t3);}

/* k2826 in k2823 in k2820 in k2814 in k2808 in k2805 in k2802 in k2790 in parse-gps-line in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_2828(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word t4;
C_word ab[10],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_2828,2,t0,t1);}
t2=(*a=C_CLOSURE_TYPE|9,a[1]=(C_word)f_2831,a[2]=((C_word*)t0)[3],a[3]=((C_word*)t0)[4],a[4]=((C_word*)t0)[5],a[5]=((C_word*)t0)[6],a[6]=t1,a[7]=((C_word*)t0)[7],a[8]=((C_word*)t0)[8],a[9]=((C_word*)t0)[9],tmp=(C_word)a,a+=10,tmp);
t3=(C_word)C_i_list_ref(((C_word*)t0)[2],C_fix(9));
C_trace("string->number");
C_string_to_number(3,0,t2,t3);}

/* k2829 in k2826 in k2823 in k2820 in k2814 in k2808 in k2805 in k2802 in k2790 in parse-gps-line in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_2831(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word t4;
C_word t5;
C_word ab[18],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_2831,2,t0,t1);}
if(C_truep((C_word)C_i_nequalp(((C_word*)t0)[9],C_fix(0)))){
C_trace("print");
t2=*((C_word*)lf[99]+1);
((C_proc4)C_retrieve_proc(t2))(4,t2,((C_word*)t0)[8],lf[100],((C_word*)t0)[7]);}
else{
t2=(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_2842,a[2]=((C_word)li28),tmp=(C_word)a,a+=3,tmp);
if(C_truep((C_word)C_i_lessp(((C_word*)t0)[9],C_fix(1)))){
C_trace("print");
t3=*((C_word*)lf[99]+1);
((C_proc3)C_retrieve_proc(t3))(3,t3,((C_word*)t0)[8],lf[103]);}
else{
t3=(*a=C_CLOSURE_TYPE|10,a[1]=(C_word)f_2874,a[2]=((C_word*)t0)[3],a[3]=((C_word*)t0)[7],a[4]=((C_word*)t0)[9],a[5]=((C_word*)t0)[4],a[6]=((C_word*)t0)[5],a[7]=t2,a[8]=t1,a[9]=((C_word*)t0)[8],a[10]=((C_word*)t0)[6],tmp=(C_word)a,a+=11,tmp);
if(C_truep((C_word)C_i_equalp(lf[115],((C_word*)t0)[2]))){
t4=(*a=C_CLOSURE_TYPE|3,a[1]=(C_word)f_2934,a[2]=t3,a[3]=((C_word*)t0)[4],tmp=(C_word)a,a+=4,tmp);
C_trace("-");
C_minus(3,0,t4,((C_word*)((C_word*)t0)[4])[1]);}
else{
t4=t3;
f_2874(t4,C_retrieve(lf[22]));}}}}

/* k2932 in k2829 in k2826 in k2823 in k2820 in k2814 in k2808 in k2805 in k2802 in k2790 in parse-gps-line in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_2934(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word *a;
t2=C_mutate(((C_word *)((C_word*)t0)[3])+1,t1);
t3=((C_word*)t0)[2];
f_2874(t3,t2);}

/* k2872 in k2829 in k2826 in k2823 in k2820 in k2814 in k2808 in k2805 in k2802 in k2790 in parse-gps-line in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_fcall f_2874(C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word t4;
C_word ab[14],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)trf_2874,NULL,2,t0,t1);}
t2=(*a=C_CLOSURE_TYPE|9,a[1]=(C_word)f_2877,a[2]=((C_word*)t0)[3],a[3]=((C_word*)t0)[4],a[4]=((C_word*)t0)[5],a[5]=((C_word*)t0)[6],a[6]=((C_word*)t0)[7],a[7]=((C_word*)t0)[8],a[8]=((C_word*)t0)[9],a[9]=((C_word*)t0)[10],tmp=(C_word)a,a+=10,tmp);
if(C_truep((C_word)C_i_equalp(lf[114],((C_word*)t0)[2]))){
t3=(*a=C_CLOSURE_TYPE|3,a[1]=(C_word)f_2924,a[2]=t2,a[3]=((C_word*)t0)[6],tmp=(C_word)a,a+=4,tmp);
C_trace("-");
C_minus(3,0,t3,((C_word*)((C_word*)t0)[6])[1]);}
else{
t3=t2;
f_2877(t3,C_retrieve(lf[22]));}}

/* k2922 in k2872 in k2829 in k2826 in k2823 in k2820 in k2814 in k2808 in k2805 in k2802 in k2790 in parse-gps-line in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_2924(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word *a;
t2=C_mutate(((C_word *)((C_word*)t0)[3])+1,t1);
t3=((C_word*)t0)[2];
f_2877(t3,t2);}

/* k2875 in k2872 in k2829 in k2826 in k2823 in k2820 in k2814 in k2808 in k2805 in k2802 in k2790 in parse-gps-line in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_fcall f_2877(C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word t4;
C_word t5;
C_word ab[8],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)trf_2877,NULL,2,t0,t1);}
t2=(*a=C_CLOSURE_TYPE|7,a[1]=(C_word)f_2880,a[2]=((C_word*)t0)[4],a[3]=((C_word*)t0)[5],a[4]=((C_word*)t0)[6],a[5]=((C_word*)t0)[7],a[6]=((C_word*)t0)[8],a[7]=((C_word*)t0)[9],tmp=(C_word)a,a+=8,tmp);
if(C_truep(C_retrieve(lf[106]))){
t3=(C_word)C_i_nequalp(C_fix(1),((C_word*)t0)[3]);
t4=(C_truep(t3)?lf[107]:lf[108]);
C_trace("print");
t5=*((C_word*)lf[99]+1);
((C_proc13)C_retrieve_proc(t5))(13,t5,t2,((C_word*)((C_word*)t0)[4])[1],lf[109],((C_word*)((C_word*)t0)[5])[1],lf[110],((C_word*)t0)[7],lf[111],((C_word*)t0)[9],lf[112],t4,((C_word*)t0)[2],lf[113]);}
else{
t3=t2;
f_2880(2,t3,C_retrieve(lf[22]));}}

/* k2878 in k2875 in k2872 in k2829 in k2826 in k2823 in k2820 in k2814 in k2808 in k2805 in k2802 in k2790 in parse-gps-line in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_2880(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word t4;
C_word t5;
C_word ab[13],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_2880,2,t0,t1);}
t2=(*a=C_CLOSURE_TYPE|3,a[1]=(C_word)f_2884,a[2]=((C_word*)t0)[6],a[3]=((C_word*)t0)[7],tmp=(C_word)a,a+=4,tmp);
t3=(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_2892,a[2]=t2,tmp=(C_word)a,a+=3,tmp);
t4=(*a=C_CLOSURE_TYPE|5,a[1]=(C_word)f_2900,a[2]=((C_word*)t0)[3],a[3]=((C_word*)t0)[4],a[4]=((C_word*)t0)[5],a[5]=t3,tmp=(C_word)a,a+=6,tmp);
C_trace("convert-deg.18121825");
f_2842(t4,((C_word*)((C_word*)t0)[2])[1]);}

/* k2898 in k2878 in k2875 in k2872 in k2829 in k2826 in k2823 in k2820 in k2814 in k2808 in k2805 in k2802 in k2790 in parse-gps-line in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_2900(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word ab[5],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_2900,2,t0,t1);}
t2=(*a=C_CLOSURE_TYPE|4,a[1]=(C_word)f_2904,a[2]=((C_word*)t0)[4],a[3]=t1,a[4]=((C_word*)t0)[5],tmp=(C_word)a,a+=5,tmp);
C_trace("convert-deg.18121825");
f_2842(t2,((C_word*)((C_word*)t0)[2])[1]);}

/* k2902 in k2898 in k2878 in k2875 in k2872 in k2829 in k2826 in k2823 in k2820 in k2814 in k2808 in k2805 in k2802 in k2790 in parse-gps-line in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_2904(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word *a;
C_trace("convert-position");
t2=C_retrieve(lf[97]);
((C_proc5)C_retrieve_proc(t2))(5,t2,((C_word*)t0)[4],((C_word*)t0)[3],t1,((C_word*)t0)[2]);}

/* k2890 in k2878 in k2875 in k2872 in k2829 in k2826 in k2823 in k2820 in k2814 in k2808 in k2805 in k2802 in k2790 in parse-gps-line in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_2892(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word ab[3],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_2892,2,t0,t1);}
t2=(C_word)C_a_i_list(&a,1,C_retrieve(lf[91]));
C_trace("append");
t3=*((C_word*)lf[105]+1);
((C_proc4)C_retrieve_proc(t3))(4,t3,((C_word*)t0)[2],t1,t2);}

/* k2882 in k2878 in k2875 in k2872 in k2829 in k2826 in k2823 in k2820 in k2814 in k2808 in k2805 in k2802 in k2790 in parse-gps-line in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_2884(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word t4;
C_word *a;
t2=C_mutate((C_word*)lf[14]+1,t1);
t3=C_mutate((C_word*)lf[12]+1,((C_word*)t0)[3]);
C_trace("reportdata");
t4=C_retrieve(lf[104]);
((C_proc3)C_retrieve_proc(t4))(3,t4,((C_word*)t0)[2],C_retrieve(lf[14]));}

/* convert-deg.1812 in k2829 in k2826 in k2823 in k2820 in k2814 in k2808 in k2805 in k2802 in k2790 in parse-gps-line in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_fcall f_2842(C_word t1,C_word t2){
C_word tmp;
C_word t3;
C_word t4;
C_word t5;
C_word ab[8],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)trf_2842,NULL,2,t1,t2);}
t3=(*a=C_CLOSURE_TYPE|3,a[1]=(C_word)f_2850,a[2]=t2,a[3]=t1,tmp=(C_word)a,a+=4,tmp);
t4=(C_word)C_a_i_divide(&a,2,t2,C_fix(100));
C_trace("floor");
t5=*((C_word*)lf[102]+1);
((C_proc3)C_retrieve_proc(t5))(3,t5,t3,t4);}

/* k2848 in convert-deg.1812 in k2829 in k2826 in k2823 in k2820 in k2814 in k2808 in k2805 in k2802 in k2790 in parse-gps-line in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_2850(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word ab[4],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_2850,2,t0,t1);}
t2=(*a=C_CLOSURE_TYPE|3,a[1]=(C_word)f_2858,a[2]=t1,a[3]=((C_word*)t0)[3],tmp=(C_word)a,a+=4,tmp);
C_trace("remainder");
t3=*((C_word*)lf[101]+1);
((C_proc4)C_retrieve_proc(t3))(4,t3,t2,((C_word*)t0)[2],C_fix(100));}

/* k2856 in k2848 in convert-deg.1812 in k2829 in k2826 in k2823 in k2820 in k2814 in k2808 in k2805 in k2802 in k2790 in parse-gps-line in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_2858(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word ab[8],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_2858,2,t0,t1);}
t2=(C_word)C_a_i_divide(&a,2,t1,C_fix(60));
t3=((C_word*)t0)[3];
((C_proc2)(void*)(*((C_word*)t3+1)))(2,t3,(C_word)C_a_i_plus(&a,2,((C_word*)t0)[2],t2));}

/* convert-position in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_2722(C_word c,C_word t0,C_word t1,C_word t2,C_word t3,C_word t4){
C_word tmp;
C_word t5;
C_word t6;
C_word t7;
C_word t8;
C_word t9;
C_word ab[9],*a=ab;
if(c!=5) C_bad_argc_2(c,5,t0);
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr5,(void*)f_2722,5,t0,t1,t2,t3,t4);}
t5=(*a=C_CLOSURE_TYPE|8,a[1]=(C_word)f_2726,a[2]=((C_word*)t0)[2],a[3]=t3,a[4]=((C_word*)t0)[3],a[5]=t2,a[6]=t1,a[7]=((C_word*)t0)[4],a[8]=t4,tmp=(C_word)a,a+=9,tmp);
if(C_truep((C_word)C_i_nequalp(((C_word*)((C_word*)t0)[3])[1],C_fix(100)))){
t6=C_mutate(((C_word *)((C_word*)t0)[3])+1,t2);
t7=C_mutate(((C_word *)((C_word*)t0)[2])+1,t3);
t8=C_mutate(((C_word *)((C_word*)t0)[4])+1,t4);
t9=t5;
f_2726(t9,t8);}
else{
t6=t5;
f_2726(t6,C_retrieve(lf[22]));}}

/* k2724 in convert-position in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_fcall f_2726(C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word t4;
C_word t5;
C_word ab[15],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)trf_2726,NULL,2,t0,t1);}
t2=(*a=C_CLOSURE_TYPE|6,a[1]=(C_word)f_2733,a[2]=((C_word*)t0)[4],a[3]=((C_word*)t0)[5],a[4]=((C_word*)t0)[6],a[5]=((C_word*)t0)[7],a[6]=((C_word*)t0)[8],tmp=(C_word)a,a+=7,tmp);
t3=(*a=C_CLOSURE_TYPE|3,a[1]=(C_word)f_2773,a[2]=((C_word*)t0)[5],a[3]=t2,tmp=(C_word)a,a+=4,tmp);
t4=(C_word)C_a_i_minus(&a,2,((C_word*)t0)[3],((C_word*)((C_word*)t0)[2])[1]);
C_trace("deg2rad");
t5=C_retrieve(lf[11]);
((C_proc3)C_retrieve_proc(t5))(3,t5,t3,t4);}

/* k2771 in k2724 in convert-position in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_2773(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word t4;
C_word ab[8],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_2773,2,t0,t1);}
t2=(C_word)C_a_i_sin(&a,1,t1);
t3=(*a=C_CLOSURE_TYPE|3,a[1]=(C_word)f_2769,a[2]=t2,a[3]=((C_word*)t0)[3],tmp=(C_word)a,a+=4,tmp);
C_trace("deg2rad");
t4=C_retrieve(lf[11]);
((C_proc3)C_retrieve_proc(t4))(3,t4,t3,((C_word*)t0)[2]);}

/* k2767 in k2771 in k2724 in convert-position in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_2769(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word ab[4],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_2769,2,t0,t1);}
t2=(C_word)C_a_i_cos(&a,1,t1);
C_trace("*");
C_times(5,0,((C_word*)t0)[3],C_fix(6360000),((C_word*)t0)[2],t2);}

/* k2731 in k2724 in convert-position in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_2733(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word t4;
C_word ab[10],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_2733,2,t0,t1);}
t2=(*a=C_CLOSURE_TYPE|5,a[1]=(C_word)f_2753,a[2]=t1,a[3]=((C_word*)t0)[4],a[4]=((C_word*)t0)[5],a[5]=((C_word*)t0)[6],tmp=(C_word)a,a+=6,tmp);
t3=(C_word)C_a_i_minus(&a,2,((C_word*)t0)[3],((C_word*)((C_word*)t0)[2])[1]);
C_trace("deg2rad");
t4=C_retrieve(lf[11]);
((C_proc3)C_retrieve_proc(t4))(3,t4,t2,t3);}

/* k2751 in k2731 in k2724 in convert-position in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_2753(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word t4;
C_word t5;
C_word t6;
C_word ab[25],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_2753,2,t0,t1);}
t2=(C_word)C_a_i_sin(&a,1,t1);
t3=(C_word)C_a_i_times(&a,2,C_fix(6360000),t2);
t4=(C_word)C_a_i_minus(&a,2,((C_word*)t0)[5],((C_word*)((C_word*)t0)[4])[1]);
t5=(C_word)C_a_i_divide(&a,2,t4,C_fix(10));
t6=((C_word*)t0)[3];
((C_proc2)(void*)(*((C_word*)t6+1)))(2,t6,(C_word)C_a_i_list(&a,3,((C_word*)t0)[2],t3,t5));}

/* parse-mag-line in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_2669(C_word c,C_word t0,C_word t1,C_word t2){
C_word tmp;
C_word t3;
C_word t4;
C_word ab[3],*a=ab;
if(c!=3) C_bad_argc_2(c,3,t0);
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr3,(void*)f_2669,3,t0,t1,t2);}
t3=(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_2673,a[2]=t1,tmp=(C_word)a,a+=3,tmp);
C_trace("string-split");
t4=C_retrieve(lf[96]);
((C_proc3)C_retrieve_proc(t4))(3,t4,t3,t2);}

/* k2671 in parse-mag-line in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_2673(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word t4;
C_word t5;
C_word ab[4],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_2673,2,t0,t1);}
t2=(*a=C_CLOSURE_TYPE|3,a[1]=(C_word)f_2679,a[2]=t1,a[3]=((C_word*)t0)[2],tmp=(C_word)a,a+=4,tmp);
t3=(C_word)C_i_length(t1);
if(C_truep((C_word)C_i_greater_or_equalp(t3,C_fix(3)))){
t4=(C_word)C_i_car(t1);
t5=t2;
f_2679(t5,(C_word)C_i_equalp(t4,lf[95]));}
else{
t4=t2;
f_2679(t4,C_SCHEME_FALSE);}}

/* k2677 in k2671 in parse-mag-line in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_fcall f_2679(C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word t4;
C_word t5;
C_word ab[6],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)trf_2679,NULL,2,t0,t1);}
if(C_truep(t1)){
t2=(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_2682,a[2]=((C_word*)t0)[3],tmp=(C_word)a,a+=3,tmp);
t3=(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_2699,a[2]=t2,tmp=(C_word)a,a+=3,tmp);
t4=(C_word)C_i_cdr(((C_word*)t0)[2]);
C_trace("take");
t5=C_retrieve(lf[94]);
((C_proc4)C_retrieve_proc(t5))(4,t5,t3,t4,C_fix(3));}
else{
t2=((C_word*)t0)[3];
((C_proc2)(void*)(*((C_word*)t2+1)))(2,t2,C_retrieve(lf[22]));}}

/* k2697 in k2677 in k2671 in parse-mag-line in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_2699(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word *a;
C_trace("map");
t2=*((C_word*)lf[4]+1);
((C_proc4)(void*)(*((C_word*)t2+1)))(4,t2,((C_word*)t0)[2],*((C_word*)lf[93]+1),t1);}

/* k2680 in k2677 in k2671 in parse-mag-line in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_2682(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word ab[4],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_2682,2,t0,t1);}
t2=(*a=C_CLOSURE_TYPE|3,a[1]=(C_word)f_2688,a[2]=t1,a[3]=((C_word*)t0)[2],tmp=(C_word)a,a+=4,tmp);
C_trace("none-false");
t3=C_retrieve(lf[89]);
((C_proc3)C_retrieve_proc(t3))(3,t3,t2,t1);}

/* k2686 in k2680 in k2677 in k2671 in parse-mag-line in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_2688(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word ab[3],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_2688,2,t0,t1);}
if(C_truep(t1)){
t2=(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_2692,a[2]=((C_word*)t0)[3],tmp=(C_word)a,a+=3,tmp);
C_trace("magnitude");
t3=C_retrieve(lf[2]);
((C_proc3)C_retrieve_proc(t3))(3,t3,t2,((C_word*)t0)[2]);}
else{
t2=((C_word*)t0)[3];
((C_proc2)(void*)(*((C_word*)t2+1)))(2,t2,C_retrieve(lf[22]));}}

/* k2690 in k2686 in k2680 in k2677 in k2671 in parse-mag-line in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_2692(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word *a;
t2=C_mutate((C_word*)lf[91]+1,t1);
t3=((C_word*)t0)[2];
((C_proc2)(void*)(*((C_word*)t3+1)))(2,t3,t2);}

/* none-false in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_2656(C_word c,C_word t0,C_word t1,C_word t2){
C_word tmp;
C_word t3;
C_word t4;
C_word ab[3],*a=ab;
if(c!=3) C_bad_argc_2(c,3,t0);
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr3,(void*)f_2656,3,t0,t1,t2);}
t3=(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_2662,a[2]=((C_word)li24),tmp=(C_word)a,a+=3,tmp);
C_trace("fold");
t4=C_retrieve(lf[90]);
((C_proc5)C_retrieve_proc(t4))(5,t4,t1,t3,C_SCHEME_TRUE,t2);}

/* a2661 in none-false in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_2662(C_word c,C_word t0,C_word t1,C_word t2,C_word t3){
C_word tmp;
C_word t4;
C_word *a;
if(c!=4) C_bad_argc_2(c,4,t0);
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr4,(void*)f_2662,4,t0,t1,t2,t3);}
t4=t1;
((C_proc2)(void*)(*((C_word*)t4+1)))(2,t4,(C_truep(t2)?t3:C_SCHEME_FALSE));}

/* gl-display in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_2632(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word ab[3],*a=ab;
if(c!=2) C_bad_argc_2(c,2,t0);
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_2632,2,t0,t1);}
t2=(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_2636,a[2]=t1,tmp=(C_word)a,a+=3,tmp);
C_trace("gl:Clear");
t3=C_retrieve(lf[87]);
((C_proc3)C_retrieve_proc(t3))(3,t3,t2,C_retrieve(lf[88]));}

/* k2634 in gl-display in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_2636(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word ab[3],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_2636,2,t0,t1);}
t2=(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_2639,a[2]=((C_word*)t0)[2],tmp=(C_word)a,a+=3,tmp);
C_trace("gl:PushMatrix");
t3=C_retrieve(lf[79]);
((C_proc2)C_retrieve_proc(t3))(2,t3,t2);}

/* k2637 in k2634 in gl-display in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_2639(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word ab[3],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_2639,2,t0,t1);}
t2=(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_2642,a[2]=((C_word*)t0)[2],tmp=(C_word)a,a+=3,tmp);
C_trace("gl:Translatef");
t3=C_retrieve(lf[56]);
((C_proc5)C_retrieve_proc(t3))(5,t3,t2,C_fix(0),C_fix(0),C_fix(-1));}

/* k2640 in k2637 in k2634 in gl-display in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_2642(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word ab[3],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_2642,2,t0,t1);}
t2=(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_2645,a[2]=((C_word*)t0)[2],tmp=(C_word)a,a+=3,tmp);
C_trace("trianglegrid-draw");
t3=C_retrieve(lf[31]);
((C_proc2)C_retrieve_proc(t3))(2,t3,t2);}

/* k2643 in k2640 in k2637 in k2634 in gl-display in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_2645(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word ab[3],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_2645,2,t0,t1);}
t2=(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_2648,a[2]=((C_word*)t0)[2],tmp=(C_word)a,a+=3,tmp);
C_trace("pos-draw");
t3=C_retrieve(lf[32]);
((C_proc5)C_retrieve_proc(t3))(5,t3,t2,C_retrieve(lf[14]),C_retrieve(lf[12]),lf[86]);}

/* k2646 in k2643 in k2640 in k2637 in k2634 in gl-display in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_2648(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word ab[3],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_2648,2,t0,t1);}
t2=(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_2651,a[2]=((C_word*)t0)[2],tmp=(C_word)a,a+=3,tmp);
C_trace("gl:PopMatrix");
t3=C_retrieve(lf[72]);
((C_proc2)C_retrieve_proc(t3))(2,t3,t2);}

/* k2649 in k2646 in k2643 in k2640 in k2637 in k2634 in gl-display in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_2651(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word *a;
C_trace("glut:SwapBuffers");
t2=C_retrieve(lf[85]);
((C_proc2)C_retrieve_proc(t2))(2,t2,((C_word*)t0)[2]);}

/* pos-draw in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_2515(C_word c,C_word t0,C_word t1,C_word t2,C_word t3,C_word t4){
C_word tmp;
C_word t5;
C_word t6;
C_word ab[6],*a=ab;
if(c!=5) C_bad_argc_2(c,5,t0);
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr5,(void*)f_2515,5,t0,t1,t2,t3,t4);}
t5=(*a=C_CLOSURE_TYPE|5,a[1]=(C_word)f_2519,a[2]=t2,a[3]=t4,a[4]=t3,a[5]=t1,tmp=(C_word)a,a+=6,tmp);
C_trace("gl:BlendFunc");
t6=C_retrieve(lf[81]);
((C_proc4)C_retrieve_proc(t6))(4,t6,t5,C_retrieve(lf[82]),C_retrieve(lf[83]));}

/* k2517 in pos-draw in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_2519(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word ab[6],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_2519,2,t0,t1);}
t2=(*a=C_CLOSURE_TYPE|5,a[1]=(C_word)f_2522,a[2]=((C_word*)t0)[2],a[3]=((C_word*)t0)[3],a[4]=((C_word*)t0)[4],a[5]=((C_word*)t0)[5],tmp=(C_word)a,a+=6,tmp);
C_trace("gl:Enable");
t3=C_retrieve(lf[80]);
((C_proc3)C_retrieve_proc(t3))(3,t3,t2,C_retrieve(lf[71]));}

/* k2520 in k2517 in pos-draw in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_2522(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word ab[6],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_2522,2,t0,t1);}
t2=(*a=C_CLOSURE_TYPE|5,a[1]=(C_word)f_2525,a[2]=((C_word*)t0)[2],a[3]=((C_word*)t0)[3],a[4]=((C_word*)t0)[4],a[5]=((C_word*)t0)[5],tmp=(C_word)a,a+=6,tmp);
C_trace("gl:PushMatrix");
t3=C_retrieve(lf[79]);
((C_proc2)C_retrieve_proc(t3))(2,t3,t2);}

/* k2523 in k2520 in k2517 in pos-draw in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_2525(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word t4;
C_word t5;
C_word t6;
C_word ab[5],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_2525,2,t0,t1);}
t2=(*a=C_CLOSURE_TYPE|4,a[1]=(C_word)f_2528,a[2]=((C_word*)t0)[3],a[3]=((C_word*)t0)[4],a[4]=((C_word*)t0)[5],tmp=(C_word)a,a+=5,tmp);
t3=(C_word)C_i_car(((C_word*)t0)[2]);
t4=(C_word)C_i_cadr(((C_word*)t0)[2]);
t5=(C_word)C_i_caddr(((C_word*)t0)[2]);
C_trace("gl:Translatef");
t6=C_retrieve(lf[56]);
((C_proc5)C_retrieve_proc(t6))(5,t6,t2,t3,t4,t5);}

/* k2526 in k2523 in k2520 in k2517 in pos-draw in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_2528(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word ab[5],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_2528,2,t0,t1);}
t2=(*a=C_CLOSURE_TYPE|4,a[1]=(C_word)f_2531,a[2]=((C_word*)t0)[2],a[3]=((C_word*)t0)[3],a[4]=((C_word*)t0)[4],tmp=(C_word)a,a+=5,tmp);
C_trace("gl:Begin");
t3=C_retrieve(lf[44]);
((C_proc3)C_retrieve_proc(t3))(3,t3,t2,C_retrieve(lf[78]));}

/* k2529 in k2526 in k2523 in k2520 in k2517 in pos-draw in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_2531(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word ab[5],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_2531,2,t0,t1);}
t2=(*a=C_CLOSURE_TYPE|4,a[1]=(C_word)f_2534,a[2]=((C_word*)t0)[2],a[3]=((C_word*)t0)[3],a[4]=((C_word*)t0)[4],tmp=(C_word)a,a+=5,tmp);
C_trace("gl:Color4f");
t3=C_retrieve(lf[74]);
((C_proc6)C_retrieve_proc(t3))(6,t3,t2,lf[75],lf[76],lf[77],C_fix(1));}

/* k2532 in k2529 in k2526 in k2523 in k2520 in k2517 in pos-draw in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_2534(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word ab[5],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_2534,2,t0,t1);}
t2=(*a=C_CLOSURE_TYPE|4,a[1]=(C_word)f_2537,a[2]=((C_word*)t0)[2],a[3]=((C_word*)t0)[3],a[4]=((C_word*)t0)[4],tmp=(C_word)a,a+=5,tmp);
C_trace("gl:Vertex3f");
t3=C_retrieve(lf[29]);
((C_proc5)C_retrieve_proc(t3))(5,t3,t2,C_fix(0),C_fix(0),C_fix(0));}

/* k2535 in k2532 in k2529 in k2526 in k2523 in k2520 in k2517 in pos-draw in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_2537(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word t4;
C_word t5;
C_word t6;
C_word ab[4],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_2537,2,t0,t1);}
t2=(*a=C_CLOSURE_TYPE|3,a[1]=(C_word)f_2540,a[2]=((C_word*)t0)[3],a[3]=((C_word*)t0)[4],tmp=(C_word)a,a+=4,tmp);
t3=(C_word)C_i_car(((C_word*)t0)[2]);
t4=(C_word)C_i_cadr(((C_word*)t0)[2]);
t5=(C_word)C_i_caddr(((C_word*)t0)[2]);
C_trace("gl:Color4f");
t6=C_retrieve(lf[74]);
((C_proc6)C_retrieve_proc(t6))(6,t6,t2,t3,t4,t5,C_fix(0));}

/* k2538 in k2535 in k2532 in k2529 in k2526 in k2523 in k2520 in k2517 in pos-draw in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_2540(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word t4;
C_word t5;
C_word t6;
C_word ab[10],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_2540,2,t0,t1);}
t2=(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_2543,a[2]=((C_word*)t0)[3],tmp=(C_word)a,a+=3,tmp);
t3=C_SCHEME_UNDEFINED;
t4=(*a=C_VECTOR_TYPE|1,a[1]=t3,tmp=(C_word)a,a+=2,tmp);
t5=C_set_block_item(t4,0,(*a=C_CLOSURE_TYPE|4,a[1]=(C_word)f_2554,a[2]=((C_word*)t0)[2],a[3]=t4,a[4]=((C_word)li21),tmp=(C_word)a,a+=5,tmp));
t6=((C_word*)t4)[1];
f_2554(t6,t2,C_fix(0));}

/* doloop.1746 in k2538 in k2535 in k2532 in k2529 in k2526 in k2523 in k2520 in k2517 in pos-draw in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_fcall f_2554(C_word t0,C_word t1,C_word t2){
C_word tmp;
C_word t3;
C_word t4;
C_word t5;
C_word t6;
C_word t7;
C_word t8;
C_word t9;
C_word t10;
C_word ab[29],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)trf_2554,NULL,3,t0,t1,t2);}
t3=(C_word)C_a_i_times(&a,2,C_fix(2),C_retrieve(lf[9]));
t4=(C_word)C_a_i_plus(&a,2,t3,lf[73]);
if(C_truep((C_word)C_i_greaterp(t2,t4))){
t5=t1;
((C_proc2)(void*)(*((C_word*)t5+1)))(2,t5,C_retrieve(lf[22]));}
else{
t5=(*a=C_CLOSURE_TYPE|4,a[1]=(C_word)f_2564,a[2]=t1,a[3]=((C_word*)t0)[3],a[4]=t2,tmp=(C_word)a,a+=5,tmp);
t6=(C_word)C_a_i_cos(&a,1,t2);
t7=(C_word)C_a_i_times(&a,2,((C_word*)t0)[2],t6);
t8=(C_word)C_a_i_sin(&a,1,t2);
t9=(C_word)C_a_i_times(&a,2,((C_word*)t0)[2],t8);
C_trace("gl:Vertex3f");
t10=C_retrieve(lf[29]);
((C_proc5)C_retrieve_proc(t10))(5,t10,t5,t7,t9,C_fix(0));}}

/* k2562 in doloop.1746 in k2538 in k2535 in k2532 in k2529 in k2526 in k2523 in k2520 in k2517 in pos-draw in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_2564(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word t4;
C_word ab[8],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_2564,2,t0,t1);}
t2=(C_word)C_a_i_divide(&a,2,C_retrieve(lf[9]),C_fix(10));
t3=(C_word)C_a_i_plus(&a,2,((C_word*)t0)[4],t2);
C_trace("doloop.17461751");
t4=((C_word*)((C_word*)t0)[3])[1];
f_2554(t4,((C_word*)t0)[2],t3);}

/* k2541 in k2538 in k2535 in k2532 in k2529 in k2526 in k2523 in k2520 in k2517 in pos-draw in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_2543(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word ab[3],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_2543,2,t0,t1);}
t2=(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_2546,a[2]=((C_word*)t0)[2],tmp=(C_word)a,a+=3,tmp);
C_trace("gl:End");
t3=C_retrieve(lf[42]);
((C_proc2)C_retrieve_proc(t3))(2,t3,t2);}

/* k2544 in k2541 in k2538 in k2535 in k2532 in k2529 in k2526 in k2523 in k2520 in k2517 in pos-draw in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_2546(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word ab[3],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_2546,2,t0,t1);}
t2=(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_2549,a[2]=((C_word*)t0)[2],tmp=(C_word)a,a+=3,tmp);
C_trace("gl:PopMatrix");
t3=C_retrieve(lf[72]);
((C_proc2)C_retrieve_proc(t3))(2,t3,t2);}

/* k2547 in k2544 in k2541 in k2538 in k2535 in k2532 in k2529 in k2526 in k2523 in k2520 in k2517 in pos-draw in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_2549(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word *a;
C_trace("gl:Disable");
t2=C_retrieve(lf[70]);
((C_proc3)C_retrieve_proc(t2))(3,t2,((C_word*)t0)[2],C_retrieve(lf[71]));}

/* reshape in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_2480(C_word c,C_word t0,C_word t1,C_word t2,C_word t3){
C_word tmp;
C_word t4;
C_word t5;
C_word ab[5],*a=ab;
if(c!=4) C_bad_argc_2(c,4,t0);
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr4,(void*)f_2480,4,t0,t1,t2,t3);}
t4=(*a=C_CLOSURE_TYPE|4,a[1]=(C_word)f_2484,a[2]=t1,a[3]=t3,a[4]=t2,tmp=(C_word)a,a+=5,tmp);
C_trace("gl:Viewport");
t5=C_retrieve(lf[69]);
((C_proc6)C_retrieve_proc(t5))(6,t5,t4,C_fix(0),C_fix(0),t2,t3);}

/* k2482 in reshape in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_2484(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word ab[5],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_2484,2,t0,t1);}
t2=(*a=C_CLOSURE_TYPE|4,a[1]=(C_word)f_2487,a[2]=((C_word*)t0)[2],a[3]=((C_word*)t0)[3],a[4]=((C_word*)t0)[4],tmp=(C_word)a,a+=5,tmp);
C_trace("gl:MatrixMode");
t3=C_retrieve(lf[60]);
((C_proc3)C_retrieve_proc(t3))(3,t3,t2,C_retrieve(lf[68]));}

/* k2485 in k2482 in reshape in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_2487(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word ab[5],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_2487,2,t0,t1);}
t2=(*a=C_CLOSURE_TYPE|4,a[1]=(C_word)f_2490,a[2]=((C_word*)t0)[2],a[3]=((C_word*)t0)[3],a[4]=((C_word*)t0)[4],tmp=(C_word)a,a+=5,tmp);
C_trace("gl:LoadIdentity");
t3=C_retrieve(lf[51]);
((C_proc2)C_retrieve_proc(t3))(2,t3,t2);}

/* k2488 in k2485 in k2482 in reshape in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_2490(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word t4;
C_word t5;
C_word t6;
C_word ab[15],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_2490,2,t0,t1);}
t2=(C_word)C_a_i_divide(&a,2,((C_word*)t0)[4],((C_word*)t0)[3]);
t3=(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_2496,a[2]=((C_word*)t0)[2],tmp=(C_word)a,a+=3,tmp);
t4=(C_word)C_a_i_times(&a,2,lf[62],t2);
t5=(C_word)C_a_i_times(&a,2,lf[63],t2);
C_trace("gl:Frustum");
t6=C_retrieve(lf[64]);
((C_proc8)C_retrieve_proc(t6))(8,t6,t3,t4,t5,lf[65],lf[66],lf[67],C_fix(100));}

/* k2494 in k2488 in k2485 in k2482 in reshape in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_2496(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word ab[3],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_2496,2,t0,t1);}
t2=(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_2499,a[2]=((C_word*)t0)[2],tmp=(C_word)a,a+=3,tmp);
C_trace("gl:MatrixMode");
t3=C_retrieve(lf[60]);
((C_proc3)C_retrieve_proc(t3))(3,t3,t2,C_retrieve(lf[61]));}

/* k2497 in k2494 in k2488 in k2485 in k2482 in reshape in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_2499(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word ab[3],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_2499,2,t0,t1);}
t2=(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_2502,a[2]=((C_word*)t0)[2],tmp=(C_word)a,a+=3,tmp);
C_trace("gl:LoadIdentity");
t3=C_retrieve(lf[51]);
((C_proc2)C_retrieve_proc(t3))(2,t3,t2);}

/* k2500 in k2497 in k2494 in k2488 in k2485 in k2482 in reshape in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_2502(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word *a;
C_trace("gl:Translatef");
t2=C_retrieve(lf[56]);
((C_proc5)C_retrieve_proc(t2))(5,t2,((C_word*)t0)[2],lf[57],lf[58],lf[59]);}

/* RotateAfter in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_2462(C_word c,C_word t0,C_word t1,C_word t2,C_word t3,C_word t4,C_word t5){
C_word tmp;
C_word t6;
C_word t7;
C_word ab[7],*a=ab;
if(c!=6) C_bad_argc_2(c,6,t0);
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr6,(void*)f_2462,6,t0,t1,t2,t3,t4,t5);}
t6=(*a=C_CLOSURE_TYPE|6,a[1]=(C_word)f_2466,a[2]=t5,a[3]=t4,a[4]=t3,a[5]=t2,a[6]=t1,tmp=(C_word)a,a+=7,tmp);
C_trace("f32vector");
t7=C_retrieve(lf[54]);
((C_proc2)C_retrieve_proc(t7))(2,t7,t6);}

/* k2464 in RotateAfter in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_2466(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word ab[8],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_2466,2,t0,t1);}
t2=(*a=C_CLOSURE_TYPE|7,a[1]=(C_word)f_2469,a[2]=((C_word*)t0)[2],a[3]=((C_word*)t0)[3],a[4]=((C_word*)t0)[4],a[5]=((C_word*)t0)[5],a[6]=t1,a[7]=((C_word*)t0)[6],tmp=(C_word)a,a+=8,tmp);
C_trace("gl:GetFloatv");
t3=C_retrieve(lf[52]);
((C_proc4)C_retrieve_proc(t3))(4,t3,t2,C_retrieve(lf[53]),t1);}

/* k2467 in k2464 in RotateAfter in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_2469(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word ab[8],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_2469,2,t0,t1);}
t2=(*a=C_CLOSURE_TYPE|7,a[1]=(C_word)f_2472,a[2]=((C_word*)t0)[2],a[3]=((C_word*)t0)[3],a[4]=((C_word*)t0)[4],a[5]=((C_word*)t0)[5],a[6]=((C_word*)t0)[6],a[7]=((C_word*)t0)[7],tmp=(C_word)a,a+=8,tmp);
C_trace("gl:LoadIdentity");
t3=C_retrieve(lf[51]);
((C_proc2)C_retrieve_proc(t3))(2,t3,t2);}

/* k2470 in k2467 in k2464 in RotateAfter in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_2472(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word ab[4],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_2472,2,t0,t1);}
t2=(*a=C_CLOSURE_TYPE|3,a[1]=(C_word)f_2475,a[2]=((C_word*)t0)[6],a[3]=((C_word*)t0)[7],tmp=(C_word)a,a+=4,tmp);
C_trace("gl:Rotatef");
t3=C_retrieve(lf[50]);
((C_proc6)C_retrieve_proc(t3))(6,t3,t2,((C_word*)t0)[5],((C_word*)t0)[4],((C_word*)t0)[3],((C_word*)t0)[2]);}

/* k2473 in k2470 in k2467 in k2464 in RotateAfter in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_2475(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word *a;
C_trace("gl:MultMatrixf");
t2=C_retrieve(lf[49]);
((C_proc3)C_retrieve_proc(t2))(3,t2,((C_word*)t0)[3],((C_word*)t0)[2]);}

/* draw-triangles in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_2420(C_word c,C_word t0,C_word t1,C_word t2){
C_word tmp;
C_word t3;
C_word t4;
C_word ab[4],*a=ab;
if(c!=3) C_bad_argc_2(c,3,t0);
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr3,(void*)f_2420,3,t0,t1,t2);}
t3=(*a=C_CLOSURE_TYPE|3,a[1]=(C_word)f_2424,a[2]=t2,a[3]=t1,tmp=(C_word)a,a+=4,tmp);
C_apply(4,0,t3,*((C_word*)lf[47]+1),C_retrieve(lf[16]));}

/* k2422 in draw-triangles in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_2424(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word ab[4],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_2424,2,t0,t1);}
t2=(*a=C_CLOSURE_TYPE|3,a[1]=(C_word)f_2427,a[2]=((C_word*)t0)[2],a[3]=((C_word*)t0)[3],tmp=(C_word)a,a+=4,tmp);
C_apply(4,0,t2,*((C_word*)lf[46]+1),C_retrieve(lf[16]));}

/* k2425 in k2422 in draw-triangles in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_2427(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word ab[4],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_2427,2,t0,t1);}
t2=(*a=C_CLOSURE_TYPE|3,a[1]=(C_word)f_2430,a[2]=((C_word*)t0)[2],a[3]=((C_word*)t0)[3],tmp=(C_word)a,a+=4,tmp);
C_trace("gl:Begin");
t3=C_retrieve(lf[44]);
((C_proc3)C_retrieve_proc(t3))(3,t3,t2,C_retrieve(lf[45]));}

/* k2428 in k2425 in k2422 in draw-triangles in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_2430(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word t4;
C_word ab[6],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_2430,2,t0,t1);}
t2=(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_2433,a[2]=((C_word*)t0)[3],tmp=(C_word)a,a+=3,tmp);
t3=(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_2438,a[2]=((C_word)li17),tmp=(C_word)a,a+=3,tmp);
C_trace("for-each");
t4=*((C_word*)lf[23]+1);
((C_proc4)(void*)(*((C_word*)t4+1)))(4,t4,t2,t3,((C_word*)t0)[2]);}

/* a2437 in k2428 in k2425 in k2422 in draw-triangles in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_2438(C_word c,C_word t0,C_word t1,C_word t2){
C_word tmp;
C_word t3;
C_word t4;
C_word t5;
C_word ab[3],*a=ab;
if(c!=3) C_bad_argc_2(c,3,t0);
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr3,(void*)f_2438,3,t0,t1,t2);}
t3=(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_2442,a[2]=t1,tmp=(C_word)a,a+=3,tmp);
t4=(C_word)C_i_car(C_retrieve(lf[43]));
C_trace("draw-point");
t5=C_retrieve(lf[39]);
((C_proc3)C_retrieve_proc(t5))(3,t5,t3,t4);}

/* k2440 in a2437 in k2428 in k2425 in k2422 in draw-triangles in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_2442(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word t4;
C_word ab[3],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_2442,2,t0,t1);}
t2=(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_2445,a[2]=((C_word*)t0)[2],tmp=(C_word)a,a+=3,tmp);
t3=(C_word)C_i_cadr(C_retrieve(lf[43]));
C_trace("draw-point");
t4=C_retrieve(lf[39]);
((C_proc3)C_retrieve_proc(t4))(3,t4,t2,t3);}

/* k2443 in k2440 in a2437 in k2428 in k2425 in k2422 in draw-triangles in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_2445(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word *a;
t2=(C_word)C_i_caddr(C_retrieve(lf[43]));
C_trace("draw-point");
t3=C_retrieve(lf[39]);
((C_proc3)C_retrieve_proc(t3))(3,t3,((C_word*)t0)[2],t2);}

/* k2431 in k2428 in k2425 in k2422 in draw-triangles in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_2433(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word *a;
C_trace("gl:End");
t2=C_retrieve(lf[42]);
((C_proc2)C_retrieve_proc(t2))(2,t2,((C_word*)t0)[2]);}

/* draw-point in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_2400(C_word c,C_word t0,C_word t1,C_word t2){
C_word tmp;
C_word t3;
C_word t4;
C_word t5;
C_word ab[7],*a=ab;
if(c!=3) C_bad_argc_2(c,3,t0);
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr3,(void*)f_2400,3,t0,t1,t2);}
t3=(*a=C_CLOSURE_TYPE|3,a[1]=(C_word)f_2404,a[2]=t2,a[3]=t1,tmp=(C_word)a,a+=4,tmp);
t4=(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_2418,a[2]=t3,tmp=(C_word)a,a+=3,tmp);
C_trace("point-color");
t5=C_retrieve(lf[36]);
((C_proc3)C_retrieve_proc(t5))(3,t5,t4,t2);}

/* k2416 in draw-point in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_2418(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word *a;
C_trace("colorspace");
t2=C_retrieve(lf[24]);
((C_proc3)C_retrieve_proc(t2))(3,t2,((C_word*)t0)[2],t1);}

/* k2402 in draw-point in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_2404(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word ab[4],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_2404,2,t0,t1);}
t2=(*a=C_CLOSURE_TYPE|3,a[1]=(C_word)f_2407,a[2]=((C_word*)t0)[2],a[3]=((C_word*)t0)[3],tmp=(C_word)a,a+=4,tmp);
C_trace("gl:Color3f");
t3=C_retrieve(lf[30]);
((C_proc5)C_retrieve_proc(t3))(5,t3,t2,t1,t1,t1);}

/* k2405 in k2402 in draw-point in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_2407(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word ab[3],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_2407,2,t0,t1);}
t2=(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_2414,a[2]=((C_word*)t0)[3],tmp=(C_word)a,a+=3,tmp);
C_trace("point-pos");
t3=C_retrieve(lf[34]);
((C_proc3)C_retrieve_proc(t3))(3,t3,t2,((C_word*)t0)[2]);}

/* k2412 in k2405 in k2402 in draw-point in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_2414(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word *a;
C_trace("gl:Vertex3fv");
t2=C_retrieve(lf[40]);
((C_proc3)C_retrieve_proc(t2))(3,t2,((C_word*)t0)[2],t1);}

/* new-point in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_2394(C_word c,C_word t0,C_word t1,C_word t2,C_word t3){
C_word tmp;
C_word t4;
C_word ab[6],*a=ab;
if(c!=4) C_bad_argc_2(c,4,t0);
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr4,(void*)f_2394,4,t0,t1,t2,t3);}
t4=t1;
((C_proc2)(void*)(*((C_word*)t4+1)))(2,t4,(C_word)C_a_i_list(&a,2,t2,t3));}

/* trianglegrid-draw in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_2369(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word ab[3],*a=ab;
if(c!=2) C_bad_argc_2(c,2,t0);
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_2369,2,t0,t1);}
t2=(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_2373,a[2]=t1,tmp=(C_word)a,a+=3,tmp);
C_trace("recalc-colorspace");
t3=C_retrieve(lf[21]);
((C_proc2)C_retrieve_proc(t3))(2,t3,t2);}

/* k2371 in trianglegrid-draw in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_2373(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word ab[3],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_2373,2,t0,t1);}
t2=(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_2378,a[2]=((C_word)li13),tmp=(C_word)a,a+=3,tmp);
C_trace("for-each");
t3=*((C_word*)lf[23]+1);
((C_proc4)(void*)(*((C_word*)t3+1)))(4,t3,((C_word*)t0)[2],t2,C_retrieve(lf[16]));}

/* a2377 in k2371 in trianglegrid-draw in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_2378(C_word c,C_word t0,C_word t1,C_word t2){
C_word tmp;
C_word t3;
C_word t4;
C_word t5;
C_word ab[4],*a=ab;
if(c!=3) C_bad_argc_2(c,3,t0);
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr3,(void*)f_2378,3,t0,t1,t2);}
t3=(*a=C_CLOSURE_TYPE|3,a[1]=(C_word)f_2386,a[2]=t2,a[3]=t1,tmp=(C_word)a,a+=4,tmp);
t4=(C_word)C_i_cadddr(t2);
C_trace("colorspace");
t5=C_retrieve(lf[24]);
((C_proc3)C_retrieve_proc(t5))(3,t5,t3,t4);}

/* k2384 in a2377 in k2371 in trianglegrid-draw in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_2386(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word *a;
C_trace("pos-draw");
t2=C_retrieve(lf[32]);
((C_proc5)C_retrieve_proc(t2))(5,t2,((C_word*)t0)[3],((C_word*)t0)[2],lf[33],t1);}

/* make-gl-point in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_2340(C_word c,C_word t0,C_word t1,C_word t2){
C_word tmp;
C_word t3;
C_word t4;
C_word t5;
C_word t6;
C_word ab[7],*a=ab;
if(c!=3) C_bad_argc_2(c,3,t0);
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr3,(void*)f_2340,3,t0,t1,t2);}
t3=(*a=C_CLOSURE_TYPE|3,a[1]=(C_word)f_2344,a[2]=t1,a[3]=t2,tmp=(C_word)a,a+=4,tmp);
t4=(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_2363,a[2]=t3,tmp=(C_word)a,a+=3,tmp);
t5=(C_word)C_i_cadddr(t2);
C_trace("colorspace");
t6=C_retrieve(lf[24]);
((C_proc3)C_retrieve_proc(t6))(3,t6,t4,t5);}

/* k2361 in make-gl-point in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_2363(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word *a;
C_apply(4,0,((C_word*)t0)[2],C_retrieve(lf[30]),t1);}

/* k2342 in make-gl-point in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_2344(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word t4;
C_word t5;
C_word *a;
t2=(C_word)C_i_car(((C_word*)t0)[3]);
t3=(C_word)C_i_cadr(((C_word*)t0)[3]);
t4=(C_word)C_i_caddr(((C_word*)t0)[3]);
C_trace("gl:Vertex3f");
t5=C_retrieve(lf[29]);
((C_proc5)C_retrieve_proc(t5))(5,t5,((C_word*)t0)[2],t2,t3,t4);}

/* colorspace in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_2284(C_word c,C_word t0,C_word t1,C_word t2){
C_word tmp;
C_word t3;
C_word t4;
C_word t5;
C_word t6;
C_word t7;
C_word t8;
C_word ab[11],*a=ab;
if(c!=3) C_bad_argc_2(c,3,t0);
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr3,(void*)f_2284,3,t0,t1,t2);}
t3=(C_word)C_a_i_minus(&a,2,C_retrieve(lf[20]),C_retrieve(lf[19]));
t4=t3;
t5=(*a=C_VECTOR_TYPE|1,a[1]=t4,tmp=(C_word)a,a+=2,tmp);
t6=(*a=C_CLOSURE_TYPE|4,a[1]=(C_word)f_2291,a[2]=t1,a[3]=t5,a[4]=t2,tmp=(C_word)a,a+=5,tmp);
if(C_truep((C_word)C_i_nequalp(((C_word*)t5)[1],C_fix(0)))){
t7=C_set_block_item(t5,0,lf[27]);
t8=t6;
f_2291(t8,t7);}
else{
t7=t6;
f_2291(t7,C_retrieve(lf[22]));}}

/* k2289 in colorspace in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_fcall f_2291(C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word t4;
C_word t5;
C_word t6;
C_word t7;
C_word t8;
C_word ab[24],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)trf_2291,NULL,2,t0,t1);}
t2=(C_word)C_a_i_plus(&a,2,C_retrieve(lf[19]),C_retrieve(lf[20]));
t3=(C_word)C_a_i_divide(&a,2,t2,C_fix(2));
t4=(C_word)C_a_i_minus(&a,2,((C_word*)t0)[4],t3);
t5=(C_word)C_a_i_divide(&a,2,t4,((C_word*)((C_word*)t0)[3])[1]);
t6=(C_word)C_a_i_plus(&a,2,t5,lf[25]);
t7=(*a=C_CLOSURE_TYPE|3,a[1]=(C_word)f_2299,a[2]=t6,a[3]=((C_word)li10),tmp=(C_word)a,a+=4,tmp);
C_trace("map");
t8=*((C_word*)lf[4]+1);
((C_proc4)(void*)(*((C_word*)t8+1)))(4,t8,((C_word*)t0)[2],t7,lf[26]);}

/* a2298 in k2289 in colorspace in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_2299(C_word c,C_word t0,C_word t1,C_word t2){
C_word tmp;
C_word t3;
C_word t4;
C_word t5;
C_word t6;
C_word ab[16],*a=ab;
if(c!=3) C_bad_argc_2(c,3,t0);
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr3,(void*)f_2299,3,t0,t1,t2);}
t3=(C_word)C_a_i_minus(&a,2,t2,((C_word*)t0)[2]);
t4=(C_word)C_a_i_abs(&a,1,t3);
t5=(C_word)C_a_i_times(&a,2,C_fix(2),t4);
t6=t1;
((C_proc2)(void*)(*((C_word*)t6+1)))(2,t6,(C_word)C_a_i_minus(&a,2,C_fix(1),t5));}

/* recalc-colorspace in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_2252(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word ab[3],*a=ab;
if(c!=2) C_bad_argc_2(c,2,t0);
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_2252,2,t0,t1);}
t2=(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_2258,a[2]=((C_word)li8),tmp=(C_word)a,a+=3,tmp);
C_trace("for-each");
t3=*((C_word*)lf[23]+1);
((C_proc4)(void*)(*((C_word*)t3+1)))(4,t3,t1,t2,C_retrieve(lf[16]));}

/* a2257 in recalc-colorspace in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_2258(C_word c,C_word t0,C_word t1,C_word t2){
C_word tmp;
C_word t3;
C_word t4;
C_word t5;
C_word t6;
C_word ab[4],*a=ab;
if(c!=3) C_bad_argc_2(c,3,t0);
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr3,(void*)f_2258,3,t0,t1,t2);}
t3=(C_word)C_i_cadddr(t2);
t4=(*a=C_CLOSURE_TYPE|3,a[1]=(C_word)f_2265,a[2]=t1,a[3]=t3,tmp=(C_word)a,a+=4,tmp);
if(C_truep((C_word)C_i_lessp(t3,C_retrieve(lf[19])))){
t5=C_mutate((C_word*)lf[19]+1,t3);
t6=t4;
f_2265(t6,t5);}
else{
t5=t4;
f_2265(t5,C_retrieve(lf[22]));}}

/* k2263 in a2257 in recalc-colorspace in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_fcall f_2265(C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word *a;
if(C_truep((C_word)C_i_greaterp(((C_word*)t0)[3],C_retrieve(lf[20])))){
t2=C_mutate((C_word*)lf[20]+1,((C_word*)t0)[3]);
t3=((C_word*)t0)[2];
((C_proc2)(void*)(*((C_word*)t3+1)))(2,t3,t2);}
else{
t2=((C_word*)t0)[2];
((C_proc2)(void*)(*((C_word*)t2+1)))(2,t2,C_retrieve(lf[22]));}}

/* trianglegrid-addpoint in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_2240(C_word c,C_word t0,C_word t1,C_word t2,C_word t3,C_word t4,C_word t5){
C_word tmp;
C_word t6;
C_word t7;
C_word t8;
C_word t9;
C_word ab[15],*a=ab;
if(c!=6) C_bad_argc_2(c,6,t0);
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr6,(void*)f_2240,6,t0,t1,t2,t3,t4,t5);}
t6=(C_word)C_a_i_list(&a,4,t2,t3,t4,t5);
t7=(C_word)C_a_i_cons(&a,2,t6,C_retrieve(lf[16]));
t8=C_mutate((C_word*)lf[16]+1,t7);
t9=t1;
((C_proc2)(void*)(*((C_word*)t9+1)))(2,t9,t8);}

/* deg2rad in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_2226(C_word c,C_word t0,C_word t1,C_word t2){
C_word tmp;
C_word t3;
C_word t4;
C_word ab[8],*a=ab;
if(c!=3) C_bad_argc_2(c,3,t0);
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr3,(void*)f_2226,3,t0,t1,t2);}
t3=(C_word)C_a_i_divide(&a,2,t2,C_fix(180));
t4=t1;
((C_proc2)(void*)(*((C_word*)t4+1)))(2,t4,(C_word)C_a_i_times(&a,2,C_retrieve(lf[9]),t3));}

/* dot in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_2212(C_word c,C_word t0,C_word t1,...){
C_word tmp;
C_word t2;
va_list v;
C_word *a,c2=c;
C_save_rest(t1,c2,2);
C_check_for_interrupt;
if(!C_demand(c*C_SIZEOF_PAIR+3)){
C_save_and_reclaim((void*)tr2r,(void*)f_2212r,2,t0,t1);}
else{
a=C_alloc((c-2)*3);
t2=C_restore_rest(a,C_rest_count(0));
f_2212r(t0,t1,t2);}}

static void C_ccall f_2212r(C_word t0,C_word t1,C_word t2){
C_word tmp;
C_word t3;
C_word t4;
C_word *a=C_alloc(3);
t3=(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_2220,a[2]=t1,tmp=(C_word)a,a+=3,tmp);
C_apply(5,0,t3,*((C_word*)lf[7]+1),*((C_word*)lf[8]+1),t2);}

/* k2218 in dot in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_2220(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word *a;
C_apply(4,0,((C_word*)t0)[2],*((C_word*)lf[3]+1),t1);}

/* normalize in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_2197(C_word c,C_word t0,C_word t1,C_word t2){
C_word tmp;
C_word t3;
C_word t4;
C_word ab[4],*a=ab;
if(c!=3) C_bad_argc_2(c,3,t0);
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr3,(void*)f_2197,3,t0,t1,t2);}
t3=(*a=C_CLOSURE_TYPE|3,a[1]=(C_word)f_2201,a[2]=t2,a[3]=t1,tmp=(C_word)a,a+=4,tmp);
C_trace("magnitude");
t4=C_retrieve(lf[2]);
((C_proc3)C_retrieve_proc(t4))(3,t4,t3,t2);}

/* k2199 in normalize in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_2201(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word t3;
C_word ab[4],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_2201,2,t0,t1);}
t2=(*a=C_CLOSURE_TYPE|3,a[1]=(C_word)f_2206,a[2]=t1,a[3]=((C_word)li3),tmp=(C_word)a,a+=4,tmp);
C_trace("map");
t3=*((C_word*)lf[4]+1);
((C_proc4)(void*)(*((C_word*)t3+1)))(4,t3,((C_word*)t0)[3],t2,((C_word*)t0)[2]);}

/* a2205 in k2199 in normalize in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_2206(C_word c,C_word t0,C_word t1,C_word t2){
C_word tmp;
C_word t3;
C_word ab[4],*a=ab;
if(c!=3) C_bad_argc_2(c,3,t0);
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr3,(void*)f_2206,3,t0,t1,t2);}
t3=t1;
((C_proc2)(void*)(*((C_word*)t3+1)))(2,t3,(C_word)C_a_i_divide(&a,2,t2,((C_word*)t0)[2]));}

/* magnitude in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_2183(C_word c,C_word t0,C_word t1,C_word t2){
C_word tmp;
C_word t3;
C_word t4;
C_word t5;
C_word ab[6],*a=ab;
if(c!=3) C_bad_argc_2(c,3,t0);
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr3,(void*)f_2183,3,t0,t1,t2);}
t3=(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_2191,a[2]=t1,tmp=(C_word)a,a+=3,tmp);
t4=(*a=C_CLOSURE_TYPE|2,a[1]=(C_word)f_2195,a[2]=t3,tmp=(C_word)a,a+=3,tmp);
C_trace("map");
t5=*((C_word*)lf[4]+1);
((C_proc4)(void*)(*((C_word*)t5+1)))(4,t5,t4,C_retrieve(lf[0]),t2);}

/* k2193 in magnitude in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_2195(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word *a;
C_apply(4,0,((C_word*)t0)[2],*((C_word*)lf[3]+1),t1);}

/* k2189 in magnitude in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_2191(C_word c,C_word t0,C_word t1){
C_word tmp;
C_word t2;
C_word ab[4],*a=ab;
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr2,(void*)f_2191,2,t0,t1);}
t2=((C_word*)t0)[2];
((C_proc2)(void*)(*((C_word*)t2+1)))(2,t2,(C_word)C_a_i_sqrt(&a,1,t1));}

/* cube in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_2177(C_word c,C_word t0,C_word t1,C_word t2){
C_word tmp;
C_word t3;
C_word *a;
if(c!=3) C_bad_argc_2(c,3,t0);
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr3,(void*)f_2177,3,t0,t1,t2);}
C_trace("*");
C_times(5,0,t1,t2,t2,t2);}

/* square in k2167 in k2164 in k2161 in k2158 in k2155 in k2152 in k2149 in k2146 in k2143 in k2140 in k2137 in k2134 in k2131 in k2128 */
static void C_ccall f_2171(C_word c,C_word t0,C_word t1,C_word t2){
C_word tmp;
C_word t3;
C_word ab[4],*a=ab;
if(c!=3) C_bad_argc_2(c,3,t0);
C_check_for_interrupt;
if(!C_stack_probe(&a)){
C_save_and_reclaim((void*)tr3,(void*)f_2171,3,t0,t1,t2);}
t3=t1;
((C_proc2)(void*)(*((C_word*)t3+1)))(2,t3,(C_word)C_a_i_times(&a,2,t2,t2));}

#ifdef C_ENABLE_PTABLES
static C_PTABLE_ENTRY ptable[279] = {
{"toplevelmagplot.scm",(void*)C_toplevel},
{"f_2130magplot.scm",(void*)f_2130},
{"f_2133magplot.scm",(void*)f_2133},
{"f_2136magplot.scm",(void*)f_2136},
{"f_2139magplot.scm",(void*)f_2139},
{"f_2142magplot.scm",(void*)f_2142},
{"f_2145magplot.scm",(void*)f_2145},
{"f_2148magplot.scm",(void*)f_2148},
{"f_2151magplot.scm",(void*)f_2151},
{"f_2154magplot.scm",(void*)f_2154},
{"f_2157magplot.scm",(void*)f_2157},
{"f_2160magplot.scm",(void*)f_2160},
{"f_2163magplot.scm",(void*)f_2163},
{"f_2166magplot.scm",(void*)f_2166},
{"f_2169magplot.scm",(void*)f_2169},
{"f_3033magplot.scm",(void*)f_3033},
{"f_3036magplot.scm",(void*)f_3036},
{"f_3099magplot.scm",(void*)f_3099},
{"f_3114magplot.scm",(void*)f_3114},
{"f_3117magplot.scm",(void*)f_3117},
{"f_3120magplot.scm",(void*)f_3120},
{"f_4005magplot.scm",(void*)f_4005},
{"f_3338magplot.scm",(void*)f_3338},
{"f_3341magplot.scm",(void*)f_3341},
{"f_3344magplot.scm",(void*)f_3344},
{"f_3347magplot.scm",(void*)f_3347},
{"f_3350magplot.scm",(void*)f_3350},
{"f_3353magplot.scm",(void*)f_3353},
{"f_3356magplot.scm",(void*)f_3356},
{"f_3359magplot.scm",(void*)f_3359},
{"f_3362magplot.scm",(void*)f_3362},
{"f_3950magplot.scm",(void*)f_3950},
{"f_3978magplot.scm",(void*)f_3978},
{"f_3991magplot.scm",(void*)f_3991},
{"f_3989magplot.scm",(void*)f_3989},
{"f_3985magplot.scm",(void*)f_3985},
{"f_3967magplot.scm",(void*)f_3967},
{"f_3975magplot.scm",(void*)f_3975},
{"f_3960magplot.scm",(void*)f_3960},
{"f_3948magplot.scm",(void*)f_3948},
{"f_3525magplot.scm",(void*)f_3525},
{"f_3874magplot.scm",(void*)f_3874},
{"f_3921magplot.scm",(void*)f_3921},
{"f_3929magplot.scm",(void*)f_3929},
{"f_3902magplot.scm",(void*)f_3902},
{"f_3915magplot.scm",(void*)f_3915},
{"f_3913magplot.scm",(void*)f_3913},
{"f_3909magplot.scm",(void*)f_3909},
{"f_3891magplot.scm",(void*)f_3891},
{"f_3899magplot.scm",(void*)f_3899},
{"f_3884magplot.scm",(void*)f_3884},
{"f_3872magplot.scm",(void*)f_3872},
{"f_3529magplot.scm",(void*)f_3529},
{"f_3800magplot.scm",(void*)f_3800},
{"f_3857magplot.scm",(void*)f_3857},
{"f_3862magplot.scm",(void*)f_3862},
{"f_3852magplot.scm",(void*)f_3852},
{"f_3828magplot.scm",(void*)f_3828},
{"f_3841magplot.scm",(void*)f_3841},
{"f_3839magplot.scm",(void*)f_3839},
{"f_3835magplot.scm",(void*)f_3835},
{"f_3817magplot.scm",(void*)f_3817},
{"f_3825magplot.scm",(void*)f_3825},
{"f_3810magplot.scm",(void*)f_3810},
{"f_3798magplot.scm",(void*)f_3798},
{"f_3533magplot.scm",(void*)f_3533},
{"f_3745magplot.scm",(void*)f_3745},
{"f_3784magplot.scm",(void*)f_3784},
{"f_3782magplot.scm",(void*)f_3782},
{"f_3778magplot.scm",(void*)f_3778},
{"f_3762magplot.scm",(void*)f_3762},
{"f_3770magplot.scm",(void*)f_3770},
{"f_3755magplot.scm",(void*)f_3755},
{"f_3743magplot.scm",(void*)f_3743},
{"f_3537magplot.scm",(void*)f_3537},
{"f_3690magplot.scm",(void*)f_3690},
{"f_3729magplot.scm",(void*)f_3729},
{"f_3727magplot.scm",(void*)f_3727},
{"f_3723magplot.scm",(void*)f_3723},
{"f_3707magplot.scm",(void*)f_3707},
{"f_3715magplot.scm",(void*)f_3715},
{"f_3700magplot.scm",(void*)f_3700},
{"f_3688magplot.scm",(void*)f_3688},
{"f_3541magplot.scm",(void*)f_3541},
{"f_3619magplot.scm",(void*)f_3619},
{"f_3678magplot.scm",(void*)f_3678},
{"f_3647magplot.scm",(void*)f_3647},
{"f_3669magplot.scm",(void*)f_3669},
{"f_3674magplot.scm",(void*)f_3674},
{"f_3650magplot.scm",(void*)f_3650},
{"f_3663magplot.scm",(void*)f_3663},
{"f_3661magplot.scm",(void*)f_3661},
{"f_3657magplot.scm",(void*)f_3657},
{"f_3636magplot.scm",(void*)f_3636},
{"f_3644magplot.scm",(void*)f_3644},
{"f_3629magplot.scm",(void*)f_3629},
{"f_3617magplot.scm",(void*)f_3617},
{"f_3545magplot.scm",(void*)f_3545},
{"f_3555magplot.scm",(void*)f_3555},
{"f_3602magplot.scm",(void*)f_3602},
{"f_3607magplot.scm",(void*)f_3607},
{"f_3583magplot.scm",(void*)f_3583},
{"f_3596magplot.scm",(void*)f_3596},
{"f_3594magplot.scm",(void*)f_3594},
{"f_3590magplot.scm",(void*)f_3590},
{"f_3572magplot.scm",(void*)f_3572},
{"f_3580magplot.scm",(void*)f_3580},
{"f_3565magplot.scm",(void*)f_3565},
{"f_3553magplot.scm",(void*)f_3553},
{"f_3549magplot.scm",(void*)f_3549},
{"f_3521magplot.scm",(void*)f_3521},
{"f_3493magplot.scm",(void*)f_3493},
{"f_3507magplot.scm",(void*)f_3507},
{"f_3511magplot.scm",(void*)f_3511},
{"f_3496magplot.scm",(void*)f_3496},
{"f_3502magplot.scm",(void*)f_3502},
{"f_3499magplot.scm",(void*)f_3499},
{"f_3482magplot.scm",(void*)f_3482},
{"f_3444magplot.scm",(void*)f_3444},
{"f_3448magplot.scm",(void*)f_3448},
{"f_3451magplot.scm",(void*)f_3451},
{"f_3454magplot.scm",(void*)f_3454},
{"f_3457magplot.scm",(void*)f_3457},
{"f_3466magplot.scm",(void*)f_3466},
{"f_3399magplot.scm",(void*)f_3399},
{"f_3435magplot.scm",(void*)f_3435},
{"f_3415magplot.scm",(void*)f_3415},
{"f_3418magplot.scm",(void*)f_3418},
{"f_3364magplot.scm",(void*)f_3364},
{"f_3392magplot.scm",(void*)f_3392},
{"f_3368magplot.scm",(void*)f_3368},
{"f_3371magplot.scm",(void*)f_3371},
{"f_3384magplot.scm",(void*)f_3384},
{"f_3374magplot.scm",(void*)f_3374},
{"f_3377magplot.scm",(void*)f_3377},
{"f_3193magplot.scm",(void*)f_3193},
{"f_3203magplot.scm",(void*)f_3203},
{"f_3266magplot.scm",(void*)f_3266},
{"f_3323magplot.scm",(void*)f_3323},
{"f_3301magplot.scm",(void*)f_3301},
{"f_3288magplot.scm",(void*)f_3288},
{"f_3206magplot.scm",(void*)f_3206},
{"f_3197magplot.scm",(void*)f_3197},
{"f_3139magplot.scm",(void*)f_3139},
{"f_3179magplot.scm",(void*)f_3179},
{"f_3161magplot.scm",(void*)f_3161},
{"f_3171magplot.scm",(void*)f_3171},
{"f_3143magplot.scm",(void*)f_3143},
{"f_3122magplot.scm",(void*)f_3122},
{"f_3126magplot.scm",(void*)f_3126},
{"f_3137magplot.scm",(void*)f_3137},
{"f_3107magplot.scm",(void*)f_3107},
{"f_3101magplot.scm",(void*)f_3101},
{"f_3037magplot.scm",(void*)f_3037},
{"f_3076magplot.scm",(void*)f_3076},
{"f_3080magplot.scm",(void*)f_3080},
{"f_3086magplot.scm",(void*)f_3086},
{"f_3041magplot.scm",(void*)f_3041},
{"f_3052magplot.scm",(void*)f_3052},
{"f_3056magplot.scm",(void*)f_3056},
{"f_3062magplot.scm",(void*)f_3062},
{"f_3044magplot.scm",(void*)f_3044},
{"f_2984magplot.scm",(void*)f_2984},
{"f_2986magplot.scm",(void*)f_2986},
{"f_2993magplot.scm",(void*)f_2993},
{"f_2998magplot.scm",(void*)f_2998},
{"f_3002magplot.scm",(void*)f_3002},
{"f_3022magplot.scm",(void*)f_3022},
{"f_3017magplot.scm",(void*)f_3017},
{"f_2788magplot.scm",(void*)f_2788},
{"f_2792magplot.scm",(void*)f_2792},
{"f_2804magplot.scm",(void*)f_2804},
{"f_2807magplot.scm",(void*)f_2807},
{"f_2810magplot.scm",(void*)f_2810},
{"f_2816magplot.scm",(void*)f_2816},
{"f_2822magplot.scm",(void*)f_2822},
{"f_2825magplot.scm",(void*)f_2825},
{"f_2828magplot.scm",(void*)f_2828},
{"f_2831magplot.scm",(void*)f_2831},
{"f_2934magplot.scm",(void*)f_2934},
{"f_2874magplot.scm",(void*)f_2874},
{"f_2924magplot.scm",(void*)f_2924},
{"f_2877magplot.scm",(void*)f_2877},
{"f_2880magplot.scm",(void*)f_2880},
{"f_2900magplot.scm",(void*)f_2900},
{"f_2904magplot.scm",(void*)f_2904},
{"f_2892magplot.scm",(void*)f_2892},
{"f_2884magplot.scm",(void*)f_2884},
{"f_2842magplot.scm",(void*)f_2842},
{"f_2850magplot.scm",(void*)f_2850},
{"f_2858magplot.scm",(void*)f_2858},
{"f_2722magplot.scm",(void*)f_2722},
{"f_2726magplot.scm",(void*)f_2726},
{"f_2773magplot.scm",(void*)f_2773},
{"f_2769magplot.scm",(void*)f_2769},
{"f_2733magplot.scm",(void*)f_2733},
{"f_2753magplot.scm",(void*)f_2753},
{"f_2669magplot.scm",(void*)f_2669},
{"f_2673magplot.scm",(void*)f_2673},
{"f_2679magplot.scm",(void*)f_2679},
{"f_2699magplot.scm",(void*)f_2699},
{"f_2682magplot.scm",(void*)f_2682},
{"f_2688magplot.scm",(void*)f_2688},
{"f_2692magplot.scm",(void*)f_2692},
{"f_2656magplot.scm",(void*)f_2656},
{"f_2662magplot.scm",(void*)f_2662},
{"f_2632magplot.scm",(void*)f_2632},
{"f_2636magplot.scm",(void*)f_2636},
{"f_2639magplot.scm",(void*)f_2639},
{"f_2642magplot.scm",(void*)f_2642},
{"f_2645magplot.scm",(void*)f_2645},
{"f_2648magplot.scm",(void*)f_2648},
{"f_2651magplot.scm",(void*)f_2651},
{"f_2515magplot.scm",(void*)f_2515},
{"f_2519magplot.scm",(void*)f_2519},
{"f_2522magplot.scm",(void*)f_2522},
{"f_2525magplot.scm",(void*)f_2525},
{"f_2528magplot.scm",(void*)f_2528},
{"f_2531magplot.scm",(void*)f_2531},
{"f_2534magplot.scm",(void*)f_2534},
{"f_2537magplot.scm",(void*)f_2537},
{"f_2540magplot.scm",(void*)f_2540},
{"f_2554magplot.scm",(void*)f_2554},
{"f_2564magplot.scm",(void*)f_2564},
{"f_2543magplot.scm",(void*)f_2543},
{"f_2546magplot.scm",(void*)f_2546},
{"f_2549magplot.scm",(void*)f_2549},
{"f_2480magplot.scm",(void*)f_2480},
{"f_2484magplot.scm",(void*)f_2484},
{"f_2487magplot.scm",(void*)f_2487},
{"f_2490magplot.scm",(void*)f_2490},
{"f_2496magplot.scm",(void*)f_2496},
{"f_2499magplot.scm",(void*)f_2499},
{"f_2502magplot.scm",(void*)f_2502},
{"f_2462magplot.scm",(void*)f_2462},
{"f_2466magplot.scm",(void*)f_2466},
{"f_2469magplot.scm",(void*)f_2469},
{"f_2472magplot.scm",(void*)f_2472},
{"f_2475magplot.scm",(void*)f_2475},
{"f_2420magplot.scm",(void*)f_2420},
{"f_2424magplot.scm",(void*)f_2424},
{"f_2427magplot.scm",(void*)f_2427},
{"f_2430magplot.scm",(void*)f_2430},
{"f_2438magplot.scm",(void*)f_2438},
{"f_2442magplot.scm",(void*)f_2442},
{"f_2445magplot.scm",(void*)f_2445},
{"f_2433magplot.scm",(void*)f_2433},
{"f_2400magplot.scm",(void*)f_2400},
{"f_2418magplot.scm",(void*)f_2418},
{"f_2404magplot.scm",(void*)f_2404},
{"f_2407magplot.scm",(void*)f_2407},
{"f_2414magplot.scm",(void*)f_2414},
{"f_2394magplot.scm",(void*)f_2394},
{"f_2369magplot.scm",(void*)f_2369},
{"f_2373magplot.scm",(void*)f_2373},
{"f_2378magplot.scm",(void*)f_2378},
{"f_2386magplot.scm",(void*)f_2386},
{"f_2340magplot.scm",(void*)f_2340},
{"f_2363magplot.scm",(void*)f_2363},
{"f_2344magplot.scm",(void*)f_2344},
{"f_2284magplot.scm",(void*)f_2284},
{"f_2291magplot.scm",(void*)f_2291},
{"f_2299magplot.scm",(void*)f_2299},
{"f_2252magplot.scm",(void*)f_2252},
{"f_2258magplot.scm",(void*)f_2258},
{"f_2265magplot.scm",(void*)f_2265},
{"f_2240magplot.scm",(void*)f_2240},
{"f_2226magplot.scm",(void*)f_2226},
{"f_2212magplot.scm",(void*)f_2212},
{"f_2220magplot.scm",(void*)f_2220},
{"f_2197magplot.scm",(void*)f_2197},
{"f_2201magplot.scm",(void*)f_2201},
{"f_2206magplot.scm",(void*)f_2206},
{"f_2183magplot.scm",(void*)f_2183},
{"f_2195magplot.scm",(void*)f_2195},
{"f_2191magplot.scm",(void*)f_2191},
{"f_2177magplot.scm",(void*)f_2177},
{"f_2171magplot.scm",(void*)f_2171},
{NULL,NULL}};
#endif

static C_PTABLE_ENTRY *create_ptable(void){
#ifdef C_ENABLE_PTABLES
return ptable;
#else
return NULL;
#endif
}
/* end of file */
