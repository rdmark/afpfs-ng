executable(
  'afpgetstatus',
  sources: 'getstatus.c',
  include_directories: incdir,
  link_with: libafpclient,
  c_args: cflags,
  install: true)

executable(
  'afpcmd',
  sources: [
    'cmdline_afp.c',
    'cmdline_main.c',
    'cmdline_testafp.c'
  ],
  include_directories: incdir,
  link_with: libafpclient,
  dependencies: [root_dependencies, readline_dep, ncurses_dep],
  c_args: cflags,
  install: true
)
