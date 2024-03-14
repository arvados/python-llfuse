..
  NOTE: We cannot use sophisticated ReST syntax (like
  e.g. :file:`foo`) here because this isn't rendered correctly
  by PyPi.

The Python-LLFUSE Module
========================


.. start-intro

This is a fork of Python-LLFUSE used by the Arvados project to support
"arv-mount".  **If you are trying to install "arv-mount" you want the
"arvados_fuse" package.**

Python-LLFUSE is a set of Python bindings for the low level FUSE_
API. It requires at least FUSE 2.8.0 and supports both Python 2.x and
3.x. Like FUSE itself, Python-LLFUSE is developed for Linux systems,
but it should be compatible with OS-X, FreeBSD and NetBSD as well.

Python-LLFUSE releases can be downloaded from PyPi_. The documentation
can be `read online`__ and is also included in the ``doc/html``
directory of the Python-LLFUSE tarball.


Getting Help
------------

Please report any bugs on the `issue tracker`_. For discussion and
questions, please use the general `FUSE mailing list`_. A searchable
`mailing list archive`_ is kindly provided by Gmane_.


Contributing
------------

The Python-LLFUSE source code is available on GitHub_.


.. __: https://llfuse.readthedocs.io/
.. _FUSE: http://github.com/libfuse/libfuse
.. _FUSE mailing list: https://lists.sourceforge.net/lists/listinfo/fuse-devel
.. _issue tracker: https://github.com/arvados/python-llfuse/issues
.. _mailing list archive: http://dir.gmane.org/gmane.comp.file-systems.fuse.devel
.. _Gmane: http://www.gmane.org/
.. _PyPi: https://pypi.python.org/pypi/arvados-llfuse/
.. _GitHub: https://github.com/arvados/python-llfuse
