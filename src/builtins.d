module builtins;
import std.stdio;
import core.stdc.stdlib;
import core.sys.posix.unistd;

int cd(string[] args)
{
    if (args[1] == null)
    {
        writeln("dsh: expected arguments to cd");
    }
    else if (chdir(cast(char*)args[1]) != 0)
    {
      perror("dsh");
    }
    return 1;
}

int exit(string[] args)
{
  return 0;
}
