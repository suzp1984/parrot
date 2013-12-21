import sys
import os
import locale

enc = locale.getpreferredencoding()

def console_print(st=u"", f=sys.stdout, linebreak=True):
    global enc
    assert type(st) is unicode
    f.write(st.encode(enc))
    if linebreak: f.write(os.linesep)

def console_flush(f=sys.stdout):
    f.flush()

commands = {}
aliases = {}

def command(meth):
    global commands, aliases
    assert meth.__doc__, "All commands need properly formatted docstrings (even %r!!)" % meth
    if hasattr(meth, 'im_func'): # bound method, if we ever have one
        meth = meth.im_func
    commands[meth.func_name] = meth
#    print "-----"
#    print meth.func_name
    meth_aliases = [unicode(alias) for alias in aliases.iterkeys() if aliases[alias].func_name == meth.func_name]
    if meth_aliases:
        meth.__doc__ += u"\nAliases: %s" % ",".join(meth_aliases)
    return meth

def alias(name):
    def decorator(meth):
        global commands, aliases
        assert name not in commands, "This alias is the name of a command."
        aliases[name] = meth
        return meth
    return decorator

def usage(argv):
    console_print(u"command-line interface\n")
    console_print(u"commands:\n")
    console_print(u"Note: use CmdPattern help <command> to view usage for a specific command.\n")
    out = []
    for command in commands:
        out.append((command, commands[command].__doc__.splitlines()[0]))
    spacing = max(len(o[0])+3 for o in out)
    for o in out:
        console_print(" %-*s%s" % (spacing, o[0], o[1]))
    console_print()

@command
@alias('c1')
def cmd1(argv):
    """command one
    """
    console_print(u'cmd1')

@command
@alias('c2')
def cmd2(argv):
    """command two.
    """
    console_print(u'cmd2')

    
@command
@alias('hel')
def help(argv):
    u"""provide help
python CmdPattern.py help [COMMAND]

With no arguments, print a list of commands and a short description of each. With a command, print descriptive help on how to use the command.
"""
    if not argv:
        return usage(argv)
    for command in commands:
        if command == argv[0]:
            console_print(commands[command].__doc__.split('\n', 1)[1].decode('ascii'))
            return
    for alias in aliases:
        if alias == argv[0]:
            console_print(aliases[alias].__doc__.split('\n', 1)[1].decode('ascii'))
            return
    console_print(u"unknown command '%s'" % argv[0], f=sys.stderr)

    
def main(argv):
    global commands
    
    cut = None
    for i in range(len(argv)):
        if argv[i] in commands or argv[i] in aliases:
            cut = i
            break

    if cut == None:
        usage(argv)
        os._exit(0)
        return

    # now dispatch and run
    result = None
    if argv[i] in commands:
        result = commands[argv[i]](argv[i+1:])
    elif argv[i] in aliases:
        result = aliases[argv[i]](argv[i+1:])

    # flush, in case output is rerouted to a file.
    console_flush()

    # done
    return result

if __name__ == '__main__':
    ret = main(sys.argv)
    if ret is not None:
        sys.exit(ret)
