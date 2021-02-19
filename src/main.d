import std.stdio;
import shell;
void main(string[] args)
{

    if (args.length == 1)
    {

        shell.prompt();
    }

    else
    {
        shell.parse_args(args[1]);
    }
}
