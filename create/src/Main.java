package create;

import js.Io;
import java.io.IOException;
import java.io.File;

class Main {

    private static void panic(String msg, Exception e) {
	Io.println("ERROR: "+msg+(e==null ? "" : ": "+e.getMessage()));
	Io.print("USAGE: <dir> <projectname>\n");
	System.exit(1);
    }

    private static final String[] dirs = {"src", "libs", "rsc"};
    
    public static void main(String[] args) {	
	if(args.length!=2) {
	    panic("Please provide exactly two arguments", null);
	}

	final String path = args[0] + "/" + args[1];
	final String name = args[1];

	//Create root
	File root = null;
	try{
	    root = Io.createDir(path);
	}
	catch(Exception e) {
	    panic("Can not create root directory", e);
	}

	//Create libs, src, rsc
	try{
	    for(String dir : dirs) {
		Io.createDir(root, dir);
	    }
	}
	catch(Exception e) {
	    panic("Can not create root directory", e);
	}

	//Create make.bat, run.bat and Main.java
	try{
	    Io.writeFile(path+"/make.bat", make(), "\n", false);
	    Io.writeFile(path+"/src/Main.java", entry(name), "\n", false);
	    Io.writeFile(path+"/run.bat", run(name), "\n", false);
	}
	catch(Exception e) {
	    panic("Can not create file: ", e);
	}
    }

    private static String make() {
	return "javac -cp \".\\libs\\*\" .\\src\\*.java -d .";
    }

    private static String entry(String name) {
	return
	    "package "+name+";\n"+
	    "\n"+
	    "import js.Io;\n"+
	    "\n"+
	    "class Main {\n"+
	    "    public static void main(String[] args) {\n"+
	    "        Io.println(\"Hello, World!\");\n"+
	    "    }\n"+
	    "}\n"+
	    "\n";
    }

    private static String run(String name) {
	return "java -cp \"./libs/*;./\" "+name+".Main";
    }
}
