import lit.formats

config.name = 'Tiny-C CodeGen Test'
config.test_format = lit.formats.ShTest(True)

config.suffixes = ['.c']

config.test_source_root = os.path.dirname(__file__)
config.test_exec_root = os.path.join(config.my_obj_root, 'test')

config.substitutions.append(('%tiny-c', os.path.join(config.my_obj_root, 'tools/tiny-c')))