
public class Assignment {
	String name;
	String deadline;
	String type;		//essay or multiple choice
	Assignment(String name, String deadline, String type) {
		this.name = name;
		this.deadline = deadline;
		this.type = type;
	}
	
	public String getName() {
		return name;
	}
	public String getDeadline() {
		return deadline;
	}
	public String getType() {
		return type;
	}
	
	public void setName(String name) {
		this.name = name;
	}
	public void setDeadLine(String deadline) {
		this.deadline = deadline;
	}
	public void setType(String type) {
		this.type = type;
	}
}
