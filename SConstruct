# SConstruct for building balder2d with scons
includepath = ['/usr/include/python2.4','include']
env = Environment(CPPPATH=includepath)
#avoid leaving .sconsign files all over the place, put all signatures in:
env.SConsignFile("scons-signatures")

debug = ARGUMENTS.get('debug', 0)
if int(debug):
    env.Append(CCFLAGS = '-g')

import glob
files = glob.glob("src/*.cpp") + glob.glob("src/*/*.cpp") + glob.glob("src/menu/*/*.cpp")+ glob.glob("src/*/*.c")
libs = Split('guichan_sdl guichan SDL_image SDL_mixer SDL_gfx python2.4 physfs')

env.ParseConfig('sdl-config --cflags --libs')
env.Program('bin/balder2d',files, LIBS=libs)

SConscript('bin/scripts/test/SConscript')
