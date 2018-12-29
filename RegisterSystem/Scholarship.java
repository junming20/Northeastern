
class Scholarship {
	String id;
	String description;
	Scholarship(String id, String description) {
		this.id = id;
		this.description = description;
	}
	public void setid(String id) {
		this.id = id;
	}
	public String getid() {
		return id;
	}
	public void setdescription(String description) {
		this.description = description;
	}
	public String getdescription() {
		return description;
	}
}
	
class StatusScholar extends Scholarship {
	StatusScholar(String id, String description) {
		super(id, description);
	}
}
class MeritScholar extends Scholarship {
	MeritScholar(String id, String description) {
		super(id, description);
	}
}