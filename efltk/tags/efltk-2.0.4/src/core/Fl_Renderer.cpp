#include <efltk/Fl_Renderer.h>
#include <efltk/x.h>

#include <config.h>

#include <stdlib.h>
#include <string.h>

Fl_Colormap::Fl_Colormap(int cols) {
    colors=0;
    realloc(cols);
}

Fl_Colormap::~Fl_Colormap() {
	if(colors) delete []colors;
}

void Fl_Colormap::realloc(int cols) {
	if(colors) delete []colors;
	if(cols) colors = new Fl_Colormap_Color[cols]; else colors=0;
	ncolors = cols;
}

void Fl_Colormap::copy(Fl_Colormap *map) {
	realloc(map->ncolors);
	memcpy(colors, map->colors, sizeof(Fl_Colormap_Color)*map->ncolors);
}

Fl_PixelFormat::Fl_PixelFormat()
{
    palette = 0;
    bitspp = bytespp = 0;

    Rloss = Gloss = Bloss = Aloss = 0;
    Rshift = Gshift = Bshift = Ashift = 0;
    Rmask = Gmask = Bmask = Amask = 0;

    colorkey = 0;
    alpha = 128;//255;
    masktype = FL_MASK_NONE;

    dst=0;
    table=0;
    format_version=-1;
    identity=false;
}

Fl_PixelFormat::~Fl_PixelFormat()
{
    invalidate_map();
    if(palette) delete palette;
}

void Fl_PixelFormat::realloc(int bits_pp, uint32 R_mask, uint32 G_mask, uint32 B_mask, uint32 A_mask)
{
    if(palette) delete palette;
    palette = 0;
    init(bits_pp, R_mask, G_mask, B_mask, A_mask);
}

void Fl_PixelFormat::copy(Fl_PixelFormat *fmt)
{
    if(palette) delete palette;
    palette = 0;
    if(fmt->palette) {
        palette = new Fl_Colormap();
        palette->copy(fmt->palette);
    }

    bitspp = fmt->bitspp;
    bytespp = fmt->bytespp;

    Rloss = fmt->Rloss;
    Gloss = fmt->Gloss;
    Bloss = fmt->Bloss;
    Aloss = fmt->Aloss;
    Rshift = fmt->Rshift;
    Gshift = fmt->Gshift;
    Bshift = fmt->Bshift;
    Ashift = fmt->Ashift;
    Rmask = fmt->Rmask;
    Gmask = fmt->Gmask;
    Bmask = fmt->Bmask;
    Amask = fmt->Amask;

    colorkey = fmt->colorkey;
    alpha = fmt->alpha;
	masktype = fmt->masktype;

    //if(fmt->table && fmt->dst)
    //map_this(fmt);

    dst = fmt->dst;
}

