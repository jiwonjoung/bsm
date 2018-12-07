#!/usr/bin/env python

"""
------------------------------------------------------------------------------
 Authors    : Thomas Sprinkmeier
 Company    : Cohda Wireless
 Date       : 17/08/2010
------------------------------------------------------------------------------
 Copyright (c) 2010 Cohda Wireless Pty. Ltd.
------------------------------------------------------------------------------
 Description:

 Test.

------------------------------------------------------------------------------
 $LastChangedRevision: 4311 $
 $LastChangedBy: COHDA\tsprinkmeier $
 $LastChangedDate: 2010-08-17 12:50:16 +0930 (Tue, 17 Aug 2010) $
------------------------------------------------------------------------------

Mangle a set of files according to the supplied rules.

Provide a set og JSON-encode mangling rules to have them applied.

Given a rules file containing:
[
 [
  [
   "filename_glob_0",
   "filename_glob_1",
   ...
  ],
  [
   [
    "search_0",
    "replace_0",
   ],
   [
    "search_1",
    "replace_1",
   ],
   ...
  ]
 ],
 [
  [
   "filename_glob_2",
   "filename_glob_3",
    ...
  ],
  ...
 ]
]

filename_glob_0 is expanded and the regular expression replacements
  s/search_0/replace_0/g
and
  s/search_1/replace_1/g
etc.
are applied in turn to each file.
Next filename_glob_1 is expanded ...

All files are backed up to by saving them to *.orig

filename_glob can be "-" meaning STDIN will be processed to STDOUT
(verbose mode should be disabled). This is intended for testing.

"""

import glob
import json
import optparse
import os
import re
import sys

## verbose flag
VERBOSE = False
# log function
def log(arg):
    if (VERBOSE):
        print(arg)
#------------------------------------------------------------------------------
## open a file, read the contents and creat the backup file
#
#  @param filename Name of the file to open/read/back-up
#  @return     contents of original file
#
#------------------------------------------------------------------------------
def snarf(filename, clobber = False):
    # treat '-' differently (can't back up stdin)
    if (filename == '-'):
        return sys.stdin.read()

    # backup, append ".orig"
    orig = "%s.orig" % filename
    if (os.path.exists(orig)):
        if (not clobber):
            sys.stderr.write('"%s" exists, bailing!\n' % orig)
            sys.exit(1)
        log('"%s" exists, clobbering\n' % orig)

    # grab original file
    with open(filename, "r") as f:
        d = f.read()

    with open("%s.orig" % filename, "w") as f:
        f.write(d)
    log('"%s" -> "%s", %d' % (filename, orig, len(d)))
    return d

# apply the given set to search/replace rules to the given data
def mangleFile(data, mangles, filename):
    filename = os.path.splitext(os.path.split(filename)[1])[0]
    for (search, replace) in mangles:
        log("\t\ts/%s/%s/g" % (search, replace))
        search  = re.sub('%filename%', filename, search)
        search  = re.sub('%%', '%', search)
        replace = re.sub('%filename%', filename, replace)
        replace = re.sub('%%', '%', replace)
        log("\t\ts/%s/%s/g" % (search, replace))
        data = re.sub(search, replace, data)
    return data

# apply the mangle rule-set (glob patterns and search/replace rules)
def applyRuleset(DATA, mangles, clobber = False):
    # iterate through the patterns
    for ptrn in mangles[0]:
        if (ptrn == '-'):
            files = ('-',)
        else:
            files = glob.glob(ptrn)
            log('glob("%s") -> %s' % (ptrn, repr(files)))

        for f in files:
            data = DATA.get(f, snarf(f, clobber))
            log("\t%s" % f)
            DATA[f] = mangleFile(data, mangles[1],f)

#---------------------------------------------------------------------------
## Apply a set of mangling recipies
#
#  @param JSONfilenames a (set of) filenames of mangling recipies
#  @param verbose Controls noise to STDOUT
#  @param clobber Over-write ".orig" files rather than exiting
#---------------------------------------------------------------------------
def mangleFiles(JSONfilenames,
                verbose = True,
                clobber = False):
    # a bit rude....
    global VERBOSE
    VERBOSE = verbose

    # if the user supplied a file rather than
    # a set of files wrap it up
    if (isinstance(JSONfilenames,str)):
        JSONfilenames = tuple([JSONfilenames])

    log("Processing: %s" % repr(JSONfilenames))

    # all the file contents are kept in RAM during processing.
    DATA = {}

    # process one JSON recipy file at a time
    for JSONfilename in JSONfilenames:
        with open(JSONfilename, "r") as r:
            rules = json.loads(r.read())
        log("%s:\n\t%s" % (JSONfilename, json.dumps(rules)))
        # apply each rule in the ruleset
        for m in rules:
            applyRuleset(DATA, m, clobber)

    # all processing done, time to save the mangled files
    files = sorted(DATA.keys())
    log("Processed %d files" % len(files))
    for f in files:
        if (f == '-'):
            sys.stdout.write(DATA[f])
            continue
        with open(f, "w") as w:
            data = DATA[f]
            log('"%s": %d' % (f, len(data)))
            w.write(data)

def main():

    parser = optparse.OptionParser(version = "$Revision: 4311 $",
                                   epilog  = __doc__)

    parser.add_option("--clobber",    action="store_true",
                      default = False,
                      help="clobber .orig files")
    parser.add_option("--no-clobber", action="store_false",
                      dest="clobber",
                      help="fail if .orig file exists")

    parser.add_option("-v", "--verbose", action="store_true",
                      dest="verbose", help="verbose (not quiet)",
                      default = True)
    parser.add_option("-q", "--quiet",   action="store_false",
                      dest="verbose", help="quiet (not verbose)")

    (options, args) = parser.parse_args()

    mangleFiles(args,
                options.verbose,
                options.clobber)

if __name__ == "__main__":
    main()
