
// vim: set ts=2 sw=2 expandtab:

/*
 * Internal command interface
 *
 * Copyright (C) 2012  Brian Gillespie
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void
lsh_not_impl(const char *cmd)
{
  fprintf(stderr, "%s: not implemented\n", cmd);
}

#ifdef LSH_ENABLE_CD
int lsh_cd(int argc, char **argv)
{
  char *path = argv[1];
  if (!path) {
    // No path specified so the default is home
    path = getenv("HOME");
  }
  if (chdir(path) < 0) {
    perror("cd");
  };
  return 0;
}
#endif
 
