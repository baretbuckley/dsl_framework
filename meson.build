project('ast_framework', 'cpp',
  version : '0.1',
  default_options : ['warning_level=3',
                     'cpp_std=c++20'])

incdir = include_directories('include')

src_files = ['ast_framework.cpp']

exe = executable('ast_framework', src_files,
  install : true, include_directories : incdir)

test('basic', exe)