void Fl_PixelFormat::init(int bits_pp, uint32 R_mask, uint32 G_mask, uint32 B_mask, uint32 A_mask)
{
    if(bits_pp<=0) return;

	uint32 mask;

	/* Set up the format */
	bitspp = bits_pp;
	bytespp = (bits_pp+7)/8;
	switch(bits_pp) {
	case 1:
		/* Create the 2 color black-white palette */
		palette = new Fl_Colormap(2);
		palette->colors[0].r = 0xFF;
		palette->colors[0].g = 0xFF;
		palette->colors[0].b = 0xFF;
		palette->colors[1].r = 0x00;
		palette->colors[1].g = 0x00;
		palette->colors[1].b = 0x00;
		Rloss = Gloss = Bloss = Aloss = 8;
		Rshift = Gshift = Bshift = Ashift = 0;
		Rmask = Gmask = Bmask = Amask = 0;
		break;

	case 4:
		/* Create the 16 color VGA palette */
		palette = new Fl_Colormap(16);
		/* Punt for now, will this ever be used? */
		memset(palette->colors, 0, palette->ncolors*sizeof(Fl_Color));

		/* Palettized formats have no mask info */
		Rloss = Gloss = Bloss = Aloss = 8;
		Rshift = Gshift = Bshift = Ashift = 0;
		Rmask = Gmask = Bmask = Amask = 0;
		break;

	case 8:
		/* Create an empty 256 color palette */
		palette = new Fl_Colormap(256);
		memset(palette->colors, 0, palette->ncolors*sizeof(Fl_Color));

		/* Palettized formats have no mask info */
		Rloss = Gloss = Bloss = Aloss = 8;
		Rshift = Gshift = Bshift = Ashift = 0;
		Rmask = Gmask = Bmask = Amask = 0;
		break;

	default:
        /* No palette, just packed pixel info */
        palette = 0;
        Rshift = 0;
        Rloss = 8;
        if(R_mask) {
            for(mask = R_mask; !(mask&0x01); mask >>= 1 )
                ++Rshift;
            for(; (mask&0x01); mask >>= 1 )
                --Rloss;
        }
        Gshift = 0;
        Gloss = 8;
        if(G_mask) {
            for(mask = G_mask; !(mask&0x01); mask >>= 1 )
                ++Gshift;
            for(; (mask&0x01); mask >>= 1 )
                --Gloss;
        }
        Bshift = 0;
        Bloss = 8;
        if(B_mask) {
            for(mask = B_mask; !(mask&0x01); mask >>= 1 )
                ++Bshift;
            for(; (mask&0x01); mask >>= 1 )
                --Bloss;
        }
        Ashift = 0;
        Aloss = 8;
        if(A_mask) {
            for(mask = A_mask; !(mask&0x01); mask >>= 1 )
                ++Ashift;
            for(; (mask&0x01); mask >>= 1 )
                --Aloss;
        }
        Rmask = R_mask;
        Gmask = G_mask;
        Bmask = B_mask;
        Amask = A_mask;
        break;
  }
  // Calculate some standard bitmasks, if necessary
  // Note:  This could conflict with an alpha mask, if given.
  if((bits_pp > 8) && !Rmask && !Gmask && !Bmask ) {
      /* R-G-B */
      if(bits_pp > 24 )
          bits_pp = 24;
      Rloss = 8-(bits_pp/3);
      Gloss = 8-(bits_pp/3)-(bits_pp%3);
      Bloss = 8-(bits_pp/3);
      Rshift = ((bits_pp/3)+(bits_pp%3))+(bits_pp/3);
      Gshift = (bits_pp/3);
      Bshift = 0;
      Rmask = ((0xFF>>Rloss)<<Rshift);
      Gmask = ((0xFF>>Gloss)<<Gshift);
      Bmask = ((0xFF>>Bloss)<<Bshift);
  }
}

uint8 *Fl_Renderer::system_convert(Fl_PixelFormat *src_fmt, Fl_Size *src_size, uint8 *src, bool hw_surface)
{
    // Init renderer before first convert
    Fl_Renderer::system_init();

    int sbpp = src_fmt->bytespp;
    int dbpp = Fl_Renderer::system_format()->bytespp;

    int src_pitch = Fl_Renderer::calc_pitch(sbpp, src_size->w());
    int dst_pitch = Fl_Renderer::calc_pitch(dbpp, src_size->w());

    uint8 *converted = (uint8*)malloc(sizeof(uint8) * (dst_pitch*src_size->h()));

    int flags=0;
    if(hw_surface) {
        flags = FL_BLIT_HW_PALETTE;
    }

    Fl_Rect r(0, 0, src_size->w(), src_size->h());
    Fl_Renderer::blit(src, &r, src_fmt, src_pitch,
                      converted, &r, Fl_Renderer::system_format(), dst_pitch, flags);

    return converted;
}

//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////

bool fl_format_equal(Fl_PixelFormat *A, Fl_PixelFormat *B) {
    return ( A->bitspp==B->bitspp && A->Rmask==B->Rmask && A->Amask==B->Amask );
}

#define SCALE_RGB(R, G, B, dR, dG, dB) \
    int SR=R; int SG=G; int SB=B; \
    if(SR<0) SR=0; else if(SR>255) SR=255; \
    if(SG<0) SG=0; else if(SG>255) SG=255; \
    if(SB<0) SB=0; else if(SB>255) SB=255; \
    dR=SR; dG=SG; dB=SB

#define SCALE_RGBA(R, G, B, A, dR, dG, dB, dA) \
    int SR=R; int SG=G; int SB=B; int SA=A;\
    if(SR<0) SR=0; else if(SR>255) SR=255; \
    if(SG<0) SG=0; else if(SG>255) SG=255; \
    if(SB<0) SB=0; else if(SB>255) SB=255; \
    if(SA<0) SA=0; else if(SA>255) SA=255; \
    dR=SR; dG=SG; dB=SB; dA=SA

