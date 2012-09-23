// vim: set ts=2 sw=2 expandtab:

/*
 * Basic outline template of the learning shell (lsh)
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
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <regex.h>
#include <unistd.h>
#include <libgen.h>

#define SZ(t) (sizeof(t) / sizeof(t[0]))

// $ getconf ARG_MAX
#define ARG_MAX 2097152                

// For simplicity of implementation, this cmd buffer is visible to all functions
static char cmd[ARG_MAX + 1];

// Starting with a hard coded prompt string.
// TODO Make this user configurable through an environment variable setting
#define PS1 "lsh>> "

static char *progname;

static void license()
{
  fprintf(stderr, "%s Copyright (C) 2012 Brian Gillespie\n"
                  "This program comes with ABSOLUTELY NO WARRANTY; This is free software,\n"
                  "and you are welcome to redistribute it under certain conditions;\n", progname);
}

static void init()
{
  // Does nothing yet
}

// Show the command prompt
static void prompt()
{
  /*
   * We only want to show the prompt string if we are outputting
   * to a terminal and not being redirected to a pipe or file
   */
  if (isatty(fileno(stdout))) {
    fprintf(stdout, PS1);
  }
}

// Do exit cleanup
static void exiting()
{
  if (isatty(fileno(stdout))) {
    fprintf(stdout, "\n");
  }
  free(progname);
}

/*
 * Run an internal or external command
 * TODO Add support for internal commands
 */
static int dispatch()
{
  int status;
  /*
   * TODO Add support for passing argument list to called programs
   */
  execl(cmd, cmd, (char *)0);

  /*
   * Shouldn't get here if above successful has been successful
   * TODO Improve the error message
   */
  perror(cmd);
  return -1;
}

static void setvar(const char *name, const char *value)
{
  /*
   * TODO Add support for setting name=value
   */
  fprintf(stderr, "Not Implemented: %s=%s\n", name, value);
}

/*
 * Check for the valid forms of command input which are:
 *
 * <command> [<arg1> <arg2> ... <argN>]
 * <name>=<value>
 */
static int parse()
{
  regex_t regex;
  const int cflags = REG_EXTENDED|REG_NEWLINE;
  int rc;
  regmatch_t pmatch[4];

  /*
   * Kill the trailing newline character before dispatching
   * TODO Trim any trailing white space
   */
  char *nl = index(cmd, '\n');
  if (nl) {
    *nl = '\0';
  }

  // Look for patterns of the form: <name>=<value>
  (void)regcomp(&regex, "([a-zA-Z]+[a-zA-Z0-9]*)=(.*)", cflags);
  rc = regexec(&regex, cmd, SZ(pmatch), pmatch, 0);
  if (rc != 0) {
    /*
     * Not a variable setting so assume it's a command
     */
    dispatch();
  } else {
    /*
     * pmatch[1] marks var name start and end
     * pmatch[2] marks var value start and end
     */
    char *name = strndup(cmd, pmatch[1].rm_eo - pmatch[1].rm_so);
    char *value = strndup(cmd + pmatch[2].rm_so, pmatch[2].rm_eo - pmatch[2].rm_so);
    setvar(name, value);
    free(name);
    free(value);
  }
}

static int repl()
{
  int status = 0;

  for (prompt(); fgets(cmd, sizeof(cmd), stdin) != NULL; prompt()) {
    status = parse();

    // TODO Bind to the exit code of the command just run to $?
  }
}


int main(int argc, char *argv[]) 
{
  progname = strdup(basename(argv[0]));
  init();
  license();
  repl();
  exiting();
}