# Use Cases

In the following sections some typical use cases are described and it is shown how the tool can help you getting your work easily done. For clarity, I use directory names with upper case letters and file names with lowercase letters in the following examples.

## Create marker files

Within a directory `PROJECT` find all "leaf" directories that do not contain any files or sub-directories, and create a marker file `.emptydir` within each such "leaf" directory.

For example, let's assume the following source tree:

    [PROJECT]
            |
            *--[DIR1]
            |       |
            |       *--[DIR1-1]
            |
            *--[DIR2]
            |       |
            |       *--file1
            |
            *--[DIR3]

A tree with the corresponding marker files will then look like this:

    [PROJECT]
            |
            *--[DIR1]
            |       |
            |       *--[DIR1-1]
            |                 |
            |                 *--.emptydir
            |
            *--[DIR2]
            |       |
            |       *--file1
            |
            *--[DIR3]
                    |
                    *--.emptydir

To create the `.emptydir` files invoke `MarkEmptyDirs`'s `update` command:

    MarkEmptyDirs update PROJECT

This creates empty `.emptydir` files. If you want the marker files to contain special content there are two ways.

Provide the content directly on command line:

    MarkEmptyDirs update --text="Do not delete this file." PROJECT

Provide a template file:

    MarkEmptyDirs update --file=template.txt PROJECT

## Update marker files

Let's assume the `PROJECT` directory has undergone some changes. Directories and/or files have been added and/or deleted. This requires to synchronize the corresponding marker files. In particular, some now are not needed anymore and new ones may now be necessary.

For instance, have a look at the following tree:

    [PROJECT]
            |
            *--[DIR1]
            |       |
            |       *--[DIR1-1]
            |                 |
            |                 *--.emptydir
            |                 |
            |                 *--newfile.txt
            |
            *--[DIR2]
            |
            *--[DIR3]
                    |
                    *--.emptydir

`DIR1-1` now contains a new file `newfile.txt` and a previously created `.emptydir` file. The former now acts as a marker and, thus, the latter is not needed anymore. `DIR2` now is empty and requires a marker file.

Consequently, after updating the marker files the tree should look like this:

    [PROJECT]
            |
            *--[DIR1]
            |       |
            |       *--[DIR1-1]
            |                 |
            |                 *--newfile.txt
            |
            *--[DIR2]
            |       |
            |       *--.emptydir
            |
            *--[DIR3]
                    |
                    *--.emptydir

Using the `MarkEmptyDirs` tool for synchronization one would simply do (again):

    MarkEmptyDirs update PROJECT

## Delete marker files

There are situations where one wants a clean tree which does not contain any marker files. So all `.emptydir` files should be removed.

This can simply easily be achieved by using the `clean` command:

    MarkEmptyDirs clean PROJECT

## List marker files

In order to list all marker files use `list` command:

    MarkEmptyDirs list PROJECT

Note that this command is simply a shorthand for:

    MarkEmptyDirs clean --short --dry-run PROJECT

## Execute actions upon creation or deletion of marker files

Sometimes you might want to hook into the marker creation or deletion process in order to execute certain actions. For example, if you want the marker files be added to a Mercurial repository automatically you could tell MarkEmptyDirs to execute a `hg add <marker-file>` every time a new marker is created. Likewise, you can tell it to execute a `hg rm -fA <marker-file>` every time a marker is to be deleted. The following command line will do this:

    MarkEmptyDirs update --create-hook="hg add §marker§" --delete-hook="hg rm -fA §marker§" PROJECT

## Remove everything except marker files

Sometimes, directories previously marked empty using marker files should remain empty. For instance, you might want to check in an empty `bin` directory without the compiled binary files. Let's assume the following directory tree:

    [PROJECT]
            |
            *--[DIR1]
            |       |
            |       *--[DIR1-1]
            |                 |
            |                 *--newfile.txt
            |
            *--[DIR2]
            |       |
            |       *--.emptydir
            |       |
            |       *--tempfile1.dll
            |       |
            |       *--[DIR2-1]
            |                 |
            |                 *--tempfile2.obj
            |
            *--[DIR3]
                    |
                    *--.emptydir
                    |
                    *--tempfile3.so

In this example, `DIR2` contains not only a marker but also `tempfile1.dll` and a subdirectory `DIR2-1` which we want to get rid of. `DIR3` contains `tempfile3.so` which should also be removed. Actually, we want the tree to look like this:

    [PROJECT]
            |
            *--[DIR1]
            |       |
            |       *--[DIR1-1]
            |                 |
            |                 *--newfile.txt
            |
            *--[DIR2]
            |       |
            |       *--.emptydir
            |
            *--[DIR3]
                    |
                    *--.emptydir

So in order to re-empty directories containing marker files you can use `purge` command:

    MarkEmptyDirs purge PROJECT

*WARNING*: Be careful with this command! If you are not sure what the this command will do execute it with `--dry-run` and `--verbose` options first:

    MarkEmptyDirs purge --dry-run --verbose PROJECT

If your tree contains symbolic links and `--dereference` option is used I would highly recommend to do this in advance!
