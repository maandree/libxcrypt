#include "crypt-port.h"
#include "crypt-base.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *password   = "zyxwvuts";
const char *tests[][2] =
{
  /* Hashes have been computed with the following program:

     #!/usr/bin/python

     from passlib.hash import sha1_crypt
     import csv

     sha1crypt = sha1_crypt.using(rounds=1)

     with open('sha1crypt.txt') as csvfile:
         params = csv.reader(csvfile)
         for row in params:
             print(sha1crypt.using(rounds=int(row[0]),
                                   salt=row[1]).hash("zyxwvuts"))

     The used csv file had the following format:

     <rounds>,<salt(16byte)> (when no rounds parameter was given,
                              rounds were set to zero.)

     The salts have been generated by crypt_gensalt on OpenSolaris.


     Test X.0: checks the password is encryptable with the full hash
               as setting.
     Test X.1: checks the password is encryptable with the initially
               used salt as setting.
   */
  {
    "$sha1$40295$GGXpNqoJvglVTkGU$vhHjbglrEZcuASMnDHWQVamZx8j1",
    "$sha1$40295$GGXpNqoJvglVTkGU$",
  },
  {
    "$sha1$46858$32Tr4aJd.GGC5U0V$qyEzLlMXHsGvDh5X/4jugldGxTrx",
    "$sha1$46858$32Tr4aJd.GGC5U0V$",
  },
  {
    "$sha1$37586$xSZGpk6Bp4SA3.cR$UtT4FhiuYrfnL0fa4g/if5VON6qG",
    "$sha1$37586$xSZGpk6Bp4SA3.cR$",
  },
  {
    "$sha1$43007$OZZMNHpZiuBanplt$qHVrN.7fz85cYgPxy9StyT/DSXth",
    "$sha1$43007$OZZMNHpZiuBanplt$",
  },
  {
    "$sha1$37393$fQExw2gulRPUbljr$vAq9CV8JR9ux4.G/k9uQU.iyjQqQ",
    "$sha1$37393$fQExw2gulRPUbljr$",
  },
};

#define ntests (sizeof (tests) / sizeof (tests[0]))

int
main (void)
{
  struct crypt_data output;
  int result = 0;
  unsigned int i, j;
  char *previous = malloc (sizeof (output.output) + 1);

  for (i = 0; i < ntests; ++i)
    {
      for (j = 0; j < 2; ++j)
        {
          char *cp = crypt_r (password, tests[i][j], &output);
          if ((j == 0) && (strcmp (cp, tests[i][j]) != 0))
            {
              printf ("test %u.%d: expected \"%s\", got \"%s\"\n",
                      i, errno, tests[i][j], cp);
              result = 1;
            }
          if ((j == 1) && (strcmp (cp, previous) != 0))
            {
              printf ("test %u.%u: expected \"%s\", got \"%s\"\n",
                      i, j, previous, cp);
              result = 1;
            }
          strcpy (previous, tests[i][j]);
        }
    }

  free (previous);
  return result;
}
