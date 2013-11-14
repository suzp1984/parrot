##
# http://www.shutupandship.com/2012/01/understanding-python-iterables-and.html
##

class MyList(list):
    def __iter__(self):
        return MyListIter(self)
    
class MyListIter(object):
    """ A sample implementation of a list iterator. NOTE: This is just a 
    demonstration of concept!!! YOU SHOULD NEVER IMPLEMENT SOMETHING LIKE THIS!
    Even if you have to (for any reason), there are many better ways to 
    implement this."""
    def __init__(self, lst):
        self.lst = lst
        self.i = -1
    def __iter__(self):
        return self
    def next(self):
        if self.i<len(self.lst)-1:
            self.i += 1         
            return self.lst[self.i]
        else:
            raise StopIteration

if __name__ == '__main__':
    a = MyList([1, 2, 3, 4])
    ia = iter(a)
    print 'type(a): %r, type(ia): %r' %(type(a), type(ia))
    for i in a: 
        print i,
