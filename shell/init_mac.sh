#!/bin/sh
# This file is for me to init my mac machine, including common software suite and tools

# install homebrew
# ruby -e "$(curl -fsSL https://raw.github.com/Homebrew/homebrew/go/install)"  
ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"


# python package installation
pip install nose
pip install coverage
