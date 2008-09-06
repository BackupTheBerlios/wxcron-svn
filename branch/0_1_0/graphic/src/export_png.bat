set INKSCAPE=C:\Programme\Inkscape\inkscape.exe
set BF_CWD=%cd%\
set BF_SVG=%BF_CWD%%1%.svg


set BF_PNG=%BF_CWD%%1%"16.png"
%INKSCAPE% %BF_SVG% --export-png=%BF_PNG% --export-width=16 --export-height=16 --export-area-canvas
set BF_PNG=%BF_CWD%%1%"24.png"
%INKSCAPE% %BF_SVG% --export-png=%BF_PNG% --export-width=24 --export-height=24 --export-area-canvas
set BF_PNG=%BF_CWD%%1%"32.png"
%INKSCAPE% %BF_SVG% --export-png=%BF_PNG% --export-width=32 --export-height=32 --export-area-canvas
set BF_PNG=%BF_CWD%%1%"48.png"
%INKSCAPE% %BF_SVG% --export-png=%BF_PNG% --export-width=48 --export-height=48 --export-area-canvas
set BF_PNG=%BF_CWD%%1%"64.png"
%INKSCAPE% %BF_SVG% --export-png=%BF_PNG% --export-width=64 --export-height=64 --export-area-canvas
set BF_PNG=%BF_CWD%%1%"128.png"
%INKSCAPE% %BF_SVG% --export-png=%BF_PNG% --export-width=128 --export-height=128 --export-area-canvas
set BF_PNG=%BF_CWD%%1%"256.png"
%INKSCAPE% %BF_SVG% --export-png=%BF_PNG% --export-width=256 --export-height=256 --export-area-canvas