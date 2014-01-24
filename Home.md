
# Introduction

[CVS][cvs], [Mercurial][hg], [Git][git] as well as other well-known version control systems cannot version directories. In other words, you cannot add empty directories! A "workaround" for this issue is to use marker files which are placed into empty directories. These marker files can then be committed into the repository and will make sure that, upon checkout, the directory tree is entirely reconstructed. This solution is also suggested in the [Mercurial FAQ](http://mercurial.selenic.com/wiki/FAQ#FAQ.2FCommonProblems.I_tried_to_check_in_an_empty_directory_and_it_failed.21), or in the [Git FAQ](http://git.wiki.kernel.org/index.php/GitFaq#Can_I_add_empty_directories.3F), for instance. Note however, that using that workaround might not be a good idea. Creating missing directories during a build process should also be considered as an option.

Sometimes a solution where the missing directories are created by some magic is not practicable and people will face the problem of managing such marker files. In particular, the problem with using marker files is that you need to create them, and delete them, if they are not necessary anymore (because there were added sub-directories or files). With big source trees managing these marker files can be cumbersome and error prone.

In the past, I had been confronted with such a situation several times. This is why I decided to write an open source tool which can manage the creation/deletion of such marker files automatically. It creates marker files in all empty "leaf" directories. If later on new files or directories are put into such directories, the marker files are not necessary anymore and, thus, are removed automatically.

# Licensing

This tool is licensed under the [BSD License][license].

# Operating Systems

MarkEmptyDirs is written in C++ / Qt and is designed to be platform independent. Currently, build scripts for the following operating systems are included in this project:

1. Linux
2. MacOS X
3. Windows

# Further Reading

 * [getting started][]: Describes what you need to do in order to install, run, or build MarkEmptyDirs.
 * [use cases][]: Describes typical use cases for this tool and how to use it.
 * [changelog][]: Describes what has changed in the different releases.

[changelog]: CHANGELOG
[cvs]: http://www.nongnu.org/cvs "CVS"
[getting started]: GettingStarted
[git]: http://git-scm.com "Git"
[hg]: http://mercurial.selenic.com/wiki "Mercurial"
[license]: LICENSE
[use cases]: UseCases