/* Load pixel of the specified format from a buffer and get its R-G-B values */
void fl_rgb_from_pixel(uint32 pixel, Fl_PixelFormat *fmt, uint8 &r, uint8 &g, uint8 &b)
{
    SCALE_RGB((((pixel&fmt->Rmask)>>fmt->Rshift)<<fmt->Rloss),
              (((pixel&fmt->Gmask)>>fmt->Gshift)<<fmt->Gloss),
              (((pixel&fmt->Bmask)>>fmt->Bshift)<<fmt->Bloss),
              r,g,b);
}

void fl_rgb_from_rgb565(uint16 pixel, uint8 &r, uint8 &g, uint8 &b)
{
    SCALE_RGB((((pixel&0xF800)>>11)<<3),
              (((pixel&0x07E0)>>5)<<2),
              ((pixel&0x001F)<<3),
              r, g, b);
}

void fl_rgb_from_rgb555(uint16 pixel, uint8 &r, uint8 &g, uint8 &b)
{
    SCALE_RGB((((pixel&0x7C00)>>10)<<3),
              (((pixel&0x03E0)>>5)<<3),
              ((pixel&0x001F)<<3),
              r, g, b);
}

void fl_rgb_from_rgb888(uint32 pixel, uint8 &r, uint8 &g, uint8 &b)
{
    SCALE_RGB(((pixel&0xFF0000)>>16),
              ((pixel&0xFF00)>>8),
              (pixel&0xFF),
              r, g, b);
}

void fl_retrieve_rgb_pixel(uint8 *buf, int bpp, uint32 &pixel)
{
    switch (bpp) {
    case 2:
        pixel = *((uint16 *)(buf));
        break;

    case 3: {
        uint8 *B = (uint8 *)buf;
        if(Fl_Renderer::lil_endian())
            pixel = B[0] + (B[1] << 8) + (B[2] << 16);
        else
            pixel = (B[0] << 16) + (B[1] << 8) + B[2];
    }
    break;

    case 4:
        pixel = *((uint32 *)buf);
        break;

    default:
        pixel = 0; /* appease gcc */
        break;
    }
}

void fl_disemble_rgb(uint8 *buf, int bpp, Fl_PixelFormat *fmt, uint32 &pixel, uint8 &R, uint8 &G, uint8 &B)
{
    switch (bpp) {
    case 1:
        R = fmt->palette->colors[*buf].r;
        G = fmt->palette->colors[*buf].g;
        B = fmt->palette->colors[*buf].b;
        pixel = (R<<16)|(G<<8)|B;
        break;

    case 2:
        pixel = *((uint16 *)(buf));
        fl_rgb_from_pixel(pixel, fmt, R, G, B);
        break;

    case 3: {
        uint8 *BUF = (uint8 *)buf;
        if(Fl_Renderer::lil_endian())
            pixel = BUF[0] + (BUF[1] << 8) + (BUF[2] << 16);
        else
            pixel = (BUF[0] << 16) + (BUF[1] << 8) + BUF[2];
        fl_rgb_from_pixel(pixel, fmt, R, G, B);
    }
    break;

    case 4:
        pixel = *((uint32 *)buf);
        fl_rgb_from_pixel(pixel, fmt, R, G, B);
        break;

    default:
        pixel = 0;	/* prevent gcc from complaining */
        break;
    }
}

/* Assemble R-G-B values into a specified pixel format and store them */
void fl_pixel_from_rgb(uint32 &pixel, Fl_PixelFormat *fmt, uint8 r, uint8 g, uint8 b)
{
    pixel = ((r>>fmt->Rloss)<<fmt->Rshift)|
        ((g>>fmt->Gloss)<<fmt->Gshift)|
        ((b>>fmt->Bloss)<<fmt->Bshift);
}

void fl_rgb565_from_rgb(uint16 &pixel, uint8 r, uint8 g, uint8 b)
{
    pixel = ((r>>3)<<11)|((g>>2)<<5)|(b>>3);
}

void fl_rgb555_from_rgb(uint16 &pixel, uint8 r, uint8 g, uint8 b)
{
    pixel = ((r>>3)<<10)|((g>>3)<<5)|(b>>3);
}

void fl_rgb888_from_rgb(uint32 &pixel, uint8 r, uint8 g, uint8 b)
{
    pixel = (r<<16)|(g<<8)|b;
}

