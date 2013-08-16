VERSION = '0.0.1'
APPNAME = 'randcomparison'

top = '.'
out = 'build'

def options(ctx):
  ctx.load('compiler_c compiler_cxx')


def configure(ctx):
  ctx.load('compiler_c compiler_cxx')
  ctx.env.CXXFLAGS += ['-O3', '-std=c++11', '-Wno-c++0x-compat',  '-fpermissive']

def build(bld):
  bld.recurse('src')
