# -*- coding: UTF-8 -*-

import fnmatch
import os
import png

DIR="."

if __name__ == "__main__":
    print "static const unsigned char pacman_font[] PROGMEM = {"
    dateien = fnmatch.filter(os.listdir(DIR), "al_??.png")
    dateien = fnmatch.filter(os.listdir(DIR), "dot2_??.png")
    n = 0
    for datei in sorted(dateien):
        r=png.Reader(os.path.join(DIR, datei))
        d = r.read()
        w = d[0]
        h = d[1]
        cnt = w/8
        for line in d[2]:
            for i in range(cnt):
                b = 0
                for j in range(8):
                    if line[i*8+j]:
                        b |= 1<<(7-j)
                print "0x%02x," % b,
        print " // \"%c\"" % (n+53)
        n+=1
    print "};"
