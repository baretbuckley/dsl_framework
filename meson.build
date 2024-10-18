project('ast_framework', 'cpp',
  version : '0.1',
  default_options : ['warning_level=3',
                    '-Dbuildtype=debug',
                     'cpp_std=c++20'])

incdir = include_directories('include')

src_files = ['ast_framework.cpp', 'src/value.cpp', 'src/tokenizer.cpp',
  'src/sr_parser.cpp', 'src/token.cpp', 'src/tokenizerReader.cpp']

exe = executable('ast_framework', src_files,
           install : true, include_directories : incdir)

test('basic', exe)
