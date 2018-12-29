
public interface Course {
	String getName();
	int getCapacity();
	int getEnrolled();
	String getTime();
}

class offLCourse implements Course {
	String name;
	int capacity;
	String time;
	int enrolled;
	String location;
	public offLCourse(String name, int capacity, String time, String location, int enrolled) {
		// TODO Auto-generated constructor stub
		this.name = name;
		this.capacity = capacity;
		this.time = time;
		this.enrolled = enrolled;
		this.location = location;
	}
	public void setName(String name) {
		this.name = name;
	}
	public String getName() {
		return name;
	}
	
	public void setCapacity(int capacity) {
		this.capacity = capacity;
	}
	public int getCapacity() {
		return capacity;
	}
	public void setEnrolled(int enrolled) {
		this.enrolled = enrolled;
	}
	@Override
	public int getEnrolled() {
		// TODO Auto-generated method stub
		return enrolled;
	}
	public void setTime(String time) {
		this.time = time;
	}
	@Override
	public String getTime() {
		// TODO Auto-generated method stub
		return time;
	}
	public void setLocation(String location) {
		this.location = location;
	}
	public String getLocation() {
		// TODO Auto-generated method stub
		return location;
	}
}

class onLCourse implements Course {
	String name;
	int capacity;
	String time;
	int enrolled;
	String URL;
	public onLCourse(String name, int capacity, String time, String URL, int enrolled) {
		// TODO Auto-generated constructor stub
		this.name = name;
		this.capacity = capacity;
		this.time = time;
		this.enrolled = enrolled;
		this.URL = URL;
	}
	public void setName(String name) {
		this.name = name;
	}
	public String getName() {
		return name;
	}
	
	public void setCapacity(int capacity) {
		this.capacity = capacity;
	}
	public int getCapacity() {
		return capacity;
	}
	public void setEnrolled(int enrolled) {
		this.enrolled = enrolled;
	}
	@Override
	public int getEnrolled() {
		// TODO Auto-generated method stub
		return enrolled;
	}
	public void setTime(String time) {
		this.time = time;
	}
	@Override
	public String getTime() {
		// TODO Auto-generated method stub
		return null;
	}
	public void setLocation(String location) {
		this.URL = location;
	}
	public String getLocation() {
		// TODO Auto-generated method stub
		return URL;
	}
}