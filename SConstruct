# -*- mode: python -*-

#vars = Variables()
#vars.Add(BoolVariable('SYNTAX', 'Set to 1 for a syntax check',0))

#env = Environment(variables=vars)
env = Environment()

t = env.Program(target = 'ztrap',
                source = ['main.c',
                          #'player.c',
                          #'map.c',
                          #'map_view.c',
                          #'zombie_controller.c',
                          #'zombie.c',
                          #'bullet.c',
                          #'hud.c',
                          ])

env.ParseConfig('sdl-config --cflags --libs')

env.Append(CCFLAGS = ['-g', '-Wall'])
env.Append(CPPPATH = ['/usr/local/include', '../lapis'])
env.Append(LIBPATH = ['/usr/local/lib', '.'])
env.Append(LINKFLAGS = ["-Wl,-rpath,'.'"])
env.Append(LIBS = ['lapis', 'GL', 'SDL_ttf', 'SDL_image', 'lua'])
    
Default(t)
