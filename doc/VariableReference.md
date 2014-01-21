# Variable Reference

`MarkEmptyDirs` allows you to use template variables in marker text as well as marker in create and delete hooks. These variables are evaluated every time a new marker file is to be created or if a create or delete hook is to be executed. The evaluated results then replace the variables.

For instance, in order to create marker files containing a unique id, the current username, and the current time you could created a marker text template file `marker.txt`:

    CREATOR: §env:USER§
    ID: §guid§
    TIME: §datetime:MM/dd/yyyy§

    Do not delete this file.

And execute the following command:

    MarkEmptyDirs update --file=marker.txt PROJECT

You can also provide a marker text template directly on the command line. Then the following command creates marker files identical to the one shown previously:

    MarkEmptyDirs update --text="CREATOR: §env:USER§§lf§ID: §guid§§lf§TIME: §datetime:MM/dd/yyyy§§lf:2§Do not delete this file.§lf§" PROJECT

Using marker files with unique content allows to indicate renames of directories automatically. Mercurial as well as Git have such a feature. They try to guess renames by considering file similarities.

Currently, the following variables are available:

## datetime

**get current UTC time**

**SYNTAX:** `§datetime[:FORMAT]§`

ARGUMENT | DESCRIPTION
:------- | :----------
`FORMAT` | Expression determining the format of the result string.

This expressions may be used for the date:

EXPRESSION | OUTPUT
:--------- | :-----
`d`    | the day as number without a leading zero (1 to 31)
`dd`   | the day as number with a leading zero (01 to 31)
`ddd`  | the abbreviated localized day name (e.g. 'Mon' to 'Sun')
`dddd` | the long localized day name (e.g. 'Monday' to 'Qt::Sunday')
`M`    | the month as number without a leading zero (1-12)
`MM`   | the month as number with a leading zero (01-12)
`MMM`  | the abbreviated localized month name (e.g. 'Jan' to 'Dec')
`MMMM` | the long localized month name (e.g. 'January' to 'December')
`yy`   | the year as two digit number (00-99)
`yyyy` | the year as four digit number

This expressions may be used for the time:

EXPRESSION | OUTPUT
:--------- | :-----
`h`   | the hour without a leading zero (0 to 23 or 1 to 12 if AM/PM display)
`hh`  | the hour with a leading zero (00 to 23 or 01 to 12 if AM/PM display)
`m`   | the minute without a leading zero (0 to 59)
`mm`  | the minute with a leading zero (00 to 59)
`s`   | the second without a leading zero (0 to 59)
`ss`  | the second with a leading zero (00 to 59)
`z`   | the milliseconds without leading zeroes (0 to 999)
`zzz` | the milliseconds with leading zeroes (000 to 999)
`AP`  | use AM/PM display and replace by either "AM" or "PM"
`ap`  | use am/pm display and replace by either "am" or "pm"

## dir

**get base or current directory**

**SYNTAX:** `§dir[:base\|base.abs\|base.rel\|cur\|cur.abs\|cur.rel]§`

ARGUMENT | DESCRIPTION
:------- | :----------
`base`     | name of base directory
`base.abs` | absolute path of base directory
`base.rel` | path of base directory relative to current directory
`cur`      | name of current directory
`cur.abs`  | absolute path of current directory
`cur.rel`  | path of current directory relative to base directory

## env

**get the value from an environment variable**

**SYNTAX:** `§env:NAME§`

ARGUMENT | DESCRIPTION
:------- | :----------
`NAME` | the environment variable's name

## guid

**get a new globally unique identifier**

**SYNTAX:** `§guid§`

## lf

**get line feed character(s)**

**SYNTAX:** `§lf[:COUNT]§`

ARGUMENT | DESCRIPTION
:------- | :----------
`COUNT`  | integer denoting how often character is repeated

## marker

**get marker name**

**SYNTAX:** `§marker[:name|name.abs|name.rel]§`

ARGUMENT | DESCRIPTION
:------- | :----------
`name`     | name of marker file
`name.abs` | absolute path of marker file
`name.rel` | path of marker file relative to base directory

## separator

**get platform specific directory, path, or volume separator**

**SYNTAX:** `§separator:dir|path|vol§`

ARGUMENT | DESCRIPTION
:------- | :----------
`dir`  | directory separator (e.g. '\' on Windows)
`path` | path separator (e.g. ';' on Windows)
`vol`  | volume separator (e.g. ':' on Windows)

## sp

**get space character(s)**

**SYNTAX:** `§sp[:COUNT]§`

ARGUMENT | DESCRIPTION
:------- | :----------
`COUNT`  | integer denoting how often character is repeated
