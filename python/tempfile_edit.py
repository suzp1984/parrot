import sys, os, tempfile

def what_editor():
	editor = os.getenv('VISUAL') or os.getenv('EDITOR')
	if not editor:
		if sys.platform == 'windows':
			editor = 'Notepad.exe'
		else:
			editor = 'vim'
	return editor

def edited_text(starting_text=''):
	temp_fd, temp_filename = tempfile.mkstemp(text=True)
	os.write(temp_fd, starting_text)
	os.close(temp_fd)
	editor = what_editor()
	x = os.spawnlp(os.P_WAIT, editor, editor, temp_filename)
	if x:
		raise RuntimeError, "Can't run %s %s (%s)" % (editor, temp_filename, x)
	result = open(temp_filename).read()
	os.unlink(temp_filename)
	return result

if __name__ == "__main__":
	text = edited_text('''Edit this text a little,
	go ahead,
	it's just a demostration, after all...!
	''')
	print 'Edited test is:', text
