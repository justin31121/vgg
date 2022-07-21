package watch;

import js.Io;

import java.io.File;
import java.io.BufferedReader;
import java.io.InputStreamReader;

import java.util.Scanner;
import java.util.Date;

import java.nio.file.attribute.FileTime;
import java.nio.file.attribute.BasicFileAttributes;
import java.nio.file.Path;
import java.nio.file.Files;

import java.text.SimpleDateFormat;

import io.github.alexarchambault.windowsansi.*;

class Main {

    private interface Function {
	void foo();
    }

    private static Scanner in = new Scanner(System.in);
    private static SimpleDateFormat formatter = new SimpleDateFormat("MMM EE dd HH:mm:ss");

    public static void cls() throws Exception {
	new ProcessBuilder("cmd", "/c", "cls").inheritIO().start().waitFor();
    }

    private static void execute(String command) {
	try {
	    new ProcessBuilder("cmd", "/c", command).inheritIO().start().waitFor();
	}
	catch(Exception e) {
	    
	}
    }

    public static void bar(String[] args, String dir) {
	try{
	    cls();
	    
	    ProcessBuilder ps = new ProcessBuilder(args);

	    ps.redirectErrorStream(true);
	    ps.directory(new File(dir));

	    Process pr = ps.start();

	    BufferedReader in = new BufferedReader(new InputStreamReader(pr.getInputStream()));
	    String line;
	    while((line = in.readLine()) != null) {
		Io.println(line);
	    }
	    int n = pr.waitFor();

	    if(n==0) {
		Io.print("\nCompilation \u001B[32mfinished\u001B[0m");
	    }
	    else {
		Io.print("\nCompilation \u001B[31mexited abnormally\u001B[0m with code \u001B[31m"+n+"\u001B[0m");
	    }
	    String date = formatter.format(new Date());
	    Io.println(" at "+date);

	    in.close();	    
	}
	catch(Exception e) {
	    Io.print("ERROR: Provided command: \"");
	    for(int i=0;i<args.length;i++) {
		Io.print(args[i]);
		if(i!=args.length-1) {
		    Io.print(" ");
		}
	    }
	    Io.println("\" crashed.");
	    System.exit(1);
	}
    }
    
    public static void main(String[] args) {

	//PARSE INPUT
	if(args.length<2) {
	    Io.println("ERROR: Please provide enough arguments");
	    Io.println("USAGE: <filename> <process>");
	    System.exit(1);
	}
	String dir = args.length<3 ? "." : args[2];

	if(!Io.existFile(dir+"\\"+args[0])) {
	    Io.println("ERROR: The file: \""+args[0]+"\" does not exist");
	    System.exit(1);
	}

	String[] arguments = args[1].split(" ");
	
	//INIT COLOR IN CONSOLE
	try{
	    boolean init = WindowsAnsi.setup();
	    if(!init) throw new Exception();
	}
	catch(Exception e) {
	    Io.println("ERROR: Could not init WindowsAnsi");
	    System.exit(1);
	}
	
	//WHEN FILE IS CHANGED DO 
	Thread thread = new Thread(new Listener(dir+"\\"+args[0], () -> { bar(arguments, dir);}));
	thread.start();

	//STDIN
	while(true) {
	    String line = in.nextLine();

	    if("q".equals(line)) {
		break;
	    }
	    if("log".equals(line)) {
		Io.println("Dir: "+dir+", File: "+args[0]+", Command "+args[1]);
	    }
	    else if(line.toCharArray()[0]==':') {
		String command = line.substring(1);

		execute("cd "+dir+" && "+command);
	    }
	}

	//EXIT
	System.exit(0);
    }
    
    private static FileTime lastModified(String file_path) throws Exception {
	File file = new File(file_path);

	Path path = file.toPath();
        
	BasicFileAttributes fatr = Files.readAttributes(path, 
							BasicFileAttributes.class);

	return fatr.lastModifiedTime();
    }
    
    private static class Listener implements Runnable {

	private String path;
	private FileTime last;
	private long read;
	private Function f;

	public Listener(String path, Function f) {
	    this.path = path;
	    last = null;
	    read = 0;
	    this.f = f;
	}
	
	@Override
	public void run() {
	    while(true) {
		try{
		    Thread.sleep(500);
		}
		catch(Exception e){
		    
		}

		FileTime next;
		try{
		    next = lastModified(path);    
		}
		catch(Exception e) {
		    continue;
		}

		if(last==null || hasChanged(next)) {
		    try{
			f.foo();
			last = next;
		    }
		    catch(Exception e) {
			
		    }
		}
	    }
	}

	private boolean hasChanged(FileTime next) {
	    return last.compareTo(next) < 0;
	}
    }
}

