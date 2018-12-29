import java.io.*;
import java.util.*;

public class Parser {
	private Map<String, Student> studentMap;
	private Map<String, Teacher> teacherMap;
	private Map<String, FinanceMgr> financeMgrMap;
	private Map<String, CourseMgr> courseMgrMap;
	private List<Course> courseList;
	
	Parser(Map<String,Student> studentMap, Map<String,Teacher> teacherMap, Map<String,FinanceMgr> financeMgrMap, 
			Map<String,CourseMgr> courseMgrMap, List<Course> courseList) throws FileNotFoundException{
		this.studentMap = studentMap;
		this.teacherMap = teacherMap;
		this.financeMgrMap = financeMgrMap;
		this.courseMgrMap = courseMgrMap;
		this.courseList = courseList;
	}
	File file = new File("/Users/junmingzhao/Desktop/RegisterTest.txt");
	Scanner in = new Scanner(file);
	boolean hasNextLine() {
		while(in.hasNextLine()) {
			return true;
		}
		return false;
	}
	void nextLine() {
		System.out.println("New User?");
		String str = in.nextLine();
		System.out.println(str);
		login(str);
	}
	void login(String str) { //whether a new user
		if (str.equals("yes")) {
			addUser();
		} else if (str.equals("no")) {
			testbed();
		}
	}
	private void addUser() {
		System.out.println("student/teacher/financeMgr/courseMgr?");
		String str = in.nextLine();
		System.out.println(str);
		if (str.equals("student")) {
			System.out.println("Create userName & password");
			System.out.println(str = in.nextLine());
			String[] signupInfo = str.split(",");
			if (!studentMap.containsKey(signupInfo[0])) {
				studentMap.put(signupInfo[0], new Student(signupInfo[0], signupInfo[1]));//userName password
				System.out.println("account " + signupInfo[0] + " created");
			} else 
				System.out.println("Username occupied!");
		} else if (str.equals("teacher")) {
			System.out.println("Create userName & password");
			System.out.println(str = in.nextLine());
			String[] signupInfo = str.split(",");
			if (!teacherMap.containsKey(signupInfo[0])) {
				teacherMap.put(signupInfo[0], new Teacher(signupInfo[0], signupInfo[1]));
				System.out.println("account " + signupInfo[0] + " created");
			} else 
				System.out.println("Username occupied!");
		} else if (str.equals("courseMgr")) {
			System.out.println("Create userName & password");
			System.out.println(str = in.nextLine());
			String[] signupInfo = str.split(",");
			if (!courseMgrMap.containsKey(signupInfo[0])) {
				courseMgrMap.put(signupInfo[0], new CourseMgr(signupInfo[0], signupInfo[1]));
				System.out.println("account " + signupInfo[0] + " created");
			} else 
				System.out.println("Username occupied!");
		} else if (str.equals("financeMgr")) {
			System.out.println("Create userName & password");
			System.out.println(str = in.nextLine());
			String[] signupInfo = str.split(",");
			if (!financeMgrMap.containsKey(signupInfo[0])) {
				financeMgrMap.put(signupInfo[0], new FinanceMgr(signupInfo[0], signupInfo[1]));
				System.out.println("account " + signupInfo[0] + " created");
			} else 
				System.out.println("Username occupied!");
		}
	}
	