void fl_assemble_rgb(uint8 *buf, int bpp, Fl_PixelFormat *fmt, uint8 r, uint8 g, uint8 b)
{
    switch (bpp) {
    case 1: {
        fmt->palette->colors[*buf].r = r;
        fmt->palette->colors[*buf].g = g;
        fmt->palette->colors[*buf].b = b;
    }
    break;

    case 2: {
        uint32 pixel;
        fl_pixel_from_rgb(pixel, fmt, r, g, b);
        *((uint16 *)buf) = (uint16)pixel;
    }
    break;

    case 3: {
        if(Fl_Renderer::lil_endian()) {
            *((buf)+fmt->Rshift/8) = r;
            *((buf)+fmt->Gshift/8) = g;
            *((buf)+fmt->Bshift/8) = b;
        } else {
            *((buf)+2-fmt->Rshift/8) = r;
            *((buf)+2-fmt->Gshift/8) = g;
            *((buf)+2-fmt->Bshift/8) = b;
        }
    }
    break;

    case 4: {
        uint32 pixel;
        fl_pixel_from_rgb(pixel, fmt, r, g, b);
        *((uint32 *)buf) = pixel;
    }
    break;

    default:
        break;
    }
}

void fl_assemble_rgb_amask(uint8 *buf, int bpp, Fl_PixelFormat *fmt, uint8 r, uint8 g, uint8 b, uint8 Amask)
{
    switch (bpp) {
    case 2: {
        uint16 *bufp = (uint16 *)buf;
        uint32 pixel;
        fl_pixel_from_rgb(pixel, fmt, r, g, b);
        *bufp = uint16(pixel) | (*bufp & Amask);
    }
    break;

    case 3: {
        if(Fl_Renderer::lil_endian()) {
            *((buf)+fmt->Rshift/8) = r;
            *((buf)+fmt->Gshift/8) = g;
            *((buf)+fmt->Bshift/8) = b;
        } else {
            *((buf)+2-fmt->Rshift/8) = r;
            *((buf)+2-fmt->Gshift/8) = g;
            *((buf)+2-fmt->Bshift/8) = b;
        }
    }
    break;

    case 4: {
        uint32 *bufp = (uint32 *)buf;
        uint32 pixel;
        fl_pixel_from_rgb(pixel, fmt, r, g, b);
        *bufp = pixel | (*bufp & Amask);
    }
    break;

    default:
        break;
    }
}

void fl_rgba_from_pixel(uint32 pixel, Fl_PixelFormat *fmt, uint8 &r, uint8 &g, uint8 &b, uint8 &a)
{
    SCALE_RGBA((((pixel&fmt->Rmask)>>fmt->Rshift)<<fmt->Rloss),
               (((pixel&fmt->Gmask)>>fmt->Gshift)<<fmt->Gloss),
               (((pixel&fmt->Bmask)>>fmt->Bshift)<<fmt->Bloss),
               (((pixel&fmt->Amask)>>fmt->Ashift)<<fmt->Aloss),
               r,g,b,a);
}

void fl_rgba_from_8888(uint32 pixel, Fl_PixelFormat *fmt, uint8 &r, uint8 &g, uint8 &b, uint8 &a)
{
    SCALE_RGBA(((pixel&fmt->Rmask)>>fmt->Rshift),
               ((pixel&fmt->Gmask)>>fmt->Gshift),
               ((pixel&fmt->Bmask)>>fmt->Bshift),
               ((pixel&fmt->Amask)>>fmt->Ashift),
               r,g,b,a);
}

void fl_rgba_from_rgba8888(uint32 pixel, uint8 &r, uint8 &g, uint8 &b, uint8 &a)
{
    SCALE_RGBA((pixel>>24),
               ((pixel>>16)&0xFF),
               ((pixel>>8)&0xFF),
               (pixel&0xFF),
               r,g,b,a);
}

void fl_rgba_from_argb8888(uint32 pixel, uint8 &r, uint8 &g, uint8 &b, uint8 &a)
{
    SCALE_RGBA(((pixel>>16)&0xFF),
               ((pixel>>8)&0xFF),
               (pixel&0xFF),
               (pixel>>24),
               r,g,b,a);
}

void fl_rgba_from_abgr8888(uint32 pixel, uint8 &r, uint8 &g, uint8 &b, uint8 &a)
{
    SCALE_RGBA((pixel&0xFF),
               ((pixel>>8)&0xFF),
               ((pixel>>16)&0xFF),
               (pixel>>24),
               r,g,b,a);
}

