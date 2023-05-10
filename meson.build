project('BSUIR-course-work-semester-1', 'cpp',
  version : '0.1',
  default_options : [
    'warning_level=3',
    'cpp_std=c++17'
  ]
)

incdir = include_directories('include')

src = [
  'src/main.cpp',
  'src/data_structure/grid.cpp',
  'src/algorithm/grid_maze_generator.cpp',
  'src/renderer/grid_renderer.cpp',
  'src/renderer/renderer.cpp',
  'src/utility/terminal.cpp'
]

curses = dependency('curses')

compiler = meson.get_compiler('cpp')
conf = configuration_data()
check_headers = [
  ['ncursesw/menu.h', 'HAVE_NCURSESW_MENU_H'],
  ['ncurses/menu.h', 'HAVE_NCURSES_MENU_H'],
  ['menu.h', 'HAVE_MENU_H'],
  ['ncursesw/curses.h', 'HAVE_NCURSESW_CURSES_H'],
  ['ncursesw.h', 'HAVE_NCURSESW_H'],
  ['ncurses/curses.h', 'HAVE_NCURSES_CURSES_H'],
  ['ncurses.h', 'HAVE_NCURSES_H'],
  ['curses.h', 'HAVE_CURSES_H'],
]

foreach h : check_headers
  if compiler.has_header(h.get(0))
    conf.set(h.get(1), 1)
  endif
endforeach

exe = executable('pathfinder-comparison',
  sources : src,
  include_directories : incdir,
  install : true,
  install_dir : './bin',
  dependencies : curses
)

test('basic', exe)
