MYSHELL                      General Commands Manual                  MYSHELL

NAME
	myshell - C shell with command line editing

SYNOPSIS
	myshell [arg ...]

DESCRIPTION
	myshell is a basic implementation of a UNIX C shell. It includes only
	a command-line editor.

    Argument processing
    	If the first argument to the shell is a pipe or redirect, then it is
	invalid. All other arguments are processed as arguments to the
	executing command.

    Startup and shutdown
        myshell begins by executing the command myshell. This shell will run
	indefinitely until quit is entered by the user.

    Built-in commands
        Non-pipable / Non-ridirectable
	    The following are descriptions of unredirectable and unpipable
	    builtin functions

	    cd
	        Changes the current working directory of myshell. Defaults to
		staying in the same directory.

		Usage: cd [arg1] or cd

	    clr
	        Prints a series of newlines to clear the terminal screen.

		Usage: clr

	    quit
	        Exits myshell implementation.

		Usage: quit

	Other
 	    dir
	        Displays all files within a directory. Defaults to the contents
		of the current working directory.

		Usage: dir [arg1] or dir

	    environ
	        Displays all variables of the environment.

		Usage: environ
		
	    echo
		Displays the arguments passed to the command or the file
		contents passed in.

		Usage: echo [argv] or echo or echo < filename

	    help
	        Displays this readme file.

		Usage: help	    

    Redirection
        Redirects input or output file streams of a program to a file. If
	no piping is used, input redirection must occur before output
	redirection. If piping is used, input redirect may only be used
	before the first pipe and output redirection may only be used
	after the final pipe.

	    Usage:
	      executingProgram < infile > outfile or
	      executingProgram < infile | ... | otherExecutingProgram > outfile

	Commands

	    <
	       Redirects the input stream of a running program to the
	       specified file. Therefore, the contents of the file are read
	       in a running program rather than information entered through
	       the terminal. If the input file does not exist, an error is
	       printed.

	       Usage: executingProgram < inputFile
	       
	    >
	       Redirects the output stream of a running program to the
	       specified file. Therefore, a running program prints to the
	       output file rather than printing to the terminal. If the
	       output does not exist, one is created. If it exists, its
	       contents are cleared.

	       Usage: executingProgram > outputFile
	       
	    >>
	       Redirects the output stream of a running program to the
	       specified file. Therefore, a running program prints to the
	       output file rather than printing to the terminal. If the
	       output does not exist, one is created. If it exists, everything
	       written by the running program is appended to the output file.

	       Usage: executingProgram >> outputFile

    Piping
        Redirects input or output streams of a program to another program. The
	final program's output defaults to the terminal.
	Therefore in the usage below, prog1's output goes to prog2's input.
	Prog2's output goes to prog3's input. ProgN-1's output goes to progN
	and progN's output goes to the terminal.

	Usage: prog1 | prog2 | prog3 | ... | progN-1 | progN 

    Background Execution
        Sets the entered command to run as a background process. Therefore,
	the running command will fully execute, but the shell will continue
	running. This allows the user to continue executing commands while the
	origional command runs and prints its contents to the terminal or
	wherever specified. 

        Usage: prog1 [args] &

    Environment
	A list of variables storing information about the shell in a
	variable=value format. For example, a users current working directory
	is stored as PWD=/this/is/the/absolute/path/to/the/current/directory
	Environment variables are stored in all caps. These environment
	variables can be updated as the user executes commands such as "cd",
	which changes the PWD variable. To print all environment variables,
	enter the command "environ".

    Executables
	Executes an executable program if it exists according to the user's
	PATH environment variable. The executable is checked for existance by
	looping through the environment's PATH in colon delimineted sections
	from left to right. If the executable does not exist, an error is
	printed. Arguments listed to the right of the executable before any
	piping or file redirection are passed to the executable as argv in
	main.

	For Example:
	    prog arg1 arg2 arg3 < in.txt > out.txt

	    executes prog passing it a 2 dimensional array of chars with
	    the strings prog, arg1, arg2, arg3, and NULL respectively.