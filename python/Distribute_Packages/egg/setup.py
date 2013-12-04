import os
from setuptools import setup

def read(fname):
    return open(os.path.join(os.path.dirname(__file__), fname)).read()

setup(
    name="an example of egg project",
    version="0.0.1",
    author="zxsu",
    author_email="suzp1984@gmail.com",
    description=("A example python project"),
    license="BSD",
    keywords="egg project hello",
    packages=['hello', 'tests'],
    long_description=read('README')
)
