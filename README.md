arrowline - small shell prompt generator
========================================

Small shell (bash/zsh) prompt generator written in C. Can use powerline symbols.
[![Build Status](https://travis-ci.org/tryone144/arrowline.svg?branch=master)](https://travis-ci.org/tryone144/arrowline)

Features
--------

- single executable, no external processes
- configurable with a header file (like dwm)
- powerline-style
- git branch name and status


Installation
------------

Arch Linux PKGBUILD included. Generate the `.tar.xz` and install (needs root privileges) with:

    makepkg -i

Manual install:

    git clone git://github.com/tryone144/arrowline.git
    cd ./arrowline
    make

Systemwide installation (as root) into `/usr/local/bin`:

    make install

If you install by hand make sure that `git` is installed for git status support.


Configuration
-------------

All configuration takes place in `config.h`.

- Define the colors for the segments.
- Define the use of powerline symbols and escape chararcters
- Select which segments to show (NULL-terminated array of generators)

### Segment List

- *STATUS*: display red X if last command's exit status != 0
- *HOST*: display username and hostname
- *CWDPREFIX*: display the working directory prefix `home` `root` or `deep inside`
- *CWD*: display `CWD_LEN` (defaults to 3) folders of current working directory
- *VCS*`: display branch and status if current working dir is inside a version controlled directory
    
    - currently only supports git


Using arrowline
---------------

### bash

Put into your `.bashrc`:

    # arrowline prompt
    function _update_ps1() {
        export PS1="$( arrowline ${?} 2> /dev/null || echo 'arrowline failed! [\u@\h \W]\$ ' )"
    }
    export PROMPT_COMMAND=_update_ps1

KNOW ISSUES
-----------

- libgit2 updates seem to break the binary. A simple rebuild should be enough. See [this Issue](https://github.com/tryone144/arrowline/issues/2).


TODO
----

- zsh usage information
- add new segments


License
-------

MIT/X Consortium License

© 2016 Bernd Busse

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.

