class PluginMount(type):
    def __init__(cls, name, bases, attrs):
        if not hasattr(cls, 'plugins'):
            cls.plugins = []
        else:
            cls.plugins.append(cls)

class PasswordValidator(object):
    """
    Plugins extending this class will be used to validate
    passwords. Valid plugins must provide the following method.

    validate(self, password)

    """
    __metaclass__ = PluginMount

def is_valid_password(password):
    """
    Returns True if the password was fine, False if there was a problem.
    """
    for plugin in PasswordValidator.plugins:
        try:
            plugin().validate(password)
        except ValueError:
            return False
    return True

def get_password_errors(password):
    """
    Returns a list of messages indicating any problems that were found
    with the password. If it was fine, this returns any empty list.
    """
    errors = []
    for plugin in PasswordValidator.plugins:
        try:
            plugin().validate(password)
        except ValueError, e:
            errors.append(str(e))
    return errors


class MinimumLength(PasswordValidator):
    def validate(self, password):
        "Raise ValueError if the password is too short."
        if len(password) < 6:
            raise ValueError('Passwords must be at least 6 characters.')

class SpecialCharacters(PasswordValidator):
    def validate(self, password):
        "Raise ValueError if the password doesn't contain any special characters."
        if password.isalnum():
            raise  ValueError('Passwords must contain one special character.')


if __name__ == "__main__":
    for password in ('pass', 'password', 'p@ssword!'):
        print ('Checking %r...' % password)
        if is_valid_password(password):
            print 'valid!'
        else:
            print # Force a new line
            for error in get_password_errors(password):
                print ' %s' % error
                
    