	private void testbed() {
		System.out.println("student/teacher/financeMgr/courseMgr?");
		String str = in.nextLine();
		System.out.println(str);
		//student
		if (str.equals("student")) {
			System.out.println("Input username,password");
			str = in.nextLine();
			String[] loginInfo = str.split(",");
			//If user name and password match
			if (studentMap.containsKey(loginInfo[0]) && studentMap.get(loginInfo[0]).password.equals(loginInfo[1])) {
				System.out.println("Login successfully");
				while (hasNextLine()) {
					System.out.println("Input your command");
					String studentCommand = in.nextLine();
					String[] stuCom = studentCommand.split(" ");
					if (stuCom[0].equals("browse") && stuCom[1].equals("course")){ //browse course
						//studentMap.get(loginInfo[0]).browseCourse();
						studentMap.get(loginInfo[0]).browseCourse(courseList);
					} else if (stuCom[0].equals("signup")) {               //signup courseName
						studentMap.get(loginInfo[0]).stuSignup(stuCom[1], courseList);
					} else if (stuCom[0].equals("drop")) {                 //drop courseName
						studentMap.get(loginInfo[0]).stuDrop(stuCom[1], courseList);
					} else if (stuCom[0].equals("view") && stuCom[1].equals("schedule")) {             //view schedule 
						studentMap.get(loginInfo[0]).stuSchedule();
					} else if (stuCom[0].equals("view") && stuCom[1].equals("transcript")) {           //view transcript
						studentMap.get(loginInfo[0]).stuTranscript();	   
					} else if (stuCom[0].equals("browse") && stuCom[1].equals("submission")) {
						studentMap.get(loginInfo[0]).browseSub();		   			//browse submission
					} else if (stuCom[0].equals("create") && stuCom[1].equals("submission")) {
						studentMap.get(loginInfo[0]).createSub(stuCom[2]/*, stuCom[3]*/);	//create submission name 
					} else if (stuCom[0].equals("delete") && stuCom[1].equals("submission")) {
						studentMap.get(loginInfo[0]).deleteSub(stuCom[2]);					//delete submission name   			
					} else if (stuCom[0].equals("update") && stuCom[1].equals("submission")) {
						studentMap.get(loginInfo[0]).updateSub(stuCom[3], stuCom[5]);       //update submission from nameA to nameB
					} else if (stuCom[0].equals("signout")){
						break;
					}
				}
			} else {
				System.out.println("username or password is wrong!");
			}
			//teacher
		} else if (str.equals("teacher")) {
			System.out.println("Input username,password");
			str = in.nextLine();
			String[] loginInfo = str.split(",");
			
			if (teacherMap.containsKey(loginInfo[0]) && teacherMap.get(loginInfo[0]).password.equals(loginInfo[1])) {
				System.out.println("Login successfully");
				while (hasNextLine()) {
					System.out.println("Input your command");
					String teacherCommand = in.nextLine();
					String[] teaCom = teacherCommand.split(" ");
					if (teaCom[0].equals("browse") && teaCom[1].equals("assignment")) {		//browse assignment
						teacherMap.get(loginInfo[0]).browseAssignment();
					} else if (teaCom[0].equals("create") && teaCom[1].equals("assignment")) { //create assignment name deadline type(essay/multiple choice)
						teacherMap.get(loginInfo[0]).addAssignment(teaCom);
					} else if (teaCom[0].equals("delete") && teaCom[1].equals("assignment")) { //delete assignment name
						teacherMap.get(loginInfo[0]).deleteAssignment(teaCom);
					} else if (teaCom[0].equals("grade") && teaCom[2].equals("submission")) {   //grade studentName submission subName grade(A/B/C) 
						teacherMap.get(loginInfo[0]).gradeSub(teaCom, studentMap);
					} else if (teaCom[0].equals("signout")){
						break;
					}
				}
			} else {
				System.out.println("username or password is wrong!");
			}
			//courseManager
		} else if (str.equals("courseMgr")) {
			System.out.println("Input username,password");
			str = in.nextLine();
			String[] loginInfo = str.split(",");
			if (courseMgrMap.containsKey(loginInfo[0]) && courseMgrMap.get(loginInfo[0]).password.equals(loginInfo[1])) {
				System.out.println("Login successfully");
				while (hasNextLine()) {
					System.out.println("Input your command");
					String courseMgrCommand = in.nextLine();
					String[] cMgrCom = courseMgrCommand.split(" ");
					if (cMgrCom[0].equals("add")) {					//add course name capacity time location/URL
						courseMgrMap.get(loginInfo[0]).addCourse(cMgrCom, courseList);
					} else if (cMgrCom[0].equals("delete")) {   	//delete course 
						courseMgrMap.get(loginInfo[0]).deleteCourse(cMgrCom, courseList);
					} else if (cMgrCom[0].equals("browse")) {   	//browse
						courseMgrMap.get(loginInfo[0]).browseCourse(courseList);
					} else if (cMgrCom[0].equals("signout")) {
						break;
					}
				}
			} else {
				System.out.println("username or password is wrong!");
			}
			//financeManager
		} else if (str.equals("financeMgr")) {
			System.out.println("Input username,password");
			str = in.nextLine();
			String[] loginInfo = str.split(",");
			if (financeMgrMap.containsKey(loginInfo[0]) && financeMgrMap.get(loginInfo[0]).password.equals(loginInfo[1])) {
				System.out.println("Login successfully");
				while (hasNextLine()) {
					System.out.println("Input your command");
					String finMgrCommand = in.nextLine();
					String[] fMgrCom = finMgrCommand.split(" ");
					
					if (fMgrCom[0].equals("add")) {					//add xscholarship id description
						financeMgrMap.get(loginInfo[0]).addScholar(fMgrCom);
					} else if (fMgrCom[0].equals("delete")) {   	//delete scholarship id
						financeMgrMap.get(loginInfo[0]).deleteScholar(fMgrCom);	
					} else if (fMgrCom[0].equals("browse") && fMgrCom[1].equals("scholarship") && fMgrCom[2].equals("assignment")) { //browse scholarship assignment
						financeMgrMap.get(loginInfo[0]).browseScholarAssi();
					} else if (fMgrCom[0].equals("browse")) {   	//browse scholarship map
						financeMgrMap.get(loginInfo[0]).browseScholar();
					} else if (fMgrCom[0].equals("attach")) {		//attach scholarship id to stuName
						financeMgrMap.get(loginInfo[0]).attachScholar(fMgrCom, studentMap);
					} else if (fMgrCom[0].equals("remove")) {		//remove scholarship id from stuName
						financeMgrMap.get(loginInfo[0]).removeScholar(fMgrCom, studentMap);
					} else if (fMgrCom[0].equals("signout")){
						break;
					}
				}
			} else {
				System.out.println("username or password is wrong!");
			}
		}
	}
}


