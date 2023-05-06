project('BSUIR-course-work-semester-1', 'cpp',
  version : '0.1',
  default_options : [
    'warning_level=3',
    'c--_std=c++11'
  ]
)

incdir = include_directories('include')

src = [
  'src/main.cpp',
  'src/grid_location.cpp',
  'src/grid.cpp'
]

exe = executable('program',
  sources : src,
  include_directories : incdir,
  install : true,
  install_dir : './bin'
)

test('basic', exe)
