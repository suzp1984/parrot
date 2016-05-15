# play corountine generator and async

from types import coroutine

def countdown(n):
    while n >= 0:
        yield n
        n -= 1

@coroutine
def spam():
    result = yield 'somevalue'
    print('The result is ', result)

#f = spam()
#f.send(None)
#f.send(42)


async def foo():
    print('Start foo')
    await spam()
    print('End foo')

#f = foo()
#f.send(None)
#f.send(42)

async def bar():
    print('Start bar')
    await foo()
    print('End bar')


f = bar()
f.send(None)
f.send(42)
