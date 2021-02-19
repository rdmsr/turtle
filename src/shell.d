module shell;
import builtins;
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

int spawn_command(string command)
{

    string[] command_array = command.split(" ");
    pid_t pid, wait_pid;
    int status;

    pid = fork();

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

int execute_command(string args)
{

    string[] command = args.split(" ");

    if (command[0] == null)
    {
        return 1;
    }
    switch (command[0])
    {
      
    case "cd":
        builtins.cd(command);
        break;
	
    case "exit":
        builtins.exit(command);
	return 0;
        break;
	
    default:
        return spawn_command(args);
        break;
    }
    return 1;
}

void prompt()
{

    string command, prompt;

    int status;
    do
    {

        /* Take the prompt from the PROMPT environment variable, if set */
        if (environment.get("PROMPT"))
        {
            prompt = environment["PROMPT"];
        }

        /* Use a default prompt */
        else
        {
            prompt = "$ ";
        }

        /* Print out the prompt */
        write(prompt);

        /* Read the next command to execute */
        readf("%s\n", &command);

        /* Execute it */

        status = execute_command(command);
    } while (status);
}