void fl_disemble_rgba(uint8 *buf, int bpp, Fl_PixelFormat *fmt, uint32 &pixel, uint8 &r, uint8 &g, uint8 &b, uint8 &a)
{
    switch (bpp) {
    case 1:
        r = fmt->palette->colors[*buf].r;
        g = fmt->palette->colors[*buf].g;
        b = fmt->palette->colors[*buf].b;
        a = fmt->palette->colors[*buf].a;
        pixel = (r<<24)|(g<<16)|(b<<8)|a;
        pixel &= ~fmt->Amask;
        return;

    case 2:
        pixel = *((uint16 *)buf);
        break;

    case 3: {
        /* FIXME: broken code (no alpha) */
        uint8 *b = (uint8 *)buf;
        if(Fl_Renderer::lil_endian())
            pixel = b[0] + (b[1] << 8) + (b[2] << 16);
        else
            pixel = (b[0] << 16) + (b[1] << 8) + b[2];
    }
    break;

    case 4:
        pixel = *((uint32 *)buf);
        break;

    default:
        pixel = 0; /* stop gcc complaints */
        break;
    }

    fl_rgba_from_pixel(pixel, fmt, r, g, b, a);
    pixel &= ~fmt->Amask;
}

/* FIXME: this isn't correct, especially for Alpha (maximum != 255) */
void fl_pixel_from_rgba(uint32 &pixel, Fl_PixelFormat *fmt, uint8 r, uint8 g, uint8 b, uint8 a)
{
    int R=r, G=g, B=b, A=a;
    pixel = (((R)>>fmt->Rloss)<<fmt->Rshift) |
        ((G>>fmt->Gloss)<<fmt->Gshift) |
        ((B>>fmt->Bloss)<<fmt->Bshift) |
        ((A<<fmt->Aloss)<<fmt->Ashift);
}

void fl_assemble_rgba(uint8 *buf, int bpp, Fl_PixelFormat *fmt, uint8 r, uint8 g, uint8 b, uint8 a)
{
    switch (bpp) {
    case 1: {
        fmt->palette->colors[*buf].r = r;
        fmt->palette->colors[*buf].g = g;
        fmt->palette->colors[*buf].b = b;
        fmt->palette->colors[*buf].a = a;
    }
    break;
    case 2: {
        uint32 pixel;
        fl_pixel_from_rgba(pixel, fmt, r, g, b, a);
        *((uint16 *)buf) = (uint16)pixel;
    }
    break;

    case 3: { /* FIXME: broken code (no alpha) */
        if(Fl_Renderer::lil_endian()) {
            *((buf)+fmt->Rshift/8) = r;
            *((buf)+fmt->Gshift/8) = g;
            *((buf)+fmt->Bshift/8) = b;
        } else {
            *((buf)+2-fmt->Rshift/8) = r;
            *((buf)+2-fmt->Gshift/8) = g;
            *((buf)+2-fmt->Bshift/8) = b;
        }
    }
    break;

    case 4: {
        uint32 pixel;
        fl_pixel_from_rgba(pixel, fmt, r, g, b, a);
        *((uint32 *)buf) = pixel;
    }
    break;
    }
}

/* Blend the RGB values of two pixels based on a source alpha value */
void fl_alpha_blend(uint8 sR, uint8 sG, uint8 sB, uint8 A, uint8 &dR, uint8 &dG, uint8 &dB)
{
    dR = (((sR-dR)*(A))>>8)+dR;
    dG = (((sG-dG)*(A))>>8)+dG;
    dB = (((sB-dB)*(A))>>8)+dB;
}

/* The macros used to swap values */
/* Try to use superfast macros on systems that support them */
#ifdef linux
#include <asm/byteorder.h>
#endif /* linux */

uint16 fl_swap_16(uint16 d) {
#ifdef __arch__swab16
    return (__arch__swab16(d));
#else
    return ((d<<8)|(d>>8));
#endif
}

uint32 fl_swap_32(uint32 d) {
#ifdef __arch__swab32
    return (__arch__swab32(d));
#else
    return ((d<<24)|((d<<8)&0x00FF0000)|((d>>8)&0x0000FF00)|(d>>24));
#endif
}

uint16 fl_swap_le16(uint16 d) {
    return Fl_Renderer::lil_endian() ? d : fl_swap_16(d);
}
uint32 fl_swap_le32(uint32 d) {
    return Fl_Renderer::lil_endian() ? d : fl_swap_32(d);
}
uint16 fl_swap_be16(uint16 d) {
    return Fl_Renderer::lil_endian() ? fl_swap_16(d) : d;
}
uint32 fl_swap_be32(uint32 d) {
    return Fl_Renderer::lil_endian() ? fl_swap_32(d) : d;
}

