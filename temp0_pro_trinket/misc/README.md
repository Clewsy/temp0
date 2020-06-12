# Embedded bitmaps

This directory contains the bitmap images embedded in the pro trinket for displaying on the oled.

The original bitmaps have the suffic **_original**.

These original files were manipulated by reversing the order of each 8-bit pixel column, resulting in 8 "rows" flipped vertically.  This manipulation was done to suit an online bitmap-to-char-array convert I used to create the include/ssd1306_bitmaps.h header file which contains these arrays.

The online converter I used is [dot2pic.com][link_web_dot2pic]

[link_web_dot2pic]:http://dot2pic.com/
