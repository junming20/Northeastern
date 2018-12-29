import java.io.*;
import java.util.*;

public class RegisterSystem {
	public static Map<String, Student> studentMap = new HashMap<> ();
	public static Map<String, Teacher> teacherMap = new HashMap<> ();
	public static Map<String, CourseMgr> courseMgrMap = new HashMap<> ();
	public static Map<String, FinanceMgr> financeMgrMap = new HashMap<> ();
	public static List<Course> courseList = new ArrayList<Course>(); 
	
	public static void main (String[] args) throws FileNotFoundException{
		Parser parser = new Parser(studentMap, teacherMap, financeMgrMap, courseMgrMap, courseList);
		while (parser.hasNextLine()) {
			parser.nextLine();
		}
	}
}
