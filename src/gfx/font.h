/* Copyright (C) 2008 Sean D'Epagnier <sean@depagnier.com>
 *
 * This Program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * For more information on the GPL, please go to:
 * http://www.gnu.org/copyleft/gpl.html
 */

#define BYTESW(bits) ((bits-1) / 8 + 1)

struct gfx_character
{
   int8_t width, height, xoff, yoff, dwidth;
   char data[];
};

struct gfx_font
{
   int size;
   int8_t width, height;
   const struct gfx_character *characters[96]; /* start at 32 */
};

void gfx_setfont(struct gfx_font *font);
struct gfx_font *gfx_getfont(void);

int gfx_font_height(void);

int gfx_char_width(char c);
void gfx_str_width_height(const char *str, int *wlen, int *hlen);
void gfx_str_width_height_P(const char *str, int *wlen, int *hlen);
int gfx_putc(char c);
int gfx_puts(const char *str);
int gfx_puts_P(const char *str);
int gfx_printf(const char *fmt, ...);
int gfx_printf_P(const char *fmt, ...);
void gfx_setmaxfont(const char *str, struct gfx_rect *area);
void gfx_setmaxfont_P(const char *str, struct gfx_rect *area);

/* get format string out of ram */
# define GFX_PUTS(s, ...) \
    do { static const char tmp[] PROGMEM = s; \
       gfx_puts_P(tmp, ## __VA_ARGS__);	      \
    } while(0)

# define GFX_PRINTF(s, ...) \
    do { static const char tmp[] PROGMEM = s; \
       gfx_printf_P(tmp, ## __VA_ARGS__);     \
    } while(0)

#define GFX_STR_WIDTH_HEIGHT(s, ...) \
    do { static const char tmp[] PROGMEM = s; \
       gfx_str_width_P(tmp, ## __VA_ARGS__);  \
    } while(0)

extern struct gfx_font gfx_fonts[];

#define GFX_4x6_FONT (gfx_fonts+0)
#define GFX_5x8_FONT (gfx_fonts+1)
#define GFX_R08_FONT (gfx_fonts+2)
#define GFX_R12_FONT (gfx_fonts+3)

#if defined(__AVR_AT90USB1287__)
#define GFX_R18_FONT (gfx_fonts+4)
#define GFX_R24_FONT (gfx_fonts+5)
#define GFX_B08_FONT (gfx_fonts+2)
#define GFX_B12_FONT (gfx_fonts+3)
#define GFX_B18_FONT (gfx_fonts+4)
#define GFX_B24_FONT (gfx_fonts+5)
#define GFX_R24_FONT (gfx_fonts+5)
#else
#define GFX_R18_FONT (gfx_fonts+4)
#define GFX_R24_FONT (gfx_fonts+5)
#define GFX_B08_FONT (gfx_fonts+6)
#define GFX_B12_FONT (gfx_fonts+7)
#define GFX_B18_FONT (gfx_fonts+8)
#define GFX_B24_FONT (gfx_fonts+9)
#endif

MAKE_TYPICAL_STATE_SET_GET_DEF(enum gfxAlignmentType, alignment)
