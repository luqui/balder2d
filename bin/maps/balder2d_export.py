#!/usr/bin/env python
# -*- coding: utf-8 -*-
# a script for exporting balder2d maps from the gimp
# to use this script put it in your .gimp-2.2/plug-ins/ directory (make sure 
# it is excecutable) then open up
# the gimp and create/open an image in the directory where you want your map 
# created.  Create two layers named 'background' and 'geometry'. Edit them 
# and then go to python-fu->Export->Balder2D and it will save them as separate 
# png images.  You may also creat a third layer, named 'spawns'.  Any red pixel 
# (rgb value FF0000) will become a spawn location in the exported maptags.xml.
 
from gimpfu import *

def python_balder2d_export(timg, tdrawable, maptags=False):
    width = timg.width
    height = timg.height
    layers = timg.layers
    spawns = []
    path = timg.filename[0:timg.filename.rfind('/')]
    progress = 0
    max_progress = width*height + width*height/4
    gimp.progress_init("Exporting Balder2D map files...")
    for layer in layers:
        name = layer.name
        if name.lower() == 'background':
            pdb.gimp_file_save(timg, layer, path+'/background.png', 'background.png')
        elif name.lower() == 'geometry':
            pdb.gimp_file_save(timg, layer, path+'/geometry.png', 'geometry.png')
        elif maptags and name.lower() == 'spawns' :
            p = layer.get_pixel_rgn(0,0,width,height)
            for x in range(0,width):
                for y in range(0,height):
                    if p[x,y] == '\xff\x00\x00\xff': #red
                        spawns.append((x,y))
                progress = progress + width
                gimp.progress_update(float(progress) / max_progress)

    # found all the spawns, now write them out to the file
    if maptags:
        out_file = open(path+"/maptags.xml", "w")
        out_file.write('<?xml version="1.0"?>\n<!-- information about the map -->\n')
        out_file.write('<Map name="'+timg.name.rsplit('.',1)[0]+'">\n')
        out_file.write('\t<Size width="'+`width`+'" height="'+`height`+'" />\n')
        for spawn in spawns:
            out_file.write('\t<spawn>\n')
            out_file.write('\t\t<location x="'+`spawn[0]`+'" y="'+`spawn[1]`+'" />\n')
            out_file.write('\t\t<velocity x="0" y="0" />\n')
            out_file.write('\t\t<stuck status="true" />\n')
            out_file.write('\t</spawn>\n')
        out_file.write('</Map>\n')
        out_file.close()
        gimp.progress_update(max_progress)
            
register(
        "python_fu_balder2d",
        "Export into a Balder2D map", 
        "Export into a Balder2D map",
        "Bjorn Hansen",
        "Bjorn Hansen",
        "October 2006",
        "<Image>/Python-Fu/Export/_Balder2D",
        "RGB*, GRAY*",
        [
            (PF_TOGGLE, "maptags", "Write maptags.xml", False)
        ],
        [],
        python_balder2d_export)

main()
