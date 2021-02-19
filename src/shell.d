module shell;
import std.stdio;
import core.stdc.stdlib;
import std.process;
import std.array;
import core.sys.posix.unistd;
import core.sys.posix.sys.wait;
void parse_args(string args)
{
    switch (args)
    {

    case "--version":
    case "-v":
    {
        writeln("DSH -- the D shell, V0.0.1 copyright (c) Abb1x 2021");
        break;
    }
    case "--help":
    case "-h":
    {

        writeln("Usage: dsh [OPTION]");
        writeln("-v, --version: Shows the current version");
        break;
    }

    default:
    {
        writeln("Unknown command, run dsh --help to get a list of commands");
        break;
    }
    }
}

int execute_command(string command)
{

    string[] command_array = command.split(" ");
    pid_t pid, wait_pid;
    int status;

    pid = fork();

    writeln(command_array);
    
    /* Child process */
    if (pid == 0)
    {

        if (execvp(command_array[0], command_array) == -1)
        {
            perror("dsh");
        }
	
        exit(EXIT_FAILURE);
    }
    
    else if (pid < 0)
    {
        /* Error forking */
        perror("dsh");
    }
    
    else
    {
        /* Parent process */
        do
        {
            wait_pid = waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
    
    return 1;
}

void prompt()
{

    string command, prompt;

    int status;
    do
    {

        if (environment.get("PROMPT"))
        {
            prompt = environment["PROMPT"];
        }
        else
        {
            prompt = "$ ";
        }
	
        write(prompt);
        readf("%s\n", &command);
        status = execute_command(command);
    } while (status);
}
