#!/bin/env python3

version="0.0.2"

import os
# os.system('rm -rf *.egg-info/')        # Make 'really clean'

# Prevent the setuptools_scm plugin from adding (only) the contents of the git repo to the tarball:
os.system('mv -f ../.git ../.git_temp')

with open("README.md", "r") as fh:
    long_description = fh.read()


from setuptools import setup
setup(
    name='soltrack',
    description='A free, fast and accurate Python package to compute the position of the Sun',
    author='Marc van der Sluys',
    url='http://soltrack.sf.net',
    
    packages=['soltrack'],
    install_requires=['numpy','dataclasses'],
    long_description=long_description,
    long_description_content_type='text/markdown',
    
    version=version,
    license='GPLv3+',
    keywords=['astronomy','ephemeris','sun','solar'],
    
    classifiers=[
        "Development Status :: 4 - Beta",
        "Intended Audience :: Developers",
        "Intended Audience :: Education",
        "Intended Audience :: Science/Research",
        "License :: OSI Approved :: GNU General Public License v3 or later (GPLv3+)",
        "Natural Language :: English",
        "Operating System :: OS Independent",
        "Programming Language :: Python :: 3",
        "Topic :: Scientific/Engineering",
        "Topic :: Scientific/Engineering :: Astronomy",
        "Topic :: Scientific/Engineering :: Physics",
    ]
)

# Put git repo back:
os.system('mv -f ../.git_temp ../.git')

# Do some basic checks:
print("\nPython source files included in tarball:")
os.system('tar tfz dist/soltrack-'+version+r'.tar.gz |grep -E "\.py"')
print()

os.system('twine check dist/soltrack-'+version+'.tar.gz')
os.system('twine check dist/soltrack-'+version+'-py3-none-any.whl')
print()

