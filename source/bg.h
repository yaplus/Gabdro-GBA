
//{{BLOCK(bg)

//======================================================================
//
//	bg, 128x256@8, 
//	+ palette 256 entries, not compressed
//	+ 512 tiles not compressed
//	Total size: 512 + 32768 = 33280
//
//	Time-stamp: 2017-07-28, 17:27:05
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.14
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_BG_H
#define GRIT_BG_H

#define bgTilesLen 32768
extern const unsigned char bgTiles[32768];

#define bgPalLen 512
extern const unsigned short bgPal[256];

#endif // GRIT_BG_H

//}}BLOCK(bg)
