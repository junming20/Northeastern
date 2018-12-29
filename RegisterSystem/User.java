import java.util.*;

public class User {
	String name;
	String password;
	User(String name, String password){
		this.name = name;
		this.password = password;
	}
	public void setName(String name) {
		this.name = name;
	}
	public String getName() {
		return name;
	}
	public void setPassword(String password) {
		this.password = password;
	}
	public String getPassword() {
		return password;
	}
	void browseCourse(List<Course> courseList) {
		for(int i = 0; i < courseList.size(); i++) {
			System.out.println(courseList.get(i).getName() + ":" + courseList.get(i).getCapacity());
		}
	}
}

class Teacher extends User {
	List<Assignment> assignmentList = new ArrayList<> ();
	public Teacher(String name, String password) {
		super(name, password);
	}
	void addAssignment(String[] teaCom) {
		assignmentList.add(new Assignment(teaCom[2], teaCom[3], teaCom[4]));
		System.out.println(teaCom[2] + " added");
	}
	void deleteAssignment(String[] teaCom) {
		for (int i = 0; i < assignmentList.size(); i++) {
			if (teaCom[2].equals(assignmentList.get(i).getName())) {
				assignmentList.remove(i);
				System.out.println(teaCom[2] + " deleted");
			} else {
				continue;
			}
		}
	}
	void browseAssignment() {
		for (int i = 0; i < assignmentList.size(); i++) {
			System.out.println(assignmentList.get(i).name);
		}
	}
	void gradeSub(String[] teaCom, Map<String, Student> studentMap) {
		// TODO Auto-generated method stub
		studentMap.get(teaCom[1]).submissionMap.put(teaCom[3], teaCom[4]);
		System.out.println(teaCom[3] + " of " + teaCom[1] + " is graded");
	}
}

class Student extends User {
	Set<Course> stuCourse;
	Map<Course, Integer> transcriptMap;
	Map<String, String> submissionMap;	//submissionName grade(A/B/C)
	Student(String name, String password) {
		super(name, password);
		this.stuCourse = new HashSet<Course> ();
		this.transcriptMap = new HashMap<> ();
		this.submissionMap = new HashMap<> ();
	}
	void stuSchedule() {
		for (Course sc : stuCourse) {
			System.out.println(sc.getName() + ":" + sc.getTime());
		}
	}
	void stuTranscript() {
		Set<Course> keySet = transcriptMap.keySet();
		for (Course c : keySet) {
			System.out.println(c.getName() + ":" + transcriptMap.get(c));
		}
	}
	/*
	void browseCourse(List<Course> courseList) {
		for(int i = 0; i < courseList.size(); i++) {
			System.out.println(courseList.get(i).getName() + ":" + courseList.get(i).getCapacity());
		}
	}
	*/
	void stuSignup(String courseName, List<Course> courseList) {
		for (int i = 0; i < courseList.size(); i++) {
			if (courseList.get(i).getName().equals(courseName) && 
					courseList.get(i).getCapacity() > courseList.get(i).getEnrolled()) {
				System.out.println(courseName + " signed up");
				stuCourse.add(courseList.get(i));
			} else if (courseList.get(i).getCapacity() < courseList.get(i).getEnrolled()) {
				System.out.println("Capacity is not enough!");
			}
		}
	}
	void stuDrop(String courseName, List<Course> courseList) {
		for (int i = 0; i < courseList.size(); i++) {
			if (courseList.get(i).getName().equals(courseName)) {
				stuCourse.remove(courseList.get(i));
				System.out.println(courseName + " removed");
			} else if (courseList.get(i).getName().equals(null)){
				System.out.println("Remove failed");
			}
		}
	}
	void browseSub() {
		for(String key : submissionMap.keySet()) {
			System.out.println(key);
		}
	}
	void createSub(String subName/*, String lowestgrade*/) {
		submissionMap.put(subName/*, lowestgrade*/, null);
		System.out.println(subName + " is created");
	}
	void deleteSub(String subName) {
		submissionMap.remove(subName);
		System.out.println(subName + " is deleted");
	}
	void updateSub(String from, String to) {
		submissionMap.remove(from);
		submissionMap.put(to,null);
		System.out.println(from + " is updeated to" + to);
	}
}

class CourseMgr extends User {

	CourseMgr(String name, String password) {
		super(name, password);
	}
	void addCourse(String[] cMgrCom, List<Course> courseList) {
		if (cMgrCom[1].equals("online")) {
			courseList.add(new onLCourse(cMgrCom[2],Integer.parseInt(cMgrCom[3]),cMgrCom[4],cMgrCom[5], Integer.parseInt(cMgrCom[6])));
			System.out.println(cMgrCom[2] + " added");
		} else if (cMgrCom[1].equals("offline")) {
			courseList.add(new offLCourse(cMgrCom[2],Integer.parseInt(cMgrCom[3]), cMgrCom[4],cMgrCom[5], Integer.parseInt(cMgrCom[6])));
			System.out.println(cMgrCom[2] + " added");
		}
	}
	void deleteCourse(String[] cMgrCom, List<Course> courseList) {
		for (int i = 0; i < courseList.size(); i++) {
			if (cMgrCom[2].equals(courseList.get(i).getName())) {
				courseList.remove(i);
				System.out.println(cMgrCom[2] + " deleted");
			} else {
				continue;
			}
		}
	}
	
}

class FinanceMgr extends User {
	Map<String, Scholarship> scholarMap; //id
	Map<Student, Scholarship> attachMap;
	FinanceMgr(String name, String password) {
		super(name, password);
		this.scholarMap = new HashMap<>();
		this.attachMap = new HashMap<>();
	}
	void addScholar(String[] fMgrCom) {
		if (fMgrCom[1].equals("statusScholarship")) {
			scholarMap.put(fMgrCom[2], new StatusScholar(fMgrCom[2], fMgrCom[3]));
			System.out.println("scholarship " + fMgrCom[2] + " added");
		} else if (fMgrCom[1].equals("meritScholarship")) {
			scholarMap.put(fMgrCom[2], new MeritScholar(fMgrCom[2], fMgrCom[3]));
			System.out.println("scholarship " + fMgrCom[2] + " added");
		}
	}
	void deleteScholar(String[] fMgrCom) {
		scholarMap.remove(fMgrCom[2]);
		System.out.println("scholarship " + fMgrCom[2] + " removed");	
	}
	void browseScholar() {
		for (String key : scholarMap.keySet()) {
			System.out.println(key + " " +scholarMap.get(key).description);
		}
	}
	void attachScholar(String[] fMgrCom, Map<String, Student> studentMap) {
		attachMap.put(studentMap.get(fMgrCom[4]), scholarMap.get(fMgrCom[2]));  //attach scholarship id to stuName
		System.out.println("scholarship " + fMgrCom[2] + " is attached to student " + fMgrCom[4]);
	}
	void removeScholar(String[] fMgrCom, Map<String, Student> studentMap) {
		attachMap.remove(studentMap.get(fMgrCom[4]), scholarMap.get(fMgrCom[2]));  //remove scholarship id from stuName
		System.out.println("scholarship " + fMgrCom[2] + " is removed from student " + fMgrCom[4]);
	}
	void browseScholarAssi() {
		for (Student key : attachMap.keySet()) {
			System.out.println(key.name + " scholarship id: " + attachMap.get(key).id);
		}
	}
}

