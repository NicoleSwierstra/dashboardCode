import imageio as io

mcFont = io.imread('res/Minecraftglyphs.bmp')
ndFont = io.imread('res/PixelGlyph6by8.bmp')

#cv2.imshow('sh', gs)
#cv2.waitKey(0)

charnames = ['NUL','SOH','STX','ETX','EOT','ENQ','ACK','BEL','BS ','TAB','LF','VT','FF','CR','SO','SI','DLE','DC1','DC2','DC3','DC4','NAK','SYN','ETB','CAN','EM','SUB','ESC','FS','GS','RS','US','SPACE','!','"','#','$','%','&','\'','(',')','*','+',',','-','.','/','0','1','2','3','4','5','6','7','8','9',':',';','<','=','>','?','@','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z','[','\\',']','^','_','`','a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z','{','|','}','~','DEL']

charsmc57 = []
charsnd68 = []

for glyph in range(0, 128):
    charsmc57.append([])
    charsnd68.append([])
    glyphx = int(glyph / 16)
    glyphy = int(glyph % 16)
    for mcpy in range(0, 8):
        charsmc57[glyph].append('0b')
        for mcpx in range(0, 6):
            pixelx = mcpx + (6 * glyphx)
            pixely = mcpy + (8 * glyphy)
            charsmc57[glyph][mcpy] += str(1 if mcFont[pixely][pixelx][0] == 0 else 0)
    for py68 in range(0, 9):
        charsnd68[glyph].append('0b')
        for px68 in range(0, 7):
            pixelx = px68 + (7 * glyphx)
            pixely = py68 + (9 * glyphy)
            charsnd68[glyph][py68] += str(1 if ndFont[pixely][pixelx][0] == 0 else 0)

with open('display/Fonts.h', 'wt') as f:
    f.write('#pragma once\n')
    f.write('namespace fonts {\n')
    f.write('    uint8_t text5by7MC[128][8] = {\n')
    for _i, i in enumerate(charsmc57):
        f.write('        { ')
        for j in i:
            f.write(j + '00, ')
        f.write('}, /* ' + charnames[_i] + ' */ \n')
    f.write('    };\n\n')
    f.write('    uint8_t text6by8ND[128][9] = {\n')
    for _i, i in enumerate(charsnd68):
        f.write('        { ')
        for j in i:
            f.write(j + '00, ')
        f.write('}, /* ' + charnames[_i] + ' */ \n')
    f.write('    };\n\n')
    f.write('}')