#!/usr/bin/python
import os,sys
import fnmatch
import tarfile

def dir_walk(dir):
	for root,dirs,files in os.walk(dir):
		if '.svn' in root.split('/'):
			continue
		else:
			for f in files:
				yield os.path.join(root,f)

def all_files(root, patterns='*', single_level=False,  yield_folders=False):
	patterns = patterns.split(';');
	for path, subdirs, files in os.walk(root):
		if yield_folders:
			files.extend(subdirs)
		files.sort()
		for name in files:
			for pattern in patterns:
				if fnmatch.fnmatch(os.path.join(path, name), pattern) or os.path.join(path, name).endswith(pattern):
					yield os.path.join(path, name)
					break
		if single_level:
			break

def get_brn_config(roots):
	brn_configs=[]
	for root in roots:
		brn_configs.extend(list(all_files(root, 'brn_config')))
	return brn_configs


if __name__ == "__main__":
	tar = tarfile.open("tartest.tar.bz2", "w:bz2")
	for file in all_files('./', '*/res/*'):
		tar.add(file)
	tar.close()
	
	#for file in all_files('./', '*res/*'):
	#	print file
	
	#for file in dir_walk(sys.argv[1]):
	#	print file
